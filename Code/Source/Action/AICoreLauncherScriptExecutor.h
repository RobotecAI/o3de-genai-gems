/*
 * Copyright (c) Contributors to the Open 3D Engine Project.
 * For complete copyright and license terms please see the LICENSE at the root of this distribution.
 *
 * SPDX-License-Identifier: Apache-2.0 OR MIT
 *
 */

#include <AICore/AICoreScriptExecutor.h>
#include <AzCore/Script/ScriptContext.h>
#include <AzCore/RTTI/BehaviorContext.h>

#pragma once

namespace AICore
{
    class AICoreLauncherScriptExecutor : public AICoreScriptExecutor
    {
    public:
        AZ_RTTI(AICoreLauncherScriptExecutor, "{92FDEE88-91BE-4B23-87BE-BE22ED467A8B}");
        static void Reflect(AZ::ReflectContext* context);

        AICoreLauncherScriptExecutor();
    private:
        bool ScriptCall(const AZStd::string& script, AZStd::string& response) override;
        bool SupportsCurrentScope() const override;

        void StoreStringResult(const AZStd::string& result);

        AZ::ScriptContext sc;
        AZStd::string m_currentScript;
        AZStd::string m_response;
    };
} // namespace AICore
