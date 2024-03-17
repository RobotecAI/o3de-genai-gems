/*
 * Copyright (c) Contributors to the Open 3D Engine Project.
 * For complete copyright and license terms please see the LICENSE at the root of this distribution.
 *
 * SPDX-License-Identifier: Apache-2.0 OR MIT
 *
 */

#include "GenAIFrameworkSettingsRegistryManager.h"
#include "AzCore/Outcome/Outcome.h"
#include <AzCore/Settings/SettingsRegistry.h>

namespace GenAIFramework
{
    void GenAIFrameworkSettingsRegistryManager::SaveSystemConfiguration(const SaveObject& saveObject, const OnConfigSaveComplete& saveCallback)
    {
        if (saveCallback)
        {
            saveCallback(AZ::Failure());
        }
    }

    AZStd::optional<GenAIFrameworkSettingsRegistryManager::SaveObject> GenAIFrameworkSettingsRegistryManager::LoadSystemConfiguration() const
    {
        SaveObject saveObject;

        bool configurationRead = false;

        AZ::SettingsRegistryInterface* settingsRegistry = AZ::SettingsRegistry::Get();
        if (settingsRegistry)
        {
            configurationRead = settingsRegistry->GetObject(saveObject, m_GenAIFrameworkSettingsRegistryPath);
        }

        if (configurationRead)
        {
            return saveObject;
        }
        return AZStd::nullopt;
    }

} // namespace GenAIFramework
