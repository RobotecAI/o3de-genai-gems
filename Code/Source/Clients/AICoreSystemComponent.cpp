/*
* Copyright (c) Contributors to the Open 3D Engine Project.
* For complete copyright and license terms please see the LICENSE at the root of this distribution.
*
* SPDX-License-Identifier: Apache-2.0 OR MIT
*
*/

#include "AICoreSystemComponent.h"
#include <AICore/AICoreTypeIds.h>
#include <AzCore/Serialization/SerializeContext.h>
#include <AzCore/std/smart_ptr/shared_ptr.h>

#include <AICore/Prompter/BasicOllamaPrompter.h>
#include <iostream>

namespace AICore
{
    AZ_COMPONENT_IMPL(AICoreSystemComponent, "AICoreSystemComponent",
        AICoreSystemComponentTypeId);

    void AICoreSystemComponent::Reflect(AZ::ReflectContext* context)
    {
        if (auto serializeContext = azrtti_cast<AZ::SerializeContext*>(context))
        {
            serializeContext->Class<AICoreSystemComponent, AZ::Component>()
                ->Version(0)
                ;
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

        OllamaBasicRequestConfiguration config = { "mistral" };
        AZStd::string url = "http://localhost:11434/api/generate";

        BasicOllamaPrompter prompter(url, config);

        AZStd::string prompt = "This is a test prompt";

        prompter.Prompt(
            prompt,
            [](AZStd::string response, bool success)
            {
                // if (success)
                // {
                //     std::cout << std::endl << std::endl << "Response: " << response.c_str() << std::endl << std::endl;
                // }
                // else
                // {
                //     std::cout << std::endl << std::endl << "Failed to get response" << std::endl << std::endl;
                // }
            });
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
