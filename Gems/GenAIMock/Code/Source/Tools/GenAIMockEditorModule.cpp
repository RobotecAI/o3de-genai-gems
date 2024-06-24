/*
 * Copyright (c) Contributors to the Open 3D Engine Project.
 * For complete copyright and license terms please see the LICENSE at the root of this distribution.
 *
 * SPDX-License-Identifier: Apache-2.0 OR MIT
 *
 */

#include "GenAIMockEditorSystemComponent.h"
#include <GenAIMock/GenAIMockTypeIds.h>
#include <GenAIMockModuleInterface.h>

namespace GenAIMock
{
    class GenAIMockEditorModule : public GenAIMockModuleInterface
    {
    public:
        AZ_RTTI(GenAIMockEditorModule, GenAIMockEditorModuleTypeId, GenAIMockModuleInterface);
        AZ_CLASS_ALLOCATOR(GenAIMockEditorModule, AZ::SystemAllocator);

        GenAIMockEditorModule()
        {
            // Push results of [MyComponent]::CreateDescriptor() into m_descriptors here.
            // Add ALL components descriptors associated with this gem to m_descriptors.
            // This will associate the AzTypeInfo information for the components with the the SerializeContext, BehaviorContext and
            // EditContext. This happens through the [MyComponent]::Reflect() function.
            m_descriptors.insert(m_descriptors.end(), { GenAIMockEditorSystemComponent::CreateDescriptor() });
        }

        /**
         * Add required SystemComponents to the SystemEntity.
         * Non-SystemComponents should not be added here
         */
        AZ::ComponentTypeList GetRequiredSystemComponents() const override
        {
            return AZ::ComponentTypeList{
                azrtti_typeid<GenAIMockEditorSystemComponent>(),
            };
        }
    };
} // namespace GenAIMock

#if defined(O3DE_GEM_NAME)
AZ_DECLARE_MODULE_CLASS(AZ_JOIN(Gem_, O3DE_GEM_NAME, _Editor), GenAIMock::GenAIMockEditorModule)
#else
AZ_DECLARE_MODULE_CLASS(Gem_GenAIMock_Editor, GenAIMock::GenAIMockEditorModule)
#endif
