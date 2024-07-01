/*
 * Copyright (c) Contributors to the Open 3D Engine Project.
 * For complete copyright and license terms please see the LICENSE at the root of this distribution.
 *
 * SPDX-License-Identifier: Apache-2.0 OR MIT
 *
 */

#include "GenAIMockVendorModuleInterface.h"
#include <Clients/GenAIMockVendorSystemComponent.h>
#include <Communication/MockServiceComponent.h>
#include <GenAIMockVendor/GenAIMockVendorTypeIds.h>
#include <ModelConfigurations/MockModelComponent.h>

#include <AzCore/Memory/Memory.h>

namespace GenAIMockVendor
{
    AZ_TYPE_INFO_WITH_NAME_IMPL(GenAIMockVendorModuleInterface, "GenAIMockVendorModuleInterface", GenAIMockVendorModuleInterfaceTypeId);
    AZ_RTTI_NO_TYPE_INFO_IMPL(GenAIMockVendorModuleInterface, AZ::Module);
    AZ_CLASS_ALLOCATOR_IMPL(GenAIMockVendorModuleInterface, AZ::SystemAllocator);

    GenAIMockVendorModuleInterface::GenAIMockVendorModuleInterface()
    {
        // Push results of [MyComponent]::CreateDescriptor() into m_descriptors here.
        // Add ALL components descriptors associated with this gem to m_descriptors.
        // This will associate the AzTypeInfo information for the components with the the SerializeContext, BehaviorContext and EditContext.
        // This happens through the [MyComponent]::Reflect() function.
        m_descriptors.insert(
            m_descriptors.end(),
            { GenAIMockVendorSystemComponent::CreateDescriptor(),
              MockModelComponent::CreateDescriptor(),
              MockServiceComponent::CreateDescriptor() });
    }

    AZ::ComponentTypeList GenAIMockVendorModuleInterface::GetRequiredSystemComponents() const
    {
        return AZ::ComponentTypeList{
            azrtti_typeid<GenAIMockVendorSystemComponent>(),
        };
    }
} // namespace GenAIMockVendor
