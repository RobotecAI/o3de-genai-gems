#pragma once

#include <AzCore/Outcome/Outcome.h>
#include <AzCore/std/string/string.h>

namespace GenAIVendorBundle::Utils
{
    inline AZ::Outcome<bool, void> GetBooleanValue(const AZStd::string& value)
    {
        AZStd::string lowerCaseParameterValue = value;
        AZStd::transform(
            lowerCaseParameterValue.begin(),
            lowerCaseParameterValue.end(),
            lowerCaseParameterValue.begin(),
            [](unsigned char c)
            {
                return AZStd::tolower(c);
            });

        lowerCaseParameterValue.erase(
            AZStd::remove_if(
                lowerCaseParameterValue.begin(),
                lowerCaseParameterValue.end(),
                [](unsigned char c)
                {
                    return AZStd::isspace(c);
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
} // namespace GenAIVendorBundle::Utils
