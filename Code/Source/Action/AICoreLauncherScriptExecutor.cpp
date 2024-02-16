/*
 * Copyright (c) Contributors to the Open 3D Engine Project.
 * For complete copyright and license terms please see the LICENSE at the root of this distribution.
 *
 * SPDX-License-Identifier: Apache-2.0 OR MIT
 *
 */

#include <Action/AICoreLauncherScriptExecutor.h>
#include <AzCore/RTTI/BehaviorContext.h>
#include <AzCore/Script/ScriptContext.h>
#include <AzCore/Serialization/SerializeContext.h>
#include <BehaviorContextUtils/BehaviorContextDump.h>

namespace AICore
{
    void AICoreLauncherScriptExecutor::Reflect(AZ::ReflectContext* context)
    {
        if (auto serializeContext = azrtti_cast<AZ::SerializeContext*>(context))
        {
            serializeContext->Class<AICoreLauncherScriptExecutor>();
        }

        if (auto behaviorContext = azrtti_cast<AZ::BehaviorContext*>(context))
        {
            behaviorContext->Class<AICoreLauncherScriptExecutor>()
                ->Constructor()
                ->Attribute(AZ::Script::Attributes::Storage, AZ::Script::Attributes::StorageType::Value)
                ->Method("StoreStringResult", &AICoreLauncherScriptExecutor::StoreStringResult);
        }
    }

    AICoreLauncherScriptExecutor::AICoreLauncherScriptExecutor()
    {
        // TODO - only Launcher scope buses are callable. This can be regulated through attributes (scope: Launcher).
        // TODO - don't use global context. Instead, have a custom behavior context only with registered methods
        auto behaviorContext = AZ::Interface<AZ::ComponentApplicationRequests>::Get()->GetBehaviorContext();
        AZ_Warning("ScriptCall", behaviorContext, "Failed to get behaviorContext");
        if (!behaviorContext)
        {
            return;
        }

        AZ::ScriptContext sc;
        sc.SetErrorHook( // TODO - this should be communicated back as feedback in some cases.
            [this]([[maybe_unused]] AZ::ScriptContext* context, AZ::ScriptContext::ErrorType error, const char* msg)
            {
                m_response = BehaviorContextDump::ScriptErrorDump(m_currentScript, error, msg).c_str();
            });
        sc.BindTo(behaviorContext);
        sc.Execute("aiexecutor = AICoreLauncherScriptExecutor()"); // TODO - think about how to return results
    }

    bool AICoreLauncherScriptExecutor::ScriptCall(const AZStd::string& script, AZStd::string& response)
    {
        m_response = "";
        sc.Execute(script.c_str());
        response = m_response;
        return response.empty();
    }

    bool AICoreLauncherScriptExecutor::SupportsCurrentScope() const
    {
        // TODO - check if current mode is Editor
        return true;
    }

    void AICoreLauncherScriptExecutor::StoreStringResult(const AZStd::string& result)
    {
        m_response = result;
    }
} // namespace AICore
