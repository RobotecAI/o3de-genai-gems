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
} // namespace GenAIVendorBundle
