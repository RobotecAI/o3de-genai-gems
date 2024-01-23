/*
 * Copyright (c) Contributors to the Open 3D Engine Project.
 * For complete copyright and license terms please see the LICENSE at the root of this distribution.
 *
 * SPDX-License-Identifier: Apache-2.0 OR MIT
 *
 */

#include "AICoreSettingsRegistryManager.h"
#include <AzCore/Settings/SettingsRegistry.h>

namespace AICore
{
    AICoreSettingsRegistryManager::AICoreSettingsRegistryManager()
    {
        m_AICoreSettingsRegistryPath = AZStd::string::format(
            "%s/Gems/"
            "AICore",
            "/Robotec.ai");
    }

    void AICoreSettingsRegistryManager::SaveSystemConfiguration(const SaveObject& saveObject, const OnConfigSaveComplete& saveCallback)
    {
        if (saveCallback)
        {
            saveCallback(saveObject, Result::Failed);
        }
    }

    AZStd::optional<AICoreSettingsRegistryManager::SaveObject> AICoreSettingsRegistryManager::LoadSystemConfiguration() const
    {
        SaveObject saveObject;

        bool configurationRead = false;

        AZ::SettingsRegistryInterface* settingsRegistry = AZ::SettingsRegistry::Get();
        if (settingsRegistry)
        {
            configurationRead = settingsRegistry->GetObject(saveObject, m_AICoreSettingsRegistryPath);
        }

        if (configurationRead)
        {
            return saveObject;
        }
        return AZStd::nullopt;
    }

} // namespace AICore
