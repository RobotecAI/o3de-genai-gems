/*
 * Copyright (c) Contributors to the Open 3D Engine Project.
 * For complete copyright and license terms please see the LICENSE at the root of this distribution.
 *
 * SPDX-License-Identifier: Apache-2.0 OR MIT
 *
 */

#include "AICoreEditorSettingsRegistryManager.h"

namespace AICore
{
    AICoreEditorSettingsRegistryManager::AICoreEditorSettingsRegistryManager()
        : AICoreSettingsRegistryManager()
    {
        AZ::IO::FixedMaxPath projectPath = AZ::Utils::GetProjectPath();
        projectPath /= "Registry";
        m_AICoreSettingsRegistryFilePath = projectPath;
        m_AICoreSettingsRegistryFilePath /= "aicoresystemconfiguration.setreg";
    }

    void AICoreEditorSettingsRegistryManager::SaveSystemConfiguration(
        const SaveObject& saveObject, const OnConfigSaveComplete& saveCallback)
    {
        bool sourceControlActive = false;
        AzToolsFramework::SourceControlConnectionRequestBus::BroadcastResult(
            sourceControlActive, &AzToolsFramework::SourceControlConnectionRequests::IsActive);

        // If Source Control is active then use it to check out the file before saving
        // otherwise query the file info and save only if the file is not read-only
        auto SourceControlSaveCallback = [sourceControlActive](
                                             AzToolsFramework::SourceControlCommands* sourceControlCommands,
                                             const char* filePath,
                                             const AzToolsFramework::SourceControlResponseCallback& configurationSaveCallback)
        {
            if (sourceControlActive)
            {
                sourceControlCommands->RequestEdit(filePath, true, configurationSaveCallback);
            }
            else
            {
                sourceControlCommands->GetFileInfo(filePath, configurationSaveCallback);
            }
        };

        rapidjson::Document aiCoreConfigurationDocument;
        rapidjson::Value& aiCoreConfigurationValue =
            rapidjson::CreateValueByPointer(aiCoreConfigurationDocument, rapidjson::Pointer(m_AICoreSettingsRegistryPath.c_str()));

        AZ::JsonSerialization::Store(aiCoreConfigurationValue, aiCoreConfigurationDocument.GetAllocator(), saveObject);

        auto postSaveCallback = [saveObject, saveCallback](bool result)
        {
            if (saveCallback)
            {
                saveCallback(saveObject, result ? Result::Success : Result::Failed);
            }
        };

        AzToolsFramework::SourceControlCommandBus::Broadcast(
            SourceControlSaveCallback,
            m_AICoreSettingsRegistryFilePath.c_str(),
            Internal::GetConfigurationSaveCallback(Internal::WriteDocumentToString(aiCoreConfigurationDocument), postSaveCallback));
    }

} // namespace AICore
