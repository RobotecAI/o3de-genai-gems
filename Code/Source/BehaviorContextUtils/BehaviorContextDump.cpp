/*
 * Copyright (c) Contributors to the Open 3D Engine Project.
 * For complete copyright and license terms please see the LICENSE at the root of this distribution.
 *
 * SPDX-License-Identifier: Apache-2.0 OR MIT
 *
 */

#include "BehaviorContextDump.h"
#include "PythonFormatter.h"
#include <AzCore/RTTI/AttributeReader.h>
#include <AzCore/RTTI/BehaviorContext.h>
#include <AzFramework/StringFunc/StringFunc.h>

namespace AICore
{ // TODO - consider reusing (modifying) NodeListDumpAction. This needs to be AI-readable and fully documented.
    namespace Internal
    {
        AZ::Script::Attributes::ScopeFlags GetScope(const AZ::AttributeArray& attributes)
        {
            // defaults to Launcher
            AZ::Script::Attributes::ScopeFlags scopeType = AZ::Script::Attributes::ScopeFlags::Launcher;
            AZ::Attribute* scopeAttribute = AZ::FindAttribute(AZ::Script::Attributes::Scope, attributes);
            if (scopeAttribute)
            {
                AZ::AttributeReader scopeAttributeReader(nullptr, scopeAttribute);
                scopeAttributeReader.Read<AZ::Script::Attributes::ScopeFlags>(scopeType);
            }
            return scopeType;
        }

        AZStd::string GetModuleName(const AZ::AttributeArray& attributes)
        { // TODO - this is taken from PythonEditorFuncs, slightly modified.
            AZStd::string fullModuleName = "azlmbr";
            AZStd::string moduleName;
            AZ::Attribute* moduleAttribute = AZ::FindAttribute(AZ::Script::Attributes::Module, attributes);
            if (moduleAttribute)
            {
                AZ::AttributeReader scopeAttributeReader(nullptr, moduleAttribute);
                scopeAttributeReader.Read<AZStd::string>(moduleName);
            }

            if (!moduleName.empty())
            {
                fullModuleName += "." + moduleName;
            }

            return fullModuleName;
        }

        bool MatchesEditorScope(const AZ::AttributeArray& attributes)
        {
            AZ::Script::Attributes::ScopeFlags scopeType = GetScope(attributes);
            return (scopeType == AZ::Script::Attributes::ScopeFlags::Automation || scopeType == AZ::Script::Attributes::ScopeFlags::Common);
        }

        bool MatchesLauncherScope(const AZ::AttributeArray& attributes)
        {
            AZ::Script::Attributes::ScopeFlags scopeType = GetScope(attributes);
            return (scopeType == AZ::Script::Attributes::ScopeFlags::Launcher || scopeType == AZ::Script::Attributes::ScopeFlags::Common);
        }

        bool MatchesScope(bool editorOnly, const AZ::BehaviorClass* behaviorClass)
        {
            if (!behaviorClass)
            {
                AZ_Warning("MatchesScope", false, "Invalid class");
                return false;
            }
            return editorOnly ? MatchesEditorScope(behaviorClass->m_attributes) : MatchesLauncherScope(behaviorClass->m_attributes);
        }

        bool MatchesScope(bool editorOnly, const AZ::BehaviorEBus* behaviorBus)
        {
            if (!behaviorBus)
            {
                AZ_Warning("MatchesScope", false, "Invalid bus");
                return false;
            }
            return editorOnly ? MatchesEditorScope(behaviorBus->m_attributes) : MatchesLauncherScope(behaviorBus->m_attributes);
        }

        AZStd::string DumpLuaMethod(const MethodFormatterHelper& method)
        { // TODO implement
            AZStd::string methodDump = AZStd::string::format("Method %s dump (not implemented)", method.m_method.c_str());
            return methodDump;
        }

        AZStd::string DumpLuaEbus(const EbusFormatterHelper& ebus)
        { // TODO implement
            AZStd::string ebusDump = AZStd::string::format("Ebus %s dump (not implemented)", ebus.m_ebus.c_str());
            return ebusDump;
        }
    } // namespace Internal

    BehaviorContextDump::BehaviorContextDump(
        const AIContext& aiContext, const AZStd::string& filter, const AZ::BehaviorContext* behaviorContext)
        : m_aiContext(aiContext)
        , m_filter(filter)
    {
        m_context = behaviorContext ? behaviorContext : AZ::Interface<AZ::ComponentApplicationRequests>::Get()->GetBehaviorContext();
        AZ_Error("BehaviorContextDump", m_context, "Invalid context");
    }

