/*
 * Copyright (c) Contributors to the Open 3D Engine Project.
 * For complete copyright and license terms please see the LICENSE at the root of this distribution.
 *
 * SPDX-License-Identifier: Apache-2.0 OR MIT
 *
 */

#pragma once

#include <GenAIFramework/APIFormatterStructs.h>

namespace GenAIFramework
{
    class APIFormatter
    {
    public:
        virtual ~APIFormatter() = default;
        virtual AZStd::string FormatMethod(const MethodFormatterHelper& methodHelper) = 0;
        virtual AZStd::string FormatEbus(const EbusFormatterHelper& ebusHelper) = 0;
        virtual AZStd::string FormatClass(const ClassFormatterHelper& ebusHelper) = 0;
    };
} // namespace GenAIFramework
