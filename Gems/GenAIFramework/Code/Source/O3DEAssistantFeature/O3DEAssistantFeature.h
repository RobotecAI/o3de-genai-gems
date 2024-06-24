/*
 * Copyright (c) Contributors to the Open 3D Engine Project.
 * For complete copyright and license terms please see the LICENSE at the root of this distribution.
 *
 * SPDX-License-Identifier: Apache-2.0 OR MIT
 *
 */

#pragma once

#include <GenAIFramework/Feature/FeatureBase.h>
#include <GenAIFramework/GenAIFrameworkTypeIds.h>

#include <AzCore/RTTI/RTTI.h>
#include <AzCore/Serialization/SerializeContext.h>

namespace GenAIFramework
{
    class O3DEAssistantFeature : public FeatureBase
    {
    public:
        AZ_RTTI(O3DEAssistantFeature, O3DEAssistantFeatureTypeId, FeatureBase);
        AZ_CLASS_ALLOCATOR(O3DEAssistantFeature, AZ::SystemAllocator, 0);

        O3DEAssistantFeature() = default;
        O3DEAssistantFeature(AZ::u64 agentId, AZ::u64 conversationId);
        ~O3DEAssistantFeature() = default;

        static void Reflect(AZ::ReflectContext* context);

        // FeatureBase overrides
        void OnNewMessage(const AZStd::string& message) override;
        void OnAIResponse(ModelAPIExtractedResponse response) override;
    };
} // namespace GenAIFramework
