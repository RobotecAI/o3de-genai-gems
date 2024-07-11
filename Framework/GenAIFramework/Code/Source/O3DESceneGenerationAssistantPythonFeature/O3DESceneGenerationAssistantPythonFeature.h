/*
 * Copyright (c) Contributors to the Open 3D Engine Project.
 * For complete copyright and license terms please see the LICENSE at the root of this distribution.
 *
 * SPDX-License-Identifier: Apache-2.0 OR MIT
 *
 */

#pragma once

#include <GenAIFramework/Feature/FeatureBase.h>
#include <GenAIFramework/Feature/PythonFeatureBase.h>
#include <GenAIFramework/GenAIFrameworkTypeIds.h>

#include <AzCore/RTTI/RTTI.h>
#include <AzCore/Serialization/SerializeContext.h>

namespace GenAIFramework
{
    class O3DESceneGenerationAssistantPythonFeature : public PythonFeatureBase
    {
    public:
        AZ_RTTI(O3DESceneGenerationAssistantPythonFeature, "{7c524a1b-a175-4c26-beb1-dbefb7f13f50}", PythonFeatureBase);
        AZ_CLASS_ALLOCATOR(O3DESceneGenerationAssistantPythonFeature, AZ::SystemAllocator, 0);

        O3DESceneGenerationAssistantPythonFeature() = default;
        O3DESceneGenerationAssistantPythonFeature(AZ::u64 agentId, AZ::u64 conversationId);
        ~O3DESceneGenerationAssistantPythonFeature() = default;

        static void Reflect(AZ::ReflectContext* context);

    private:
        constexpr static const char* PythonSceneGenerationAssistantScript = "Editor/Scripts/scene_generation/scene_generation_assistant.py";

        AZStd::string m_sceneGenerationAssistantPythonScriptLocation;
    };
} // namespace GenAIFramework
