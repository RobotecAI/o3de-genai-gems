/*
 * Copyright (c) Contributors to the Open 3D Engine Project.
 * For complete copyright and license terms please see the LICENSE at the root of this distribution.
 *
 * SPDX-License-Identifier: Apache-2.0 OR MIT
 *
 */
#pragma once

#include <AzCore/RTTI/ReflectContext.h>
#include <AzCore/Script/ScriptContext.h>
#include <AzCore/std/containers/set.h>

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
        BehaviorContextDump(bool editorOnly, const AZStd::string& filter, const AZ::BehaviorContext* behaviorContext = nullptr);

        AZStd::string ClassesDump();
        AZStd::string MethodsDump(const AZStd::string& className);
        AZStd::string EbusesDump();

    private:
        AZStd::string EbusDump(const AZStd::string& ebusName, const AZ::BehaviorEBus* ebus);
        AZStd::string MethodDump(const AZStd::string& className, const AZStd::string& methodName, const AZ::BehaviorMethod* method);

        AZStd::string ArgumentsDump(const AZStd::span<AZ::BehaviorArgument>& arguments);
        AZStd::string ArgumentsDump(const AZ::BehaviorMethod* method, AZStd::string& documentation);

        const AZStd::set<AZStd::string> m_exclude = { "ScriptCanvas" };
        AZStd::string m_filter;
        bool m_editorOnly;
        const AZ::BehaviorContext* m_context;
    };
} // namespace AICore
