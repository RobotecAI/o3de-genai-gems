
#pragma once

#include <AzCore/std/functional.h>
#include <AzCore/std/string/string.h>
#include <AzCore/std/typetraits/is_base_of.h>

#include <AICore/Communication/Requester.h>
#include <AICore/RequestGenerator/RequestGenerator.h>

namespace AICore
{
    template<typename RequesterType, typename GeneratorType>
    class Prompter
    {
    public:
        Prompter(RequesterType requester, GeneratorType generator)
            : m_requester(requester)
            , m_generator(generator)
        {
            static_assert(
                AZStd::is_same_v<typename RequesterType::Request, typename GeneratorType::Request>,
                "Requester and Generator must have the same Request type");
            static_assert(
                AZStd::is_base_of_v<Requester<typename RequesterType::Request>, RequesterType>, "Requester must be derived from Requester");
            static_assert(
                AZStd::is_base_of_v<
                    RequestGenerator<
                        typename GeneratorType::PromptConfiguration,
                        typename GeneratorType::Request,
                        typename GeneratorType::Prompt>,
                    GeneratorType>,
                "Generator must be derived from RequestGenerator");
        };
        ~Prompter() = default;

        void Prompt(
            typename GeneratorType::Prompt prompt,
            AZStd::function<void(typename GeneratorType::Prompt, AZStd::optional<AZStd::string>)> callback)
        {
            typename RequesterType::Request request = m_generator.EncodePrompt(prompt);
            m_requester.SendRequest(
                request,
                [this, callback](typename RequesterType::Request request, AZStd::optional<AZStd::string> errorMsg)
                {
                    if (!errorMsg.has_value())
                    {
                        auto result = m_generator.DecodeRequest(request);
                        callback(result, errorMsg);
                    }
                    else
                    {
                        callback(typename GeneratorType::Prompt(), errorMsg);
                    }
                });
        }

    protected:
        RequesterType m_requester;
        GeneratorType m_generator;
    };

} // namespace AICore
