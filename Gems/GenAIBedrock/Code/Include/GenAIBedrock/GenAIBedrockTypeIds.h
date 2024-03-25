/*
 * Copyright (c) Contributors to the Open 3D Engine Project.
 * For complete copyright and license terms please see the LICENSE at the root of this distribution.
 *
 * SPDX-License-Identifier: Apache-2.0 OR MIT
 *
 */

#pragma once

namespace GenAIBedrock
{
    // System Component TypeIds
    inline constexpr const char* GenAIBedrockSystemComponentTypeId = "{3C459E4F-4E09-4956-9FAA-7C8B1489AADC}";
    inline constexpr const char* GenAIBedrockEditorSystemComponentTypeId = "{DD207F4F-636C-4D75-A1EA-DFA47557F64D}";

    // Module derived classes TypeIds
    inline constexpr const char* GenAIBedrockModuleInterfaceTypeId = "{B24E260E-415F-4834-8B84-FD39C8662069}";
    inline constexpr const char* GenAIBedrockModuleTypeId = "{831DA4EE-0990-4459-ADE4-35B7791DA6FC}";
    // The Editor Module by default is mutually exclusive with the Client Module
    // so they use the Same TypeId
    inline constexpr const char* GenAIBedrockEditorModuleTypeId = GenAIBedrockModuleTypeId;

    // Interface TypeIds
    inline constexpr const char* GenAIBedrockRequestsTypeId = "{4CBDCE2B-1857-4329-9B14-34C5A75EC407}";
} // namespace GenAIBedrock