    AZStd::string BehaviorContextDump::APIDump()
    {
        AZStd::string apiDump = ClassesDump();
        apiDump += EbusesDump();
        return apiDump;
    }

    AZStd::vector<ArgumentFormatterHelper> BehaviorContextDump::ArgumentsDump(const AZ::BehaviorMethod* method)
    {
        AZStd::vector<ArgumentFormatterHelper> argumentsFormatter;
        for (size_t i = 0; i < method->GetNumArguments(); ++i)
        {
            if (const AZ::BehaviorParameter* argument = method->GetArgument(i))
            {
                ArgumentFormatterHelper argumentFormatter;
                argumentFormatter.m_argumentType = argument->m_name;
                AzFramework::StringFunc::Replace(
                    argumentFormatter.m_argumentType,
                    "AZStd::basic_string<char, AZStd::char_traits<char>, allocator>",
                    "AZStd::string");
                const AZStd::string* argName = method->GetArgumentName(i);
                if (argName && !argName->empty())
                {
                    argumentFormatter.m_argumentName = *argName;
                    argumentFormatter.m_documentation = *method->GetArgumentToolTip(i);
                }
                else if (i == 0 && method->IsMember())
                {
                    argumentFormatter.m_argumentName = ArgumentFormatterHelper::m_thisArgument;
                }
                else if (i == 0 && method->HasBusId())
                {
                    argumentFormatter.m_argumentName = ArgumentFormatterHelper::m_ebusIdArgument;
                }
                argumentsFormatter.push_back(argumentFormatter);
            }
        }
        return argumentsFormatter;
    }

    AZStd::string BehaviorContextDump::ClassesDump(bool dumpMethods)
    {
        AZStd::string classesDump;
        for (const auto& classIter : m_context->m_classes)
        {
            const AZStd::string className = classIter.first;
            const AZ::BehaviorClass* behaviorClass = classIter.second;
            if (!behaviorClass || (!m_filter.empty() && !className.contains(m_filter)))
            {
                continue;
            }

            if (!Internal::MatchesScope(m_aiContext.m_isEditorScope, behaviorClass))
            {
                continue;
            }

            if (dumpMethods)
            {
                classesDump += MethodsDump(className);
                if (!classesDump.empty())
                {
                    classesDump = AZStd::string::format(
                        "Module %s Class %s\n%s\n",
                        Internal::GetModuleName(behaviorClass->m_attributes).c_str(),
                        className.c_str(),
                        classesDump.c_str());
                }
            }
        }
        return classesDump;
    }

    MethodFormatterHelper BehaviorContextDump::MethodDump(
        const AZStd::string& className, const AZStd::string& methodName, const AZ::BehaviorMethod* method)
    {
        MethodFormatterHelper methodFormatter;
        if (!method)
        {
            return methodFormatter;
        }
        for (const auto& excluded : m_exclude)
        {
            if (methodName.starts_with(excluded))
            {
                return methodFormatter;
            }
        }

        methodFormatter.m_method = methodName;
        const auto result = method->HasResult() ? method->GetResult() : nullptr;
        if (result)
        {
            methodFormatter.m_result = result->m_name;
        }

        AZStd::string documentation;
        const char* debugDesc = method->m_debugDescription;
        if (debugDesc && strlen(debugDesc) > 0)
        {
            methodFormatter.m_documentation = debugDesc;
        }

        if (!className.empty())
        {
            methodFormatter.m_class = className;
        }

        methodFormatter.m_arguments = ArgumentsDump(method);
        return methodFormatter;
    }

