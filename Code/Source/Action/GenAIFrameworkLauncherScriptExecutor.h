/*
 * Copyright (c) Contributors to the Open 3D Engine Project.
 * For complete copyright and license terms please see the LICENSE at the root of this distribution.
 *
 * SPDX-License-Identifier: Apache-2.0 OR MIT
 *
 */
#pragma once

#include <GenAIFramework/GenAIFrameworkScriptExecutor.h>
#include <AzCore/Script/ScriptContext.h>


namespace GenAIFramework
{
    class GenAIFrameworkLauncherScriptExecutor : public GenAIFrameworkScriptExecutor
    {
    public:
        AZ_RTTI(GenAIFrameworkLauncherScriptExecutor, "{92FDEE88-91BE-4B23-87BE-BE22ED467A8B}");
        static void Reflect(AZ::ReflectContext* context);

        GenAIFrameworkLauncherScriptExecutor();
    private:
        bool ScriptCall(const AZStd::string& script, AZStd::string& response) override;
        bool SupportsCurrentScope() const override;

        void StoreStringResult(const AZStd::string& result);

        AZ::ScriptContext m_scriptContext;
        AZStd::string m_currentScript;
        AZStd::string m_response;
    };
} // namespace GenAIFramework
