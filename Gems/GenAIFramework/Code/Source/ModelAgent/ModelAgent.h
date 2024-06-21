/*
 * Copyright (c) Contributors to the Open 3D Engine Project.
 * For complete copyright and license terms please see the LICENSE at the root of this distribution.
 *
 * SPDX-License-Identifier: Apache-2.0 OR MIT
 *
 */

#pragma once

#include <GenAIFramework/Feature/AIAgentBus.h>

#include <AzCore/Component/EntityId.h>
#include <AzCore/Outcome/Outcome.h>
#include <AzCore/std/any.h>
#include <AzCore/std/containers/vector.h>

namespace GenAIFramework
{
    class ModelAgent : public AIAgentRequestBus::Handler
    {
    public:
        ModelAgent() = delete; // Do not allow to construct the agent without ids
        ModelAgent(const AZ::EntityId& serviceProviderId, const AZ::EntityId& modelConfigurationId, AZ::u64 agentId);
        ~ModelAgent();

        // AIAgentRequestBus::Handler
        void SendPrompt(const AIMessages& prompt) override;
        AIHistory GetHistory() const override;

        void SetServiceProviderId(const AZ::EntityId& serviceProviderId);
        void SetModelConfigurationId(const AZ::EntityId& modelConfigurationId);

    private:
        AIHistory m_history;
        AIMessage m_systemMessage = { Role::System, "" };
        AZ::u64 m_agentId;

        AZ::EntityId m_serviceProviderId;
        AZ::EntityId m_modelConfigurationId;
    };
} // namespace GenAIFramework
