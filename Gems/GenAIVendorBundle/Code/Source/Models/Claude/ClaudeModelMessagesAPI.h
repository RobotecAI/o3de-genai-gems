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

namespace GenAIClaude
{
    class ClaudeModelMessagesAPI
        : public AZ::Component
        , public GenAIFramework::AIModelRequestBus::Handler
    {
    public:
        AZ_COMPONENT(ClaudeModelMessagesAPI, "{9ffed8c5-a872-4adf-bc80-74ba1fe92e95}");

        static void Reflect(AZ::ReflectContext* context);

        ClaudeModelMessagesAPI() = default;
        explicit ClaudeModelMessagesAPI(const GenAIClaude::ClaudeModelConfiguration& config);
        ~ClaudeModelMessagesAPI() = default;

        void Activate() override;
        void Deactivate() override;

        // GenAIFramework::ModelConfigurationBus overrides
        GenAIFramework::ModelAPIRequest PrepareRequest(const AZStd::string& prompt) override;
        AZ::Outcome<AZStd::string, AZStd::string> ExtractResult(const GenAIFramework::ModelAPIResponse& modelAPIResponse) override;
        void ResetModelHistory() override;
        void EnableModelHistory(bool enableHistory) override;

    private:
        GenAIClaude::ClaudeModelConfiguration m_defaultConfiguration;

        using ClaudeMessage = AZStd::pair<AZStd::string, AZStd::string>;

        bool m_enableHistory = true;
        AZStd::vector<ClaudeMessage> m_history;
    };
} // namespace GenAIClaude
