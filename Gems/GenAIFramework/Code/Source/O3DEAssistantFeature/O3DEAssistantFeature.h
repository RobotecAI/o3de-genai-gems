/*
 * Copyright (c) Contributors to the Open 3D Engine Project.
 * For complete copyright and license terms please see the LICENSE at the root of this distribution.
 *
 * SPDX-License-Identifier: Apache-2.0 OR MIT
 *
 */

#pragma once

#include <AzCore/RTTI/RTTI.h>
#include <AzCore/Serialization/SerializeContext.h>
#include <GenAIFramework/Feature/FeatureBase.h>

namespace GenAIFramework
{
    class O3DEAssistantFeature : public FeatureBase
    {
    public:
        AZ_RTTI(O3DEAssistantFeature, "{2c753228-4d1c-4632-9182-2f2e39970d9b}", FeatureBase);
        AZ_CLASS_ALLOCATOR(O3DEAssistantFeature, AZ::SystemAllocator, 0);

        O3DEAssistantFeature() = default;
        O3DEAssistantFeature(AZ::u64 agentId, AZ::u64 conversationId);
        ~O3DEAssistantFeature() = default;

        static void Reflect(AZ::ReflectContext* context);

        // FeatureBase overrides
        void OnNewMessage(const AZStd::string& message) override;
        void OnPromptResponse(ModelAPIExtractedResponse response) override;
    };
} // namespace GenAIFramework
