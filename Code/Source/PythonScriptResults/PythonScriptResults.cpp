
#include "PythonScriptResults.h"
#include <API/EditorPythonRunnerRequestsBus.h>
#include <AzCore/std/string/string.h>

namespace AICore
{
    PythonScriptResults::PythonScriptResults()
    {
        if (!m_isConnectedToPythonConsole)
        {
            AzToolsFramework::EditorPythonConsoleNotificationBus::Handler::BusConnect();
            m_isConnectedToPythonConsole = true;
        }
    }

    PythonScriptResults::~PythonScriptResults()
    {
        if (m_isConnectedToPythonConsole)
        {
            AzToolsFramework::EditorPythonConsoleNotificationBus::Handler::BusDisconnect();
            m_isConnectedToPythonConsole = false;
        }
    }

    AZ::Outcome<AZStd::string, AZStd::string> PythonScriptResults::GetVariableAsString(AZStd::string variable)
    {
        m_readMessageAsVariable = true;
        AzToolsFramework::EditorPythonRunnerRequestBus::Broadcast(
            &AzToolsFramework::EditorPythonRunnerRequestBus::Events::ExecuteByString,
            AZStd::string::format("print(str(%s))", variable.c_str()),
            false);
        m_readMessageAsVariable = false;

        AZStd::string result = "";

        for (auto& message : m_argumentMessages)
        {
            result += AZStd::string(message.data());
            result += AZStd::string("\n");
        }
        m_argumentMessages.clear();

        if (m_error_or_exception)
        {
            m_error_or_exception = false;
            return AZ::Failure(result);
        }
        return AZ::Success(result);
    }

    void PythonScriptResults::OnTraceMessage(AZStd::string_view message)
    {
        if (!m_readMessageAsVariable)
        {
            m_traceMessages.push_back(message);
        }
        else
        {
            m_argumentMessages.push_back(message);
        }
    }

    void PythonScriptResults::OnErrorMessage(AZStd::string_view message)
    {
        if (!m_readMessageAsVariable)
        {
            m_errorMessages.push_back(message);
        }
        else
        {
            m_error_or_exception = true;
            m_argumentMessages.push_back(message);
        }
    }

    void PythonScriptResults::OnExceptionMessage(AZStd::string_view message)
    {
        if (!m_readMessageAsVariable)
        {
            m_exceptionMessages.push_back(message);
        }
        else
        {
            m_error_or_exception = true;
            m_argumentMessages.push_back(message);
        }
    }

    unsigned long long PythonScriptResults::GetLinesSize(const AZStd::vector<AZStd::string_view>& lines)
    {
        return lines.size();
    }
    AZStd::string_view PythonScriptResults::GetLastMessage(const AZStd::vector<AZStd::string_view>& lines)
    {
        if (lines.empty())
        {
            return "";
        }
        return lines.back();
    }
    AZStd::string_view PythonScriptResults::GetLastUnreadMessage(AZStd::vector<AZStd::string_view>& lines, unsigned long long& readIndex)
    {
        if (lines.empty())
        {
            return "";
        }
        if (readIndex >= lines.size())
        {
            return "";
        }
        return lines[readIndex++];
    }
    AZStd::string_view PythonScriptResults::GetMessage(const AZStd::vector<AZStd::string_view>& lines, unsigned long long index)
    {
        if (index >= lines.size())
        {
            return "";
        }
        return lines[index];
    }
    const AZStd::vector<AZStd::string_view>& PythonScriptResults::GetMessages(const AZStd::vector<AZStd::string_view>& lines)
    {
        return lines;
    }

    unsigned long long PythonScriptResults::GetTraceLinesSize() const
    {
        return PythonScriptResults::GetLinesSize(m_traceMessages);
    }
    unsigned long long PythonScriptResults::GetErrorLinesSize() const
    {
        return PythonScriptResults::GetLinesSize(m_errorMessages);
    }
    unsigned long long PythonScriptResults::GetExceptionLinesSize() const
    {
        return PythonScriptResults::GetLinesSize(m_exceptionMessages);
    }

    AZStd::string_view PythonScriptResults::GetLastTraceMessage() const
    {
        return PythonScriptResults::GetLastMessage(m_traceMessages);
    }
    AZStd::string_view PythonScriptResults::GetLastErrorMessage() const
    {
        return PythonScriptResults::GetLastMessage(m_errorMessages);
    }
    AZStd::string_view PythonScriptResults::GetLastExceptionMessage() const
    {
        return PythonScriptResults::GetLastMessage(m_exceptionMessages);
    }

    AZStd::string_view PythonScriptResults::GetLastUnreadTraceMessage()
    {
        return PythonScriptResults::GetLastUnreadMessage(m_traceMessages, m_traceMessagesReadIndex);
    }
    AZStd::string_view PythonScriptResults::GetLastUnreadErrorMessage()
    {
        return PythonScriptResults::GetLastUnreadMessage(m_errorMessages, m_errorMessagesReadIndex);
    }
    AZStd::string_view PythonScriptResults::GetLastUnreadExceptionMessage()
    {
        return PythonScriptResults::GetLastUnreadMessage(m_exceptionMessages, m_exceptionMessagesReadIndex);
    }

    AZStd::string_view PythonScriptResults::GetTraceMessage(long long index) const
    {
        return PythonScriptResults::GetMessage(m_traceMessages, index);
    }
    AZStd::string_view PythonScriptResults::GetErrorMessage(long long index) const
    {
        return PythonScriptResults::GetMessage(m_errorMessages, index);
    }
    AZStd::string_view PythonScriptResults::GetExceptionMessage(long long index) const
    {
        return PythonScriptResults::GetMessage(m_exceptionMessages, index);
    }

    const AZStd::vector<AZStd::string_view>& PythonScriptResults::GetTraceMessages() const
    {
        return PythonScriptResults::GetMessages(m_traceMessages);
    }
    const AZStd::vector<AZStd::string_view>& PythonScriptResults::GetErrorMessages() const
    {
        return PythonScriptResults::GetMessages(m_errorMessages);
    }
    const AZStd::vector<AZStd::string_view>& PythonScriptResults::GetExceptionMessages() const
    {
        return PythonScriptResults::GetMessages(m_exceptionMessages);
    }

} // namespace AICore