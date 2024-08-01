/*
 * Copyright (c) Contributors to the Open 3D Engine Project.
 * For complete copyright and license terms please see the LICENSE at the root of this distribution.
 *
 * SPDX-License-Identifier: Apache-2.0 OR MIT
 *
 */

#include "GenAIMockVendorEditorSystemComponent.h"
#include <GenAIMockVendor/GenAIMockVendorTypeIds.h>
#include <GenAIMockVendorModuleInterface.h>

namespace GenAIMockVendor
{
    class GenAIMockVendorEditorModule : public GenAIMockVendorModuleInterface
    {
    public:
        AZ_RTTI(GenAIMockVendorEditorModule, GenAIMockVendorEditorModuleTypeId, GenAIMockVendorModuleInterface);
        AZ_CLASS_ALLOCATOR(GenAIMockVendorEditorModule, AZ::SystemAllocator);

        GenAIMockVendorEditorModule()
        {
            // Push results of [MyComponent]::CreateDescriptor() into m_descriptors here.
            // Add ALL components descriptors associated with this gem to m_descriptors.
            // This will associate the AzTypeInfo information for the components with the the SerializeContext, BehaviorContext and
            // EditContext. This happens through the [MyComponent]::Reflect() function.
            m_descriptors.insert(m_descriptors.end(), { GenAIMockVendorEditorSystemComponent::CreateDescriptor() });
        }

        /**
         * Add required SystemComponents to the SystemEntity.
         * Non-SystemComponents should not be added here
         */
        AZ::ComponentTypeList GetRequiredSystemComponents() const override
        {
            return AZ::ComponentTypeList{
                azrtti_typeid<GenAIMockVendorEditorSystemComponent>(),
            };
        }
    };
} // namespace GenAIMockVendor

#if defined(O3DE_GEM_NAME)
AZ_DECLARE_MODULE_CLASS(AZ_JOIN(Gem_, O3DE_GEM_NAME, _Editor), GenAIMockVendor::GenAIMockVendorEditorModule)
#else
AZ_DECLARE_MODULE_CLASS(Gem_GenAIMockVendor_Editor, GenAIMockVendor::GenAIMockVendorEditorModule)
#endif
