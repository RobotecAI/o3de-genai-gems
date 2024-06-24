/*
 * Copyright (c) Contributors to the Open 3D Engine Project.
 * For complete copyright and license terms please see the LICENSE at the root of this distribution.
 *
 * SPDX-License-Identifier: Apache-2.0 OR MIT
 *
 */

#pragma once

namespace GenAIVendorBundle
{
    // Module derived classes TypeIds
    inline constexpr const char* GenAIVendorBundleModuleInterfaceTypeId = "{1895AAC0-A20F-43A6-BAD7-889A5D1B1D43}";
    inline constexpr const char* GenAIVendorBundleModuleTypeId = "{831A9C5D-DE42-4C4F-9928-B23ADC68166F}";
    // The Editor Module by default is mutually exclusive with the Client Module
    // so they use the Same TypeId
    inline constexpr const char* GenAIVendorBundleEditorModuleTypeId = GenAIVendorBundleModuleTypeId;

    // Interface TypeIds
    inline constexpr const char* GenAIVendorBundleRequestsTypeId = "{F1D5E6A3-0D13-46F9-8B9B-92A85A145625}";

    // Models TypeIds
    inline constexpr const char* ClaudeModelConfigurationTypeId = "{5A478AB3-8AC6-4C23-8768-03C2791D6B11}";
    inline constexpr const char* ClaudeModelMessagesAPITypeId = "{9FFED8C5-A872-4ADF-BC80-74BA1FE92E95}";
    inline constexpr const char* ClaudeModelTextCompletionsTypeId = "{0310426D-3035-4240-B9E5-D1556AF98B47}";
    inline constexpr const char* OllamaChatModelConfigurationTypeId = "{CE2D1912-94EE-4BCD-9015-E118900F4051}";
    inline constexpr const char* OllamaChatModelTypeId = "{E82C0C54-0658-419A-8170-033EF06AEA96}";
    inline constexpr const char* OllamaModelConfigurationTypeId = "{C5FA34FF-2BBB-4DF4-9AA2-C27320B57F93}";
    inline constexpr const char* OllamaModelTypeId = "{3782988D-058F-4943-9862-874EBC90A240}";

    // Providers TypeIds
    inline constexpr const char* ClaudeHttpProviderConfigurationTypeId = "{4822D5EC-F30A-4402-9FCE-48826B3183BF}";
    inline constexpr const char* ClaudeHttpProviderTypeId = "{FFA395BC-1D5B-46E3-B557-10FF550CFB34}";
    inline constexpr const char* OllamaHttpServiceConfigurationTypeId = "{18DDBA8F-1FE5-4DDE-A0F4-90DCCBDA5F33}";
    inline constexpr const char* OllamaHttpServiceComponentTypeId = "{AAD62D35-C628-4141-B759-0D3764013B29}";
} // namespace GenAIVendorBundle
