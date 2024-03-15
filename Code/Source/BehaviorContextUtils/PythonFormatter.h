/*
 * Copyright (c) Contributors to the Open 3D Engine Project.
 * For complete copyright and license terms please see the LICENSE at the root of this distribution.
 *
 * SPDX-License-Identifier: Apache-2.0 OR MIT
 *
 */
#pragma once

#include <AICore/APIFormatter.h>
#include <EditorPythonBindings/PythonUtility.h>

namespace AICore
{
    class PythonFormatter : public APIFormatter
    {
    public:
        AZStd::string FormatMethod(const MethodFormatterHelper& methodHelper) override
        {
            AZStd::string methodDump = "";
            methodDump += m_pythonBehaviorDescription.MethodDefinition(
                methodHelper.m_method,
                *methodHelper.m_behaviourMethod,
                methodHelper.m_behaviourClass,
                methodHelper.m_dumpTooltips,
                methodHelper.m_dumpDebugDescription);

            return methodDump;
        }

        AZStd::string FormatEbus(const EbusFormatterHelper& ebusHelper) override
        {
            AZStd::string busDefinition = m_pythonBehaviorDescription.BusDefinition(ebusHelper.m_busName, ebusHelper.m_behaviourEBus);
            AZStd::string ebusDump = AZStd::string::format("Module %s, EBus \n%s\n", ebusHelper.m_module.c_str(), busDefinition.c_str());

            return ebusDump;
        }

        AZStd::string FormatClass(const ClassFormatterHelper& classHelper) override
        {
            AZStd::string classDefinition = m_pythonBehaviorDescription.ClassDefinition(
                classHelper.m_behaviourClass,
                classHelper.m_class,
                classHelper.m_dumpProperties,
                classHelper.m_dumpMethods,
                classHelper.m_dumpTooltips);
            AZStd::string classDump = AZStd::string::format("Module %s\n%s", classHelper.m_module.c_str(), classDefinition.c_str());

            return classDump;
        }

    private:
        EditorPythonBindings::Text::PythonBehaviorDescription m_pythonBehaviorDescription;
    };
} // namespace AICore
