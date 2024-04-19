/*
 * Copyright (c) Contributors to the Open 3D Engine Project.
 * For complete copyright and license terms please see the LICENSE at the root of this distribution.
 *
 * SPDX-License-Identifier: Apache-2.0 OR MIT
 *
 */

#include "ClaudeBedrockProvider.h"
#include <AzCore/Component/Component.h>
#include <AzCore/Outcome/Outcome.h>
#include <AzCore/Serialization/EditContext.h>
#include <AzCore/Serialization/EditContextConstants.inl>
#include <AzCore/Serialization/SerializeContext.h>
#include <AzCore/std/string/string.h>
#include <GenAIFramework/SystemRegistrationContext/SystemRegistrationContext.h>

#include <aws/bedrock-runtime/BedrockRuntimeErrors.h>
#include <aws/bedrock-runtime/BedrockRuntimeServiceClientModel.h>
#include <aws/bedrock-runtime/model/InvokeModelRequest.h>
#include <aws/bedrock-runtime/model/InvokeModelResult.h>
#include <aws/core/utils/memory/stl/AWSStreamFwd.h>
#include <aws/core/utils/memory/stl/AWSStringStream.h>
#include <memory>

namespace GenAIBedrockVendorBundle
{
    void ClaudeBedrockProviderConfiguration::Reflect(AZ::ReflectContext* context)
    {
        if (auto serializeContext = azrtti_cast<AZ::SerializeContext*>(context))
        {
            serializeContext->Class<ClaudeBedrockProviderConfiguration, AZ::ComponentConfig>()
                ->Version(0)
                ->Field("RegionName", &ClaudeBedrockProviderConfiguration::m_regionName)
                ->Field("ModelId", &ClaudeBedrockProviderConfiguration::m_modelId);

            if (auto editContext = serializeContext->GetEditContext())
            {
                editContext->Class<ClaudeBedrockProviderConfiguration>("Claude Bedrock Service Provider Configuration", "")
                    ->ClassElement(AZ::Edit::ClassElements::EditorData, "")
                    ->Attribute(AZ::Edit::Attributes::Category, "AI")
                    ->DataElement(AZ::Edit::UIHandlers::Default, &ClaudeBedrockProviderConfiguration::m_regionName, "Region Name", "")
                    ->DataElement(AZ::Edit::UIHandlers::Default, &ClaudeBedrockProviderConfiguration::m_modelId, "Model Id", "");
            }
        }
    }

    void ClaudeBedrockProvider::Reflect(AZ::ReflectContext* context)
    {
        ClaudeBedrockProviderConfiguration::Reflect(context);
        if (auto serializeContext = azrtti_cast<AZ::SerializeContext*>(context))
        {
            serializeContext->Class<ClaudeBedrockProvider, AZ::Component>()->Version(0)->Field(
                "Configuration", &ClaudeBedrockProvider::m_configuration);

            if (auto editContext = serializeContext->GetEditContext())
            {
                editContext->Class<ClaudeBedrockProvider>("Claude Bedrock Service Provider Configuration", "")
                    ->ClassElement(AZ::Edit::ClassElements::EditorData, "")
                    ->Attribute(AZ::Edit::Attributes::Category, "AI")
                    ->DataElement(
                        AZ::Edit::UIHandlers::Default,
                        &ClaudeBedrockProvider::m_configuration,
                        "Configuration",
                        "Configuration for the Claude Bedrock provider")
                    ->Attribute(AZ::Edit::Attributes::Visibility, AZ::Edit::PropertyVisibility::ShowChildrenOnly)
                    ->Attribute(AZ::Edit::Attributes::ChangeNotify, &ClaudeBedrockProvider::OnConfigurationChanged);
            }
        }

        if (auto registrationContext = azrtti_cast<GenAIFramework::SystemRegistrationContext*>(context))
        {
            registrationContext->RegisterGenAIFrameworkServiceProvider<ClaudeBedrockProvider>();
        }
    }

    void ClaudeBedrockProvider::OnConfigurationChanged()
    {
        if (!m_runtimeClientConfiguration)
        {
            m_runtimeClientConfiguration = AZStd::make_unique<Aws::BedrockRuntime::BedrockRuntimeClientConfiguration>();
        }
        m_runtimeClientConfiguration->region = m_configuration.m_regionName.c_str();
        // NOTE(pawel-kotowski): This is a temporary solution to set the request timeout to 60 seconds. It will be handled by UI solution in
        // the future.
        m_runtimeClientConfiguration->requestTimeoutMs = 60000;
        m_runtimeClient = AZStd::make_unique<Aws::BedrockRuntime::BedrockRuntimeClient>(*m_runtimeClientConfiguration);
    }

    ClaudeBedrockProvider::ClaudeBedrockProvider(const ClaudeBedrockProviderConfiguration& config)
        : m_configuration(config)
    {
    }

    void ClaudeBedrockProvider::Init()
    {
    }

    void ClaudeBedrockProvider::Activate()
    {
        OnConfigurationChanged();
        GenAIFramework::AIServiceProviderBus::Handler::BusConnect(GetEntityId());
    }

    void ClaudeBedrockProvider::Deactivate()
    {
        GenAIFramework::AIServiceProviderBus::Handler::BusDisconnect();
    }

    void ClaudeBedrockProvider::SetConfiguration(const ClaudeBedrockProviderConfiguration& config)
    {
        m_configuration = config;
    }

    const ClaudeBedrockProviderConfiguration& ClaudeBedrockProvider::GetConfiguration() const
    {
        return m_configuration;
    }

    void ClaudeBedrockProvider::SendRequest(
        const AZStd::string& request, AZStd::function<void(AZ::Outcome<AZStd::string, AZStd::string>)> callback)
    {
        using namespace Aws::BedrockRuntime;

        Model::InvokeModelRequest invokeModelRequest;
        invokeModelRequest.SetModelId(m_configuration.m_modelId.c_str());
        std::shared_ptr<Aws::StringStream> ss = std::make_shared<Aws::StringStream>();
        *ss << request.c_str();

        invokeModelRequest.SetBody(ss);
        invokeModelRequest.SetContentType("application/json");

        InvokeModelResponseReceivedHandler handler = [callback](
                                                         const BedrockRuntimeClient* client,
                                                         const Model::InvokeModelRequest& request,
                                                         Model::InvokeModelOutcome outcome,
                                                         const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context)
        {
            if (outcome.IsSuccess())
            {
                auto* resultStream = &outcome.GetResult().GetBody();
                std::string result(std::istreambuf_iterator<char>(*resultStream), {});

                AZStd::string azStringResult(result.c_str());

                callback(AZ::Success(azStringResult));
            }
            else
            {
                const BedrockRuntimeError& error = outcome.GetError();
                callback(AZ::Failure(error.GetMessage().c_str()));
            }
        };

        m_runtimeClient->InvokeModelAsync(invokeModelRequest, handler);
    }
} // namespace GenAIBedrockVendorBundle
