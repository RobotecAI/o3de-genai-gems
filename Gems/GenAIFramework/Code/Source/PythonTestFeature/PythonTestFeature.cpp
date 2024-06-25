/*
 * Copyright (c) Contributors to the Open 3D Engine Project.
 * For complete copyright and license terms please see the LICENSE at the root of this distribution.
 *
 * SPDX-License-Identifier: Apache-2.0 OR MIT
 *
 */

#include "PythonTestFeature.h"
#include "GenAIFramework/Feature/PythonFeatureBase.h"
#include <GenAIFramework/Communication/AIModelRequestBus.h>
#include <GenAIFramework/Feature/AIAgentBus.h>
#include <GenAIFramework/Feature/ConversationBus.h>
#include <GenAIFramework/SystemRegistrationContext/SystemRegistrationContext.h>

#include <AzCore/RTTI/BehaviorContext.h>
#include <AzCore/std/containers/vector.h>
#include <iostream>

namespace GenAIFramework
{

    PythonTestFeature::PythonTestFeature(AZ::u64 agentId, AZ::u64 conversationId)
        : PythonFeatureBase(agentId, conversationId)
    {
        std::cout << "PythonTestFeature created" << std::endl;
        std::cout << "Agent ID: " << agentId << std::endl;
        std::cout << "Conversation ID: " << conversationId << std::endl;
    }

    void PythonTestFeature::Reflect(AZ::ReflectContext* context)
    {
        if (auto* serializeContext = azrtti_cast<AZ::SerializeContext*>(context))
        {
            serializeContext->Class<PythonTestFeature, PythonFeatureBase>()->Version(0);
        }

        if (auto registrationContext = azrtti_cast<GenAIFramework::SystemRegistrationContext*>(context))
        {
            registrationContext->RegisterFeature<PythonTestFeature>("O3DE Python test feature");
        }
    }

} // namespace GenAIFramework
