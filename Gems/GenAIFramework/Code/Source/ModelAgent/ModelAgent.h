/*
 * Copyright (c) Contributors to the Open 3D Engine Project.
 * For complete copyright and license terms please see the LICENSE at the root of this distribution.
 *
 * SPDX-License-Identifier: Apache-2.0 OR MIT
 *
 */

#pragma once

#include <AzCore/Component/EntityId.h>
#include <AzCore/Outcome/Outcome.h>
#include <AzCore/std/any.h>
#include <AzCore/std/containers/vector.h>
#include <GenAIFramework/Communication/AIModelRequestBus.h>
#include <GenAIFramework/GenAIFrameworkModelAgentBus.h>

namespace GenAIFramework
{

    class ModelAgent : public GenAIFrameworkModelAgentBus::Handler
    {
    public:
        ModelAgent(const AZ::EntityId& serviceProviderId, const AZ::EntityId& modelConfigurationId, AZ::u64 agentId);
        ~ModelAgent();

        // GenAIFrameworkModelAgentBus::Handler
        void SendPrompt(const ModelAPIPrompt& prompt) override;
        AIHistory GetHistory() const override;

        void SetServiceProviderId(const AZ::EntityId& serviceProviderId);
        void SetModelConfigurationId(const AZ::EntityId& modelConfigurationId);

    private:
        AIHistory m_history;
        AZ::u64 m_agentId;

        AZ::EntityId m_serviceProviderId;
        AZ::EntityId m_modelConfigurationId;
    };
} // namespace GenAIFramework
