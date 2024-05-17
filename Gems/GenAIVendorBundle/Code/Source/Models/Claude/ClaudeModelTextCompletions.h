/*
 * Copyright (c) Contributors to the Open 3D Engine Project.
 * For complete copyright and license terms please see the LICENSE at the root of this distribution.
 *
 * SPDX-License-Identifier: Apache-2.0 OR MIT
 *
 */

#pragma once

#include "ClaudeModelConfiguration.h"
#include <AzCore/Component/Component.h>
#include <GenAIFramework/Communication/AIModelRequestBus.h>
#include <aws/core/utils/json/JsonSerializer.h>

namespace GenAIVendorBundle
{
    class ClaudeModelTextCompletions
        : public AZ::Component
        , public GenAIFramework::AIModelRequestBus::Handler
    {
    public:
        AZ_COMPONENT(ClaudeModelTextCompletions, "{0310426d-3035-4240-b9e5-d1556af98b47}");

        static void Reflect(AZ::ReflectContext* context);

        ClaudeModelTextCompletions() = default;
        explicit ClaudeModelTextCompletions(const ClaudeModelConfiguration& config);
        ~ClaudeModelTextCompletions() = default;

        void Activate() override;
        void Deactivate() override;

        // GenAIFramework::ModelConfigurationBus overrides
        GenAIFramework::ModelAPIRequest PrepareRequest(const GenAIFramework::ModelAPIPrompt& prompt) override;
        GenAIFramework::ModelAPIExtractedResponse ExtractResult(const GenAIFramework::ModelAPIResponse& modelAPIResponse) override;

    private:
        ClaudeModelConfiguration m_configuration;

        AZ::Outcome<void, AZStd::string> SetModelParameter(const AZ::Name& parameterName, const AZStd::string& parameterValue) override;
    };
} // namespace GenAIVendorBundle
