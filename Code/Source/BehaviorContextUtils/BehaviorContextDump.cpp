/*
 * Copyright (c) Contributors to the Open 3D Engine Project.
 * For complete copyright and license terms please see the LICENSE at the root of this distribution.
 *
 * SPDX-License-Identifier: Apache-2.0 OR MIT
 *
 */

#include "BehaviorContextDump.h"
#include "GenAIFramework/APIFormatterStructs.h"
#include "PythonFormatter.h"
#include <AzCore/RTTI/AttributeReader.h>
#include <AzCore/RTTI/BehaviorContext.h>
#include <AzFramework/StringFunc/StringFunc.h>

namespace GenAIFramework
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

        // TODO - instead, expose this from TranslationGeneration to utils
        AZStd::string ReadStringAttribute(const AZ::AttributeArray& attributes, const AZ::Crc32& attribute)
        {
            AZStd::string attributeValue = "";
            if (auto attributeItem = azrtti_cast<AZ::AttributeData<AZStd::string>*>(AZ::FindAttribute(attribute, attributes)))
            {
                attributeValue = attributeItem->Get(nullptr);
                return attributeValue;
            }

            if (auto attributeItem = azrtti_cast<AZ::AttributeData<const char*>*>(AZ::FindAttribute(attribute, attributes)))
            {
                attributeValue = attributeItem->Get(nullptr);
                return attributeValue;
            }
            return {};
        }

        AZStd::string DumpLuaClass(const ClassFormatterHelper& classHelper)
        { // TODO implement
            AZStd::string classDump = AZStd::string::format("Class %s dump (not implemented)", classHelper.m_class.c_str());
            return classDump;
        }

        AZStd::string DumpLuaMethod(const MethodFormatterHelper& method)
        { // TODO implement
            AZStd::string methodDump = AZStd::string::format("Method %s dump (not implemented)", method.m_method.c_str());
            return methodDump;
        }

        AZStd::string DumpLuaEbus(const EbusFormatterHelper& ebus)
        { // TODO implement
            AZStd::string ebusDump = AZStd::string::format("Lua Ebus dump (not implemented)");
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
                    argumentFormatter.m_argumentType, "AZStd::basic_string<char, AZStd::char_traits<char>, allocator>", "AZStd::string");
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

            ClassFormatterHelper classFormatter = ClassDump(className, behaviorClass, dumpMethods);
            classesDump += DumpFormattedClass(classFormatter);
        }
        return classesDump;
    }

    ClassFormatterHelper BehaviorContextDump::ClassDump(
        const AZStd::string& className, const AZ::BehaviorClass* behaviorClass, bool dumpMethods)
    {
        ClassFormatterHelper classFormatter;
        if (!behaviorClass)
        {
            return classFormatter;
        }

        classFormatter.m_class = className;
        classFormatter.m_behaviourClass = const_cast<AZ::BehaviorClass*>(behaviorClass);
        classFormatter.m_module = Internal::GetModuleName(behaviorClass->m_attributes);
        classFormatter.m_dumpMethods = dumpMethods;
        return classFormatter;
    }

    MethodFormatterHelper BehaviorContextDump::MethodDump(
        const AZStd::string& className,
        const AZStd::string& methodName,
        const AZ::BehaviorMethod* method,
        const AZ::BehaviorClass* behaviorClass)
    {
        MethodFormatterHelper methodFormatter;

        methodFormatter.m_method = methodName;
        methodFormatter.m_behaviourMethod = const_cast<AZ::BehaviorMethod*>(method);
        methodFormatter.m_behaviourClass = const_cast<AZ::BehaviorClass*>(behaviorClass);

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
        ebusFormatter.m_busName = ebus->m_name;
        ebusFormatter.m_behaviourEBus = const_cast<AZ::BehaviorEBus*>(ebus);

        return ebusFormatter;
    }

    AZStd::string BehaviorContextDump::MethodsDump(const AZStd::string& className)
    {
        AZStd::string methodsString;
        AZStd::unordered_map<AZStd::string, AZ::BehaviorMethod*> methods;
        AZ::BehaviorClass* reflectedClass = nullptr;
        if (className.empty())
        { // print only global methods
            methods = m_context->m_methods;
        }
        else
        {
            reflectedClass = const_cast<AZ::BehaviorClass*>(m_context->FindClassByReflectedName(className));
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

            methodsString += DumpFormattedMethod(MethodDump(className, methodName, method, reflectedClass));
            methodsString += "\n";
        }
        return methodsString;
    }

    AZStd::string BehaviorContextDump::EbusesDump()
    {
        AZStd::string ebusesString;
        for (const auto& [ebusName, ebus] : m_context->m_ebuses)
        {
            if (!ebus)
            {
                continue;
            }

            if (!m_filter.empty() && !ebusName.contains(m_filter))
            {
                continue;
            }

            if (!Internal::MatchesScope(m_aiContext.m_isEditorScope, ebus))
            {
                continue;
            }

            ebusesString += DumpFormattedEbus(EbusDump(ebusName, ebus));
        }
        return ebusesString;
    }

    AZStd::string BehaviorContextDump::DumpFormattedClass(const ClassFormatterHelper& classHelper)
    {
        PythonFormatter pf;
        return m_aiContext.m_isEditorScope ? pf.FormatClass(classHelper) : Internal::DumpLuaClass(classHelper);
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
} // namespace GenAIFramework
