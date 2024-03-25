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

namespace GenAIBedrock
{
    class ClaudePromptInputConfiguration : public AZ::ComponentConfig
    {
    public:
        AZ_RTTI(ClaudePromptInputConfiguration, "{5a478ab3-8ac6-4c23-8768-03c2791d6b11}");
        AZ_CLASS_ALLOCATOR(ClaudePromptInputConfiguration, AZ::SystemAllocator);

        ClaudePromptInputConfiguration() = default;
        ~ClaudePromptInputConfiguration() = default;

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

    class ClaudePromptInput
    {
    public:
        AZ_RTTI(ClaudePromptInput, "{345d0942-abc0-4334-96cb-fb109dc98a7b}");

        ClaudePromptInput() = default;
        virtual ~ClaudePromptInput() = default;

        AZStd::string m_prompt;
        AZStd::optional<ClaudePromptInputConfiguration> m_configuration;
    };

    class ClaudePromptOutput
    {
    public:
        AZ_RTTI(ClaudePromptOutput, "{e8ee9752-a1c2-4a3f-bd51-189253b8edac}");

        ClaudePromptOutput() = default;
        virtual ~ClaudePromptOutput() = default;

        AZStd::string m_completion;
        AZStd::string m_stopReason;
        AZStd::optional<AZStd::string> stop;
    };
} // namespace GenAIBedrock
