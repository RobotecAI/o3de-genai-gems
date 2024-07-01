/*
 * Copyright (c) Contributors to the Open 3D Engine Project.
 * For complete copyright and license terms please see the LICENSE at the root of this distribution.
 *
 * SPDX-License-Identifier: Apache-2.0 OR MIT
 *
 */

#include "GenAIMockVendorEditorSystemComponent.h"
#include <GenAIMockVendor/GenAIMockVendorTypeIds.h>

#include <AzCore/Serialization/SerializeContext.h>

namespace GenAIMockVendor
{
    AZ_COMPONENT_IMPL(
        GenAIMockVendorEditorSystemComponent,
        "GenAIMockVendorEditorSystemComponent",
        GenAIMockVendorEditorSystemComponentTypeId,
        BaseSystemComponent);

    void GenAIMockVendorEditorSystemComponent::Reflect(AZ::ReflectContext* context)
    {
        if (auto serializeContext = azrtti_cast<AZ::SerializeContext*>(context))
        {
            serializeContext->Class<GenAIMockVendorEditorSystemComponent, GenAIMockVendorSystemComponent>()->Version(0);
        }
    }

    GenAIMockVendorEditorSystemComponent::GenAIMockVendorEditorSystemComponent() = default;

    GenAIMockVendorEditorSystemComponent::~GenAIMockVendorEditorSystemComponent() = default;

    void GenAIMockVendorEditorSystemComponent::GetProvidedServices(AZ::ComponentDescriptor::DependencyArrayType& provided)
    {
        BaseSystemComponent::GetProvidedServices(provided);
        provided.push_back(AZ_CRC_CE("GenAIMockVendorEditorService"));
    }

    void GenAIMockVendorEditorSystemComponent::GetIncompatibleServices(AZ::ComponentDescriptor::DependencyArrayType& incompatible)
    {
        BaseSystemComponent::GetIncompatibleServices(incompatible);
        incompatible.push_back(AZ_CRC_CE("GenAIMockVendorEditorService"));
    }

    void GenAIMockVendorEditorSystemComponent::GetRequiredServices([[maybe_unused]] AZ::ComponentDescriptor::DependencyArrayType& required)
    {
        BaseSystemComponent::GetRequiredServices(required);
    }

    void GenAIMockVendorEditorSystemComponent::GetDependentServices(
        [[maybe_unused]] AZ::ComponentDescriptor::DependencyArrayType& dependent)
    {
        BaseSystemComponent::GetDependentServices(dependent);
    }

    void GenAIMockVendorEditorSystemComponent::Activate()
    {
        GenAIMockVendorSystemComponent::Activate();
        AzToolsFramework::EditorEvents::Bus::Handler::BusConnect();
    }

    void GenAIMockVendorEditorSystemComponent::Deactivate()
    {
        AzToolsFramework::EditorEvents::Bus::Handler::BusDisconnect();
        GenAIMockVendorSystemComponent::Deactivate();
    }

} // namespace GenAIMockVendor
