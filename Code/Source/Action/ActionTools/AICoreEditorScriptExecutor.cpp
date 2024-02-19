/*
 * Copyright (c) Contributors to the Open 3D Engine Project.
 * For complete copyright and license terms please see the LICENSE at the root of this distribution.
 *
 * SPDX-License-Identifier: Apache-2.0 OR MIT
 *
 */

#include "AICoreEditorScriptExecutor.h"
#include <AzToolsFramework/API/EditorPythonRunnerRequestsBus.h>

namespace AICore
{
    bool AICoreEditorScriptExecutor::ScriptCall(const AZStd::string& script, AZStd::string& response)
    {
        AzToolsFramework::EditorPythonRunnerRequestBus::Broadcast(
            &AzToolsFramework::EditorPythonRunnerRequestBus::Events::ExecuteByString, script, true);
        // TODO - get response (error or human-readable return value).

        auto variableStringOutcome = m_pythonScriptResults.GetVariableAsString("result");
        if (variableStringOutcome.IsSuccess())
        {
            response = variableStringOutcome.GetValue();
            return true;
        }
        else
        {
            response = variableStringOutcome.GetError();
            return false;
        }
    }

    bool AICoreEditorScriptExecutor::SupportsCurrentScope() const
    {
        // TODO - check if current mode is Editor
        return true;
    }
} // namespace AICore
