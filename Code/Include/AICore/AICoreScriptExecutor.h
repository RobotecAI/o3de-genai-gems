/*
 * Copyright (c) Contributors to the Open 3D Engine Project.
 * For complete copyright and license terms please see the LICENSE at the root of this distribution.
 *
 * SPDX-License-Identifier: Apache-2.0 OR MIT
 *
 */

#pragma once

#include <AzCore/std/string/string.h>

namespace AICore
{   // TODO - API to pass / modify behavior context
    class AICoreScriptExecutor
    {
    public:
        virtual bool ScriptCall(const AZStd::string& script, AZStd::string& response) = 0;
        virtual bool SupportsCurrentScope() const = 0;
        virtual ~AICoreScriptExecutor() = default;
    };
} // namespace AICore
