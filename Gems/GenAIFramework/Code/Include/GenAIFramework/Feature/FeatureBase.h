/*
 * Copyright (c) Contributors to the Open 3D Engine Project.
 * For complete copyright and license terms please see the LICENSE at the root of this distribution.
 *
 * SPDX-License-Identifier: Apache-2.0 OR MIT
 *
 */

#pragma once

#include <AzCore/RTTI/RTTI.h>
#include <AzCore/Serialization/SerializeContext.h>
#include <GenAIFramework/Communication/AIModelAgentBus.h>
#include <GenAIFramework/Feature/ConversationBus.h>

namespace GenAIFramework
{
    class FeatureBase
        : public ConversationNotificationBus::Handler
        , public AIModelAgentNotificationBus::Handler
    {
    public:
        AZ_RTTI(FeatureBase, "{4050040a-31c3-4949-8ce0-52fad7562f4d}");

        FeatureBase() = default;
        FeatureBase(AZ::u64 agentId, AZ::u64 conversationId);
        virtual ~FeatureBase();

        static void Reflect(AZ::ReflectContext* context);

    protected:
        AZ::u64 m_agentId;
        AZ::u64 m_conversationId;
    };

} // namespace GenAIFramework
