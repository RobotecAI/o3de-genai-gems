/*
 * Copyright (c) Contributors to the Open 3D Engine Project.
 * For complete copyright and license terms please see the LICENSE at the root of this distribution.
 *
 * SPDX-License-Identifier: Apache-2.0 OR MIT
 *
 */

#include "GenAIMockModuleInterface.h"
#include <Clients/GenAIMockSystemComponent.h>
#include <Communication/MockServiceComponent.h>
#include <GenAIMock/GenAIMockTypeIds.h>
#include <ModelConfigurations/PassthroughModelComponent.h>

#include <AzCore/Memory/Memory.h>

namespace GenAIMock
{
    AZ_TYPE_INFO_WITH_NAME_IMPL(GenAIMockModuleInterface, "GenAIMockModuleInterface", GenAIMockModuleInterfaceTypeId);
    AZ_RTTI_NO_TYPE_INFO_IMPL(GenAIMockModuleInterface, AZ::Module);
    AZ_CLASS_ALLOCATOR_IMPL(GenAIMockModuleInterface, AZ::SystemAllocator);

    GenAIMockModuleInterface::GenAIMockModuleInterface()
    {
        // Push results of [MyComponent]::CreateDescriptor() into m_descriptors here.
        // Add ALL components descriptors associated with this gem to m_descriptors.
        // This will associate the AzTypeInfo information for the components with the the SerializeContext, BehaviorContext and EditContext.
        // This happens through the [MyComponent]::Reflect() function.
        m_descriptors.insert(
            m_descriptors.end(),
            { GenAIMockSystemComponent::CreateDescriptor(),
              PassthroughModelComponent::CreateDescriptor(),
              MockServiceComponent::CreateDescriptor() });
    }

    AZ::ComponentTypeList GenAIMockModuleInterface::GetRequiredSystemComponents() const
    {
        return AZ::ComponentTypeList{
            azrtti_typeid<GenAIMockSystemComponent>(),
        };
    }
} // namespace GenAIMock
