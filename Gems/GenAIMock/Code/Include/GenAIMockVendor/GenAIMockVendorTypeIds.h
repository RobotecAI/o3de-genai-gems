/*
 * Copyright (c) Contributors to the Open 3D Engine Project.
 * For complete copyright and license terms please see the LICENSE at the root of this distribution.
 *
 * SPDX-License-Identifier: Apache-2.0 OR MIT
 *
 */

#pragma once

namespace GenAIMockVendor
{
    // System Component TypeIds
    inline constexpr const char* GenAIMockVendorSystemComponentTypeId = "{302345A1-F4C2-481E-B7A5-F86A49976DFF}";
    inline constexpr const char* GenAIMockVendorEditorSystemComponentTypeId = "{60230B38-1389-4642-A658-163171D639DE}";

    // Module derived classes TypeIds
    inline constexpr const char* GenAIMockVendorModuleInterfaceTypeId = "{6655F958-25D8-48C4-B51E-3FA4E3DF1BC9}";
    inline constexpr const char* GenAIMockVendorModuleTypeId = "{BBEA3144-EF92-4818-856E-09F9DE65858D}";
    // The Editor Module by default is mutually exclusive with the Client Module
    // so they use the Same TypeId
    inline constexpr const char* GenAIMockVendorEditorModuleTypeId = GenAIMockVendorModuleTypeId;

    // Interface TypeIds
    inline constexpr const char* GenAIMockVendorRequestsTypeId = "{F51BC484-EC06-4B28-A2EB-745E1B948EE0}";

    // Communication TypeIds
    inline constexpr const char* MockServiceComponentConfigurationTypeId = "{47AB736F-33F1-453B-9CDB-96FAFFD5D33E}";
    inline constexpr const char* MockServiceComponentTypeId = "{DB01BE29-EC0B-41E1-BF68-12D70DD6B630}";

    // ModelConfigurations TypeIds
    inline constexpr const char* MockModelComponentTypeId = "{B873470C-0CA8-49AF-82B6-4DE26C242C7B}";
} // namespace GenAIMockVendor
