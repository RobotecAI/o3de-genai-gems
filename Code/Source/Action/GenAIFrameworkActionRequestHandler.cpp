/*
 * Copyright (c) Contributors to the Open 3D Engine Project.
 * For complete copyright and license terms please see the LICENSE at the root of this distribution.
 *
 * SPDX-License-Identifier: Apache-2.0 OR MIT
 *
 */

#include "GenAIFrameworkActionRequestHandler.h"
#include <AzCore/std/smart_ptr/make_shared.h>
#include <AzCore/std/utility/pair.h>

namespace GenAIFramework
{

    void GenAIFrameworkActionRequestHandler::RegisterAIContext(const AIContext& context)
    {
        AZ_Warning("GenAIFrameworkActionRequestHandler", !m_handlers.contains(context.m_key), "handler for context exists!");
        m_handlers.emplace(AZStd::make_pair(context.m_key, AZStd::make_shared<GenAIFrameworkActionHandler>(context)));
    }

    void GenAIFrameworkActionRequestHandler::UnregisterAIContext(const AIContext& context)
    {
        UnregisterAIContext(context.m_key);
    }

    void GenAIFrameworkActionRequestHandler::UnregisterAIContext(const AZStd::string& key)
    {
        auto it = m_handlers.find(key);
        if (it == m_handlers.end())
        {
            AZ_Warning("GenAIFrameworkActionRequestHandler", false, "Handler not registered, but unregister ");
            return;
        }
        m_handlers.erase(it);
    }

    void GenAIFrameworkActionRequestHandler::RegisterBehaviorMethod(const AZ::BehaviorMethod* method, const AIContext& aiContext)
    {
        if (!m_handlers.contains(aiContext.m_key))
        {
            AZ_Error("GenAIFrameworkActionRequestHandler", true, "handler for context not registered!");
            return;
        }
        m_handlers[aiContext.m_key]->RegisterBehaviorMethod(method);
    }

    bool GenAIFrameworkActionRequestHandler::ScriptCall(const AZStd::string& script, AZStd::string& response, const AIContext& aiContext)
    {
        if (!m_handlers.contains(aiContext.m_key))
        {
            AZ_Error("GenAIFrameworkActionRequestHandler", false, "handler for context not registered!");
            return false;
        }
        return m_handlers[aiContext.m_key]->ScriptCall(script, response);
    }

    void GenAIFrameworkActionRequestHandler::Connect()
    {
        GenAIFrameworkActionRequestBus::Handler::BusConnect();
    }

    void GenAIFrameworkActionRequestHandler::Disconnect()
    {
        GenAIFrameworkActionRequestBus::Handler::BusDisconnect();
    }
} // namespace GenAIFramework
