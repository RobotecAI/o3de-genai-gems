/*
 * Copyright (c) Contributors to the Open 3D Engine Project.
 * For complete copyright and license terms please see the LICENSE at the root of this distribution.
 *
 * SPDX-License-Identifier: Apache-2.0 OR MIT
 *
 */

#include "AICoreModuleInterface.h"
#include "Test/AICoreTestComponent.h"
#include <AICore/AICoreTypeIds.h>
#include <AzCore/Memory/Memory.h>
#include <Clients/AICoreSystemComponent.h>

namespace AICore
{
    AZ_TYPE_INFO_WITH_NAME_IMPL(AICoreModuleInterface, "AICoreModuleInterface", AICoreModuleInterfaceTypeId);
    AZ_RTTI_NO_TYPE_INFO_IMPL(AICoreModuleInterface, AZ::Module);
    AZ_CLASS_ALLOCATOR_IMPL(AICoreModuleInterface, AZ::SystemAllocator);

    AICoreModuleInterface::AICoreModuleInterface()
    {
        // Push results of [MyComponent]::CreateDescriptor() into m_descriptors here.
        // Add ALL components descriptors associated with this gem to m_descriptors.
        // This will associate the AzTypeInfo information for the components with the the SerializeContext, BehaviorContext and EditContext.
        // This happens through the [MyComponent]::Reflect() function.
        m_descriptors.insert(
            m_descriptors.end(),
            {
                AICoreSystemComponent::CreateDescriptor(),
                AICoreTestComponent::CreateDescriptor()
            });
    }

    AZ::ComponentTypeList AICoreModuleInterface::GetRequiredSystemComponents() const
    {
        return AZ::ComponentTypeList{
            azrtti_typeid<AICoreSystemComponent>(),
        };
    }
} // namespace AICore
