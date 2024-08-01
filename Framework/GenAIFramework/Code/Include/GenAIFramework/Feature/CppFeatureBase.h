/*
 * Copyright (c) Contributors to the Open 3D Engine Project.
 * For complete copyright and license terms please see the LICENSE at the root of this distribution.
 *
 * SPDX-License-Identifier: Apache-2.0 OR MIT
 *
 */

#pragma once

#include <GenAIFramework/Feature/AIAgentBus.h>
#include <GenAIFramework/Feature/ConversationBus.h>
#include <GenAIFramework/Feature/FeatureBase.h>
#include <GenAIFramework/GenAIFrameworkTypeIds.h>

#include <AzCore/RTTI/RTTI.h>
#include <AzCore/Serialization/SerializeContext.h>

namespace GenAIFramework
{
    //! Base class for all Generative AI Features implemented in C++ programming language
    class CppFeatureBase
        : public ConversationNotificationBus::Handler
        , public AIAgentNotificationBus::Handler
        , public FeatureBase
    {
    public:
        AZ_RTTI(CppFeatureBase, CppFeatureBaseTypeId, FeatureBase);

        CppFeatureBase() = default;
        CppFeatureBase(AZ::u64 agentId, AZ::u64 conversationId)
            : FeatureBase(agentId, conversationId)
        {
            ConversationNotificationBus::Handler::BusConnect(conversationId);
            AIAgentNotificationBus::Handler::BusConnect(agentId);
        }

        virtual ~CppFeatureBase()
        {
            AIAgentNotificationBus::Handler::BusDisconnect();
            ConversationNotificationBus::Handler::BusDisconnect();
        }

        static void Reflect(AZ::ReflectContext* context)
        {
            if (auto* serializeContext = azrtti_cast<AZ::SerializeContext*>(context))
            {
                serializeContext->Class<CppFeatureBase>()->Version(0);
            }
        }
    };

} // namespace GenAIFramework
