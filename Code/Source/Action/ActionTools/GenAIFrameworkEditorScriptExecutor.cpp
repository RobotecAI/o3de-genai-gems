/*
 * Copyright (c) Contributors to the Open 3D Engine Project.
 * For complete copyright and license terms please see the LICENSE at the root of this distribution.
 *
 * SPDX-License-Identifier: Apache-2.0 OR MIT
 *
 */

#include "GenAIFrameworkEditorScriptExecutor.h"
#include <AzToolsFramework/API/EditorPythonRunnerRequestsBus.h>

namespace GenAIFramework
{
    bool GenAIFrameworkEditorScriptExecutor::ScriptCall(const AZStd::string& script, AZStd::string& response)
    {
        AzToolsFramework::EditorPythonRunnerRequestBus::Broadcast(
            &AzToolsFramework::EditorPythonRunnerRequestBus::Events::ExecuteByString, script, true);
        // TODO - get response (error or human-readable return value).
        response = "OK";
        return true;
    }

    bool GenAIFrameworkEditorScriptExecutor::SupportsCurrentScope() const
    {
        // TODO - check if current mode is Editor
        return true;
    }
} // namespace GenAIFramework
