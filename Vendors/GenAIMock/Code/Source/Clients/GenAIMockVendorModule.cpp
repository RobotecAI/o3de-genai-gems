/*
 * Copyright (c) Contributors to the Open 3D Engine Project.
 * For complete copyright and license terms please see the LICENSE at the root of this distribution.
 *
 * SPDX-License-Identifier: Apache-2.0 OR MIT
 *
 */

#include "GenAIMockVendorSystemComponent.h"
#include <GenAIMockVendor/GenAIMockVendorTypeIds.h>
#include <GenAIMockVendorModuleInterface.h>

namespace GenAIMockVendor
{
    class GenAIMockVendorModule : public GenAIMockVendorModuleInterface
    {
    public:
        AZ_RTTI(GenAIMockVendorModule, GenAIMockVendorModuleTypeId, GenAIMockVendorModuleInterface);
        AZ_CLASS_ALLOCATOR(GenAIMockVendorModule, AZ::SystemAllocator);
    };
} // namespace GenAIMockVendor

#if defined(O3DE_GEM_NAME)
AZ_DECLARE_MODULE_CLASS(AZ_JOIN(Gem_, O3DE_GEM_NAME), GenAIMockVendor::GenAIMockVendorModule)
#else
AZ_DECLARE_MODULE_CLASS(Gem_GenAIMockVendor, GenAIMockVendor::GenAIMockVendorModule)
#endif
