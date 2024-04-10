/*
 * Copyright (c) Contributors to the Open 3D Engine Project.
 * For complete copyright and license terms please see the LICENSE at the root of this distribution.
 *
 * SPDX-License-Identifier: Apache-2.0 OR MIT
 *
 */

#include <Action/GenAIFrameworkLauncherScriptExecutor.h>
#include <AzCore/RTTI/BehaviorContext.h>
#include <AzCore/Script/ScriptContext.h>
#include <AzCore/Serialization/SerializeContext.h>
#include <BehaviorContextUtils/BehaviorContextDump.h>

namespace GenAIFramework
{
    namespace Internal
    {
        AZStd::string ScriptErrorString(const AZStd::string& script, AZ::ScriptContext::ErrorType error, const char* errorMessage)
        {
            auto verboseType = [](AZ::ScriptContext::ErrorType error) -> AZStd::string
            {
                switch (error)
                {
                case AZ::ScriptContext::ErrorType::Log:
                    return "Log";
                case AZ::ScriptContext::ErrorType::Warning:
                    return "Warning";
                case AZ::ScriptContext::ErrorType::Error:
                default:
                    return "Error";
                }
            };
            const AZStd::string result{ AZStd::string::format(
                "Script (%s) call had the following [%s]: %s", script.c_str(), verboseType(error).c_str(), errorMessage) };
            return result;
        }
    } // namespace Internal

    void GenAIFrameworkLauncherScriptExecutor::Reflect(AZ::ReflectContext* context)
    {
        if (auto serializeContext = azrtti_cast<AZ::SerializeContext*>(context))
        {
            serializeContext->Class<GenAIFrameworkLauncherScriptExecutor>();
        }

        if (auto behaviorContext = azrtti_cast<AZ::BehaviorContext*>(context))
        {
            behaviorContext->Class<GenAIFrameworkLauncherScriptExecutor>()
                ->Constructor()
                ->Attribute(AZ::Script::Attributes::Storage, AZ::Script::Attributes::StorageType::Value)
                ->Method("StoreStringResult", &GenAIFrameworkLauncherScriptExecutor::StoreStringResult);
        }
    }

    GenAIFrameworkLauncherScriptExecutor::GenAIFrameworkLauncherScriptExecutor()
    {
        // TODO - only Launcher scope buses are callable. This can be regulated through attributes (scope: Launcher).
        // TODO - don't use global context. Instead, have a custom behavior context only with registered methods
        auto behaviorContext = AZ::Interface<AZ::ComponentApplicationRequests>::Get()->GetBehaviorContext();
        AZ_Warning("ScriptCall", behaviorContext, "Failed to get behaviorContext");
        if (!behaviorContext)
        {
            return;
        }

        m_scriptContext.SetErrorHook( // TODO - this should be communicated back as feedback in some cases.
            [this]([[maybe_unused]] AZ::ScriptContext* context, AZ::ScriptContext::ErrorType error, const char* msg)
            {
                m_response = Internal::ScriptErrorString(m_currentScript, error, msg).c_str();
            });
        m_scriptContext.BindTo(behaviorContext);
        m_scriptContext.Execute("aiexecutor = GenAIFrameworkLauncherScriptExecutor()"); // TODO - think about how to return results
    }

    bool GenAIFrameworkLauncherScriptExecutor::ScriptCall(const AZStd::string& script, AZStd::string& response)
    {
        m_response = "";
        m_scriptContext.Execute(script.c_str());
        response = m_response;
        return response.empty();
    }

    bool GenAIFrameworkLauncherScriptExecutor::SupportsCurrentScope() const
    {
        // TODO - check if current mode is Editor
        return true;
    }

    void GenAIFrameworkLauncherScriptExecutor::StoreStringResult(const AZStd::string& result)
    {
        m_response = result;
    }
} // namespace GenAIFramework
