/*
 * Copyright (c) Contributors to the Open 3D Engine Project.
 * For complete copyright and license terms please see the LICENSE at the root of this distribution.
 *
 * SPDX-License-Identifier: Apache-2.0 OR MIT
 *
 */

#include "GenAIFrameworkActionHandler.h"
#include <AzCore/RTTI/BehaviorContext.h>
#include <GenAIFramework/GenAIFrameworkBus.h>

namespace GenAIFramework
{
    GenAIFrameworkActionHandler::GenAIFrameworkActionHandler(const AIContext& context)
        : m_context(context)
    {
        m_scriptExecutor = GenAIFrameworkInterface::Get()->MakeScriptExecutor(context);
    }

    void GenAIFrameworkActionHandler::RegisterBehaviorMethod(const AZ::BehaviorMethod* method)
    {
        AZ_Error("RegisterBehaviorMethod", method, "Null method provided");
        if (!m_registeredMethods.contains(method->m_name))
        {
            m_registeredMethods.insert(method->m_name);
        }

        // TODO - this is currently not used. Add ebuses, props. Also add check whether the method fits the context!
    }

    bool GenAIFrameworkActionHandler::ScriptCall(const AZStd::string& script, AZStd::string& response)
    {
        if (!m_scriptExecutor->SupportsCurrentScope())
        {
            // TODO - enrich message
            response = "script called in the wrong scope (Editor, Launcher)";
            return false;
        }

        return m_scriptExecutor->ScriptCall(script, response);
    }
} // namespace GenAIFramework
