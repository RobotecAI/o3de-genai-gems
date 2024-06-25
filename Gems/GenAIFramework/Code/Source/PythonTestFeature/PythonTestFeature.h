/*
 * Copyright (c) Contributors to the Open 3D Engine Project.
 * For complete copyright and license terms please see the LICENSE at the root of this distribution.
 *
 * SPDX-License-Identifier: Apache-2.0 OR MIT
 *
 */

#pragma once

#include "GenAIFramework/Feature/PythonFeatureBase.h"
#include <GenAIFramework/Feature/FeatureBase.h>
#include <GenAIFramework/GenAIFrameworkTypeIds.h>

#include <AzCore/RTTI/RTTI.h>
#include <AzCore/Serialization/SerializeContext.h>

namespace GenAIFramework
{
    class PythonTestFeature : public PythonFeatureBase
    {
    public:
        AZ_RTTI(PythonTestFeature, "{7c524a1b-a175-4c26-beb1-dbefb7f13f54}", PythonFeatureBase);
        AZ_CLASS_ALLOCATOR(PythonTestFeature, AZ::SystemAllocator, 0);

        PythonTestFeature() = default;
        PythonTestFeature(AZ::u64 agentId, AZ::u64 conversationId);
        ~PythonTestFeature() = default;

        static void Reflect(AZ::ReflectContext* context);
    };
} // namespace GenAIFramework
