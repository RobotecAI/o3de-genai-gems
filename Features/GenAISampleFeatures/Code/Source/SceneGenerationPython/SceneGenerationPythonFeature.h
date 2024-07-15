/*
 * Copyright (c) Contributors to the Open 3D Engine Project.
 * For complete copyright and license terms please see the LICENSE at the root of this distribution.
 *
 * SPDX-License-Identifier: Apache-2.0 OR MIT
 *
 */

#pragma once

#include <GenAISampleFeatures/GenAISampleFeaturesTypeIds.h>

#include <GenAIFramework/Feature/FeatureBase.h>
#include <GenAIFramework/Feature/PythonFeatureBase.h>

#include <AzCore/RTTI/RTTI.h>
#include <AzCore/Serialization/SerializeContext.h>

namespace GenAISampleFeatures
{
    class SceneGenerationPythonFeature : public GenAIFramework::PythonFeatureBase
    {
    public:
        AZ_RTTI(SceneGenerationPythonFeature, SceneGenerationPythonFeatureTypeId, GenAIFramework::PythonFeatureBase);
        AZ_CLASS_ALLOCATOR(SceneGenerationPythonFeature, AZ::SystemAllocator, 0);

        SceneGenerationPythonFeature() = default;
        SceneGenerationPythonFeature(AZ::u64 agentId, AZ::u64 conversationId);
        ~SceneGenerationPythonFeature() = default;

        static void Reflect(AZ::ReflectContext* context);

    private:
        constexpr static const char* SceneGenerationPythonScript = "Editor/Scripts/scene_generation/scene_generation.py";

        AZStd::string m_pythonScriptLocation;
    };
} // namespace GenAISampleFeatures
