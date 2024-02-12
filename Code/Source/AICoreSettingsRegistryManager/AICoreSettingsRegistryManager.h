
#pragma once

#include "AzCore/std/smart_ptr/shared_ptr.h"
#include "AzCore/std/string/string.h"
#include "Communication/JSONHttp/BasicJSONRequester.h"
#include <AzCore/IO/ByteContainerStream.h>
#include <AzCore/IO/Path/Path.h>
#include <AzCore/IO/SystemFile.h>
#include <AzCore/IO/TextStreamWriters.h>
#include <AzCore/JSON/document.h>
#include <AzCore/JSON/pointer.h>
#include <AzCore/JSON/prettywriter.h>
#include <AzCore/Serialization/Json/JsonSerialization.h>
#include <AzCore/Settings/SettingsRegistry.h>
#include <AzCore/Settings/SettingsRegistryMergeUtils.h>
#include <AzCore/Utils/Utils.h>
#include <AzCore/base.h>
#include <AzCore/std/functional.h>
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

    class AICoreSettingsRegistryManager
    {
    public:
        AICoreSettingsRegistryManager()
        {
            AZ::IO::FixedMaxPath projectPath = AZ::Utils::GetProjectPath();
            projectPath /= "Registry";

            m_AICoreSettingsRegistryPath = projectPath;
            m_AICoreSettingsRegistryPath /= "aicoresystemconfiguration.setreg";
        }
        ~AICoreSettingsRegistryManager() = default;

        enum class Result : AZ::u8
        {
            Success,
            Failed
        };
        template<typename Object>
        using OnConfigSaveComplete = AZStd::function<void(const Object&, Result)>;

        template<typename Object>
        void SaveSystemConfiguration(const Object& object, const AZStd::string& id, const OnConfigSaveComplete<Object>& saveCallback)
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

            AZStd::string settingsRegistryPath = AZStd::string::format(
                "%s/Gems/"
                "AICore"
                "/Requester"
                "/%s",
                "/Robotec.ai",
                id.c_str());

            rapidjson::Document aiCoreConfigurationDocument;
            rapidjson::Value& aiCoreConfigurationValue =
                rapidjson::CreateValueByPointer(aiCoreConfigurationDocument, rapidjson::Pointer(settingsRegistryPath.c_str()));
            // aiCoreConfigurationValue.
            AZ::JsonSerialization::Store(aiCoreConfigurationValue, aiCoreConfigurationDocument.GetAllocator(), object);
            // rapidjson::Value::ConstMemberIterator itr = aiCoreConfigurationDocument.FindMember(BasicJSONRequester::TYPEINFO_Name());
            // if (itr != aiCoreConfigurationDocument.MemberEnd())
            // {
            //     rapidjson::Value typeKey("type", aiCoreConfigurationDocument.GetAllocator());
            //     rapidjson::Value typeValue(
            //         BasicJSONRequester::TYPEINFO_Uuid().ToFixedString().c_str(), aiCoreConfigurationDocument.GetAllocator());
            //     itr->value.AddMember(
            //         "type", BasicJSONRequester::TYPEINFO_Uuid().ToFixedString().c_str(), aiCoreConfigurationDocument.GetAllocator());
            // }

            auto postSaveCallback = [object, saveCallback](bool result)
            {
                if (saveCallback)
                {
                    saveCallback(object, result ? Result::Success : Result::Failed);
                }
            };

            AzToolsFramework::SourceControlCommandBus::Broadcast(
                SourceControlSaveCallback,
                m_AICoreSettingsRegistryPath.c_str(),
                Internal::GetConfigurationSaveCallback(Internal::WriteDocumentToString(aiCoreConfigurationDocument), postSaveCallback));
        }

        // template<typename Object>
        // AZStd::vector<AZStd::shared_ptr<Object>> LoadSystemConfiguration()
        // {

        // }

    private:
        AZ::IO::FixedMaxPath m_AICoreSettingsRegistryPath;
    };

} // namespace AICore