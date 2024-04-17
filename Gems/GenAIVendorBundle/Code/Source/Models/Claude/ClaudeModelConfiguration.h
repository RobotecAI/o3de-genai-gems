/*
 * Copyright (c) Contributors to the Open 3D Engine Project.
 * For complete copyright and license terms please see the LICENSE at the root of this distribution.
 *
 * SPDX-License-Identifier: Apache-2.0 OR MIT
 *
 */

#pragma once

#include <AzCore/Component/Component.h>
#include <AzCore/Memory/Memory_fwd.h>
#include <AzCore/RTTI/RTTIMacros.h>
#include <AzCore/RTTI/ReflectContext.h>
#include <AzCore/std/string/string.h>

namespace GenAIClaude
{
    class ClaudeModelConfiguration : public AZ::ComponentConfig
    {
    public:
        AZ_RTTI(ClaudeModelConfiguration, "{5a478ab3-8ac6-4c23-8768-03c2791d6b11}");
        AZ_CLASS_ALLOCATOR(ClaudeModelConfiguration, AZ::SystemAllocator);

        ClaudeModelConfiguration() = default;
        ~ClaudeModelConfiguration() = default;

        static void Reflect(AZ::ReflectContext* context);

        int m_maxTokensToSample = 200;
        float m_temperature = 0.5f;
        bool m_useDefaultTemperature = true;
        float m_topP = 1.f;
        bool m_useDefaultTopP = true;
        int m_topK = 250;
        bool m_useDefaultTopK = true;
        AZStd::string m_stopSequence = "";
        bool m_useDefaultStopSequence = true;
        AZStd::string m_anthropicVersion = "";
        AZStd::string m_systemMessage = "";
        bool m_useSystemMessage = false;

        bool IsDefaultTemperature() const
        {
            return m_useDefaultTemperature;
        }
        bool IsDefaultTopP() const
        {
            return m_useDefaultTopP;
        }
        bool IsDefaultTopK() const
        {
            return m_useDefaultTopK;
        }
        bool IsDefaultStopSequence() const
        {
            return m_useDefaultStopSequence;
        }
    };
} // namespace GenAIClaude
