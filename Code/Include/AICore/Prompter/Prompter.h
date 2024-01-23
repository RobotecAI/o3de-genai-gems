
#pragma once

#include "AzCore/RTTI/RTTIMacros.h"
#include "AzCore/RTTI/TemplateInfo.h"
#include <AzCore/std/functional.h>
#include <AzCore/std/string/string.h>
#include <AzCore/std/typetraits/is_base_of.h>

#include <AICore/Communication/Requester.h>
#include <AICore/RequestGenerator/RequestGenerator.h>

#include <iostream>

namespace AICore
{
    template<class RequesterType, class GeneratorType>
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

    AZ_TYPE_INFO_TEMPLATE(Prompter, "{cd95b401-5aad-456f-9920-40bc543207b9}", AZ_TYPE_INFO_TYPENAME, AZ_TYPE_INFO_TYPENAME);

} // namespace AICore
