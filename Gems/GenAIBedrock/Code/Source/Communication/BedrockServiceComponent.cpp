/*
 * Copyright (c) Contributors to the Open 3D Engine Project.
 * For complete copyright and license terms please see the LICENSE at the root of this distribution.
 *
 * SPDX-License-Identifier: Apache-2.0 OR MIT
 *
 */

#include "BedrockServiceComponent.h"
#include <GenAIFramework/SystemRegistrationContext/SystemRegistrationContext.h>

#include <AzCore/Component/Component.h>
#include <AzCore/Outcome/Outcome.h>
#include <AzCore/Serialization/EditContext.h>
#include <AzCore/Serialization/EditContextConstants.inl>
#include <AzCore/Serialization/SerializeContext.h>
#include <AzCore/std/string/string.h>

#include <aws/bedrock-runtime/BedrockRuntimeErrors.h>
#include <aws/bedrock-runtime/BedrockRuntimeServiceClientModel.h>
#include <aws/bedrock-runtime/model/InvokeModelRequest.h>
#include <aws/bedrock-runtime/model/InvokeModelResult.h>
#include <aws/bedrock/BedrockClient.h>
#include <aws/core/utils/memory/stl/AWSStreamFwd.h>
#include <aws/core/utils/memory/stl/AWSStringStream.h>

#include <memory>

namespace GenAIBedrock
{
    void BedrockServiceConfiguration::Reflect(AZ::ReflectContext* context)
    {
        if (auto serializeContext = azrtti_cast<AZ::SerializeContext*>(context))
        {
            serializeContext->Class<BedrockServiceConfiguration, AZ::ComponentConfig>()
                ->Version(0)
                ->Field("RegionName", &BedrockServiceConfiguration::m_regionName)
                ->Field("ModelId", &BedrockServiceConfiguration::m_modelId);

            if (auto editContext = serializeContext->GetEditContext())
            {
                editContext->Class<BedrockServiceConfiguration>("AWS Bedrock Service Provider Configuration", "")
                    ->ClassElement(AZ::Edit::ClassElements::EditorData, "")
                    ->Attribute(AZ::Edit::Attributes::Category, "AI")
                    ->DataElement(AZ::Edit::UIHandlers::Default, &BedrockServiceConfiguration::m_regionName, "Region Name", "")
                    ->DataElement(AZ::Edit::UIHandlers::Default, &BedrockServiceConfiguration::m_modelId, "Model Id", "");
            }
        }
    }

    void BedrockServiceComponent::Reflect(AZ::ReflectContext* context)
    {
        BedrockServiceConfiguration::Reflect(context);
        if (auto serializeContext = azrtti_cast<AZ::SerializeContext*>(context))
        {
            serializeContext->Class<BedrockServiceComponent, AZ::Component>()->Version(0)->Field(
                "Configuration", &BedrockServiceComponent::m_configuration);

            if (auto editContext = serializeContext->GetEditContext())
            {
                editContext->Class<BedrockServiceComponent>("AWS Bedrock Service Provider", "")
                    ->ClassElement(AZ::Edit::ClassElements::EditorData, "")
                    ->Attribute(AZ::Edit::Attributes::Category, "AI")
                    ->DataElement(
                        AZ::Edit::UIHandlers::Default,
                        &BedrockServiceComponent::m_configuration,
                        "Configuration",
                        "Configuration of the AWS Bedrock Service Provider")
                    ->Attribute(AZ::Edit::Attributes::Visibility, AZ::Edit::PropertyVisibility::ShowChildrenOnly)
                    ->Attribute(AZ::Edit::Attributes::ChangeNotify, &BedrockServiceComponent::OnConfigurationChanged);
            }
        }

        if (auto registrationContext = azrtti_cast<GenAIFramework::SystemRegistrationContext*>(context))
        {
            registrationContext->RegisterGenAIFrameworkServiceProvider<BedrockServiceComponent>();
        }
    }

    void BedrockServiceComponent::OnConfigurationChanged()
    {
        if (!m_clientConfiguration)
        {
            m_clientConfiguration = AZStd::make_unique<Aws::Bedrock::BedrockClientConfiguration>();
        }
        if (!m_runtimeClientConfiguration)
        {
            m_runtimeClientConfiguration = AZStd::make_unique<Aws::BedrockRuntime::BedrockRuntimeClientConfiguration>();
        }
        m_clientConfiguration->region = m_configuration.m_regionName.c_str();
        m_runtimeClientConfiguration->region = m_configuration.m_regionName.c_str();

        // NOTE(pawel-kotowski): This is a temporary solution to set the request timeout to 60 seconds. It will be handled by UI solution in
        // the future.
        m_runtimeClientConfiguration->requestTimeoutMs = 60000;

        m_client = AZStd::make_unique<Aws::Bedrock::BedrockClient>(*m_clientConfiguration);
        m_runtimeClient = AZStd::make_unique<Aws::BedrockRuntime::BedrockRuntimeClient>(*m_runtimeClientConfiguration);
    }

    BedrockServiceComponent::BedrockServiceComponent(const BedrockServiceConfiguration& config)
        : m_configuration(config)
    {
    }

    void BedrockServiceComponent::Init()
    {
    }

    void BedrockServiceComponent::Activate()
    {
        OnConfigurationChanged();
        GenAIFramework::AIServiceProviderBus::Handler::BusConnect(GetEntityId());
    }

    void BedrockServiceComponent::Deactivate()
    {
        GenAIFramework::AIServiceProviderBus::Handler::BusDisconnect();
    }

    void BedrockServiceComponent::SetConfiguration(const BedrockServiceConfiguration& config)
    {
        m_configuration = config;
    }

    const BedrockServiceConfiguration& BedrockServiceComponent::GetConfiguration() const
    {
        return m_configuration;
    }

    void BedrockServiceComponent::SendRequest(
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
} // namespace GenAIBedrock
