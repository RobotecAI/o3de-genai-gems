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

#include "AzCore/std/smart_ptr/make_shared.h"
#include "Communication/JSONHttp/JSONRequester.h"
#include "Prompter/Prompter.h"
#include "RequestGenerator/ollama/OllamaBasicRequestGenerator.h"

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

        // OllamaBasicRequestGenerator ollamaRequestGenerator;
        // JSONRequester jsonRequester("http://localhost:11434/api/generate");

        auto ollamaRequestGenerator = AZStd::make_shared<OllamaBasicRequestGenerator>();
        AZStd::string url = "http://localhost:11434/api/generate";
        auto jsonRequester = AZStd::make_shared<JSONRequester>(url);

        auto castJsonRequester = AZStd::static_pointer_cast<CommunicationInterface<Aws::Utils::Json::JsonView>>(jsonRequester);
        auto castOllamaRequestGenerator =
            AZStd::static_pointer_cast<RequestGenerator<Aws::Utils::Json::JsonView, AZStd::string>>(ollamaRequestGenerator);

        Prompter prompter(castJsonRequester, castOllamaRequestGenerator);
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
