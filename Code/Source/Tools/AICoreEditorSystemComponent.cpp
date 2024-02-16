/*
 * Copyright (c) Contributors to the Open 3D Engine Project.
 * For complete copyright and license terms please see the LICENSE at the root of this distribution.
 *
 * SPDX-License-Identifier: Apache-2.0 OR MIT
 *
 */

#include "AICoreEditorSystemComponent.h"
#include <Action/ActionTools/AICoreEditorScriptExecutor.h>
#include <AzCore/Serialization/SerializeContext.h>

#include <AICore/AICoreTypeIds.h>

namespace AICore
{
    AZ_COMPONENT_IMPL(AICoreEditorSystemComponent, "AICoreEditorSystemComponent", AICoreEditorSystemComponentTypeId, BaseSystemComponent);

    AZStd::unique_ptr<AICoreScriptExecutor> AICoreEditorSystemComponent::MakeScriptExecutor([[maybe_unused]] const AIContext& aiContext)
    {
        return AZStd::make_unique<AICoreEditorScriptExecutor>();
    }

    void AICoreEditorSystemComponent::Reflect(AZ::ReflectContext* context)
    {
        //AICoreEditorScriptExecutor::Reflect(context);
        if (auto serializeContext = azrtti_cast<AZ::SerializeContext*>(context))
        {
            serializeContext->Class<AICoreEditorSystemComponent, AICoreSystemComponent>()->Version(0);
        }
    }

    AICoreEditorSystemComponent::AICoreEditorSystemComponent() = default;

    AICoreEditorSystemComponent::~AICoreEditorSystemComponent() = default;

    void AICoreEditorSystemComponent::GetProvidedServices(AZ::ComponentDescriptor::DependencyArrayType& provided)
    {
        BaseSystemComponent::GetProvidedServices(provided);
        provided.push_back(AZ_CRC_CE("AICoreEditorService"));
    }

    void AICoreEditorSystemComponent::GetIncompatibleServices(AZ::ComponentDescriptor::DependencyArrayType& incompatible)
    {
        BaseSystemComponent::GetIncompatibleServices(incompatible);
        incompatible.push_back(AZ_CRC_CE("AICoreEditorService"));
    }

    void AICoreEditorSystemComponent::GetRequiredServices([[maybe_unused]] AZ::ComponentDescriptor::DependencyArrayType& required)
    {
        BaseSystemComponent::GetRequiredServices(required);
    }

    void AICoreEditorSystemComponent::GetDependentServices([[maybe_unused]] AZ::ComponentDescriptor::DependencyArrayType& dependent)
    {
        BaseSystemComponent::GetDependentServices(dependent);
    }

    void AICoreEditorSystemComponent::Activate()
    {
        AZ_Printf("AICoreEditorSystemComponent", "Activate");
        AICoreSystemComponent::Activate();
        AzToolsFramework::EditorEvents::Bus::Handler::BusConnect();
        m_actionRequestHandler.Connect();
    }

    void AICoreEditorSystemComponent::Deactivate()
    {
        m_actionRequestHandler.Disconnect();
        AzToolsFramework::EditorEvents::Bus::Handler::BusDisconnect();
        AICoreSystemComponent::Deactivate();
    }

    void AICoreEditorSystemComponent::OnStartPlayInEditorBegin()
    {
        AZ_Printf("AICoreEditorSystemComponent", "OnStartPlayInEditorBegin");
    }
    void AICoreEditorSystemComponent::OnStopPlayInEditor()
    {
    }
} // namespace AICore
