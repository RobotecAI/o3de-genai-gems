/*
 * Copyright (c) Contributors to the Open 3D Engine Project.
 * For complete copyright and license terms please see the LICENSE at the root of this distribution.
 *
 * SPDX-License-Identifier: Apache-2.0 OR MIT
 *
 */

#include <GenAIVendorBundle/GenAIVendorBundleTypeIds.h>
#include <GenAIVendorBundleModuleInterface.h>

namespace GenAIVendorBundle
{
    class GenAIVendorBundleModule : public GenAIVendorBundleModuleInterface
    {
    public:
        AZ_RTTI(GenAIVendorBundleModule, GenAIVendorBundleModuleTypeId, GenAIVendorBundleModuleInterface);
        AZ_CLASS_ALLOCATOR(GenAIVendorBundleModule, AZ::SystemAllocator);
    };
} // namespace GenAIVendorBundle

#if defined(O3DE_GEM_NAME)
AZ_DECLARE_MODULE_CLASS(AZ_JOIN(Gem_, O3DE_GEM_NAME), GenAIVendorBundle::GenAIVendorBundleModule)
#else
AZ_DECLARE_MODULE_CLASS(Gem_GenAIVendorBundle, GenAIVendorBundle::GenAIVendorBundleModule)
#endif
