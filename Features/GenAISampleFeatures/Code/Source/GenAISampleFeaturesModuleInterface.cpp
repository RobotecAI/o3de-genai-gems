/*
 * Copyright (c) Contributors to the Open 3D Engine Project.
 * For complete copyright and license terms please see the LICENSE at the root of this distribution.
 *
 * SPDX-License-Identifier: Apache-2.0 OR MIT
 *
 */

#include "GenAISampleFeaturesModuleInterface.h"
#include <Clients/GenAISampleFeaturesSystemComponent.h>
#include <GenAISampleFeatures/GenAISampleFeaturesTypeIds.h>

#include <AzCore/Memory/Memory.h>

namespace GenAISampleFeatures
{
    AZ_TYPE_INFO_WITH_NAME_IMPL(
        GenAISampleFeaturesModuleInterface, "GenAISampleFeaturesModuleInterface", GenAISampleFeaturesModuleInterfaceTypeId);
    AZ_RTTI_NO_TYPE_INFO_IMPL(GenAISampleFeaturesModuleInterface, AZ::Module);
    AZ_CLASS_ALLOCATOR_IMPL(GenAISampleFeaturesModuleInterface, AZ::SystemAllocator);

    GenAISampleFeaturesModuleInterface::GenAISampleFeaturesModuleInterface()
    {
        // Push results of [MyComponent]::CreateDescriptor() into m_descriptors here.
        // Add ALL components descriptors associated with this gem to m_descriptors.
        // This will associate the AzTypeInfo information for the components with the the SerializeContext, BehaviorContext and EditContext.
        // This happens through the [MyComponent]::Reflect() function.
        m_descriptors.insert(
            m_descriptors.end(),
            {
                GenAISampleFeaturesSystemComponent::CreateDescriptor(),
            });
    }

    AZ::ComponentTypeList GenAISampleFeaturesModuleInterface::GetRequiredSystemComponents() const
    {
        return AZ::ComponentTypeList{
            azrtti_typeid<GenAISampleFeaturesSystemComponent>(),
        };
    }
} // namespace GenAISampleFeatures
