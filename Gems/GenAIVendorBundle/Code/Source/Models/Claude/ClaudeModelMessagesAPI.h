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
#include <AzCore/std/string/string.h>
#include <AzCore/std/utility/pair.h>
#include <GenAIFramework/Communication/AIModelRequestBus.h>
#include <aws/core/utils/json/JsonSerializer.h>

namespace GenAIVendorBundle
{
    class ClaudeModelMessagesAPI
        : public AZ::Component
        , public GenAIFramework::AIModelRequestBus::Handler
    {
    public:
        AZ_COMPONENT(ClaudeModelMessagesAPI, "{9ffed8c5-a872-4adf-bc80-74ba1fe92e95}");

        static void Reflect(AZ::ReflectContext* context);

        ClaudeModelMessagesAPI() = default;
        explicit ClaudeModelMessagesAPI(const ClaudeModelConfiguration& config);
        ~ClaudeModelMessagesAPI() = default;

        void Activate() override;
        void Deactivate() override;

        // GenAIFramework::ModelConfigurationBus overrides
        GenAIFramework::ModelAPIRequest PrepareRequest(const GenAIFramework::ModelAPIPrompt& prompt) override;
        GenAIFramework::ModelAPIResponse ExtractResult(const GenAIFramework::ModelAPIRequest& modelAPIRequest) override;

    private:
        ClaudeModelConfiguration m_configuration;
    };
} // namespace GenAIVendorBundle
