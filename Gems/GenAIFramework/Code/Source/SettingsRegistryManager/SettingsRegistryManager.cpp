/*
 * Copyright (c) Contributors to the Open 3D Engine Project.
 * For complete copyright and license terms please see the LICENSE at the root of this distribution.
 *
 * SPDX-License-Identifier: Apache-2.0 OR MIT
 *
 */

#include "SettingsRegistryManager.h"
#include "AzCore/Outcome/Outcome.h"
#include <AzCore/Settings/SettingsRegistry.h>

namespace GenAIFramework
{
    void SettingsRegistryManager::SaveSystemConfiguration(
        [[maybe_unused]] const SaveObject& saveObject, const OnConfigSaveComplete& saveCallback)
    {
        if (saveCallback)
        {
            saveCallback(AZ::Failure());
        }
    }

    AZStd::optional<SettingsRegistryManager::SaveObject> SettingsRegistryManager::LoadSystemConfiguration() const
    {
        SaveObject saveObject;

        bool configurationRead = false;

        AZ::SettingsRegistryInterface* settingsRegistry = AZ::SettingsRegistry::Get();
        AZ_Assert(settingsRegistry, "Settings Registry is not available");
        if (settingsRegistry)
        {
            configurationRead = settingsRegistry->GetObject(saveObject, m_GenAIFrameworkSettingsRegistryPath);
        }
        AZ_Assert(configurationRead, "Failed to read GenAIFramework settings from the Settings Registry");
        if (configurationRead)
        {
            for (const auto& [id, e] : saveObject.m_serviceRequesters)
            {
                AZ_Printf("SettingsRegistryManager", "Service Requester: %s, %llu\n", e->GetName().c_str(), id);
            }
            for (const auto& [id, e] : saveObject.m_modelConfigurations)
            {
                AZ_Printf("SettingsRegistryManager", "Model configuration: %s, %llu\n", e->GetName().c_str(), id);
            }
            return saveObject;
        }
        return AZStd::nullopt;
    }

} // namespace GenAIFramework
