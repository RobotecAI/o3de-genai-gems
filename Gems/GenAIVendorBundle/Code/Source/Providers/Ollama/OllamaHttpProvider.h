/*
 * Copyright (c) Contributors to the Open 3D Engine Project.
 * For complete copyright and license terms please see the LICENSE at the root of this distribution.
 *
 * SPDX-License-Identifier: Apache-2.0 OR MIT
 *
 */

#pragma once

#include <GenAIFramework/Communication/AIServiceProviderBus.h>

#include <AzCore/Component/Component.h>
#include <AzFramework/Components/ComponentAdapter.h>

namespace GenAIVendorBundle
{
    class OllamaHttpServiceConfiguration : public AZ::ComponentConfig
    {
    public:
        AZ_RTTI(OllamaHttpServiceConfiguration, "{18ddba8f-1fe5-4dde-a0f4-90dccbda5f33}");

        OllamaHttpServiceConfiguration() = default;
        ~OllamaHttpServiceConfiguration() = default;

        static void Reflect(AZ::ReflectContext* context);

        AZStd::string m_url = "";
        AZStd::string m_contentType = "application/json";
    };

    class OllamaHttpServiceComponent
        : public AZ::Component
        , public GenAIFramework::AIServiceProviderBus::Handler

    {
    public:
        AZ_COMPONENT(OllamaHttpServiceComponent, "{aad62d35-c628-4141-b759-0d3764013b29}", AZ::Component)

        OllamaHttpServiceComponent() = default;
        OllamaHttpServiceComponent(const OllamaHttpServiceConfiguration& config);
        ~OllamaHttpServiceComponent() = default;

        static void Reflect(AZ::ReflectContext* context);

        //////////////////////////////////////////////////////////////////////////
        // Component overrides
        void Activate() override;
        void Deactivate() override;
        //////////////////////////////////////////////////////////////////////////

    private:
        //////////////////////////////////////////////////////////////////////////
        // AIServiceProviderBus::Handler
        void SendRequest(const AZStd::string& request, AZStd::function<void(AZ::Outcome<AZStd::string, AZStd::string>)> callback) override;
        //////////////////////////////////////////////////////////////////////////

        OllamaHttpServiceConfiguration m_configuration;
    };
} // namespace GenAIVendorBundle
