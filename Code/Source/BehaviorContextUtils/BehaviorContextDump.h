/*
 * Copyright (c) Contributors to the Open 3D Engine Project.
 * For complete copyright and license terms please see the LICENSE at the root of this distribution.
 *
 * SPDX-License-Identifier: Apache-2.0 OR MIT
 *
 */
#pragma once

#include <AzCore/Script/ScriptContext.h>
#include <AzCore/std/containers/set.h>

namespace AZ
{
    class BehaviorContext;
    class BehaviorMethod;
    class BehaviorEBus;
}

namespace AICore
{ // TODO - consider reusing (modifying) NodeListDumpAction. This needs to be AI-readable and fully documented.
    namespace BehaviorContextDump
    {
        AZStd::string ScriptErrorDump(const AZStd::string& script, AZ::ScriptContext::ErrorType error, const char* errorMessage);
        AZStd::string ArgumentsDump(const AZStd::span<AZ::BehaviorArgument>& arguments);
        AZStd::string ArgumentsDump(const AZ::BehaviorMethod* method, AZStd::string& documentation);

        AZStd::string ClassesDump(const AZStd::string& filter, const AZ::BehaviorContext* ctx = nullptr);
        AZStd::string MethodsDump(const AZStd::string& className, const AZStd::string& filter, const AZ::BehaviorContext* ctx = nullptr);
        AZStd::string EbusesDump(const AZStd::string& filter, const AZ::BehaviorContext* ctx = nullptr);

        //! Produce a signature for calling the method including documentation (format: returnType Class.Method(arg1, arg2))
        AZStd::string MethodDump(const AZStd::string& className, const AZStd::string& methodName, const AZ::BehaviorMethod* method);

        //! Produce a signature for calling the ebus event including documentation (format: returnType Ebus.Event(arg1, arg2))
        AZStd::string EbusDump(
            const AZStd::string& ebusName, const AZ::BehaviorEBus* ebus, const AZStd::set<AZStd::string>& exclude = { "ScriptCanvas" });

    } // namespace BehaviorContextDump
} // namespace AICore
