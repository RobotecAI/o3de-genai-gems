/*
 * Copyright (c) Contributors to the Open 3D Engine Project.
 * For complete copyright and license terms please see the LICENSE at the root of this distribution.
 *
 * SPDX-License-Identifier: Apache-2.0 OR MIT
 *
 */

#include "GenAIMockVendorSystemComponent.h"
#include <GenAIMockVendor/GenAIMockVendorTypeIds.h>

#include <AzCore/Serialization/SerializeContext.h>

namespace GenAIMockVendor
{
    AZ_COMPONENT_IMPL(GenAIMockVendorSystemComponent, "GenAIMockVendorSystemComponent", GenAIMockVendorSystemComponentTypeId);

    void GenAIMockVendorSystemComponent::Reflect(AZ::ReflectContext* context)
    {
        if (auto serializeContext = azrtti_cast<AZ::SerializeContext*>(context))
        {
            serializeContext->Class<GenAIMockVendorSystemComponent, AZ::Component>()->Version(0);
        }
    }

    void GenAIMockVendorSystemComponent::GetProvidedServices(AZ::ComponentDescriptor::DependencyArrayType& provided)
    {
        provided.push_back(AZ_CRC_CE("GenAIMockVendorService"));
    }

    void GenAIMockVendorSystemComponent::GetIncompatibleServices(AZ::ComponentDescriptor::DependencyArrayType& incompatible)
    {
        incompatible.push_back(AZ_CRC_CE("GenAIMockVendorService"));
    }

    void GenAIMockVendorSystemComponent::GetRequiredServices([[maybe_unused]] AZ::ComponentDescriptor::DependencyArrayType& required)
    {
    }

    void GenAIMockVendorSystemComponent::GetDependentServices([[maybe_unused]] AZ::ComponentDescriptor::DependencyArrayType& dependent)
    {
    }

    void GenAIMockVendorSystemComponent::Activate()
    {
    }

    void GenAIMockVendorSystemComponent::Deactivate()
    {
    }

} // namespace GenAIMockVendor
