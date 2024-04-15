/*
 * Copyright (c) Contributors to the Open 3D Engine Project.
 * For complete copyright and license terms please see the LICENSE at the root of this distribution.
 *
 * SPDX-License-Identifier: Apache-2.0 OR MIT
 *
 */

#include "MockServiceComponent.h"
#include <GenAIFramework/SystemRegistrationContext/SystemRegistrationContext.h>

#include <API/EditorAssetSystemAPI.h>
#include <AzCore/Component/Component.h>
#include <AzCore/JSON/document.h>
#include <AzCore/JSON/rapidjson.h>
#include <AzCore/Outcome/Outcome.h>
#include <AzCore/Serialization/EditContext.h>
#include <AzCore/Serialization/EditContextConstants.inl>
#include <AzCore/Serialization/Json/JsonUtils.h>
#include <AzCore/Serialization/SerializeContext.h>
#include <AzCore/std/algorithm.h>
#include <AzCore/std/string/regex.h>
#include <AzCore/std/string/string.h>

namespace GenAIMock
{
    void MockServiceComponentConfiguration::Reflect(AZ::ReflectContext* context)
    {
        if (auto serializeContext = azrtti_cast<AZ::SerializeContext*>(context))
        {
            serializeContext->Class<MockServiceComponentConfiguration, AZ::ComponentConfig>()->Version(0)->Field(
                "assetPath", &MockServiceComponentConfiguration::m_assetPath);

            if (auto editContext = serializeContext->GetEditContext())
            {
                editContext->Class<MockServiceComponentConfiguration>("Mock Service Provider Configuration", "")
                    ->ClassElement(AZ::Edit::ClassElements::EditorData, "")
                    ->Attribute(AZ::Edit::Attributes::Category, "AI")
                    ->DataElement(
                        AZ::Edit::UIHandlers::Default,
                        &MockServiceComponentConfiguration::m_assetPath,
                        "JSON Asset Path",
                        "JSON Asset Path")
                    ->Attribute(AZ::Edit::Attributes::SourceAssetFilterPattern, "*.json");
            }
        }
    }

    AZ::IO::Path MockServiceComponentConfiguration::GetAssetPath() const
    {
        return m_assetPath;
    }

    void MockServiceComponent::Reflect(AZ::ReflectContext* context)
    {
        MockServiceComponentConfiguration::Reflect(context);
        if (auto serializeContext = azrtti_cast<AZ::SerializeContext*>(context))
        {
            serializeContext->Class<MockServiceComponent, AZ::Component>()->Version(0)->Field(
                "Configuration", &MockServiceComponent::m_configuration);

            if (auto editContext = serializeContext->GetEditContext())
            {
                editContext->Class<MockServiceComponent>("Mock Service Provider", "")
                    ->ClassElement(AZ::Edit::ClassElements::EditorData, "")
                    ->Attribute(AZ::Edit::Attributes::Category, "AI")
                    ->DataElement(
                        AZ::Edit::UIHandlers::Default,
                        &MockServiceComponent::m_configuration,
                        "Configuration",
                        "Configuration for the Mock Service Provider")
                    ->Attribute(AZ::Edit::Attributes::Visibility, AZ::Edit::PropertyVisibility::ShowChildrenOnly)
                    ->Attribute(AZ::Edit::Attributes::ChangeNotify, &MockServiceComponent::ReloadAsset);
            }
        }

        if (auto registrationContext = azrtti_cast<GenAIFramework::SystemRegistrationContext*>(context))
        {
            registrationContext->RegisterGenAIFrameworkServiceProvider<MockServiceComponent>();
        }
    }

    MockServiceComponent::MockServiceComponent(const MockServiceComponentConfiguration& config)
        : m_configuration(config)
    {
    }

    void MockServiceComponent::ReloadAsset()
    {
        const auto relativePath = m_configuration.GetAssetPath().String();
        bool hasResult = false;
        AZStd::string fullPath;
        AzToolsFramework::AssetSystemRequestBus::BroadcastResult(
            hasResult,
            &AzToolsFramework::AssetSystem::AssetSystemRequest::GetFullSourcePathFromRelativeProductPath,
            relativePath,
            fullPath);

        if (!hasResult)
        {
            AZ_Warning("MockServiceComponent", false, "File %s not found, mock data was not reloaded.", fullPath.c_str());
            return;
        }

        AZ_Printf("MockServiceComponent", "Reloading data from file %s", fullPath.c_str());
        m_testData.clear();
        auto result = AZ::JsonSerializationUtils::ReadJsonFile(fullPath);
        if (result.IsSuccess())
        {
            const auto& document = result.GetValue();
            if (document.HasMember("testData"))
            {
                const rapidjson::Value& testData = document["testData"];
                if (testData.IsArray())
                {
                    for (rapidjson::SizeType i = 0; i < testData.Size(); ++i)
                    {
                        if (testData[i].IsArray())
                        {
                            AZStd::string multiLineOut;
                            const auto& multiLineIn = testData[i];
                            for (rapidjson::SizeType l = 0; l < multiLineIn.Size(); ++l)
                            {
                                multiLineOut.append(multiLineIn[l].GetString());
                                multiLineOut.append("\n");
                            }
                            m_testData.emplace_back(AZStd::move(multiLineOut));
                        }
                        else
                        {
                            m_testData.push_back(testData[i].GetString());
                        }
                    }
                }
            }
        }
        else
        {
            AZ_Warning("MockServiceComponent", false, "Failed to parse file %s: %s", fullPath.c_str(), result.GetError().c_str());
        }

        m_lastCompleted = -1;
    }

    void MockServiceComponent::Init()
    {
    }

    void MockServiceComponent::Activate()
    {
        GenAIFramework::AIServiceProviderBus::Handler::BusConnect(GetEntityId());
    }

    void MockServiceComponent::Deactivate()
    {
        GenAIFramework::AIServiceProviderBus::Handler::BusDisconnect();
    }

    void MockServiceComponent::SetConfiguration(const MockServiceComponentConfiguration& config)
    {
        m_configuration = config;
    }

    const MockServiceComponentConfiguration& MockServiceComponent::GetConfiguration() const
    {
        return m_configuration;
    }

    void MockServiceComponent::SendRequest(
        const AZStd::string& request, AZStd::function<void(AZ::Outcome<AZStd::string, AZStd::string>)> callback)
    {
        if (m_testData.empty())
        {
            // Try to reload data if not available
            ReloadAsset();
        }

        // This service accepts two types of prompts:
        // 1) "restart" or "reset" - restart the counter and get the first mock outcome from the model
        // 2) [n] - set the counter to 'n' and get the 'n-th' mock outcome from the model
        // The model increases the counter and returns the next mock outcome otherwise.
        int index;
        if (request == "restart" || request == "reset")
        {
            index = 0;
        }
        else if (!request.empty() && AZStd::all_of(request.begin(), request.end(), ::isdigit))
        {
            index = std::atoi(request.c_str());
        }
        else
        {
            index = m_lastCompleted + 1;
        }

        if (index < m_testData.size())
        {
            AZ::Outcome<AZStd::string, AZStd::string> outcomeResult = AZ::Success(m_testData[index]);
            m_lastCompleted = index;
            callback(outcomeResult);
        }
        else
        {
            AZStd::string azStringResult("Out of range prompt for given mock data.");
            AZ::Outcome<AZStd::string, AZStd::string> outcomeError = AZ::Failure(azStringResult);
            callback(outcomeError);
        }
    }
} // namespace GenAIMock
