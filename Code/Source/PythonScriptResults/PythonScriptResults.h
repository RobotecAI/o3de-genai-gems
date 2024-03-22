/*
 * Copyright (c) Contributors to the Open 3D Engine Project.
 * For complete copyright and license terms please see the LICENSE at the root of this distribution.
 *
 * SPDX-License-Identifier: Apache-2.0 OR MIT
 *
 */

#pragma once

#include <AzCore/RTTI/RTTIMacros.h>
#include <AzCore/RTTI/TypeInfo.h>
#include <AzCore/std/containers/vector.h>
#include <AzCore/std/string/string.h>
#include <AzCore/std/string/string_view.h>
#include <AzToolsFramework/API/EditorPythonConsoleBus.h>

namespace GenAIFramework
{
    class PythonScriptResults : public AzToolsFramework::EditorPythonConsoleNotificationBus::Handler
    {
    public:
        AZ_RTTI(PythonScriptResults, "{73c597bf-52e0-4255-ae6d-6a5182dec5f8}");

        PythonScriptResults() = default;
        ~PythonScriptResults() = default;

        AZ::Outcome<AZStd::string, AZStd::string> GetVariableAsString(AZStd::string variable);

    private:
        //////////////////////////////////////////////////////////////////////////
        // AzToolsFramework::EditorPythonConsoleNotificationBus::Handler
        void OnTraceMessage(AZStd::string_view message) override;
        void OnErrorMessage(AZStd::string_view message) override;
        void OnExceptionMessage(AZStd::string_view message) override;
        //////////////////////////////////////////////////////////////////////////

        AZStd::vector<AZStd::string_view> m_pythonMessages;
        bool m_errorOrException = false;

        bool m_isConnectedToPythonConsole = false;
        void ConnectToPythonConsole();
        void DisconnectFromPythonConsole();
    };
} // namespace GenAIFramework
