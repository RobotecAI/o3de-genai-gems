/*
 * Copyright (c) Contributors to the Open 3D Engine Project.
 * For complete copyright and license terms please see the LICENSE at the root of this distribution.
 *
 * SPDX-License-Identifier: Apache-2.0 OR MIT
 *
 */

#pragma once

#include <AzCore/std/containers/vector.h>
#include <AzCore/std/string/string.h>

namespace AICore
{
    struct ArgumentFormatterHelper
    {
        AZStd::string m_argumentType;
        AZStd::string m_argumentName;
        AZStd::string m_documentation;
        static constexpr const char* m_thisArgument{ "_this" };
        static constexpr const char* m_ebusIdArgument{ "_ebusid" };
    };

    struct MethodFormatterHelper
    {
        bool m_isEbus{ false };
        AZStd::string m_method;
        AZStd::string m_class;
        AZStd::string m_eventType;
        AZStd::string m_result; // empty means void or none or whatever no result signifier is
        AZStd::string m_documentation;
        AZStd::vector<ArgumentFormatterHelper> m_arguments;
    };

    struct EbusFormatterHelper
    {
        AZStd::string m_ebus;
        AZStd::string m_module;
        AZStd::vector<MethodFormatterHelper> m_events;
    };

    struct ClassFormatterHelper
    {
        AZStd::string m_class;
        AZStd::string m_module;
        AZStd::vector<MethodFormatterHelper> m_methods;
    };
} // namespace AICore
