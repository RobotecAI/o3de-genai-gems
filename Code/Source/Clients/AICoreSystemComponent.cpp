/*
* Copyright (c) Contributors to the Open 3D Engine Project.
* For complete copyright and license terms please see the LICENSE at the root of this distribution.
*
* SPDX-License-Identifier: Apache-2.0 OR MIT
*
*/

#include "AICoreSystemComponent.h"
#include "AzCore/std/utility/move.h"
#include "Communication/JSONHttp/BasicJSONRequester.h"
#include "RequestGenerator/ollama/OllamaBasicPromptConfiguration.h"
#include "RequestGenerator/ollama/OllamaBasicRequestGenerator.h"
#include "RequestGenerator/ollama/OllamaContextRequestGenerator.h"

#include <AICore/AICoreTypeIds.h>
#include <AICore/Prompter/Prompter.h>
#include <AzCore/Serialization/SerializeContext.h>

#include <iostream>

namespace AICore
{
    AZ_COMPONENT_IMPL(AICoreSystemComponent, "AICoreSystemComponent",
        AICoreSystemComponentTypeId);

    void AICoreSystemComponent::Reflect(AZ::ReflectContext* context)
    {
        BasicJSONRequester::Reflect(context);
        if (auto serializeContext = azrtti_cast<AZ::SerializeContext*>(context))
        {
            serializeContext->Class<AICoreSystemComponent, AZ::Component>()->Version(0)->Field(
                "Requesters", &AICoreSystemComponent::m_requesters);
        }
    }

    void AICoreSystemComponent::GetProvidedServices(AZ::ComponentDescriptor::DependencyArrayType& provided)
    {
        provided.push_back(AZ_CRC_CE("AICoreService"));
    }

    void AICoreSystemComponent::GetIncompatibleServices(AZ::ComponentDescriptor::DependencyArrayType& incompatible)
    {
        incompatible.push_back(AZ_CRC_CE("AICoreService"));
    }

    void AICoreSystemComponent::GetRequiredServices([[maybe_unused]] AZ::ComponentDescriptor::DependencyArrayType& required)
    {
        required.push_back(AZ_CRC_CE("HTTPRequestorService"));
    }

    void AICoreSystemComponent::GetDependentServices([[maybe_unused]] AZ::ComponentDescriptor::DependencyArrayType& dependent)
    {
    }

    AICoreSystemComponent::AICoreSystemComponent()
    {
        if (AICoreInterface::Get() == nullptr)
        {
            AICoreInterface::Register(this);
        }
    }

    AICoreSystemComponent::~AICoreSystemComponent()
    {
        if (AICoreInterface::Get() == this)
        {
            AICoreInterface::Unregister(this);
        }
    }

    void AICoreSystemComponent::Init()
    {
    }

    void AICoreSystemComponent::Activate()
    {
        AICoreRequestBus::Handler::BusConnect();
        AZ::TickBus::Handler::BusConnect();

        // // OllamaBasicRequestConfiguration config = { "mistral" };
        // // AZStd::string url = "http://localhost:11434/api/generate";

        // // BasicOllamaPrompter prompter(url, config);

        // // AZStd::string prompt = "This is a test prompt";

        // // prompter.Prompt(
        // //     prompt,
        // //     [](AZStd::string response, bool success)
        // //     {
        // //         // if (success)
        // //         // {
        // //         //     std::cout << std::endl << std::endl << "Response: " << response.c_str() << std::endl << std::endl;
        // //         // }
        // //         // else
        // //         // {
        // //         //     std::cout << std::endl << std::endl << "Failed to get response" << std::endl << std::endl;
        // //         // }
        // //     });

        // AZStd::shared_ptr<Requester<AZStd::string>> request =
        // AZStd::make_shared<BasicJSONRequester>("http://localhost:11434/api/generate"); request->SendRequest(
        //     "This is a test prompt",
        //     [](AZStd::string response, bool success)
        //     {
        //         // if (success)
        //         // {
        //         //     std::cout << std::endl << std::endl << "Response: " << response.c_str() << std::endl << std::endl;
        //         // }
        //         // else
        //         // {
        //         //     std::cout << std::endl << std::endl << "Failed to get response" << std::endl << std::endl;
        //         // }
        //     });

        // AZStd::shared_ptr<RequestGenerator<AZStd::string, AZStd::string, AZStd::string>> RequestGenerator =
        //     AZStd::make_shared<RequestGeneratorTest>("config");

        // auto secodnRequestGenerator = RequestGeneratorTest("config");
        // auto secondRequester = BasicJSONRequester("http://localhost:11434/api/generate");

        // std::cout << std::endl << std::endl << "Encoded" << RequestGenerator->EncodePrompt("test").c_str() << std::endl << std::endl;
        // std::cout << std::endl << std::endl << "Decoded" << RequestGenerator->DecodeRequest("test").c_str() << std::endl << std::endl;

        // Prompter<BasicJSONRequester, RequestGeneratorTest> prompter(secondRequester, secodnRequestGenerator);

        // prompter.Prompt(
        //     "{\"model\": \"mistral\", \"prompt\": \"Hello World\", \"stream\": false}",
        //     [](AZStd::string response, bool success)
        //     {
        //         if (success)
        //         {
        //             std::cout << std::endl << std::endl << "Response: " << response.c_str() << std::endl << std::endl;
        //         }
        //         else
        //         {
        //             std::cout << std::endl << std::endl << "Failed to get response" << std::endl << std::endl;
        //         }
        //     });
        // BasicJSONRequester requester("http://localhost:11434/api/generate");
        // OllamaContextRequestGenerator generator(OllamaBasicPromptConfiguration({ "mistral" }));

        // Prompter<BasicJSONRequester, OllamaContextRequestGenerator> prompter(AZStd::move(requester), AZStd::move(generator));

        // AZStd::vector<int> testVector;
        // testVector.push_back(1);

        // prompter.Prompt(
        //     "Hello world",
        //     [prompter](AZStd::string response, AZStd::optional<AZStd::string> errorMsg)
        //     {
        //         if (!errorMsg.has_value())
        //         {
        //             std::cout << std::endl << std::endl << "Response: " << response.c_str() << std::endl << std::endl;
        //         }
        //         else
        //         {
        //             std::cout << std::endl << std::endl << "Failed to get response" << std::endl << std::endl;
        //         }
        //     });

        // std::cout << testVector.size() << std::endl;
        // generator.EncodePrompt("test");
    }

    void AICoreSystemComponent::Deactivate()
    {
        AZ::TickBus::Handler::BusDisconnect();
        AICoreRequestBus::Handler::BusDisconnect();
    }

    void AICoreSystemComponent::OnTick([[maybe_unused]] float deltaTime, [[maybe_unused]] AZ::ScriptTimePoint time)
    {
    }

} // namespace AICore
