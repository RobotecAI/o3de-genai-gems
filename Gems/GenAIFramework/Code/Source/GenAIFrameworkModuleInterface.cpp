/*
 * Copyright (c) Contributors to the Open 3D Engine Project.
 * For complete copyright and license terms please see the LICENSE at the root of this distribution.
 *
 * SPDX-License-Identifier: Apache-2.0 OR MIT
 *
 */

#include "GenAIFrameworkModuleInterface.h"
#include "Test/GenAIFrameworkTestComponent.h"
#include <AzCore/Component/ComponentApplication.h>
#include <AzCore/Memory/Memory.h>
#include <Clients/GenAIAsyncRequestSystemComponent.h>
#include <Clients/GenAIFrameworkSystemComponent.h>
#include <GenAIFramework/GenAIFrameworkTypeIds.h>
#include <GenAIFramework/SystemRegistrationContext/SystemRegistrationContext.h>

namespace GenAIFramework
{
    AZ_TYPE_INFO_WITH_NAME_IMPL(GenAIFrameworkModuleInterface, "GenAIFrameworkModuleInterface", GenAIFrameworkModuleInterfaceTypeId);
    AZ_RTTI_NO_TYPE_INFO_IMPL(GenAIFrameworkModuleInterface, AZ::Module);
    AZ_CLASS_ALLOCATOR_IMPL(GenAIFrameworkModuleInterface, AZ::SystemAllocator);

    GenAIFrameworkModuleInterface::GenAIFrameworkModuleInterface()
    {
        // Push results of [MyComponent]::CreateDescriptor() into m_descriptors here.
        // Add ALL components descriptors associated with this gem to m_descriptors.
        // This will associate the AzTypeInfo information for the components with the the SerializeContext, BehaviorContext and EditContext.
        // This happens through the [MyComponent]::Reflect() function.
        m_descriptors.insert(
            m_descriptors.end(),
            {
                GenAIFrameworkSystemComponent::CreateDescriptor(),
                GenAIFrameworkTestComponent::CreateDescriptor(),
                GenAIAsyncRequestSystemComponent::CreateDescriptor(),
            });

        // Create a new reflection context for reflecting serviceProviders and modelConfigurations
        AZ::ReflectionEnvironment::GetReflectionManager()->AddReflectContext<SystemRegistrationContext>();
    }

    AZ::ComponentTypeList GenAIFrameworkModuleInterface::GetRequiredSystemComponents() const
    {
        return AZ::ComponentTypeList{ azrtti_typeid<GenAIFrameworkSystemComponent>(), azrtti_typeid<GenAIAsyncRequestSystemComponent>() };
    }
} // namespace GenAIFramework
