/*
 * Copyright (c) Contributors to the Open 3D Engine Project.
 * For complete copyright and license terms please see the LICENSE at the root of this distribution.
 *
 * SPDX-License-Identifier: Apache-2.0 OR MIT
 *
 */

#pragma once

#include <AzCore/Component/Component.h>
#include <AzCore/std/string/string.h>
#include <AzFramework/Components/ComponentAdapter.h>
#include <GenAIFramework/Communication/AIServiceProviderBus.h>

namespace GenAIVendorBundle
{
    class ClaudeHttpProviderConfiguration : public AZ::ComponentConfig
    {
    public:
        AZ_RTTI(ClaudeHttpProviderConfiguration, "{4822d5ec-f30a-4402-9fce-48826b3183bf}");

        ClaudeHttpProviderConfiguration();
        ~ClaudeHttpProviderConfiguration() = default;

        static void Reflect(AZ::ReflectContext* context);

        AZStd::string m_url = "https://api.anthropic.com/v1/messages";
        AZStd::string m_contentType = "application/json";
        AZStd::string m_model = "";
        AZStd::string m_xAPIKey = "";
        int m_timeout = 10000;

    private:
        bool IsKeyInEnvironment();
    };

    class ClaudeHttpProvider
        : public AZ::Component
        , public GenAIFramework::AIServiceProviderBus::Handler

    {
    public:
        AZ_COMPONENT(ClaudeHttpProvider, "{ffa395bc-1d5b-46e3-b557-10ff550cfb34}", AZ::Component)

        ClaudeHttpProvider() = default;
        ClaudeHttpProvider(const ClaudeHttpProviderConfiguration& config);
        ~ClaudeHttpProvider() = default;

        static void Reflect(AZ::ReflectContext* context);

        //////////////////////////////////////////////////////////////////////////
        // Component overrides
        void Activate() override;
        void Deactivate() override;
        //////////////////////////////////////////////////////////////////////////

    private:
        //////////////////////////////////////////////////////////////////////////
        // AIServiceRequesterBus::Handler
        void SendRequest(const AZStd::string& request, AZStd::function<void(GenAIFramework::ModelAPIResponse)> callback) override;
        //////////////////////////////////////////////////////////////////////////

        ClaudeHttpProviderConfiguration m_configuration;
    };
} // namespace GenAIVendorBundle
