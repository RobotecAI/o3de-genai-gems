/*
 * Copyright (c) Contributors to the Open 3D Engine Project.
 * For complete copyright and license terms please see the LICENSE at the root of this distribution.
 *
 * SPDX-License-Identifier: Apache-2.0 OR MIT
 *
 */

#include "MockRequesterComponent.h"
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
#include <AzCore/std/string/string.h>

namespace GenAIMock
{
    void MockRequesterComponentConfiguration::Reflect(AZ::ReflectContext* context)
    {
        if (auto serializeContext = azrtti_cast<AZ::SerializeContext*>(context))
        {
            serializeContext->Class<MockRequesterComponentConfiguration, AZ::ComponentConfig>()->Version(0)->Field(
                "assetPath", &MockRequesterComponentConfiguration::m_assetPath);

            if (auto editContext = serializeContext->GetEditContext())
            {
                editContext->Class<MockRequesterComponentConfiguration>("Mock Requester configuration", "")
                    ->ClassElement(AZ::Edit::ClassElements::EditorData, "")
                    ->Attribute(AZ::Edit::Attributes::Category, "AI")
                    ->DataElement(
                        AZ::Edit::UIHandlers::Default,
                        &MockRequesterComponentConfiguration::m_assetPath,
                        "JSON Asset Path",
                        "JSON Asset Path")
                    ->Attribute(AZ::Edit::Attributes::SourceAssetFilterPattern, "*.json");
            }
        }
    }

    AZ::IO::Path MockRequesterComponentConfiguration::GetAssetPath() const
    {
        return m_assetPath;
    }

    void MockRequesterComponent::Reflect(AZ::ReflectContext* context)
    {
        MockRequesterComponentConfiguration::Reflect(context);
        if (auto serializeContext = azrtti_cast<AZ::SerializeContext*>(context))
        {
            serializeContext->Class<MockRequesterComponent, AZ::Component>()
                ->Version(0)
                ->Field("Configuration", &MockRequesterComponent::m_configuration)
                ->Field("testData", &MockRequesterComponent::m_testData);

            if (auto editContext = serializeContext->GetEditContext())
            {
                editContext->Class<MockRequesterComponent>("Mock Requester component", "")
                    ->ClassElement(AZ::Edit::ClassElements::EditorData, "")
                    ->Attribute(AZ::Edit::Attributes::Category, "AI")
                    ->DataElement(
                        AZ::Edit::UIHandlers::Default,
                        &MockRequesterComponent::m_configuration,
                        "Configuration",
                        "Configuration for the Mock requester")
                    ->Attribute(AZ::Edit::Attributes::Visibility, AZ::Edit::PropertyVisibility::ShowChildrenOnly)
                    ->Attribute(AZ::Edit::Attributes::ChangeNotify, &MockRequesterComponent::ReloadAsset);
            }
        }

        if (auto registrationContext = azrtti_cast<GenAIFramework::SystemRegistrationContext*>(context))
        {
            registrationContext->RegisterGenAIFrameworkServiceRequester<MockRequesterComponent>();
        }
    }

    MockRequesterComponent::MockRequesterComponent(const MockRequesterComponentConfiguration& config)
        : m_configuration(config)
    {
    }

    void MockRequesterComponent::ReloadAsset()
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
            AZ_Warning("MockRequesterComponent", false, "File %s not found, mock data was not reloaded.", fullPath.c_str());
            return;
        }

        AZ_Printf("MockRequesterComponent", "Reloading data from file %s", fullPath.c_str());
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
                        m_testData.push_back(testData[i].GetString());
                    }
                }
            }
        }
        else
        {
            AZ_Warning("MockRequesterComponent", false, "Failed to parse file %s: %s", fullPath.c_str(), result.GetError().c_str());
        }
    }

    void MockRequesterComponent::Init()
    {
    }

    void MockRequesterComponent::Activate()
    {
        GenAIFramework::AIServiceRequesterBus::Handler::BusConnect(GetEntityId());
    }

    void MockRequesterComponent::Deactivate()
    {
        GenAIFramework::AIServiceRequesterBus::Handler::BusDisconnect();
    }

    void MockRequesterComponent::SetConfiguration(const MockRequesterComponentConfiguration& config)
    {
        m_configuration = config;
    }

    const MockRequesterComponentConfiguration& MockRequesterComponent::GetConfiguration() const
    {
        return m_configuration;
    }

    void MockRequesterComponent::SendRequest(
        const AZStd::string& request, AZStd::function<void(AZ::Outcome<AZStd::string, AZStd::string>)> callback)
    {
        const int index = std::atoi(request.c_str());
        if (index >= 0 && index < m_testData.size())
        {
            AZ::Outcome<AZStd::string, AZStd::string> outcomeResult = AZ::Success(m_testData[index]);
            callback(outcomeResult);
        }
        else
        {
            AZStd::string azStringResult("I have no answer for that.");
            AZ::Outcome<AZStd::string, AZStd::string> outcomeError = AZ::Failure(azStringResult);
            callback(outcomeError);
        }
    }
} // namespace GenAIMock
