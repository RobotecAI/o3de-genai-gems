/*
 * Copyright (c) Contributors to the Open 3D Engine Project.
 * For complete copyright and license terms please see the LICENSE at the root of this distribution.
 *
 * SPDX-License-Identifier: Apache-2.0 OR MIT
 *
 */

#pragma once

#include "ClaudeModelConfiguration.h"
#include <GenAIFramework/Communication/AIModelRequestBus.h>

#include <AzCore/Component/Component.h>
#include <AzCore/std/string/string.h>

#include <aws/core/utils/json/JsonSerializer.h>

namespace GenAIVendorBundle
{
    class ClaudeModelMessagesAPI
        : public AZ::Component
        , public GenAIFramework::AIModelRequestBus::Handler
    {
    public:
        AZ_COMPONENT(ClaudeModelMessagesAPI, ClaudeModelMessagesAPITypeId);

        static void Reflect(AZ::ReflectContext* context);

        ClaudeModelMessagesAPI() = default;
        explicit ClaudeModelMessagesAPI(const ClaudeModelConfiguration& config);
        ~ClaudeModelMessagesAPI() = default;

        void Activate() override;
        void Deactivate() override;

        // GenAIFramework::ModelConfigurationBus overrides
        GenAIFramework::ModelAPIRequest PrepareRequest(const GenAIFramework::AIMessages& prompt) override;
        GenAIFramework::ModelAPIExtractedResponse ExtractResult(const GenAIFramework::ModelAPIResponse& modelAPIResponse) override;

        AZ::Outcome<void, AZStd::string> SetModelParameter(const AZ::Name& parameterName, const AZStd::string& parameterValue) override;

    private:
        ClaudeModelConfiguration m_configuration;
    };
} // namespace GenAIVendorBundle
