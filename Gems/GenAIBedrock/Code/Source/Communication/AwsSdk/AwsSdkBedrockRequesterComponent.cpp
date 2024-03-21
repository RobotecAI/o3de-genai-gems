
#include "AwsSdkBedrockRequesterComponent.h"
#include <AzCore/Component/Component.h>
#include <AzCore/Outcome/Outcome.h>
#include <AzCore/Serialization/EditContext.h>
#include <AzCore/Serialization/EditContextConstants.inl>
#include <AzCore/Serialization/SerializeContext.h>
#include <AzCore/std/string/string.h>
#include <GenAIFramework/SystemRegistrationContext/GenAIFrameworkSystemRegistrationContext.h>

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
    void AwsSdkBedrockRequesterConfiguration::Reflect(AZ::ReflectContext* context)
    {
        if (auto serializeContext = azrtti_cast<AZ::SerializeContext*>(context))
        {
            serializeContext->Class<AwsSdkBedrockRequesterConfiguration, AZ::ComponentConfig>()
                ->Version(0)
                ->Field("RegionName", &AwsSdkBedrockRequesterConfiguration::m_regionName)
                ->Field("ModelId", &AwsSdkBedrockRequesterConfiguration::m_modelId);

            if (auto editContext = serializeContext->GetEditContext())
            {
                editContext->Class<AwsSdkBedrockRequesterConfiguration>("AWS SDK Requester configuration", "")
                    ->ClassElement(AZ::Edit::ClassElements::EditorData, "")
                    ->Attribute(AZ::Edit::Attributes::Category, "AI")
                    ->DataElement(AZ::Edit::UIHandlers::Default, &AwsSdkBedrockRequesterConfiguration::m_regionName, "Region Name", "")
                    ->DataElement(AZ::Edit::UIHandlers::Default, &AwsSdkBedrockRequesterConfiguration::m_modelId, "Model Id", "");
            }
        }
    }

    void AwsSdkBedrockRequesterComponent::Reflect(AZ::ReflectContext* context)
    {
        AwsSdkBedrockRequesterConfiguration::Reflect(context);
        if (auto serializeContext = azrtti_cast<AZ::SerializeContext*>(context))
        {
            serializeContext->Class<AwsSdkBedrockRequesterComponent, AZ::Component>()->Version(0)->Field(
                "Configuration", &AwsSdkBedrockRequesterComponent::m_configuration);

            if (auto editContext = serializeContext->GetEditContext())
            {
                editContext->Class<AwsSdkBedrockRequesterComponent>("AWS SDK Requester component", "")
                    ->ClassElement(AZ::Edit::ClassElements::EditorData, "")
                    ->Attribute(AZ::Edit::Attributes::Category, "AI")
                    ->DataElement(
                        AZ::Edit::UIHandlers::Default,
                        &AwsSdkBedrockRequesterComponent::m_configuration,
                        "Configuration",
                        "Configuration for the AWS SDK requester")
                    ->Attribute(AZ::Edit::Attributes::Visibility, AZ::Edit::PropertyVisibility::ShowChildrenOnly)
                    ->Attribute(AZ::Edit::Attributes::ChangeNotify, &AwsSdkBedrockRequesterComponent::OnConfigurationChanged);
            }
        }

        if (auto registrationContext = azrtti_cast<GenAIFramework::GenAIFrameworkSystemRegistrationContext*>(context))
        {
            registrationContext->RegisterGenAIFrameworkServiceRequester<AwsSdkBedrockRequesterComponent>();
        }
    }

    void AwsSdkBedrockRequesterComponent::OnConfigurationChanged()
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
        m_client = AZStd::make_unique<Aws::Bedrock::BedrockClient>(*m_clientConfiguration);
        m_runtimeClient = AZStd::make_unique<Aws::BedrockRuntime::BedrockRuntimeClient>(*m_runtimeClientConfiguration);
    }

    AwsSdkBedrockRequesterComponent::AwsSdkBedrockRequesterComponent(const AwsSdkBedrockRequesterConfiguration& config)
        : m_configuration(config)
    {
    }

    void AwsSdkBedrockRequesterComponent::Init()
    {
    }

    void AwsSdkBedrockRequesterComponent::Activate()
    {
        OnConfigurationChanged();
        GenAIFramework::AIServiceRequesterBus::Handler::BusConnect(GetEntityId());
    }

    void AwsSdkBedrockRequesterComponent::Deactivate()
    {
        GenAIFramework::AIServiceRequesterBus::Handler::BusDisconnect();
    }

    void AwsSdkBedrockRequesterComponent::SetConfiguration(const AwsSdkBedrockRequesterConfiguration& config)
    {
        m_configuration = config;
    }

    const AwsSdkBedrockRequesterConfiguration& AwsSdkBedrockRequesterComponent::GetConfiguration() const
    {
        return m_configuration;
    }

    void AwsSdkBedrockRequesterComponent::SendRequest(
        const AZStd::string& request, AZStd::function<void(AZ::Outcome<AZStd::string, AZStd::string>)> callback)
    {
        using namespace Aws::BedrockRuntime;

        Aws::BedrockRuntime::Model::InvokeModelRequest invokeModelRequest;
        invokeModelRequest.SetModelId(m_configuration.m_modelId.c_str());
        std::shared_ptr<Aws::StringStream> ss = std::make_shared<Aws::StringStream>();
        *ss << request.c_str();

        invokeModelRequest.SetBody(ss);
        invokeModelRequest.SetContentType("application/json");

        Aws::BedrockRuntime::InvokeModelResponseReceivedHandler handler =
            [callback](
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
                const Aws::BedrockRuntime::BedrockRuntimeError& error = outcome.GetError();
                callback(AZ::Failure(error.GetMessage().c_str()));
            }
        };

        m_runtimeClient->InvokeModelAsync(invokeModelRequest, handler);
    }
} // namespace GenAIBedrock
