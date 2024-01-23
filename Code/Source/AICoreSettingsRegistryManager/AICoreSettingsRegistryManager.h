/*
 * Copyright (c) Contributors to the Open 3D Engine Project.
 * For complete copyright and license terms please see the LICENSE at the root of this distribution.
 *
 * SPDX-License-Identifier: Apache-2.0 OR MIT
 *
 */

#pragma once

#include "Clients/AICoreSystemComponentConfiguration.h"
#include <AzCore/Component/Entity.h>
#include <AzCore/std/containers/map.h>
#include <AzCore/std/optional.h>

namespace AICore
{
    class AICoreSettingsRegistryManager
    {
    public:
        AICoreSettingsRegistryManager();
        ~AICoreSettingsRegistryManager() = default;

        enum class Result : AZ::u8
        {
            Success,
            Failed
        };

        using SaveObject = AICoreSystemComponentConfiguration;
        using OnConfigSaveComplete = AZStd::function<void(const SaveObject&, Result)>;

        void SaveSystemConfiguration(const SaveObject& saveObject, const OnConfigSaveComplete& saveCallback);
        AZStd::optional<SaveObject> LoadSystemConfiguration() const;

    protected:
        AZStd::string m_AICoreSettingsRegistryPath;
    };

} // namespace AICore
