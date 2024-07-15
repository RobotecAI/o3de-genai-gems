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
    class O3DEAssistantPythonFeature : public GenAIFramework::PythonFeatureBase
    {
    public:
        AZ_RTTI(O3DEAssistantPythonFeature, O3DEAssistantPythonFeatureTypeId, GenAIFramework::PythonFeatureBase);
        AZ_CLASS_ALLOCATOR(O3DEAssistantPythonFeature, AZ::SystemAllocator, 0);

        O3DEAssistantPythonFeature() = default;
        O3DEAssistantPythonFeature(AZ::u64 agentId, AZ::u64 conversationId);
        ~O3DEAssistantPythonFeature() = default;

        static void Reflect(AZ::ReflectContext* context);

    private:
        constexpr static const char* O3DEAssistantPythonScript = "Editor/Scripts/o3de_assistant/o3de_assistant.py";

        AZStd::string m_pythonScriptLocation;
    };
} // namespace GenAISampleFeatures
