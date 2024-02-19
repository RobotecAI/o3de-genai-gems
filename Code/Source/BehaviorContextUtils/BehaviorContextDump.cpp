/*
 * Copyright (c) Contributors to the Open 3D Engine Project.
 * For complete copyright and license terms please see the LICENSE at the root of this distribution.
 *
 * SPDX-License-Identifier: Apache-2.0 OR MIT
 *
 */

#include "BehaviorContextDump.h"
#include <AzCore/RTTI/BehaviorContext.h>

namespace AICore
{ // TODO - consider reusing (modifying) NodeListDumpAction. This needs to be AI-readable and fully documented.
    namespace BehaviorContextDump
    {
        AZStd::string ScriptErrorDump(const AZStd::string& script, AZ::ScriptContext::ErrorType error, const char* errorMessage)
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
            AZStd::string result = AZStd::string::format(
                "Script (%s) call had the following [%s]: %s", script.c_str(), verboseType(error).c_str(), errorMessage);
            return result;
        }

        AZStd::string ArgumentsDump(const AZStd::span<AZ::BehaviorArgument>& arguments)
        {
            AZStd::string argumentsString("Arguments: ");
            for (const auto& argument : arguments)
            {
                argumentsString +=
                    AZStd::string::format(" [ %s, %s ]", argument.m_name, argument.m_typeId.ToString<AZStd::string>().c_str());
            }
            return argumentsString;
        }

        AZStd::string ArgumentsDump(const AZ::BehaviorMethod* method, AZStd::string& documentation)
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

        AZStd::string ClassesDump(const AZStd::string& filter, const AZ::BehaviorContext* behaviorContext)
        {
            auto* context =
                behaviorContext ? behaviorContext : AZ::Interface<AZ::ComponentApplicationRequests>::Get()->GetBehaviorContext();

            AZStd::string classesDump;
            if (!context)
            {
                return classesDump;
            }

            for (const auto& classIter : context->m_classes)
            {
                const AZStd::string className = classIter.first;
                const AZ::BehaviorClass* behaviorClass = classIter.second;
                if (!behaviorClass || (!filter.empty() && !className.contains(filter)))
                {
                    continue;
                }

                classesDump += "class " + classIter.first + "\n";
            }
            return classesDump;
        }

        // Produce a signature for calling the method including documentation (format: returnType Class.Method(arg1, arg2))
        AZStd::string MethodDump(const AZStd::string& className, const AZStd::string& methodName, const AZ::BehaviorMethod* method)
        {
            AZStd::string methodDump;
            //AZ_Printf("MethodDump", "Dumping method: %s\n", methodName.c_str());
            if (!method || methodName.starts_with("ScriptCanvas"))
            {
                return methodDump;
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
        AZStd::string EbusDump(const AZStd::string& ebusName, const AZ::BehaviorEBus* ebus)
        {
            AZStd::string ebusDump;
            if (!ebus || ebusName.starts_with("ScriptCanvas"))
            {
                return ebusDump;
            }

            //AZ_Printf("EbusDump", "Dumping ebus: %s\n", ebusName.c_str());

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
                                documentation += AZStd::string::format(
                                    "@param %s: %s\n", argName.c_str(), event.m_metadataParameters[i].m_toolTip.c_str());
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

        AZStd::string MethodsDump(const AZStd::string& className, const AZStd::string& filter, const AZ::BehaviorContext* behaviorContext)
        {
            auto* context =
                behaviorContext ? behaviorContext : AZ::Interface<AZ::ComponentApplicationRequests>::Get()->GetBehaviorContext();

            AZStd::string methodsString;
            AZStd::unordered_map<AZStd::string, AZ::BehaviorMethod*> methods;
            if (className.empty())
            { // print only global methods
                methods = context->m_methods;
            }
            else
            {
                auto* reflectedClass = context->FindClassByReflectedName(className);
                if (!reflectedClass)
                {
                    AZ_Printf("MethodsDump", "Invalid class name: %s\n", className.c_str());
                    return methodsString;
                }
                methods = reflectedClass->m_methods;
            }

            for (const auto& [methodName, method] : methods)
            {
                if (!filter.empty() && !methodName.contains(filter))
                {
                    continue;
                }
                methodsString += MethodDump(className, methodName, method);
            }
            return methodsString;
        }

        AZStd::string EbusesDump(const AZStd::string& filter, const AZ::BehaviorContext* behaviorContext)
        {
            auto* context =
                behaviorContext ? behaviorContext : AZ::Interface<AZ::ComponentApplicationRequests>::Get()->GetBehaviorContext();

            AZStd::string ebusesString;
            for (const auto& [ebusName, ebus] : context->m_ebuses)
            {
                if (!filter.empty() && !ebusName.contains(filter))
                {
                    continue;
                }
                ebusesString += EbusDump(ebusName, ebus);
            }
            return ebusesString;
        }
    } // namespace BehaviorContextDump
} // namespace AICore
