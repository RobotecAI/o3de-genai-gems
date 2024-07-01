/*
 * Copyright (c) Contributors to the Open 3D Engine Project.
 * For complete copyright and license terms please see the LICENSE at the root of this distribution.
 *
 * SPDX-License-Identifier: Apache-2.0 OR MIT
 *
 */

#pragma once

#include <SettingsRegistryManager/SettingsRegistryManager.h>

#include <AzCore/IO/ByteContainerStream.h>
#include <AzCore/IO/TextStreamWriters.h>
#include <AzCore/JSON/pointer.h>
#include <AzCore/JSON/prettywriter.h>
#include <AzCore/Utils/Utils.h>
#include <AzToolsFramework/SourceControl/SourceControlAPI.h>

namespace GenAIFramework
{
    namespace Internal
    {
        inline AZStd::string WriteDocumentToString(const rapidjson::Document& document)
        {
            AZStd::string stringBuffer;
            AZ::IO::ByteContainerStream stringStream(&stringBuffer);
            AZ::IO::RapidJSONStreamWriter stringWriter(&stringStream);
            rapidjson::PrettyWriter writer(stringWriter);
            document.Accept(writer);
            return stringBuffer;
        }

        inline AzToolsFramework::SourceControlResponseCallback GetConfigurationSaveCallback(
            AZStd::string configurationPayload, AZStd::function<void(bool)> postSaveCallback)
        {
            return [payloadBuffer = AZStd::move(configurationPayload),
                    postSaveCB = AZStd::move(postSaveCallback)](bool success, const AzToolsFramework::SourceControlFileInfo& info)
            {
                if (info.IsLockedByOther())
                {
                    AZ_Warning(
                        "GenAIFramework", false, R"(The file "%s" already exclusively opened by another user)", info.m_filePath.c_str());
                    return;
                }
                else if (info.IsReadOnly() && AZ::IO::SystemFile::Exists(info.m_filePath.c_str()))
                {
                    AZ_Warning("GenAIFramework", false, R"(The file "%s" is read-only)", info.m_filePath.c_str());
                    return;
                }

                bool saved = false;
                constexpr auto configurationMode =
                    AZ::IO::SystemFile::SF_OPEN_CREATE | AZ::IO::SystemFile::SF_OPEN_CREATE_PATH | AZ::IO::SystemFile::SF_OPEN_WRITE_ONLY;
                if (AZ::IO::SystemFile outputFile; outputFile.Open(info.m_filePath.c_str(), configurationMode))
                {
                    saved = outputFile.Write(payloadBuffer.data(), payloadBuffer.size()) == payloadBuffer.size();
                }

                if (!success)
                {
                    AZ_Warning("GenAIFramework", false, "Registry file operation failed");
                    return;
                }

                AZ_Warning("GenAIFramework", saved, "Failed to save GenAIFramework configuration");
                if (postSaveCB)
                {
                    postSaveCB(saved);
                }
            };
        }
    } // namespace Internal

    class EditorSettingsRegistryManager : public SettingsRegistryManager
    {
    public:
        EditorSettingsRegistryManager();
        ~EditorSettingsRegistryManager() = default;

        void SaveSystemConfiguration(const SaveObject& saveObject, const OnConfigSaveComplete& saveCallback);

    private:
        AZ::IO::FixedMaxPath m_settingsRegistryFilePath;
    };

} // namespace GenAIFramework
