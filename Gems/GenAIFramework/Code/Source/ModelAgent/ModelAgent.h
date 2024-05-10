/*
 * Copyright (c) Contributors to the Open 3D Engine Project.
 * For complete copyright and license terms please see the LICENSE at the root of this distribution.
 *
 * SPDX-License-Identifier: Apache-2.0 OR MIT
 *
 */

#pragma once

#include "GenAIFramework/Communication/AIModelRequestBus.h"
#include <AzCore/Component/EntityId.h>
#include <AzCore/Outcome/Outcome.h>
#include <AzCore/std/any.h>
#include <AzCore/std/containers/vector.h>

namespace GenAIFramework
{
    enum class HistoryTag
    {
        Prompt,
        Response
    };

    class ModelAgent
    {
    public:
        ModelAgent() = default;
        ModelAgent(const AZ::EntityId& serviceProviderId, const AZ::EntityId& modelConfigurationId);
        ~ModelAgent() = default;

        void SendPrompt(const ModelAPIPrompt& prompt, AZStd::function<void(ModelAPIExtractedResponse)> callback);

        void SetServiceProviderId(const AZ::EntityId& serviceProviderId);
        void SetModelConfigurationId(const AZ::EntityId& modelConfigurationId);

    private:
        AZStd::vector<AZStd::pair<HistoryTag, AZStd::any>> m_history;

        AZ::EntityId m_serviceProviderId;
        AZ::EntityId m_modelConfigurationId;
    };
} // namespace GenAIFramework
