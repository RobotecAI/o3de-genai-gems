/*
* Copyright (c) Contributors to the Open 3D Engine Project.
* For complete copyright and license terms please see the LICENSE at the root of this distribution.
*
* SPDX-License-Identifier: Apache-2.0 OR MIT
*
*/

#pragma once

namespace AICore
{
    // System Component TypeIds
    inline constexpr const char* AICoreSystemComponentTypeId = "{C1447E9E-C102-4FDA-8D62-34BEE2DD4D31}";
    inline constexpr const char* AICoreEditorSystemComponentTypeId = "{CD496C8B-7D2A-447B-A1F9-B449A9BFEC04}";

    // Module derived classes TypeIds
    inline constexpr const char* AICoreModuleInterfaceTypeId = "{4ACED336-C28C-4163-996C-4645A0EF6510}";
    inline constexpr const char* AICoreModuleTypeId = "{C448193F-A197-481A-9CF5-73BBAD978E3A}";
    // The Editor Module by default is mutually exclusive with the Client Module
    // so they use the Same TypeId
    inline constexpr const char* AICoreEditorModuleTypeId = AICoreModuleTypeId;

    // Interface TypeIds
    inline constexpr const char* AICoreRequestsTypeId = "{3C39C889-9AA7-43F9-820A-DAF53095BAAD}";
} // namespace AICore
