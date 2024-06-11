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
    class ChatFeature : public FeatureBase
    {
    public:
        AZ_RTTI(ChatFeature, "{2c753228-4d1c-4632-9182-2f2e39970d9b}", FeatureBase);
        AZ_CLASS_ALLOCATOR(ChatFeature, AZ::SystemAllocator, 0);

        ChatFeature() = default;
        ChatFeature(AZ::u64 agentId, AZ::u64 conversationId);
        ~ChatFeature() override;

        static void Reflect(AZ::ReflectContext* context);

        // FeatureBase overrides
        void OnNewMessage(const AZStd::string& message) override;
        void OnPromptResponse(ModelAPIExtractedResponse response) override;
    };
} // namespace GenAIFramework
