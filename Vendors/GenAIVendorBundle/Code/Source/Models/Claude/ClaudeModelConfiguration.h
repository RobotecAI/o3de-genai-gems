/*
 * Copyright (c) Contributors to the Open 3D Engine Project.
 * For complete copyright and license terms please see the LICENSE at the root of this distribution.
 *
 * SPDX-License-Identifier: Apache-2.0 OR MIT
 *
 */

#pragma once

#include <GenAIVendorBundle/GenAIVendorBundleTypeIds.h>

#include <AzCore/Component/Component.h>
#include <AzCore/RTTI/RTTIMacros.h>
#include <AzCore/RTTI/ReflectContext.h>
#include <AzCore/std/containers/map.h>
#include <AzCore/std/string/string.h>

namespace GenAIVendorBundle
{
    class ClaudeModelConfiguration : public AZ::ComponentConfig
    {
    public:
        AZ_RTTI(ClaudeModelConfiguration, ClaudeModelConfigurationTypeId);
        AZ_CLASS_ALLOCATOR(ClaudeModelConfiguration, AZ::SystemAllocator);

        ClaudeModelConfiguration() = default;
        ~ClaudeModelConfiguration() = default;

        static void Reflect(AZ::ReflectContext* context);

        int m_maxTokensToSample = 2000;
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
        bool m_useSystemMessage = true;

        inline bool IsDefaultTemperature() const
        {
            return m_useDefaultTemperature;
        }
        inline bool IsDefaultTopP() const
        {
            return m_useDefaultTopP;
        }
        inline bool IsDefaultTopK() const
        {
            return m_useDefaultTopK;
        }
        inline bool IsDefaultStopSequence() const
        {
            return m_useDefaultStopSequence;
        }

        enum class Parameters
        {
            MaxTokensToSample,
            Temperature,
            TopP,
            TopK,
            StopSequence,
            AnthropicVersion,
            SystemMessage,
            useDefaultTemperature,
            useDefaultTopP,
            useDefaultTopK,
            useDefaultStopSequence,
            useSystemMessage
        };

        const static AZStd::map<AZStd::string_view, Parameters> m_parameterNameToEnum;

        AZ::Outcome<void, AZStd::string> SetModelParameter(const AZ::Name& parameterName, const AZStd::string& parameterValue);
    };
} // namespace GenAIVendorBundle
