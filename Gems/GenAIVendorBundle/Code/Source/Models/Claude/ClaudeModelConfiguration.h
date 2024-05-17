/*
 * Copyright (c) Contributors to the Open 3D Engine Project.
 * For complete copyright and license terms please see the LICENSE at the root of this distribution.
 *
 * SPDX-License-Identifier: Apache-2.0 OR MIT
 *
 */

#pragma once

#include <AzCore/Component/Component.h>
#include <AzCore/RTTI/RTTIMacros.h>
#include <AzCore/RTTI/ReflectContext.h>
#include <AzCore/std/string/string.h>

namespace GenAIVendorBundle
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
        bool m_useSystemMessage = true;

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

        AZ::Outcome<void, AZStd::string> SetModelParameter(const AZ::Name& parameterName, const AZStd::string& parameterValue);

        static AZ::Outcome<bool, void> GetBooleanValue(const AZStd::string& value)
        {
            AZStd::string lowerCaseParameterValue = value;
            std::transform(
                lowerCaseParameterValue.begin(),
                lowerCaseParameterValue.end(),
                lowerCaseParameterValue.begin(),
                [](unsigned char c)
                {
                    return std::tolower(c);
                });

            lowerCaseParameterValue.erase(
                std::remove_if(
                    lowerCaseParameterValue.begin(),
                    lowerCaseParameterValue.end(),
                    [](unsigned char c)
                    {
                        return std::isspace(c);
                    }),
                lowerCaseParameterValue.end());
            if (lowerCaseParameterValue == "true")
            {
                return AZ::Success(true);
            }
            else if (lowerCaseParameterValue == "false")
            {
                return AZ::Success(false);
            }
            else
            {
                return AZ::Failure();
            }
        }
    };
} // namespace GenAIVendorBundle
