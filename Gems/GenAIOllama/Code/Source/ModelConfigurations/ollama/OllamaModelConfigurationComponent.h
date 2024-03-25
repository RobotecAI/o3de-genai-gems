/*
 * Copyright (c) Contributors to the Open 3D Engine Project.
 * For complete copyright and license terms please see the LICENSE at the root of this distribution.
 *
 * SPDX-License-Identifier: Apache-2.0 OR MIT
 *
 */

#pragma once

#include <AzCore/Component/Component.h>
#include <AzFramework/Components/ComponentAdapter.h>
#include <GenAIFramework/Communication/AIModelRequestBus.h>

namespace GenAIOllama
{

    class OllamaModelConfiguration : public AZ::ComponentConfig
    {
    public:
        AZ_RTTI(OllamaModelConfiguration, "{c5fa34ff-2bbb-4df4-9aa2-c27320b57f93}");
        OllamaModelConfiguration() = default;
        ~OllamaModelConfiguration() = default;

        static void Reflect(AZ::ReflectContext* context);

        AZStd::string m_model = "";
        AZStd::string m_format = "";
        AZStd::string m_options = "";
        AZStd::string m_system = "";
        AZStd::string m_template = "";
        bool m_stream = false;
        bool m_raw = false;
        AZStd::string m_keepAlive = "5m";
        bool m_useDefaultFormat = true;
        bool m_useDefaultOptions = true;
        bool m_useDefaultSystem = true;
        bool m_useDefaultTemplate = true;
        bool m_useDefaultKeepAlive = true;
    };

    class OllamaModelConfigurationComponent
        : public AZ::Component
        , public GenAIFramework::AIModelRequestBus::Handler
    {
    public:
        AZ_COMPONENT(OllamaModelConfigurationComponent, "{3782988d-058f-4943-9862-874ebc90a240}", AZ::Component);

        OllamaModelConfigurationComponent() = default;
        OllamaModelConfigurationComponent(const OllamaModelConfiguration& other);
        ~OllamaModelConfigurationComponent() = default;

        static void Reflect(AZ::ReflectContext* context);

        //////////////////////////////////////////////////////////////////////////
        // Component overrides
        void Activate() override;
        void Deactivate() override;
        //////////////////////////////////////////////////////////////////////////

    private:
        //////////////////////////////////////////////////////////////////////////
        // ModelConfigurationBus::Handler
        AZStd::string PrepareRequest(const AZStd::string& prompt) override;
        AZ::Outcome<AZStd::string, AZStd::string> ExtractResult(const GenAIFramework::ModelAPIResponse& modelAPIResponse) override;
        //////////////////////////////////////////////////////////////////////////

        OllamaModelConfiguration m_configuration;
    };
} // namespace GenAIOllama
