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

namespace GenAIOllama
{
    class BasicHttpServiceConfiguration : public AZ::ComponentConfig
    {
    public:
        AZ_RTTI(BasicHttpServiceConfiguration, "{18ddba8f-1fe5-4dde-a0f4-90dccbda5f33}");

        BasicHttpServiceConfiguration() = default;
        ~BasicHttpServiceConfiguration() = default;

        static void Reflect(AZ::ReflectContext* context);

        AZStd::string m_url = "";
        AZStd::string m_contentType = "application/json";
    };

    class BasicHttpServiceComponent
        : public AZ::Component
        , public GenAIFramework::AIServiceProviderBus::Handler

    {
    public:
        AZ_COMPONENT(BasicHttpServiceComponent, "{aad62d35-c628-4141-b759-0d3764013b29}", AZ::Component)

        BasicHttpServiceComponent() = default;
        BasicHttpServiceComponent(const BasicHttpServiceConfiguration& config);
        ~BasicHttpServiceComponent() = default;

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

        BasicHttpServiceConfiguration m_configuration;
    };
} // namespace GenAIOllama
