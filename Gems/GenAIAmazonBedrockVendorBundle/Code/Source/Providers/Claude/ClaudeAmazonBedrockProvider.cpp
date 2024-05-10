/*
 * Copyright (c) Contributors to the Open 3D Engine Project.
 * For complete copyright and license terms please see the LICENSE at the root of this distribution.
 *
 * SPDX-License-Identifier: Apache-2.0 OR MIT
 *
 */

#include "ClaudeAmazonBedrockProvider.h"
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

namespace GenAIAmazonBedrockVendorBundle
{
    void ClaudeAmazonBedrockProviderConfiguration::Reflect(AZ::ReflectContext* context)
    {
        if (auto serializeContext = azrtti_cast<AZ::SerializeContext*>(context))
        {
            serializeContext->Class<ClaudeAmazonBedrockProviderConfiguration, AZ::ComponentConfig>()
                ->Version(0)
                ->Field("RegionName", &ClaudeAmazonBedrockProviderConfiguration::m_regionName)
                ->Field("ModelId", &ClaudeAmazonBedrockProviderConfiguration::m_modelId);

            if (auto editContext = serializeContext->GetEditContext())
            {
                editContext->Class<ClaudeAmazonBedrockProviderConfiguration>("Claude Amazon Bedrock Service Provider Configuration", "")
                    ->ClassElement(AZ::Edit::ClassElements::EditorData, "")
                    ->Attribute(AZ::Edit::Attributes::Category, "AI")
                    ->DataElement(AZ::Edit::UIHandlers::Default, &ClaudeAmazonBedrockProviderConfiguration::m_regionName, "Region Name", "")
                    ->DataElement(AZ::Edit::UIHandlers::Default, &ClaudeAmazonBedrockProviderConfiguration::m_modelId, "Model Id", "");
            }
        }
    }

    void ClaudeAmazonBedrockProvider::Reflect(AZ::ReflectContext* context)
    {
        ClaudeAmazonBedrockProviderConfiguration::Reflect(context);
        if (auto serializeContext = azrtti_cast<AZ::SerializeContext*>(context))
        {
            serializeContext->Class<ClaudeAmazonBedrockProvider, AZ::Component>()->Version(0)->Field(
                "Configuration", &ClaudeAmazonBedrockProvider::m_configuration);

            if (auto editContext = serializeContext->GetEditContext())
            {
                editContext->Class<ClaudeAmazonBedrockProvider>("Claude Amazon Bedrock Service Provider Configuration", "")
                    ->ClassElement(AZ::Edit::ClassElements::EditorData, "")
                    ->Attribute(AZ::Edit::Attributes::Category, "AI")
                    ->DataElement(
                        AZ::Edit::UIHandlers::Default,
                        &ClaudeAmazonBedrockProvider::m_configuration,
                        "Configuration",
                        "Configuration for the Claude Amazon Bedrock provider")
                    ->Attribute(AZ::Edit::Attributes::Visibility, AZ::Edit::PropertyVisibility::ShowChildrenOnly)
                    ->Attribute(AZ::Edit::Attributes::ChangeNotify, &ClaudeAmazonBedrockProvider::OnConfigurationChanged);
            }
        }

        if (auto registrationContext = azrtti_cast<GenAIFramework::SystemRegistrationContext*>(context))
        {
            registrationContext->RegisterGenAIFrameworkServiceProvider<ClaudeAmazonBedrockProvider>();
        }
    }

    void ClaudeAmazonBedrockProvider::OnConfigurationChanged()
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

    ClaudeAmazonBedrockProvider::ClaudeAmazonBedrockProvider(const ClaudeAmazonBedrockProviderConfiguration& config)
        : m_configuration(config)
    {
    }

    void ClaudeAmazonBedrockProvider::Init()
    {
    }

    void ClaudeAmazonBedrockProvider::Activate()
    {
        OnConfigurationChanged();
        GenAIFramework::AIServiceProviderBus::Handler::BusConnect(GetEntityId());
    }

    void ClaudeAmazonBedrockProvider::Deactivate()
    {
        GenAIFramework::AIServiceProviderBus::Handler::BusDisconnect();
    }

    void ClaudeAmazonBedrockProvider::SetConfiguration(const ClaudeAmazonBedrockProviderConfiguration& config)
    {
        m_configuration = config;
    }

    const ClaudeAmazonBedrockProviderConfiguration& ClaudeAmazonBedrockProvider::GetConfiguration() const
    {
        return m_configuration;
    }

    void ClaudeAmazonBedrockProvider::SendRequest(
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
} // namespace GenAIAmazonBedrockVendorBundle
