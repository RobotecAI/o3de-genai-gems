/*
 * Copyright (c) Contributors to the Open 3D Engine Project.
 * For complete copyright and license terms please see the LICENSE at the root of this distribution.
 *
 * SPDX-License-Identifier: Apache-2.0 OR MIT
 *
 */

#include <AICore/AICoreActionBus.h>
#include <AzCore/Script/ScriptContext.h>
#include <AzCore/Serialization/SerializeContext.h>
#include <AzCore/std/containers/set.h>
#include <BehaviorContextUtils/BehaviorContextDump.h>

#pragma once

namespace AICore
{
    class AICoreActionTest : public AICoreActionRequestBus::Handler
    {
    public:
        AZ_RTTI(AICoreActionTest, "{1F4A2598-DF69-4CE1-A766-8C94988D27A1}");

        static void Reflect(AZ::ReflectContext* context)
        {
            if (auto serializeContext = azrtti_cast<AZ::SerializeContext*>(context))
            {
                serializeContext->Class<AICoreActionTest>();
            }

            if (auto behaviorContext = azrtti_cast<AZ::BehaviorContext*>(context))
            {
                behaviorContext->Class<AICoreActionTest>()
                    ->Constructor()
                    ->Attribute(AZ::Script::Attributes::Storage, AZ::Script::Attributes::StorageType::Value)
                    ->Method("StaticPrintResult", &AICoreActionTest::StaticPrintResult)
                    ->Method("PrintResult", &AICoreActionTest::PrintResult);
            }
        }

        // TODO - this is temp, replace with proper non-static object / context handling
        static void StaticPrintResult(const AZStd::string& result)
        {
            AZ_Printf("PrintResult", "Returned result is %s", result.c_str());
        }

        void PrintResult(const AZStd::string& result)
        {
            StaticPrintResult(result);
        }

        void RegisterBehaviorMethod(const AZ::BehaviorMethod* method, const AIContext& aiContext) override
        {
            AZ_Error("RegisterBehaviorMethod", method, "Null method provided");
            if (m_registeredMethods.find(method->m_name) == m_registeredMethods.end())
            {
                m_registeredMethods.insert(method->m_name);
            }
        }

        void ScriptCall(const AZStd::string& script)
        {
            // TODO - only Launcher scope buses are callable. This can be regulated through attributes.

            auto behaviorContext = AZ::Interface<AZ::ComponentApplicationRequests>::Get()->GetBehaviorContext();
            AZ_Warning("ScriptCall", behaviorContext, "Failed to get behaviorContext");
            if (!behaviorContext)
            {
                return;
            }
            // TODO - don't use global context. Instead, have a custom behavior context only with registered methods
            // TODO - instead, persist the context
            AZ::ScriptContext sc;
            sc.BindTo(behaviorContext);
            sc.SetErrorHook( // TODO - this should be communicated back as feedback in some cases.
                [script]([[maybe_unused]] AZ::ScriptContext* context, AZ::ScriptContext::ErrorType error, const char* msg)
                {
                    AZ_Printf("ScriptCall", "%s:", BehaviorContextDump::ScriptErrorDump(script, error, msg).c_str());
                });
            sc.Execute(AZStd::string::format(
                           "%s\n%s\n%s", "aicore = AICoreActionTest()", script.c_str(), "aicore.PrintResult(aicore, tostring(result))")
                           .c_str());
        }

        AZStd::string CallRegisteredMethod(
            const AZ::BehaviorMethod* method,
            AZStd::span<AZ::BehaviorArgument> arguments,
            const AIContext& aiContext,
            AZ::BehaviorArgument* result = nullptr) override
        {
            if (!method)
            {
                return "Null method provided to call";
            }

            if (m_registeredMethods.find(method->m_name) == m_registeredMethods.end())
            {
                return AZStd::string::format("Method %s not registered, cannot be called", method->m_name.c_str());
            }

            auto resultOutcome = method->IsCallable(arguments);
            if (!resultOutcome.IsSuccess())
            {
                return AZStd::string::format(
                    "Method %s is not callable with provided %s. Reason: %s",
                    method->m_name.c_str(),
                    BehaviorContextDump::ArgumentsDump(arguments).c_str(),
                    resultOutcome.GetError().c_str());
            }

            bool success = method->Call(arguments, result);
            if (!success)
            {
                return AZStd::string::format("Method %s call was unsuccessful", method->m_name.c_str());
            }
            AZ_Printf("CallMethod", "Calling  %s successful%s", method->m_name.c_str(), result ? ", returned a result." : "");
            AZ_Printf("CallMethod", "Result is %s", result->m_value ? "ok" : "empty");

            return "Success"; // TODO - serialize / change return type.
        }

