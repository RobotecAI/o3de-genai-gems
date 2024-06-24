/*
 * Copyright (c) Contributors to the Open 3D Engine Project.
 * For complete copyright and license terms please see the LICENSE at the root of this distribution.
 *
 * SPDX-License-Identifier: Apache-2.0 OR MIT
 *
 */

#pragma once

#include <GenAIFramework/Communication/AIModelRequestBus.h>
#include <GenAIVendorBundle/GenAIVendorBundleTypeIds.h>

#include <AzCore/Component/Component.h>
#include <AzFramework/Components/ComponentAdapter.h>

namespace GenAIVendorBundle
{
    class OllamaGenerateModelConfiguration : public AZ::ComponentConfig
    {
    public:
        AZ_RTTI(OllamaGenerateModelConfiguration, OllamaGenerateModelConfigurationTypeId);
        OllamaGenerateModelConfiguration() = default;
        ~OllamaGenerateModelConfiguration() = default;

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

    class OllamaGenerateModel
        : public AZ::Component
        , public GenAIFramework::AIModelRequestBus::Handler
    {
    public:
        AZ_COMPONENT(OllamaGenerateModel, OllamaGenerateModelTypeId, AZ::Component);

        OllamaGenerateModel() = default;
        OllamaGenerateModel(const OllamaGenerateModelConfiguration& other);
        ~OllamaGenerateModel() = default;

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

        OllamaGenerateModelConfiguration m_configuration;
    };
} // namespace GenAIVendorBundle
