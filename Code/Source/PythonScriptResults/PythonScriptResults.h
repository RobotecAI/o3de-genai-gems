
#pragma once

#include <AzCore/RTTI/RTTIMacros.h>
#include <AzCore/RTTI/TypeInfo.h>
#include <AzCore/std/containers/vector.h>
#include <AzCore/std/string/string.h>
#include <AzCore/std/string/string_view.h>
#include <AzToolsFramework/API/EditorPythonConsoleBus.h>

namespace AICore
{
    class PythonScriptResults : public AzToolsFramework::EditorPythonConsoleNotificationBus::Handler
    {
    public:
        AZ_RTTI(PythonScriptResults, "{73c597bf-52e0-4255-ae6d-6a5182dec5f8}");

        PythonScriptResults();
        ~PythonScriptResults();

        AZ::Outcome<AZStd::string, AZStd::string> GetVariableAsString(AZStd::string variable);

        unsigned long long GetTraceLinesSize() const;
        unsigned long long GetErrorLinesSize() const;
        unsigned long long GetExceptionLinesSize() const;

        AZStd::string_view GetLastTraceMessage() const;
        AZStd::string_view GetLastErrorMessage() const;
        AZStd::string_view GetLastExceptionMessage() const;

        AZStd::string_view GetLastUnreadTraceMessage();
        AZStd::string_view GetLastUnreadErrorMessage();
        AZStd::string_view GetLastUnreadExceptionMessage();

        AZStd::string_view GetTraceMessage(long long index) const;
        AZStd::string_view GetErrorMessage(long long index) const;
        AZStd::string_view GetExceptionMessage(long long index) const;

        const AZStd::vector<AZStd::string_view>& GetTraceMessages() const;
        const AZStd::vector<AZStd::string_view>& GetErrorMessages() const;
        const AZStd::vector<AZStd::string_view>& GetExceptionMessages() const;

        void ClearTraceMessages();
        void ClearErrorMessages();
        void ClearExceptionMessages();
        void ClearAllMessages();

    private:
        //////////////////////////////////////////////////////////////////////////
        // AzToolsFramework::EditorPythonConsoleNotificationBus::Handler
        void OnTraceMessage(AZStd::string_view message) override;
        void OnErrorMessage(AZStd::string_view message) override;
        void OnExceptionMessage(AZStd::string_view message) override;
        //////////////////////////////////////////////////////////////////////////

        static unsigned long long GetLinesSize(const AZStd::vector<AZStd::string_view>& lines);
        static AZStd::string_view GetLastMessage(const AZStd::vector<AZStd::string_view>& lines);
        static AZStd::string_view GetLastUnreadMessage(AZStd::vector<AZStd::string_view>& lines, unsigned long long& readIndex);
        static AZStd::string_view GetMessage(const AZStd::vector<AZStd::string_view>& lines, unsigned long long index);
        static const AZStd::vector<AZStd::string_view>& GetMessages(const AZStd::vector<AZStd::string_view>& lines);

        AZStd::vector<AZStd::string_view> m_traceMessages;
        unsigned long long m_traceMessagesReadIndex = 0;
        AZStd::vector<AZStd::string_view> m_errorMessages;
        unsigned long long m_errorMessagesReadIndex = 0;
        AZStd::vector<AZStd::string_view> m_exceptionMessages;
        unsigned long long m_exceptionMessagesReadIndex = 0;
        AZStd::vector<AZStd::string_view> m_argumentMessages;

        bool m_readMessageAsVariable = false;
        bool m_error_or_exception = false;

        bool m_isConnectedToPythonConsole = false;
        void ConnectToPythonConsole();
        void DisconnectFromPythonConsole();
    };
} // namespace AICore