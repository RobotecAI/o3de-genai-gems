/*
 * Copyright (c) Contributors to the Open 3D Engine Project.
 * For complete copyright and license terms please see the LICENSE at the root of this distribution.
 *
 * SPDX-License-Identifier: Apache-2.0 OR MIT
 *
 */

#include "GenAIAmazonBedrockVendorBundleModuleInterface.h"
#include <GenAIAmazonBedrockVendorBundle/GenAIAmazonBedrockVendorBundleTypeIds.h>
#include <Providers/Claude/ClaudeAmazonBedrockProvider.h>

namespace GenAIAmazonBedrockVendorBundle
{
    AZ_TYPE_INFO_WITH_NAME_IMPL(
        GenAIAmazonBedrockVendorBundleModuleInterface,
        "GenAIAmazonBedrockVendorBundleModuleInterface",
        GenAIAmazonBedrockVendorBundleModuleInterfaceTypeId);
    AZ_RTTI_NO_TYPE_INFO_IMPL(GenAIAmazonBedrockVendorBundleModuleInterface, AZ::Module);
    AZ_CLASS_ALLOCATOR_IMPL(GenAIAmazonBedrockVendorBundleModuleInterface, AZ::SystemAllocator);

    GenAIAmazonBedrockVendorBundleModuleInterface::GenAIAmazonBedrockVendorBundleModuleInterface()
    {
        // Push results of [MyComponent]::CreateDescriptor() into m_descriptors here.
        // Add ALL components descriptors associated with this gem to m_descriptors.
        // This will associate the AzTypeInfo information for the components with the the SerializeContext, BehaviorContext and EditContext.
        // This happens through the [MyComponent]::Reflect() function.
        m_descriptors.insert(m_descriptors.end(), { ClaudeAmazonBedrockProvider::CreateDescriptor() });
    }

    AZ::ComponentTypeList GenAIAmazonBedrockVendorBundleModuleInterface::GetRequiredSystemComponents() const
    {
        return AZ::ComponentTypeList{};
    }
} // namespace GenAIAmazonBedrockVendorBundle
