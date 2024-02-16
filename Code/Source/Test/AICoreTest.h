/*
 * Copyright (c) Contributors to the Open 3D Engine Project.
 * For complete copyright and license terms please see the LICENSE at the root of this distribution.
 *
 * SPDX-License-Identifier: Apache-2.0 OR MIT
 *
 */

#include <AICore/AICoreActionBus.h>
#include <AzCore/RTTI/AttributeReader.h>
#include <AzCore/RTTI/BehaviorContext.h>
#include <AzCore/Script/ScriptContext.h>
#include <AzCore/Serialization/SerializeContext.h>
#include <AzCore/std/containers/set.h>
#include <BehaviorContextUtils/BehaviorContextDump.h>

#pragma once

namespace AICore
{
    class AICoreActionTest
    {
    public:
        void HackEditorBuses(AZ::BehaviorContext* behaviorContext, const AZStd::string& filter)
        {
            AZStd::unordered_set<AZ::Crc32> busesToHack;
            for (auto& busesIter : behaviorContext->m_ebuses)
            {
                auto behaviorEbus = busesIter.second;
                if (!behaviorEbus)
                {
                    continue;
                }

                if (!filter.empty() && !behaviorEbus->m_name.contains(filter))
                {
                    continue;
                }
                // AZ_Printf("HackEditorBuses", "Hacking ebus %s", behaviorEbus->m_name.c_str());

                AZ::Attribute* scopeAttribute = FindAttribute(AZ::Script::Attributes::Scope, behaviorEbus->m_attributes);
                AZ::Script::Attributes::ScopeFlags launcherScope = AZ::Script::Attributes::ScopeFlags::Launcher;
                AZ::Script::Attributes::ScopeFlags currentScope = AZ::Script::Attributes::ScopeFlags::Launcher;
                if (scopeAttribute)
                {
                    AZ::AttributeReader scopeAttributeReader(nullptr, scopeAttribute);
                    scopeAttributeReader.Read<AZ::Script::Attributes::ScopeFlags>(currentScope);
                }

                auto scopeAttributePair = AZStd::find_if(
                    behaviorEbus->m_attributes.begin(),
                    behaviorEbus->m_attributes.end(),
                    [](const AZ::AttributePair& pair)
                    {
                        return pair.first == AZ::Script::Attributes::Scope;
                    });
                if (scopeAttributePair != behaviorEbus->m_attributes.end())
                {
                    if (scopeAttributePair->second)
                    {
                        delete scopeAttributePair->second;
                    }
                    behaviorEbus->m_attributes.erase(scopeAttributePair);

                    /*AZ::Script::Attributes::ScopeFlags newFlags = static_cast<AZ::Script::Attributes::ScopeFlags>(
                        static_cast<AZ::u64>(launcherScope) & static_cast<AZ::u64>(currentScope));*/
                    behaviorEbus->m_attributes.push_back(AZStd::make_pair(
                        AZ::Script::Attributes::Scope,
                        scopeAttributePair->second = aznew AZ::AttributeData<AZ::Script::Attributes::ScopeFlags>(launcherScope)));
                    AZ_Printf("HackEditorBuses", "Hacking bus %s done", behaviorEbus->m_name.c_str());
                }
            }
        }

        bool ScriptCall(const AZStd::string& script, AZStd::string& response)
        {
            /*
            auto behaviorContext = AZ::Interface<AZ::ComponentApplicationRequests>::Get()->GetBehaviorContext();
            AZ_Warning("ScriptCall", behaviorContext, "Failed to get behaviorContext");
            if (!behaviorContext)
            {
                return;
            }
            // TODO - only Launcher scope buses are callable. This can be regulated through attributes.
            HackEditorBuses(behaviorContext, "PrefabPublicRequest");
            */
            if (!m_init)
            {
                m_aiContext.m_key = "Test";
                auto* aicoreinterface = AICoreActionInterface::Get();
                if (!aicoreinterface)
                {
                    AZ_Warning("ScriptCall", false, "No AICore Interface!");
                    return false;
                }
                aicoreinterface->RegisterAIContext(m_aiContext);
                m_init = true;
            }

            return AICoreActionInterface::Get()->ScriptCall(script, response, m_aiContext);
        }

        AZStd::string CallRegisteredMethod(
            const AZ::BehaviorMethod* method,
            AZStd::span<AZ::BehaviorArgument> arguments,
            const AIContext& aiContext,
            AZ::BehaviorArgument* result = nullptr)
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
        bool m_init{ false };
        AIContext m_aiContext;
        AZStd::set<AZStd::string> m_registeredMethods;
    };
} // namespace AICore
