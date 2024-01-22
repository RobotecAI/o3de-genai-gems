/*
* Copyright (c) Contributors to the Open 3D Engine Project.
* For complete copyright and license terms please see the LICENSE at the root of this distribution.
*
* SPDX-License-Identifier: Apache-2.0 OR MIT
*
*/

#include <AzCore/Serialization/SerializeContext.h>
#include "AICoreEditorSystemComponent.h"

#include <AICore/AICoreTypeIds.h>

namespace AICore
{
    AZ_COMPONENT_IMPL(AICoreEditorSystemComponent, "AICoreEditorSystemComponent",
        AICoreEditorSystemComponentTypeId, BaseSystemComponent);

    void AICoreEditorSystemComponent::Reflect(AZ::ReflectContext* context)
    {
        if (auto serializeContext = azrtti_cast<AZ::SerializeContext*>(context))
        {
            serializeContext->Class<AICoreEditorSystemComponent, AICoreSystemComponent>()
                ->Version(0);
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
        AICoreSystemComponent::Activate();
        AzToolsFramework::EditorEvents::Bus::Handler::BusConnect();
    }

    void AICoreEditorSystemComponent::Deactivate()
    {
        AzToolsFramework::EditorEvents::Bus::Handler::BusDisconnect();
        AICoreSystemComponent::Deactivate();
    }

} // namespace AICore
