/*
 * Copyright (c) Contributors to the Open 3D Engine Project.
 * For complete copyright and license terms please see the LICENSE at the root of this distribution.
 *
 * SPDX-License-Identifier: Apache-2.0 OR MIT
 *
 */

#pragma once

#include "GenAIFramework/Feature/AIAgentBus.h"
#include <GenAIFramework/Communication/AIModelRequestBus.h>
#include <GenAIFramework/Feature/ConversationBus.h>
#include <GenAIFramework/Feature/FeatureBase.h>
#include <GenAIFramework/GenAIFrameworkTypeIds.h>

#include <AzCore/RTTI/BehaviorContext.h>
#include <AzCore/RTTI/RTTI.h>
#include <AzCore/Serialization/SerializeContext.h>

#include <iostream>

namespace GenAIFramework
{
    class BehaviorConversationBusHandler
        : public ConversationNotificationBus::Handler
        , public AZ::BehaviorEBusHandler
    {
    public:
        AZ_EBUS_BEHAVIOR_BINDER(BehaviorConversationBusHandler, BehaviorConversationBusHandlerTypeId, AZ::SystemAllocator, OnNewMessage);

        void OnNewMessage(const AZStd::string& message) override
        {
            Call(FN_OnNewMessage, message);
        }
    };

    class BehaviourAIAgentNotificationsBusHandler
        : public AIAgentNotificationBus::Handler
        , public AZ::BehaviorEBusHandler
    {
    public:
        AZ_EBUS_BEHAVIOR_BINDER(
            BehaviourAIAgentNotificationsBusHandler,
            BehaviourAIAgentNotificationsBusHandlerTypeId,
            AZ::SystemAllocator,
            OnAIResponse,
            OnAIJsonStringResponse);

        void OnAIResponse(ModelAPIExtractedResponse response) override
        {
            AZStd::string stringResponse = Internal::ModelAPIExtractedResponseToString(response);
            std::cout << "OnAIResponse: " << stringResponse.c_str() << std::endl;
            Call(FN_OnAIResponse, stringResponse);
        }

        void OnAIJsonStringResponse(const AZStd::string& response) override
        {
            Call(FN_OnAIJsonStringResponse, response);
        }
    };

    class PythonFeatureBase : public FeatureBase
    {
    public:
        AZ_RTTI(PythonFeatureBase, PythonFeatureBaseTypeId, FeatureBase);

        PythonFeatureBase() = default;
        PythonFeatureBase(AZ::u64 agentId, AZ::u64 conversationId)
            : FeatureBase(agentId, conversationId)
        {
        }
        virtual ~PythonFeatureBase()
        {
        }

        static void Reflect(AZ::ReflectContext* context)
        {
            if (auto* serializeContext = azrtti_cast<AZ::SerializeContext*>(context))
            {
                serializeContext->Class<PythonFeatureBase>()->Version(0);
            }

            if (auto* behaviorContext = azrtti_cast<AZ::BehaviorContext*>(context))
            {
                behaviorContext->EBus<ConversationNotificationBus>("ConversationNotificationBus")
                    ->Attribute(AZ::Script::Attributes::Category, "AI")
                    ->Attribute(AZ::Script::Attributes::Scope, AZ::Script::Attributes::ScopeFlags::Common)
                    ->Attribute(AZ::Script::Attributes::Module, "ai")
                    ->Event("OnFeatureResponse", &ConversationNotificationBus::Events::OnFeatureResponse)
                    ->Handler<BehaviorConversationBusHandler>()
                    ->Event("OnNewMessage", &ConversationNotificationBus::Events::OnNewMessage);

                behaviorContext->EBus<AIAgentNotificationBus>("AIAgentNotificationBus")
                    ->Attribute(AZ::Script::Attributes::Category, "AI")
                    ->Attribute(AZ::Script::Attributes::Scope, AZ::Script::Attributes::ScopeFlags::Common)
                    ->Attribute(AZ::Script::Attributes::Module, "ai")
                    ->Handler<BehaviourAIAgentNotificationsBusHandler>()
                    ->Event("OnAIResponse", &AIAgentNotificationBus::Events::OnAIJsonStringResponse);

                behaviorContext->EBus<AIAgentRequestBus>("AIAgentRequestBus")
                    ->Attribute(AZ::Script::Attributes::Category, "AI")
                    ->Attribute(AZ::Script::Attributes::Scope, AZ::Script::Attributes::ScopeFlags::Common)
                    ->Attribute(AZ::Script::Attributes::Module, "ai")
                    ->Event("SendPrompt", &AIAgentRequestBus::Events::SendPromptAsJsonString)
                    ->Event("GetHistory", &AIAgentRequestBus::Events::GetHistoryAsJson);

                behaviorContext->Class<PythonFeatureBase>("PythonFeature")
                    ->Attribute(AZ::Script::Attributes::Category, "AI")
                    ->Attribute(AZ::Script::Attributes::Scope, AZ::Script::Attributes::ScopeFlags::Common)
                    ->Attribute(AZ::Script::Attributes::Module, "ai")
                    ->Enum<static_cast<int>(Role::Assistant)>("Role_Assistant")
                    ->Enum<static_cast<int>(Role::User)>("Role_User")
                    ->Enum<static_cast<int>(Role::System)>("Role_System");
            }
        }
    };

} // namespace GenAIFramework
