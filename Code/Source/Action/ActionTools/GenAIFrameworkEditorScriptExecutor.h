/*
 * Copyright (c) Contributors to the Open 3D Engine Project.
 * For complete copyright and license terms please see the LICENSE at the root of this distribution.
 *
 * SPDX-License-Identifier: Apache-2.0 OR MIT
 *
 */
#pragma once

#include <GenAIFramework/GenAIFrameworkScriptExecutor.h>

namespace GenAIFramework
{
    class GenAIFrameworkEditorScriptExecutor : public GenAIFrameworkScriptExecutor
    {
    private:
        bool ScriptCall(const AZStd::string& script, AZStd::string& response) override;
        bool SupportsCurrentScope() const override;
    };
} // namespace GenAIFramework
