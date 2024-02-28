
#pragma once

#include <AICore/Communication/RequesterBus.h>
#include <AzCore/Component/Component.h>
#include <AzCore/RTTI/RTTIMacros.h>
#include <AzCore/std/string/string.h>
#include <aws/bedrock-runtime/BedrockRuntimeClient.h>
#include <aws/bedrock-runtime/BedrockRuntimeErrors.h>
#include <aws/bedrock-runtime/BedrockRuntimeServiceClientModel.h>
#include <aws/bedrock/BedrockClient.h>
#include <aws/core/utils/json/JsonSerializer.h>

namespace AICore
{
    using AwsSdkBedrockRequesterBus = RequesterBus<Aws::Utils::Json::JsonValue, AZStd::string, Aws::BedrockRuntime::BedrockRuntimeError>;

    class AwsSdkBedrockRequesterConfiguration : public AZ::ComponentConfig
    {
    public:
        AZ_RTTI(AwsSdkBedrockRequesterConfiguration, "{769166cb-bd18-4fb0-b9ff-93dbaaaf82a8}");
        AZ_CLASS_ALLOCATOR(AwsSdkBedrockRequesterConfiguration, AZ::SystemAllocator);

        AwsSdkBedrockRequesterConfiguration();
        ~AwsSdkBedrockRequesterConfiguration() = default;

        static void Reflect(AZ::ReflectContext* context);

        AZStd::string m_modelId = "anthropic.claude-v2:1";
        AZStd::string m_regionName = "eu-central-1";

        // Internal configuration for the AWS SDK client
        Aws::Bedrock::BedrockClientConfiguration m_clientConfiguration;
        Aws::BedrockRuntime::BedrockRuntimeClientConfiguration m_runtimeClientConfiguration;

    private:
        void UpdateInternalConfigurationRegion()
        {
            m_clientConfiguration.region = m_regionName.c_str();
            m_runtimeClientConfiguration.region = m_regionName.c_str();
        }
    };

    class AwsSdkBedrockRequesterComponent
        : public AZ::Component
        , public AwsSdkBedrockRequesterBus::Handler
    {
    public:
        AZ_COMPONENT(AwsSdkBedrockRequesterComponent, "{77f1b8e1-e616-44c0-a6c6-9d1bd26cf751}");

        AwsSdkBedrockRequesterComponent() = default;
        explicit AwsSdkBedrockRequesterComponent(const AwsSdkBedrockRequesterConfiguration& config);

        static void Reflect(AZ::ReflectContext* context);

        void OnConfigurationChanged();

        //////////////////////////////////////////////////////////////////////////
        // Controller component
        void Init();
        void Activate();
        void Deactivate();
        void SetConfiguration(const AwsSdkBedrockRequesterConfiguration& config);
        const AwsSdkBedrockRequesterConfiguration& GetConfiguration() const;
        //////////////////////////////////////////////////////////////////////////

        //////////////////////////////////////////////////////////////////////////
        // RequesterBus<Aws::Utils::Json::JsonValue> overrides
        void SendRequest(
            Aws::Utils::Json::JsonValue request,
            AZStd::function<void(AZ::Outcome<AZStd::string, Aws::BedrockRuntime::BedrockRuntimeError>)> callback);
        //////////////////////////////////////////////////////////////////////////

        AwsSdkBedrockRequesterConfiguration m_configuration;

        Aws::Bedrock::BedrockClient m_client;
        Aws::BedrockRuntime::BedrockRuntimeClient m_runtimeClient;
    };
} // namespace AICore
