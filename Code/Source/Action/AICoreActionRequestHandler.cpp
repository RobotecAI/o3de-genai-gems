/*
 * Copyright (c) Contributors to the Open 3D Engine Project.
 * For complete copyright and license terms please see the LICENSE at the root of this distribution.
 *
 * SPDX-License-Identifier: Apache-2.0 OR MIT
 *
 */

#include "AICoreActionRequestHandler.h"
#include <AzCore/std/smart_ptr/make_shared.h>
#include <AzCore/std/utility/pair.h>

namespace AICore
{
    void AICoreActionRequestHandler::RegisterAIContext(const AIContext& context)
    {
        AZ_Warning("AICoreActionRequestHandler", m_handlers.find(context.m_key) == m_handlers.end(), "handler for context exists!");
        m_handlers.emplace(AZStd::make_pair(context.m_key, AZStd::make_shared<AICoreActionHandler>(context)));
    }

    void AICoreActionRequestHandler::UnregisterAIContext(const AZStd::string& key)
    {
        auto it = m_handlers.find(key);
        if (it == m_handlers.end())
        {
            AZ_Warning("AICoreActionRequestHandler", false, "Handler not registered, but unregister ");
            return;
        }
        m_handlers.erase(it);
    }

    void AICoreActionRequestHandler::RegisterBehaviorMethod(const AZ::BehaviorMethod* method, const AIContext& aiContext)
    {
        AZ_Error("AICoreActionRequestHandler", m_handlers.find(aiContext.m_key) != m_handlers.end(), "handler for context not registered!");
        m_handlers[aiContext.m_key]->RegisterBehaviorMethod(method);
    }

    bool AICoreActionRequestHandler::ScriptCall(const AZStd::string& script, AZStd::string& response, const AIContext& aiContext)
    {
        AZ_Error("AICoreActionRequestHandler", m_handlers.find(aiContext.m_key) != m_handlers.end(), "handler for context not registered!");
        return m_handlers[aiContext.m_key]->ScriptCall(script, response);
    }

    void AICoreActionRequestHandler::Connect()
    {
        AICoreActionRequestBus::Handler::BusConnect();
    }

    void AICoreActionRequestHandler::Disconnect()
    {
        AICoreActionRequestBus::Handler::BusDisconnect();
    }
} // namespace AICore
