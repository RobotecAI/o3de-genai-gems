/*
 * Copyright (c) Contributors to the Open 3D Engine Project.
 * For complete copyright and license terms please see the LICENSE at the root of this distribution.
 *
 * SPDX-License-Identifier: Apache-2.0 OR MIT
 *
 */

#pragma once

#include <GenAIFramework/Communication/AIModelRequestBus.h>

#include <AzCore/Component/Component.h>
#include <AzFramework/Components/ComponentAdapter.h>

namespace GenAIVendorBundle
{
    class OllamaChatModelConfiguration : public AZ::ComponentConfig
    {
    public:
        AZ_RTTI(OllamaChatModelConfiguration, "{ce2d1912-94ee-4bcd-9015-e118900f4051}");
        OllamaChatModelConfiguration() = default;
        ~OllamaChatModelConfiguration() = default;

        static void Reflect(AZ::ReflectContext* context);

        AZStd::string m_model = "";
        AZStd::string m_format = "";
        AZStd::string m_options = "";
        bool m_stream = false;
        bool m_raw = false;
        AZStd::string m_keepAlive = "5m";
        bool m_useDefaultFormat = true;
        bool m_useDefaultOptions = true;
        bool m_useDefaultKeepAlive = true;
    };

    class OllamaChatModel
        : public AZ::Component
        , public GenAIFramework::AIModelRequestBus::Handler
    {
    public:
        AZ_COMPONENT(OllamaChatModel, "{e82c0c54-0658-419a-8170-033ef06aea96}", AZ::Component);

        OllamaChatModel() = default;
        OllamaChatModel(const OllamaChatModelConfiguration& other);
        ~OllamaChatModel() = default;

        static void Reflect(AZ::ReflectContext* context);

        //////////////////////////////////////////////////////////////////////////
        // Component overrides
        void Activate() override;
        void Deactivate() override;
        //////////////////////////////////////////////////////////////////////////

    private:
        //////////////////////////////////////////////////////////////////////////
        // ModelConfigurationBus::Handler
        GenAIFramework::ModelAPIRequest PrepareRequest(const GenAIFramework::AIMessages& prompt) override;
        GenAIFramework::ModelAPIExtractedResponse ExtractResult(const GenAIFramework::ModelAPIResponse& modelAPIResponse) override;
        //////////////////////////////////////////////////////////////////////////

        OllamaChatModelConfiguration m_configuration;
    };
} // namespace GenAIVendorBundle
