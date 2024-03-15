/*
 * Copyright (c) Contributors to the Open 3D Engine Project.
 * For complete copyright and license terms please see the LICENSE at the root of this distribution.
 *
 * SPDX-License-Identifier: Apache-2.0 OR MIT
 *
 */
#pragma once

#include "AzCore/RTTI/BehaviorContext.h"
#include <AICore/AIContext.h>
#include <AICore/APIFormatterStructs.h>
#include <AzCore/std/containers/set.h>
#include <AzCore/std/containers/vector.h>
#include <AzCore/std/string/string.h>

namespace AZ
{
    class BehaviorContext;
    class BehaviorMethod;
    class BehaviorEBus;
} // namespace AZ

namespace AICore
{ // TODO - consider reusing (modifying) NodeListDumpAction and PythonEditorFunc.cpp GetExposedPythonClasses.
    struct BehaviorContextDump
    {
    public:
        BehaviorContextDump(
            const AIContext& aiContext, const AZStd::string& filter = "", const AZ::BehaviorContext* behaviorContext = nullptr);

        AZStd::string APIDump();
        AZStd::string ClassesDump(bool dumpMethods = true);
        AZStd::string MethodsDump(const AZStd::string& className);
        AZStd::string EbusesDump();

    private:
        AZStd::string DumpFormattedClass(const ClassFormatterHelper& classHelper);
        AZStd::string DumpFormattedMethod(const MethodFormatterHelper& method);
        AZStd::string DumpFormattedEbus(const EbusFormatterHelper& method);

        ClassFormatterHelper ClassDump(const AZStd::string& className, const AZ::BehaviorClass* behaviorClass, bool dumpMethods = true);
        EbusFormatterHelper EbusDump(const AZStd::string& ebusName, const AZ::BehaviorEBus* ebus);
        MethodFormatterHelper MethodDump(
            const AZStd::string& className,
            const AZStd::string& methodName,
            const AZ::BehaviorMethod* method,
            const AZ::BehaviorClass* behaviorClass);
        AZStd::vector<ArgumentFormatterHelper> ArgumentsDump(const AZ::BehaviorMethod* method);

        const AZStd::set<AZStd::string> m_exclude = { "ScriptCanvas" };
        AZStd::string m_filter;
        AIContext m_aiContext;
        const AZ::BehaviorContext* m_context;
    };
} // namespace AICore
