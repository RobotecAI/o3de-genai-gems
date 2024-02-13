/*
 * Copyright (c) Contributors to the Open 3D Engine Project.
 * For complete copyright and license terms please see the LICENSE at the root of this distribution.
 *
 * SPDX-License-Identifier: Apache-2.0 OR MIT
 *
 */

#include "AICoreSystemComponent.h"
#include "AzCore/std/utility/move.h"

#include <AICore/AICoreTypeIds.h>
#include <Action/AICoreLauncherScriptExecutor.h>
#include <AzCore/Serialization/SerializeContext.h>

namespace AICore
{
    AZ_COMPONENT_IMPL(AICoreSystemComponent, "AICoreSystemComponent", AICoreSystemComponentTypeId);

    AZStd::unique_ptr<AICoreScriptExecutor> AICoreSystemComponent::MakeScriptExecutor([[maybe_unused]] const AIContext& aiContext)
    {
        return AZStd::make_unique<AICoreLauncherScriptExecutor>();
    }

    void AICoreSystemComponent::Reflect(AZ::ReflectContext* context)
    {
        AICoreLauncherScriptExecutor::Reflect(context);

        if (auto serializeContext = azrtti_cast<AZ::SerializeContext*>(context))
        {
            serializeContext->Class<AICoreSystemComponent, AZ::Component>()->Version(0);
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

        if (AICoreActionInterface::Get() == nullptr)
        {
            AICoreActionInterface::Register(&m_actionRequestHandler);
        }
    }

    AICoreSystemComponent::~AICoreSystemComponent()
    {
        if (AICoreInterface::Get() == this)
        {
            AICoreInterface::Unregister(this);
        }

        if (AICoreActionInterface::Get() == &m_actionRequestHandler)
        {
            AICoreActionInterface::Unregister(&m_actionRequestHandler);
        }
    }

    void AICoreSystemComponent::Init()
    {
    }

    void AICoreSystemComponent::Activate()
    {
        AICoreRequestBus::Handler::BusConnect();
        AZ::TickBus::Handler::BusConnect();
        m_actionRequestHandler.Connect();
    }

    void AICoreSystemComponent::Deactivate()
    {
        m_actionRequestHandler.Disconnect();
        AZ::TickBus::Handler::BusDisconnect();
        AICoreRequestBus::Handler::BusDisconnect();
    }

    void AICoreSystemComponent::OnTick([[maybe_unused]] float deltaTime, [[maybe_unused]] AZ::ScriptTimePoint time)
    {
    }
} // namespace AICore
