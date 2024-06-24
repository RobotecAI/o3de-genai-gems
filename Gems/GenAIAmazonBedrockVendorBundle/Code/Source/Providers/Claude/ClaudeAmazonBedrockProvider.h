/*
 * Copyright (c) Contributors to the Open 3D Engine Project.
 * For complete copyright and license terms please see the LICENSE at the root of this distribution.
 *
 * SPDX-License-Identifier: Apache-2.0 OR MIT
 *
 */

#pragma once

#include <GenAIAmazonBedrockVendorBundle/GenAIAmazonBedrockVendorBundleTypeIds.h>
#include <GenAIFramework/Communication/AIServiceProviderBus.h>

#include <AzCore/Component/Component.h>
#include <AzCore/RTTI/RTTIMacros.h>
#include <AzCore/std/string/string.h>

#include <aws/bedrock-runtime/BedrockRuntimeClient.h>
#include <aws/bedrock-runtime/BedrockRuntimeErrors.h>
#include <aws/bedrock-runtime/BedrockRuntimeServiceClientModel.h>
#include <aws/core/utils/json/JsonSerializer.h>

namespace GenAIAmazonBedrockVendorBundle
{

    class ClaudeAmazonBedrockProviderConfiguration : public AZ::ComponentConfig
    {
    public:
        AZ_RTTI(ClaudeAmazonBedrockProviderConfiguration, ClaudeAmazonBedrockProviderConfigurationTypeId);
        AZ_CLASS_ALLOCATOR(ClaudeAmazonBedrockProviderConfiguration, AZ::SystemAllocator);

        ClaudeAmazonBedrockProviderConfiguration() = default;
        ~ClaudeAmazonBedrockProviderConfiguration() = default;

        static void Reflect(AZ::ReflectContext* context);

        AZStd::string m_modelId = "anthropic.claude-v2:1";
        AZStd::string m_regionName = "eu-central-1";
    };

    class ClaudeAmazonBedrockProvider
        : public AZ::Component
        , private GenAIFramework::AIServiceProviderBus::Handler
    {
    public:
        AZ_COMPONENT(ClaudeAmazonBedrockProvider, ClaudeAmazonBedrockProviderTypeId);

        ClaudeAmazonBedrockProvider() = default;
        explicit ClaudeAmazonBedrockProvider(const ClaudeAmazonBedrockProviderConfiguration& config);

        static void Reflect(AZ::ReflectContext* context);
        void OnConfigurationChanged();

        // Controller component overrides
        void Init() override;
        void Activate() override;
        void Deactivate() override;

        void SetConfiguration(const ClaudeAmazonBedrockProviderConfiguration& config);
        const ClaudeAmazonBedrockProviderConfiguration& GetConfiguration() const;

    private:
        // GenAIFramework::AIServiceProviderBus::Handler overrides
        void SendRequest(const AZStd::string& request, AZStd::function<void(AZ::Outcome<AZStd::string, AZStd::string>)> callback) override;

        // Detects if the AWS credentials are detected
        bool AreAwsCredentialsNotDetected() const;

        // Internal configuration for the AWS SDK client
        AZStd::unique_ptr<Aws::BedrockRuntime::BedrockRuntimeClientConfiguration> m_runtimeClientConfiguration;
        AZStd::unique_ptr<Aws::BedrockRuntime::BedrockRuntimeClient> m_runtimeClient;

        ClaudeAmazonBedrockProviderConfiguration m_configuration;
    };
} // namespace GenAIAmazonBedrockVendorBundle
