/*
 * Copyright (c) Contributors to the Open 3D Engine Project.
 * For complete copyright and license terms please see the LICENSE at the root of this distribution.
 *
 * SPDX-License-Identifier: Apache-2.0 OR MIT
 *
 */

#pragma once

namespace GenAIOllama
{
    // System Component TypeIds
    inline constexpr const char* GenAIOllamaSystemComponentTypeId = "{51817EF0-FAF6-4AFA-95B5-086F80F31F6D}";
    inline constexpr const char* GenAIOllamaEditorSystemComponentTypeId = "{CA7A9C17-CD08-4AAD-ABCD-D7905E4E83CB}";

    // Module derived classes TypeIds
    inline constexpr const char* GenAIOllamaModuleInterfaceTypeId = "{70265B24-71C7-4AC9-B72A-85477FD2664D}";
    inline constexpr const char* GenAIOllamaModuleTypeId = "{ADD45667-EF00-42A3-B13E-102395D635B1}";
    // The Editor Module by default is mutually exclusive with the Client Module
    // so they use the Same TypeId
    inline constexpr const char* GenAIOllamaEditorModuleTypeId = GenAIOllamaModuleTypeId;

    // Interface TypeIds
    inline constexpr const char* GenAIOllamaRequestsTypeId = "{33DF318C-3947-4A18-B86C-6E44CD7A11A8}";
} // namespace GenAIOllama
