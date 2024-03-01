
#include "PythonScriptResults.h"
#include <API/EditorPythonRunnerRequestsBus.h>
#include <AzCore/std/string/string.h>

namespace AICore
{
    void PythonScriptResults::ConnectToPythonConsole()
    {
        if (!m_isConnectedToPythonConsole)
        {
            AzToolsFramework::EditorPythonConsoleNotificationBus::Handler::BusConnect();
            m_isConnectedToPythonConsole = true;
        }
    }

    void PythonScriptResults::DisconnectFromPythonConsole()
    {
        if (m_isConnectedToPythonConsole)
        {
            AzToolsFramework::EditorPythonConsoleNotificationBus::Handler::BusDisconnect();
            m_isConnectedToPythonConsole = false;
        }
    }

    AZ::Outcome<AZStd::string, AZStd::string> PythonScriptResults::GetVariableAsString(AZStd::string variable)
    {
        ConnectToPythonConsole();

        AzToolsFramework::EditorPythonRunnerRequestBus::Broadcast(
            &AzToolsFramework::EditorPythonRunnerRequestBus::Events::ExecuteByString,
            AZStd::string::format("print(str(%s))", variable.c_str()),
            false);

        DisconnectFromPythonConsole();

        AZStd::string result = "";

        for (auto& message : m_pythonMessages)
        {
            result += AZStd::string(message.data());
            result += AZStd::string("\n");
        }
        m_pythonMessages.clear();

        if (m_errorOrException)
        {
            m_errorOrException = false;
            return AZ::Failure(result);
        }
        return AZ::Success(result);
    }

    void PythonScriptResults::OnTraceMessage(AZStd::string_view message)
    {
        m_pythonMessages.push_back(message);
    }

    void PythonScriptResults::OnErrorMessage(AZStd::string_view message)
    {
        m_errorOrException = true;
        m_pythonMessages.push_back(message);
    }

    void PythonScriptResults::OnExceptionMessage(AZStd::string_view message)
    {
        OnErrorMessage(message);
    }
} // namespace AICore
