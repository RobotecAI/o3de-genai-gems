/*
 * Copyright (c) Contributors to the Open 3D Engine Project.
 * For complete copyright and license terms please see the LICENSE at the root of this distribution.
 *
 * SPDX-License-Identifier: Apache-2.0 OR MIT
 *
 */

#pragma once

#include <AICoreSettingsRegistryManager/AICoreSettingsRegistryManager.h>
#include <AzCore/IO/ByteContainerStream.h>
#include <AzCore/IO/TextStreamWriters.h>
#include <AzCore/JSON/pointer.h>
#include <AzCore/JSON/prettywriter.h>
#include <AzCore/Utils/Utils.h>
#include <AzToolsFramework/SourceControl/SourceControlAPI.h>

namespace AICore
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
                    postSaveCB = AZStd::move(postSaveCallback)](bool, const AzToolsFramework::SourceControlFileInfo& info)
            {
                if (info.IsLockedByOther())
                {
                    AZ_Warning("AICore", false, R"(The file "%s" already exclusively opened by another user)", info.m_filePath.c_str());
                    return;
                }
                else if (info.IsReadOnly() && AZ::IO::SystemFile::Exists(info.m_filePath.c_str()))
                {
                    AZ_Warning("AICore", false, R"(The file "%s" is read-only)", info.m_filePath.c_str());
                    return;
                }

                bool saved = false;
                constexpr auto configurationMode =
                    AZ::IO::SystemFile::SF_OPEN_CREATE | AZ::IO::SystemFile::SF_OPEN_CREATE_PATH | AZ::IO::SystemFile::SF_OPEN_WRITE_ONLY;
                if (AZ::IO::SystemFile outputFile; outputFile.Open(info.m_filePath.c_str(), configurationMode))
                {
                    saved = outputFile.Write(payloadBuffer.data(), payloadBuffer.size()) == payloadBuffer.size();
                }

                AZ_Warning("AICore", saved, "Failed to save AICore configuration");
                if (postSaveCB)
                {
                    postSaveCB(saved);
                }
            };
        }
    } // namespace Internal

    class AICoreEditorSettingsRegistryManager : public AICoreSettingsRegistryManager
    {
    public:
        AICoreEditorSettingsRegistryManager();
        ~AICoreEditorSettingsRegistryManager() = default;

        void SaveSystemConfiguration(const SaveObject& saveObject, const OnConfigSaveComplete& saveCallback);

    private:
        AZ::IO::FixedMaxPath m_AICoreSettingsRegistryFilePath;
    };

} // namespace AICore
