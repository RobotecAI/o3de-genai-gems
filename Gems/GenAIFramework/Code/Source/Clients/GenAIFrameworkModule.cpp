/*
 * Copyright (c) Contributors to the Open 3D Engine Project.
 * For complete copyright and license terms please see the LICENSE at the root of this distribution.
 *
 * SPDX-License-Identifier: Apache-2.0 OR MIT
 *
 */

#include "GenAIFrameworkSystemComponent.h"
#include <GenAIFramework/GenAIFrameworkTypeIds.h>
#include <GenAIFrameworkModuleInterface.h>

namespace GenAIFramework
{
    class GenAIFrameworkModule : public GenAIFrameworkModuleInterface
    {
    public:
        AZ_RTTI(GenAIFrameworkModule, GenAIFrameworkModuleTypeId, GenAIFrameworkModuleInterface);
        AZ_CLASS_ALLOCATOR(GenAIFrameworkModule, AZ::SystemAllocator);
    };
} // namespace GenAIFramework

AZ_DECLARE_MODULE_CLASS(Gem_GenAIFramework, GenAIFramework::GenAIFrameworkModule)
