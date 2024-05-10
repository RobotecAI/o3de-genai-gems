/*
 * Copyright (c) Contributors to the Open 3D Engine Project.
 * For complete copyright and license terms please see the LICENSE at the root of this distribution.
 *
 * SPDX-License-Identifier: Apache-2.0 OR MIT
 *
 */

#include <GenAIAmazonBedrockVendorBundle/GenAIAmazonBedrockVendorBundleTypeIds.h>
#include <GenAIAmazonBedrockVendorBundleModuleInterface.h>

namespace GenAIAmazonBedrockVendorBundle
{
    class GenAIAmazonBedrockVendorBundleModule : public GenAIAmazonBedrockVendorBundleModuleInterface
    {
    public:
        AZ_RTTI(
            GenAIAmazonBedrockVendorBundleModule,
            GenAIAmazonBedrockVendorBundleModuleTypeId,
            GenAIAmazonBedrockVendorBundleModuleInterface);
        AZ_CLASS_ALLOCATOR(GenAIAmazonBedrockVendorBundleModule, AZ::SystemAllocator);
    };
} // namespace GenAIAmazonBedrockVendorBundle

#if defined(O3DE_GEM_NAME)
AZ_DECLARE_MODULE_CLASS(AZ_JOIN(Gem_, O3DE_GEM_NAME), GenAIAmazonBedrockVendorBundle::GenAIAmazonBedrockVendorBundleModule)
#else
AZ_DECLARE_MODULE_CLASS(Gem_GenAIAmazonBedrockVendorBundle, GenAIAmazonBedrockVendorBundle::GenAIAmazonBedrockVendorBundleModule)
#endif
