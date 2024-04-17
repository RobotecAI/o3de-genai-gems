/*
 * Copyright (c) Contributors to the Open 3D Engine Project.
 * For complete copyright and license terms please see the LICENSE at the root of this distribution.
 *
 * SPDX-License-Identifier: Apache-2.0 OR MIT
 *
 */

#include "GenAIOllamaModuleInterface.h"

#include <Clients/GenAIOllamaSystemComponent.h>
#include <Communication/BasicHttpServiceComponent.h>
#include <GenAIOllama/GenAIOllamaTypeIds.h>
#include <ModelConfigurations/OllamaModelComponent.h>

#include <AzCore/Memory/Memory.h>

namespace GenAIOllama
{
    AZ_TYPE_INFO_WITH_NAME_IMPL(GenAIOllamaModuleInterface, "GenAIOllamaModuleInterface", GenAIOllamaModuleInterfaceTypeId);
    AZ_RTTI_NO_TYPE_INFO_IMPL(GenAIOllamaModuleInterface, AZ::Module);
    AZ_CLASS_ALLOCATOR_IMPL(GenAIOllamaModuleInterface, AZ::SystemAllocator);

    GenAIOllamaModuleInterface::GenAIOllamaModuleInterface()
    {
        // Push results of [MyComponent]::CreateDescriptor() into m_descriptors here.
        // Add ALL components descriptors associated with this gem to m_descriptors.
        // This will associate the AzTypeInfo information for the components with the the SerializeContext, BehaviorContext and EditContext.
        // This happens through the [MyComponent]::Reflect() function.
        m_descriptors.insert(
            m_descriptors.end(),
            {
                GenAIOllamaSystemComponent::CreateDescriptor(),
                OllamaModelComponent::CreateDescriptor(),
                OllamaHttpServiceComponent::CreateDescriptor(),
            });
    }

    AZ::ComponentTypeList GenAIOllamaModuleInterface::GetRequiredSystemComponents() const
    {
        return AZ::ComponentTypeList{
            azrtti_typeid<GenAIOllamaSystemComponent>(),
        };
    }
} // namespace GenAIOllama
