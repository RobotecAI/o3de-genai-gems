/*
 * Copyright (c) Contributors to the Open 3D Engine Project.
 * For complete copyright and license terms please see the LICENSE at the root of this distribution.
 *
 * SPDX-License-Identifier: Apache-2.0 OR MIT
 *
 */

#include <AICore/AIContext.h>
#include <AICore/AICoreActionBus.h>
#include <AICore/AICoreScriptExecutor.h>
#include <AzCore/RTTI/BehaviorContext.h>
#include <AzCore/RTTI/TypeInfo.h>
#include <AzCore/std/containers/set.h>
#include <AzCore/std/string/string.h>

#pragma once

namespace AICore
{
    class AICoreActionHandler
    {
    public:
        AICoreActionHandler(const AIContext& context);

        void RegisterBehaviorMethod(const AZ::BehaviorMethod* method);
        bool ScriptCall(const AZStd::string& script, AZStd::string& response);

    private:
        AIContext m_context;
        AZStd::unique_ptr<AICoreScriptExecutor> m_scriptExecutor;
        AZStd::set<AZStd::string> m_registeredMethods;
    };
} // namespace AICore
