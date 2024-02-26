/*
 * Copyright (c) Contributors to the Open 3D Engine Project.
 * For complete copyright and license terms please see the LICENSE at the root of this distribution.
 *
 * SPDX-License-Identifier: Apache-2.0 OR MIT
 *
 */

#include "BehaviorContextDump.h"
#include <AzCore/RTTI/AttributeReader.h>
#include <AzCore/RTTI/BehaviorContext.h>

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
    } // namespace Internal

    BehaviorContextDump::BehaviorContextDump(bool editorOnly, const AZStd::string& filter, const AZ::BehaviorContext* behaviorContext)
        : m_editorOnly(editorOnly)
        , m_filter(filter)
    {
        m_context = behaviorContext ? behaviorContext : AZ::Interface<AZ::ComponentApplicationRequests>::Get()->GetBehaviorContext();
        AZ_Error("BehaviorContextDump", m_context, "Invalid context");
    }

    AZStd::string BehaviorContextDump::ArgumentsDump(const AZStd::span<AZ::BehaviorArgument>& arguments)
    {
        AZStd::string argumentsString("Arguments: ");
        for (const auto& argument : arguments)
        {
            argumentsString += AZStd::string::format(" [ %s, %s ]", argument.m_name, argument.m_typeId.ToString<AZStd::string>().c_str());
        }
        return argumentsString;
    }

    AZStd::string BehaviorContextDump::ArgumentsDump(const AZ::BehaviorMethod* method, AZStd::string& documentation)
    {
        AZStd::string argumentsDump("(");
        for (size_t i = 0; i < method->GetNumArguments(); ++i)
        {
            if (i > 0)
            {
                argumentsDump += ", ";
            }

            if (const AZ::BehaviorParameter* argument = method->GetArgument(i))
            {
                AZStd::string argType = argument->m_name;
                argumentsDump += argType;
                const AZStd::string* argName = method->GetArgumentName(i);
                if (argName && !argName->empty())
                {
                    argumentsDump += " " + *argName;
                    const AZStd::string* argToolTip = method->GetArgumentToolTip(i);
                    documentation += AZStd::string::format("@param %s: %s\n", argName->c_str(), argToolTip->c_str());
                }
                else if (i == 0 && method->IsMember())
                {
                    argumentsDump += "_this";
                    documentation += "@param _this: a pointer to this class object\n";
                }
                else if (i == 0 && method->HasBusId())
                {
                    argumentsDump += " _EBusID";
                    documentation += "@param _EBusID: ID of the bus to call\n";
                }
            }
        }
        argumentsDump += ")";
        return argumentsDump;
    }

    AZStd::string BehaviorContextDump::ClassesDump()
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

            if (!Internal::MatchesScope(m_editorOnly, behaviorClass))
            {
                continue;
            }

            classesDump += "class " + classIter.first + "\n";
        }
        return classesDump;
    }

    // Produce a signature for calling the method including documentation (format: returnType Class.Method(arg1, arg2))
    AZStd::string BehaviorContextDump::MethodDump(
        const AZStd::string& className, const AZStd::string& methodName, const AZ::BehaviorMethod* method)
    {
        AZStd::string methodDump;
        if (!method)
        {
            return methodDump;
        }
        for (const auto& excluded : m_exclude)
        {
            if (methodName.starts_with(excluded))
            {
                return methodDump;
            }
        }

        const auto result = method->HasResult() ? method->GetResult() : nullptr;
        if (result)
        {
            methodDump += AZStd::string::format("%s ", result->m_name);
        }
        else
        {
            methodDump += "void ";
        }

        AZStd::string documentation;
        const char* debugDesc = method->m_debugDescription;
        if (debugDesc && strlen(debugDesc) > 0)
        {
            documentation += AZStd::string::format("%s\n", debugDesc);
        }

        if (!className.empty())
        {
            methodDump += className + ".";
        }

        methodDump += methodName;
        methodDump += ArgumentsDump(method, documentation);
        methodDump += "\n\n";
        return documentation + methodDump;
    }

    // Produce a signature for calling the ebus event including documentation (format: returnType Ebus.Event(arg1, arg2))
    AZStd::string BehaviorContextDump::EbusDump(const AZStd::string& ebusName, const AZ::BehaviorEBus* ebus)
    {
        AZStd::string ebusDump;
        if (!ebus)
        {
            return ebusDump;
        }
        for (const auto& excluded : m_exclude)
        {
            if (ebusName.starts_with(excluded))
            {
                return ebusDump;
            }
        }

        // Dump handlers
        AZ::BehaviorEBusHandler* handler;
        if (ebus->m_createHandler && ebus->m_destroyHandler && ebus->m_createHandler->InvokeResult(handler))
        {
            for (const AZ::BehaviorEBusHandler::BusForwarderEvent& event : handler->GetEvents())
            {
                if (event.HasResult())
                {
                    const AZ::BehaviorParameter& resultParam = event.m_parameters[AZ::eBehaviorBusForwarderEventIndices::Result];
                    ebusDump += resultParam.m_name; // type
                }
                else
                {
                    ebusDump += "void";
                }
                ebusDump += AZStd::string::format(" %s.%s(", ebus->m_name.c_str(), event.m_name);

                for (size_t i = AZ::eBehaviorBusForwarderEventIndices::ParameterFirst; i < event.m_parameters.size(); ++i)
                {
                    if (i > AZ::eBehaviorBusForwarderEventIndices::ParameterFirst)
                    {
                        ebusDump += ", ";
                    }

                    const AZ::BehaviorParameter& argParam = event.m_parameters[i];
                    AZStd::string argType = argParam.m_name;
                    ebusDump += argType;

                    AZStd::string documentation;
                    if (event.m_metadataParameters.size() > i)
                    {
                        auto argName = event.m_metadataParameters[i].m_name;
                        if (!argName.empty())
                        {
                            ebusDump += " " + argName;
                            documentation +=
                                AZStd::string::format("@param %s: %s\n", argName.c_str(), event.m_metadataParameters[i].m_toolTip.c_str());
                        }
                    }
                    ebusDump = documentation + ebusDump;
                }
                ebusDump += ")\n";
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
            auto interfaceName = AZStd::string::format("%s.%s", ebusName.c_str(), eventIter.second.m_event ? "Event" : "Broadcast");
            ebusDump += MethodDump(interfaceName, eventIter.first, method);
        }

        return ebusDump;
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

            methodsString += MethodDump(className, methodName, method);
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

            if (!Internal::MatchesScope(m_editorOnly, ebus))
            {
                continue;
            }

            ebusesString += EbusDump(ebusName, ebus);
        }
        return ebusesString;
    }
} // namespace AICore
