/*
 * Copyright (c) Contributors to the Open 3D Engine Project.
 * For complete copyright and license terms please see the LICENSE at the root of this distribution.
 *
 * SPDX-License-Identifier: Apache-2.0 OR MIT
 *
 */

#include "AICoreActionHandler.h"
#include <AICore/AICoreBus.h>
#include <AzCore/RTTI/BehaviorContext.h>

namespace AICore
{
    AICoreActionHandler::AICoreActionHandler(const AIContext& context)
        : m_context(context)
    {
        m_scriptExecutor = AICoreInterface::Get()->MakeScriptExecutor(context);
    }

    void AICoreActionHandler::RegisterBehaviorMethod(const AZ::BehaviorMethod* method)
    {
        AZ_Error("RegisterBehaviorMethod", method, "Null method provided");
        if (m_registeredMethods.find(method->m_name) == m_registeredMethods.end())
        {
            m_registeredMethods.insert(method->m_name);
        }

        // TODO - this is currently not used. Add ebuses, props. Also add check whether the method fits the context!
    }

    bool AICoreActionHandler::ScriptCall(const AZStd::string& script, AZStd::string& response)
    {
        if (!m_scriptExecutor->SupportsCurrentScope())
        {
            // TODO - enrich message
            response = "script called in the wrong scope (Editor, Launcher)";
            return false;
        }

        return m_scriptExecutor->ScriptCall(script, response);
    }
} // namespace AICore