    // Produce a signature for calling the ebus event including documentation (format: returnType Ebus.Event(arg1, arg2))
    EbusFormatterHelper BehaviorContextDump::EbusDump(const AZStd::string& ebusName, const AZ::BehaviorEBus* ebus)
    {
        EbusFormatterHelper ebusFormatter;
        if (!ebus)
        {
            return ebusFormatter;
        }
        for (const auto& excluded : m_exclude)
        {
            if (ebusName.starts_with(excluded))
            {
                return ebusFormatter;
            }
        }

        ebusFormatter.m_module = Internal::GetModuleName(ebus->m_attributes);
        ebusFormatter.m_ebus = ebus->m_name;

        // Dump handlers
        AZ::BehaviorEBusHandler* handler;
        if (ebus->m_createHandler && ebus->m_destroyHandler && ebus->m_createHandler->InvokeResult(handler))
        {
            for (const AZ::BehaviorEBusHandler::BusForwarderEvent& event : handler->GetEvents())
            {
                MethodFormatterHelper eventFormatter;
                eventFormatter.m_method = event.m_name;
                eventFormatter.m_isEbus = true;
                if (event.HasResult())
                {
                    const AZ::BehaviorParameter& resultParam = event.m_parameters[AZ::eBehaviorBusForwarderEventIndices::Result];
                    eventFormatter.m_result = resultParam.m_name; // type
                }
                else
                {
                    eventFormatter.m_result = "void";
                }

                for (size_t i = AZ::eBehaviorBusForwarderEventIndices::ParameterFirst; i < event.m_parameters.size(); ++i)
                {
                    ArgumentFormatterHelper argumentFormatter;
                    const AZ::BehaviorParameter& argParam = event.m_parameters[i];
                    argumentFormatter.m_argumentType = argParam.m_name;
                    AzFramework::StringFunc::Replace(
                        argumentFormatter.m_argumentType,
                        "AZStd::basic_string<char, AZStd::char_traits<char>, allocator>",
                        "AZStd::string");

                    if (event.m_metadataParameters.size() > i)
                    {
                        auto argName = event.m_metadataParameters[i].m_name;
                        if (!argName.empty())
                        {
                            argumentFormatter.m_argumentName = argName;
                            argumentFormatter.m_documentation = event.m_metadataParameters[i].m_toolTip;
                        }
                    }
                    eventFormatter.m_arguments.push_back(argumentFormatter);
                }
            }
            ebus->m_destroyHandler->Invoke(handler);
        }

        // Dump events (as methods)
        for (const auto& eventIter : ebus->m_events)
        {
            const AZ::BehaviorMethod* const method = eventIter.second.m_event ? eventIter.second.m_event : eventIter.second.m_broadcast;
            if (!method)
            {
                continue;
            }
            AZStd::string interfaceName = eventIter.second.m_event ? "Event" : "Broadcast";
            MethodFormatterHelper eventFormatter = MethodDump(interfaceName, eventIter.first, method);
            eventFormatter.m_isEbus = true;
            eventFormatter.m_eventType = interfaceName;
            ebusFormatter.m_events.push_back(eventFormatter);
        }

        return ebusFormatter;
    }

    AZStd::string BehaviorContextDump::MethodsDump(const AZStd::string& className)
    {
        AZStd::string methodsString;
        AZStd::unordered_map<AZStd::string, AZ::BehaviorMethod*> methods;
        if (className.empty())
        { // print only global methods
            methods = m_context->m_methods;
        }
        else
        {
            auto* reflectedClass = m_context->FindClassByReflectedName(className);
            if (!reflectedClass)
            {
                AZ_Printf("MethodsDump", "Invalid class name: %s\n", className.c_str());
                return methodsString;
            }
            methods = reflectedClass->m_methods;
        }

        for (const auto& [methodName, method] : methods)
        {
            if (!m_filter.empty() && !methodName.contains(m_filter))
            {
                continue;
            }

            methodsString += DumpFormattedMethod(MethodDump(className, methodName, method));
            methodsString += "\n";
        }
        return methodsString;
    }

    AZStd::string BehaviorContextDump::EbusesDump()
    {
        AZStd::string ebusesString;
        for (const auto& [ebusName, ebus] : m_context->m_ebuses)
        {
            if (!m_filter.empty() && !ebusName.contains(m_filter))
            {
                continue;
            }

            if (!Internal::MatchesScope(m_aiContext.m_isEditorScope, ebus))
            {
                continue;
            }

            ebusesString += DumpFormattedEbus(EbusDump(ebusName, ebus));
            ebusesString += "\n";
        }
        return ebusesString;
    }

    AZStd::string BehaviorContextDump::DumpFormattedMethod(const MethodFormatterHelper& method)
    {
        PythonFormatter pf;
        return m_aiContext.m_isEditorScope ? pf.FormatMethod(method) : Internal::DumpLuaMethod(method);
    }

    AZStd::string BehaviorContextDump::DumpFormattedEbus(const EbusFormatterHelper& ebus)
    {
        PythonFormatter pf;
        return m_aiContext.m_isEditorScope ? pf.FormatEbus(ebus) : Internal::DumpLuaEbus(ebus);
    }
} // namespace AICore
