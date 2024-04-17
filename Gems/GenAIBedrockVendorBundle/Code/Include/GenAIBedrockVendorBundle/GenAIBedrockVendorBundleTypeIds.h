/*
 * Copyright (c) Contributors to the Open 3D Engine Project.
 * For complete copyright and license terms please see the LICENSE at the root of this distribution.
 *
 * SPDX-License-Identifier: Apache-2.0 OR MIT
 *
 */

#pragma once

namespace GenAIBedrockVendorBundle
{
    // Module derived classes TypeIds
    inline constexpr const char* GenAIBedrockVendorBundleModuleInterfaceTypeId = "{95D48163-CEB9-4554-BD90-342BDBD582D6}";
    inline constexpr const char* GenAIBedrockVendorBundleModuleTypeId = "{01C381E1-AC5E-4524-8E8E-A2335D0CD073}";
    // The Editor Module by default is mutually exclusive with the Client Module
    // so they use the Same TypeId
    inline constexpr const char* GenAIBedrockVendorBundleEditorModuleTypeId = GenAIBedrockVendorBundleModuleTypeId;

    // Interface TypeIds
    inline constexpr const char* GenAIBedrockVendorBundleRequestsTypeId = "{6C8FBE18-3AC9-4436-B2E7-7BA943C26D69}";
} // namespace GenAIBedrockVendorBundle