        const AZ::BehaviorMethod* GetMethod(
            const AZStd::string& reflectedName, const AZStd::string& className, const AZ::BehaviorContext* behaviorContext)
        {
            if (className.empty())
            {
                return behaviorContext->FindMethodByReflectedName(reflectedName);
            }

            auto* reflectedClass = behaviorContext->FindClassByReflectedName(className);
            if (reflectedClass)
            {
                return reflectedClass->FindMethodByReflectedName(reflectedName);
            }

            return nullptr;
        }

        void SetMethodArguments(const AZ::BehaviorMethod* method, AZStd::span<AZ::BehaviorArgument>& arguments)
        {
            AZ_Assert(method || method->GetNumArguments() == arguments.size(), "Invalid method or number of arguments!");
            for (int i = 0; i < arguments.size(); ++i)
            {
                const AZ::BehaviorParameter* parameter = method->GetArgument(i);
                arguments[i].Set(*parameter);
                AZ_Printf("GetMethodArguments", "Argument type: %s", parameter->m_name);
            }
        }

        void CallCreateLevelNoPrompt(const AZ::BehaviorContext* behaviorContext)
        {
            const AZ::BehaviorMethod* method = behaviorContext->FindMethodByReflectedName("create_level_no_prompt");
            AZ_Warning("CallCreateLevelNoPrompt", method, "Failed to get test method");
            if (!method)
            {
                return;
            }
            RegisterBehaviorMethod(method, m_aiContext);

            size_t numArguments = method->GetNumArguments();
            AZ::BehaviorArgument arguments[numArguments];
            auto argSpan = AZStd::span<AZ::BehaviorArgument>(&arguments[0], numArguments);
            SetMethodArguments(method, argSpan);

            // TODO - generalize setting of values
            if (argSpan.size() != 5)
            {
                AZ_Warning("Test", false, "Unexpected number of arguments: %zu", argSpan.size());
                return;
            }
            const char* levelName = "NewTestLevel";
            const int heightmapResolution = 128;
            const int heightmapPixelsPerMeter = 1;
            const int terrainTextureResolution = 512;
            const bool useTerrain = false;
            argSpan[0].Set(&levelName);
            argSpan[1].Set(&heightmapResolution);
            argSpan[2].Set(&heightmapPixelsPerMeter);
            argSpan[3].Set(&terrainTextureResolution);
            argSpan[4].Set(&useTerrain);

            AZ_Printf("CallCreateLevelNoPrompt", BehaviorContextDump::ArgumentsDump(argSpan).c_str());
            auto stringResult = CallRegisteredMethod(method, argSpan, m_aiContext);
            AZ_Printf("CallCreateLevelNoPrompt", stringResult.c_str());
        }

        void CallIsSimulation(const AZ::BehaviorContext* behaviorContext)
        {
            const AZ::BehaviorMethod* method = behaviorContext->FindMethodByReflectedName("is_in_simulation_mode");
            AZ_Warning("CallIsSimulation", method, "Failed to get test method");
            if (!method)
            {
                return;
            }
            RegisterBehaviorMethod(method, m_aiContext);

            size_t numArguments = method->GetNumArguments();
            AZ::BehaviorArgument arguments[numArguments];
            auto argSpan = AZStd::span<AZ::BehaviorArgument>(&arguments[0], numArguments);
            SetMethodArguments(method, argSpan);

            if (argSpan.size() != 0)
            {
                AZ_Warning("CallIsSimulation", false, "Unexpected number of arguments: %zu", argSpan.size());
                return;
            }

            AZ_Printf("CallIsSimulation", BehaviorContextDump::ArgumentsDump(argSpan).c_str());

            if (method->HasResult())
            {
                AZ_Printf("CallIsSimulation", "Method has a result");
            }

            bool is_in_sim = false;
            AZ::BehaviorArgument result(&is_in_sim);
            auto stringResult = CallRegisteredMethod(method, argSpan, m_aiContext, &result);

            AZ_Printf("CallIsSimulation", "%s, %s", stringResult.c_str(), is_in_sim ? "in sim" : "not in sim");
            if (!result.m_value)
            {
                AZ_Printf("CallIsSimulation", "No result value");
                return;
            }

            bool* boolResult = reinterpret_cast<bool*>(result.m_value);
            if (!boolResult)
            {
                AZ_Printf("CallIsSimulation", "Not a bool result");
                return;
            }
            AZ_Printf("CallIsSimulation", "Is in simulation mode: %s", *boolResult ? "true" : "false");
        }

        void Test()
        {
            auto behaviorContext = AZ::Interface<AZ::ComponentApplicationRequests>::Get()->GetBehaviorContext();
            AZ_Warning("Test", behaviorContext, "Failed to get behaviorContext");
            if (!behaviorContext)
            {
                return;
            }

            CallIsSimulation(behaviorContext);
        }

    private:
        AIContext m_aiContext;
        AZStd::set<AZStd::string> m_registeredMethods;
    };
} // namespace AICore
