/*
 * Copyright (c) Contributors to the Open 3D Engine Project.
 * For complete copyright and license terms please see the LICENSE at the root of this distribution.
 *
 * SPDX-License-Identifier: Apache-2.0 OR MIT
 *
 */
#pragma once

#include <AICore/APIFormatter.h>

namespace AICore
{
    class PythonFormatter : public APIFormatter
    {
    public:
        AZStd::string FormatMethod(const MethodFormatterHelper& methodHelper) override
        {
            AZStd::string methodDump = AZStd::string::format("def %s(", methodHelper.m_method.c_str());
            AZStd::string argumentsDocumentation;
            bool subsequent = false;
            for (const auto& argument : methodHelper.m_arguments)
            {
                AZStd::string argumentString = argument.m_argumentType;
                const auto& name = argument.m_argumentName;
                if (!name.empty())
                {
                    if (name == ArgumentFormatterHelper::m_thisArgument)
                    {
                        argumentString = "self";
                    }
                    else if (name == ArgumentFormatterHelper::m_ebusIdArgument)
                    {
                        argumentString = "ebusID";
                    }
                    else
                    {
                        argumentString = name + ": " + argumentString;
                    }
                }
                if (subsequent)
                {
                    argumentString = ", " + argumentString;
                }
                subsequent = true;

                if (!argument.m_documentation.empty())
                {
                    argumentsDocumentation += AZStd::string::format("\t'''arg: %s'''\n", argument.m_documentation.c_str());
                }

                methodDump += argumentString;
            }
            methodDump += AZStd::string::format(") -> %s:\n", methodHelper.m_result.empty() ? "None" : methodHelper.m_result.c_str());
            if (!methodHelper.m_documentation.empty())
            {
                methodDump += AZStd::string::format("\t'''%s'''\n", methodHelper.m_documentation.c_str());
            }
            if (!argumentsDocumentation.empty())
            {
                methodDump += argumentsDocumentation;
            }

            methodDump += "\tpass\n";
            return methodDump;
        }

        AZStd::string FormatEbus(const EbusFormatterHelper& ebusHelper) override
        {
            AZStd::string ebusDump = AZStd::string::format(
                "Module %s, EBus %s\n%s\n", ebusHelper.m_module.c_str(), ebusHelper.m_ebus.c_str(), ebusHelper.m_documentation.c_str());
            for (const auto& event : ebusHelper.m_events)
            {
                ebusDump += FormatMethod(event) + "\n";
            }
            return ebusDump;
        }

        AZStd::string FormatClass(const ClassFormatterHelper& classHelper) override
        {
            AZStd::string classDump = AZStd::string::format(
                "Module %s, Class %s\n",
                classHelper.m_module.c_str(),
                classHelper.m_class.c_str());
            for (const auto& method : classHelper.m_methods)
            {
                classDump += FormatMethod(method) + "\n";
            }
            return classDump;
        }
    };
} // namespace AICore
