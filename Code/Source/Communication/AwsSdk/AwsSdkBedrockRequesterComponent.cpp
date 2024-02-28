
#include "AwsSdkBedrockRequesterComponent.h"
#include <AICore/SystemRegistrationContext/AICoreSystemRegistrationContext.h>
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

namespace AICore
{
    AwsSdkBedrockRequesterConfiguration::AwsSdkBedrockRequesterConfiguration()
    {
        UpdateInternalConfigurationRegion();
    }

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
                editContext->Class<AwsSdkBedrockRequesterConfiguration>("Aws Sdk Requester configuration", "")
                    ->ClassElement(AZ::Edit::ClassElements::EditorData, "")
                    ->Attribute(AZ::Edit::Attributes::Category, "AICore")
                    ->DataElement(AZ::Edit::UIHandlers::Default, &AwsSdkBedrockRequesterConfiguration::m_regionName, "Region Name", "")
                    ->Attribute(AZ::Edit::Attributes::ChangeNotify, &AwsSdkBedrockRequesterConfiguration::UpdateInternalConfigurationRegion)
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
                editContext->Class<AwsSdkBedrockRequesterComponent>("Aws Sdk Requester controller", "")
                    ->ClassElement(AZ::Edit::ClassElements::EditorData, "")
                    ->Attribute(AZ::Edit::Attributes::Category, "AICore")
                    ->DataElement(
                        AZ::Edit::UIHandlers::Default,
                        &AwsSdkBedrockRequesterComponent::m_configuration,
                        "Configuration",
                        "Configuration for the AWS SDK requester")
                    ->Attribute(AZ::Edit::Attributes::Visibility, AZ::Edit::PropertyVisibility::ShowChildrenOnly)
                    ->Attribute(AZ::Edit::Attributes::ChangeNotify, &AwsSdkBedrockRequesterComponent::OnConfigurationChanged);
            }
        }

        if (auto registrationContext = azrtti_cast<AICoreSystemRegistrationContext*>(context))
        {
            registrationContext->RegisterAICoreRequester<AwsSdkBedrockRequesterComponent>();
        }
    }

    void AwsSdkBedrockRequesterComponent::OnConfigurationChanged()
    {
        m_client = Aws::Bedrock::BedrockClient(m_configuration.m_clientConfiguration);
        m_runtimeClient = Aws::BedrockRuntime::BedrockRuntimeClient(m_configuration.m_runtimeClientConfiguration);
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
        AwsSdkBedrockRequesterBus::Handler::BusConnect(GetEntityId());
    }

    void AwsSdkBedrockRequesterComponent::Deactivate()
    {
        AwsSdkBedrockRequesterBus::Handler::BusDisconnect();
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
        Aws::Utils::Json::JsonValue request,
        AZStd::function<void(AZ::Outcome<AZStd::string, Aws::BedrockRuntime::BedrockRuntimeError>)> callback)
    {
        using namespace Aws::BedrockRuntime;

        Aws::BedrockRuntime::Model::InvokeModelRequest invokeModelRequest;
        invokeModelRequest.SetModelId(m_configuration.m_modelId.c_str());
        std::shared_ptr<Aws::StringStream> ss = std::make_shared<Aws::StringStream>();
        *ss << request.View().WriteCompact();

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
                std::string result;
                outcome.GetResult().GetBody() >> result;
                AZStd::string azStringResult(result.c_str());
                AZ::Outcome<AZStd::string, Aws::BedrockRuntime::BedrockRuntimeError> outcomeResult = AZ::Success(azStringResult);
                callback(outcomeResult);
            }
            else
            {
                Aws::BedrockRuntime::BedrockRuntimeError error = outcome.GetError();
                AZ::Outcome<AZStd::string, Aws::BedrockRuntime::BedrockRuntimeError> outcomeError = AZ::Failure(outcome.GetError());
                callback(outcomeError);
            }
        };

        m_runtimeClient.InvokeModelAsync(invokeModelRequest, handler);
    }
} // namespace AICore
