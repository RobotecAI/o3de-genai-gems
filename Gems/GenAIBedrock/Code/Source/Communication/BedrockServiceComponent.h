/*
 * Copyright (c) Contributors to the Open 3D Engine Project.
 * For complete copyright and license terms please see the LICENSE at the root of this distribution.
 *
 * SPDX-License-Identifier: Apache-2.0 OR MIT
 *
 */

#pragma once

#include <AzCore/Component/Component.h>
#include <AzCore/RTTI/RTTIMacros.h>
#include <AzCore/std/string/string.h>
#include <GenAIFramework/Communication/AIServiceRequesterBus.h>

#include <aws/bedrock-runtime/BedrockRuntimeClient.h>
#include <aws/bedrock-runtime/BedrockRuntimeErrors.h>
#include <aws/bedrock-runtime/BedrockRuntimeServiceClientModel.h>
#include <aws/bedrock/BedrockClient.h>
#include <aws/core/utils/json/JsonSerializer.h>

namespace GenAIBedrock
{
    class BedrockServiceConfiguration : public AZ::ComponentConfig
    {
    public:
        AZ_RTTI(BedrockServiceConfiguration, "{769166cb-bd18-4fb0-b9ff-93dbaaaf82a8}");
        AZ_CLASS_ALLOCATOR(BedrockServiceConfiguration, AZ::SystemAllocator);

        BedrockServiceConfiguration() = default;
        ~BedrockServiceConfiguration() = default;

        static void Reflect(AZ::ReflectContext* context);

        AZStd::string m_modelId = "anthropic.claude-v2:1";
        AZStd::string m_regionName = "eu-central-1";
    };

    class BedrockServiceComponent
        : public AZ::Component
        , private GenAIFramework::AIServiceRequesterBus::Handler
    {
    public:
        AZ_COMPONENT(BedrockServiceComponent, "{77f1b8e1-e616-44c0-a6c6-9d1bd26cf751}");

        BedrockServiceComponent() = default;
        explicit BedrockServiceComponent(const BedrockServiceConfiguration& config);

        static void Reflect(AZ::ReflectContext* context);
        void OnConfigurationChanged();

        // Controller component overrides
        void Init() override;
        void Activate() override;
        void Deactivate() override;

        void SetConfiguration(const BedrockServiceConfiguration& config);
        const BedrockServiceConfiguration& GetConfiguration() const;

    private:
        // GenAIFramework::AIServiceRequesterBus::Handler overrides
        void SendRequest(const AZStd::string& request, AZStd::function<void(AZ::Outcome<AZStd::string, AZStd::string>)> callback) override;

        // Internal configuration for the AWS SDK client
        AZStd::unique_ptr<Aws::Bedrock::BedrockClientConfiguration> m_clientConfiguration;
        AZStd::unique_ptr<Aws::BedrockRuntime::BedrockRuntimeClientConfiguration> m_runtimeClientConfiguration;
        AZStd::unique_ptr<Aws::Bedrock::BedrockClient> m_client;
        AZStd::unique_ptr<Aws::BedrockRuntime::BedrockRuntimeClient> m_runtimeClient;

        BedrockServiceConfiguration m_configuration;
    };
} // namespace GenAIBedrock
