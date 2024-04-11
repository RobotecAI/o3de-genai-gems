/*
 * Copyright (c) Contributors to the Open 3D Engine Project.
 * For complete copyright and license terms please see the LICENSE at the root of this distribution.
 *
 * SPDX-License-Identifier: Apache-2.0 OR MIT
 *
 */

#pragma once

#include <AzCore/RTTI/BehaviorContext.h>
#include <AzCore/std/containers/vector.h>
#include <AzCore/std/string/string.h>

namespace GenAIFramework
{
    struct ArgumentFormatterHelper
    {
        AZStd::string m_argumentType;
        AZStd::string m_argumentName;
        AZStd::string m_documentation;
        static constexpr const char* ThisArgument{ "_this" };
        static constexpr const char* EbusIdArgument{ "_ebusid" };
    };

    struct MethodFormatterHelper
    {
        bool m_isEbus{ false };
        AZStd::string m_method;
        AZ::BehaviorMethod* m_behaviourMethod;
        AZ::BehaviorClass* m_behaviourClass;
        bool m_dumpTooltips{ true };
        bool m_dumpDebugDescription{ true };
    };

    struct EbusFormatterHelper
    {
        AZStd::string m_busName;
        AZ::BehaviorEBus* m_behaviourEBus;
        AZStd::string m_module;
    };

    struct ClassFormatterHelper
    {
        AZStd::string m_class;
        AZStd::string m_module;
        AZ::BehaviorClass* m_behaviourClass;
        bool m_dumpMethods{ true };
        bool m_dumpProperties{ true };
        bool m_dumpTooltips{ true };
    };
} // namespace GenAIFramework
