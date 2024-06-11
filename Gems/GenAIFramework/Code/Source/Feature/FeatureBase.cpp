/*
 * Copyright (c) Contributors to the Open 3D Engine Project.
 * For complete copyright and license terms please see the LICENSE at the root of this distribution.
 *
 * SPDX-License-Identifier: Apache-2.0 OR MIT
 *
 */

#include <GenAIFramework/Feature/FeatureBase.h>

#include <iostream>

namespace GenAIFramework
{

    FeatureBase::FeatureBase(AZ::u64 agentId, AZ::u64 conversationId)
        : m_agentId(agentId)
        , m_conversationId(conversationId)
    {
        ConversationNotificationBus::Handler::BusConnect(conversationId);
        AIModelAgentNotificationBus::Handler::BusConnect(agentId);

        std::cout << "FeatureBase constructor" << std::endl;
        std::cout << "agentId: " << agentId << std::endl;
        std::cout << "conversationId: " << conversationId << std::endl;
    }

    FeatureBase::~FeatureBase()
    {
        AIModelAgentNotificationBus::Handler::BusDisconnect();
        ConversationNotificationBus::Handler::BusDisconnect();

        std::cout << "FeatureBase destructor" << std::endl;
    }

    void FeatureBase::Reflect(AZ::ReflectContext* context)
    {
        if (auto* serializeContext = azrtti_cast<AZ::SerializeContext*>(context))
        {
            serializeContext->Class<FeatureBase>()->Version(0);
        }
    }

} // namespace GenAIFramework
