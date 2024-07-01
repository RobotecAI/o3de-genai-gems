/*
 * Copyright (c) Contributors to the Open 3D Engine Project.
 * For complete copyright and license terms please see the LICENSE at the root of this distribution.
 *
 * SPDX-License-Identifier: Apache-2.0 OR MIT
 *
 */

#pragma once

namespace GenAIAmazonBedrockVendorBundle
{
    // Module derived classes TypeIds
    inline constexpr const char* GenAIAmazonBedrockVendorBundleModuleInterfaceTypeId = "{95D48163-CEB9-4554-BD90-342BDBD582D6}";
    inline constexpr const char* GenAIAmazonBedrockVendorBundleModuleTypeId = "{01C381E1-AC5E-4524-8E8E-A2335D0CD073}";
    // The Editor Module by default is mutually exclusive with the Client Module
    // so they use the Same TypeId
    inline constexpr const char* GenAIAmazonBedrockVendorBundleEditorModuleTypeId = GenAIAmazonBedrockVendorBundleModuleTypeId;

    // Interface TypeIds
    inline constexpr const char* GenAIAmazonBedrockVendorBundleRequestsTypeId = "{6C8FBE18-3AC9-4436-B2E7-7BA943C26D69}";

    // Providers/Claude TypeIds
    inline constexpr const char* ClaudeAmazonBedrockProviderConfigurationTypeId = "{769166CB-BD18-4FB0-B9FF-93DBAAAF82A8}";
    inline constexpr const char* ClaudeAmazonBedrockProviderTypeId = "{77F1B8E1-E616-44C0-A6C6-9D1BD26CF751}";
} // namespace GenAIAmazonBedrockVendorBundle
