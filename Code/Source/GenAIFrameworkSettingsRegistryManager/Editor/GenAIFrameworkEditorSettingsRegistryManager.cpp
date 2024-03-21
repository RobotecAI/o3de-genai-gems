/*
 * Copyright (c) Contributors to the Open 3D Engine Project.
 * For complete copyright and license terms please see the LICENSE at the root of this distribution.
 *
 * SPDX-License-Identifier: Apache-2.0 OR MIT
 *
 */

#include "GenAIFrameworkEditorSettingsRegistryManager.h"
#include "AzCore/Outcome/Outcome.h"

namespace GenAIFramework
{
    GenAIFrameworkEditorSettingsRegistryManager::GenAIFrameworkEditorSettingsRegistryManager()
        : GenAIFrameworkSettingsRegistryManager()
    {
        AZ::IO::FixedMaxPath projectPath = AZ::Utils::GetProjectPath();
        projectPath /= "Registry";
        m_GenAIFrameworkSettingsRegistryFilePath = projectPath;
        m_GenAIFrameworkSettingsRegistryFilePath /= "genaiframeworksystemconfiguration.setreg";
    }

    void GenAIFrameworkEditorSettingsRegistryManager::SaveSystemConfiguration(
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

        rapidjson::Document GenAIFrameworkConfigurationDocument;
        rapidjson::Value& GenAIFrameworkConfigurationValue = rapidjson::CreateValueByPointer(
            GenAIFrameworkConfigurationDocument, rapidjson::Pointer(m_GenAIFrameworkSettingsRegistryPath.data()));

        AZ::JsonSerialization::Store(GenAIFrameworkConfigurationValue, GenAIFrameworkConfigurationDocument.GetAllocator(), saveObject);

        auto postSaveCallback = [saveObject, saveCallback](bool result)
        {
            if (saveCallback)
            {
                if (result)
                {
                    saveCallback(AZ::Success());
                }
                else
                {
                    saveCallback(AZ::Failure());
                }
            }
        };

        AzToolsFramework::SourceControlCommandBus::Broadcast(
            SourceControlSaveCallback,
            m_GenAIFrameworkSettingsRegistryFilePath.c_str(),
            Internal::GetConfigurationSaveCallback(Internal::WriteDocumentToString(GenAIFrameworkConfigurationDocument), postSaveCallback));
    }

} // namespace GenAIFramework
