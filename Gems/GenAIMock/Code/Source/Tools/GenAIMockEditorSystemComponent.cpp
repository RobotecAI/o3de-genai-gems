/*
 * Copyright (c) Contributors to the Open 3D Engine Project.
 * For complete copyright and license terms please see the LICENSE at the root of this distribution.
 *
 * SPDX-License-Identifier: Apache-2.0 OR MIT
 *
 */

#include "GenAIMockEditorSystemComponent.h"
#include <GenAIMock/GenAIMockTypeIds.h>

#include <AzCore/Serialization/SerializeContext.h>

namespace GenAIMock
{
    AZ_COMPONENT_IMPL(
        GenAIMockEditorSystemComponent, "GenAIMockEditorSystemComponent", GenAIMockEditorSystemComponentTypeId, BaseSystemComponent);

    void GenAIMockEditorSystemComponent::Reflect(AZ::ReflectContext* context)
    {
        if (auto serializeContext = azrtti_cast<AZ::SerializeContext*>(context))
        {
            serializeContext->Class<GenAIMockEditorSystemComponent, GenAIMockSystemComponent>()->Version(0);
        }
    }

    GenAIMockEditorSystemComponent::GenAIMockEditorSystemComponent() = default;

    GenAIMockEditorSystemComponent::~GenAIMockEditorSystemComponent() = default;

    void GenAIMockEditorSystemComponent::GetProvidedServices(AZ::ComponentDescriptor::DependencyArrayType& provided)
    {
        BaseSystemComponent::GetProvidedServices(provided);
        provided.push_back(AZ_CRC_CE("GenAIMockEditorService"));
    }

    void GenAIMockEditorSystemComponent::GetIncompatibleServices(AZ::ComponentDescriptor::DependencyArrayType& incompatible)
    {
        BaseSystemComponent::GetIncompatibleServices(incompatible);
        incompatible.push_back(AZ_CRC_CE("GenAIMockEditorService"));
    }

    void GenAIMockEditorSystemComponent::GetRequiredServices([[maybe_unused]] AZ::ComponentDescriptor::DependencyArrayType& required)
    {
        BaseSystemComponent::GetRequiredServices(required);
    }

    void GenAIMockEditorSystemComponent::GetDependentServices([[maybe_unused]] AZ::ComponentDescriptor::DependencyArrayType& dependent)
    {
        BaseSystemComponent::GetDependentServices(dependent);
    }

    void GenAIMockEditorSystemComponent::Activate()
    {
        GenAIMockSystemComponent::Activate();
        AzToolsFramework::EditorEvents::Bus::Handler::BusConnect();
    }

    void GenAIMockEditorSystemComponent::Deactivate()
    {
        AzToolsFramework::EditorEvents::Bus::Handler::BusDisconnect();
        GenAIMockSystemComponent::Deactivate();
    }

} // namespace GenAIMock
