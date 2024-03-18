/*
 * Copyright (c) Contributors to the Open 3D Engine Project.
 * For complete copyright and license terms please see the LICENSE at the root of this distribution.
 *
 * SPDX-License-Identifier: Apache-2.0 OR MIT
 *
 */

#include <GenAIFramework/AIContext.h>
#include <GenAIFramework/GenAIFrameworkActionBus.h>
#include <AzCore/std/string/string.h>

#pragma once

namespace GenAIFramework
{
    class GenAIFrameworkActionTest
    {
    public:
        bool ScriptCall(const AZStd::string& script, AZStd::string& response);
        AIContext GetAIContext() const;

    private:
        bool m_init{ false };
        AIContext m_aiContext;
    };
} // namespace GenAIFramework
