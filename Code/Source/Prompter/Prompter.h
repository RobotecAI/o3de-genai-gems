
#pragma once

#include "AzCore/std/smart_ptr/shared_ptr.h"
#include "Communication/CommunicationInterface.h"
#include "RequestGenerator/RequestGenerator.h"

namespace AICore
{
    template<typename R, typename P>
    class Prompter
    {
    public:
        Prompter(
            AZStd::shared_ptr<CommunicationInterface<R>> communicationInterface, AZStd::shared_ptr<RequestGenerator<R, P>> requestGenerator)
            : m_communicationInterface(communicationInterface)
            , m_requestGenerator(requestGenerator){};
        ~Prompter() = default;

        void Prompt(const P& prompt, AZStd::function<void(const P&, bool)> callback)
        {
            auto request = m_requestGenerator->EncodePrompt(prompt);
            m_communicationInterface->Request(
                request,
                [this, callback](const AZStd::any& response, bool success)
                {
                    if (success)
                    {
                        auto prompt = m_requestGenerator->DecodePrompt(response);
                        callback(prompt, true);
                    }
                    else
                    {
                        callback(AZStd::any(), false);
                    }
                });
        }

    private:
        AZStd::shared_ptr<CommunicationInterface<R>> m_communicationInterface;
        AZStd::shared_ptr<RequestGenerator<R, P>> m_requestGenerator;
    };
} // namespace AICore