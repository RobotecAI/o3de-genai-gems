/*
 * Copyright (c) Contributors to the Open 3D Engine Project.
 * For complete copyright and license terms please see the LICENSE at the root of this distribution.
 *
 * SPDX-License-Identifier: Apache-2.0 OR MIT
 *
 */

#pragma once

#include <GenAIFramework/Communication/AIServiceProviderBus.h>
#include <GenAIVendorBundle/GenAIVendorBundleTypeIds.h>

#include <AzCore/Component/Component.h>
#include <AzCore/std/string/string.h>
#include <AzFramework/Components/ComponentAdapter.h>

namespace GenAIVendorBundle
{
    class OllamaHttpServiceConfiguration : public AZ::ComponentConfig
    {
    public:
        AZ_RTTI(OllamaHttpServiceConfiguration, OllamaHttpServiceConfigurationTypeId);

        OllamaHttpServiceConfiguration() = default;
        ~OllamaHttpServiceConfiguration() = default;

        static void Reflect(AZ::ReflectContext* context);

        AZStd::string m_url = "http://localhost:11434/api/generate";
        AZStd::string m_contentType = "application/json";
        int m_timeout = 10000;
    };

    class OllamaHttpServiceComponent
        : public AZ::Component
        , public GenAIFramework::AIServiceProviderBus::Handler

    {
    public:
        AZ_COMPONENT(OllamaHttpServiceComponent, OllamaHttpServiceComponentTypeId, AZ::Component)

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
