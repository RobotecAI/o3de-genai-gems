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
    class GenAIBedrockVendorBundleModule : public GenAIBedrockVendorBundleModuleInterface
    {
    public:
        AZ_RTTI(GenAIBedrockVendorBundleModule, GenAIBedrockVendorBundleModuleTypeId, GenAIBedrockVendorBundleModuleInterface);
        AZ_CLASS_ALLOCATOR(GenAIBedrockVendorBundleModule, AZ::SystemAllocator);
    };
} // namespace GenAIBedrockVendorBundle

#if defined(O3DE_GEM_NAME)
AZ_DECLARE_MODULE_CLASS(AZ_JOIN(Gem_, O3DE_GEM_NAME), GenAIBedrockVendorBundle::GenAIBedrockVendorBundleModule)
#else
AZ_DECLARE_MODULE_CLASS(Gem_GenAIBedrockVendorBundle, GenAIBedrockVendorBundle::GenAIBedrockVendorBundleModule)
#endif
