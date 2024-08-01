/*
 * Copyright (c) Contributors to the Open 3D Engine Project.
 * For complete copyright and license terms please see the LICENSE at the root of this distribution.
 *
 * SPDX-License-Identifier: Apache-2.0 OR MIT
 *
 */

#pragma once

#include "Clients/GenAIFrameworkSystemComponentConfiguration.h"

#include <AzCore/std/optional.h>
#include <AzCore/std/string/string_view.h>

namespace GenAIFramework
{
    class SettingsRegistryManager
    {
    public:
        SettingsRegistryManager() = default;
        ~SettingsRegistryManager() = default;

        using SaveObject = GenAIFrameworkSystemComponentConfiguration;
        using OnConfigSaveComplete = AZStd::function<void(AZ::Outcome<void, void>)>;

        void SaveSystemConfiguration(const SaveObject& saveObject, const OnConfigSaveComplete& saveCallback);
        AZStd::optional<SaveObject> LoadSystemConfiguration() const;

    protected:
        constexpr static AZStd::string_view GenAIFrameworkSettingsRegistryPath = "/Gems/GenAIFramework";
    };

} // namespace GenAIFramework
