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
    class O3DEAssistantPythonFeature : public PythonFeatureBase
    {
    public:
        AZ_RTTI(O3DEAssistantPythonFeature, "{7c524a1b-a175-4c26-beb1-dbefb7f13f54}", PythonFeatureBase);
        AZ_CLASS_ALLOCATOR(O3DEAssistantPythonFeature, AZ::SystemAllocator, 0);

        O3DEAssistantPythonFeature() = default;
        O3DEAssistantPythonFeature(AZ::u64 agentId, AZ::u64 conversationId);
        ~O3DEAssistantPythonFeature() = default;

        static void Reflect(AZ::ReflectContext* context);

    private:
        constexpr static const char* O3DEAssistantPythonScript = "Editor/Scripts/o3de_assistant/o3de_assistant.py";

        AZStd::string m_pythonScriptLocation;
    };
} // namespace GenAIFramework
