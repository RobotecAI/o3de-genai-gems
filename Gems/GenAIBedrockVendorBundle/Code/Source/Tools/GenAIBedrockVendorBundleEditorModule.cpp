/*
 * Copyright (c) Contributors to the Open 3D Engine Project.
 * For complete copyright and license terms please see the LICENSE at the root of this distribution.
 *
 * SPDX-License-Identifier: Apache-2.0 OR MIT
 *
 */

#include <GenAIBedrockVendorBundle/GenAIBedrockVendorBundleTypeIds.h>
#include <GenAIBedrockVendorBundleModuleInterface.h>

namespace GenAIBedrockVendorBundle
{
    class GenAIBedrockVendorBundleEditorModule : public GenAIBedrockVendorBundleModuleInterface
    {
    public:
        AZ_RTTI(GenAIBedrockVendorBundleEditorModule, GenAIBedrockVendorBundleEditorModuleTypeId, GenAIBedrockVendorBundleModuleInterface);
        AZ_CLASS_ALLOCATOR(GenAIBedrockVendorBundleEditorModule, AZ::SystemAllocator);

        GenAIBedrockVendorBundleEditorModule()
        {
            // Push results of [MyComponent]::CreateDescriptor() into m_descriptors here.
            // Add ALL components descriptors associated with this gem to m_descriptors.
            // This will associate the AzTypeInfo information for the components with the the SerializeContext, BehaviorContext and
            // EditContext. This happens through the [MyComponent]::Reflect() function.
            m_descriptors.insert(m_descriptors.end(), {});
        }

        /**
         * Add required SystemComponents to the SystemEntity.
         * Non-SystemComponents should not be added here
         */
        AZ::ComponentTypeList GetRequiredSystemComponents() const override
        {
            return AZ::ComponentTypeList{};
        }
    };
} // namespace GenAIBedrockVendorBundle

#if defined(O3DE_GEM_NAME)
AZ_DECLARE_MODULE_CLASS(AZ_JOIN(Gem_, O3DE_GEM_NAME, _Editor), GenAIBedrockVendorBundle::GenAIBedrockVendorBundleEditorModule)
#else
AZ_DECLARE_MODULE_CLASS(Gem_GenAIBedrockVendorBundle_Editor, GenAIBedrockVendorBundle::GenAIBedrockVendorBundleEditorModule)
#endif
