/*
 * Copyright (c) Contributors to the Open 3D Engine Project.
 * For complete copyright and license terms please see the LICENSE at the root of this distribution.
 *
 * SPDX-License-Identifier: Apache-2.0 OR MIT
 *
 */

#pragma once

#include <GenAIFramework/Communication/AIModelRequestBus.h>

#include <AzCore/JSON/document.h>
#include <AzCore/JSON/rapidjson.h>
#include <AzCore/JSON/writer.h>

//! A collection of utilities to convert (back and forth) different message types into JSON strings for seamless Python integration.
namespace GenAIFramework::JsonUtils
{
    //! Convert a single message (GenAIFramework type) into JSON data (rapidjson library type)
    //! @param message a single message (GenAIFramework::message format)
    //! @param allocator allocator used in conversion (part of rapidjson library)
    //! @return a single message in JSON value (rapidjson library type)
    inline rapidjson::Value AIMessageToJsonValue(const AIMessage& message, rapidjson::Document::AllocatorType& allocator)
    {
        rapidjson::Value AIMessageJson(rapidjson::kObjectType);

        rapidjson::Value roleValue;
        switch (message.first)
        {
        case Role::User:
            roleValue.SetString("user", allocator);
            break;
        case Role::Assistant:
            roleValue.SetString("assistant", allocator);
            break;
        case Role::System:
            roleValue.SetString("system", allocator);
            break;
        default:
            AZ_Error("ModelAPIRequest", false, "Unsupported Role selected");
            break;
        }

        AIMessageJson.AddMember("role", roleValue, allocator);

        rapidjson::Value contentJson(rapidjson::kArrayType);
        for (const auto& content : message.second)
        {
            rapidjson::Value contentValue;
            if (content.type() == AZ::AzTypeInfo<AZStd::string>().Uuid())
            {
                contentValue.SetString(AZStd::any_cast<AZStd::string>(content).c_str(), allocator);

                contentJson.PushBack(contentValue, allocator);
            }
        }

        AIMessageJson.AddMember("content", contentJson, allocator);

        return AIMessageJson;
    }

    //! Convert a single message (GenAIFramework type) into text in JSON format
    //! @param message a single message (GenAIFramework::AIMessage format)
    //! @return a single message in JSON format (plain text)
    inline AZStd::string AIMessageToJsonString(const AIMessage& message)
    {
        rapidjson::Document AIMessageJson;
        rapidjson::Value AIMessageJsonValue = AIMessageToJsonValue(message, AIMessageJson.GetAllocator());
        AIMessageJson.CopyFrom(AIMessageJsonValue, AIMessageJson.GetAllocator());

        rapidjson::StringBuffer buffer;
        rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
        AIMessageJson.Accept(writer);

        return buffer.GetString();
    }

    //! Convert a list of messages (GenAIFramework type) into text in JSON format
    //! @param messages a list of messages (GenAIFramework::AIMessages format)
    //! @return a list of messages in JSON format (plain text)
    inline AZStd::string AIMessagesToJsonString(const AIMessages& messages)
    {
        rapidjson::Document messagesDocument;
        messagesDocument.SetArray();

        for (const auto& entry : messages)
        {
            rapidjson::Value entryJson = AIMessageToJsonValue(entry, messagesDocument.GetAllocator());
            messagesDocument.PushBack(entryJson, messagesDocument.GetAllocator());
        }

        rapidjson::StringBuffer buffer;
        rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
        messagesDocument.Accept(writer);

        return buffer.GetString();
    }

    //! Convert a single message in JSON document format (rapidjson library type) into a message (GenAIFramework type)
    //! @param json a single message document (rapidjson library format)
    //! @return a single message (GenAIFramework::AIMessage format)
    inline AIMessage JsonToAIMessage(const rapidjson::Document& json)
    {
        Role role;
        AZStd::string roleString = json["role"].GetString();

        //! Map for converting string name to GenAIFramework::Role
        static AZStd::unordered_map<AZStd::string, Role> nameToRoleMap = { { "user", Role::User },
                                                                           { "assistant", Role::Assistant },
                                                                           { "system", Role::System } };

        if (const auto it = nameToRoleMap.find(roleString); it != nameToRoleMap.end())
        {
            role = it->second;
        }
        else
        {
            role = Role::User;
        }

        AIMessage message;
        message.first = role;

        for (const auto& content : json["content"].GetArray())
        {
            AZStd::string contentString = content.GetString();
            message.second.push_back(AZStd::any(contentString));
        }

        return message;
    }

    //! Convert text (assuming JSON format) into a message (GenAIFramework type)
    //! @param jsonString a single message in JSON format (in plain text)
    //! @return message (GenAIFramework::AIMessage format)
    inline AIMessage JsonStringToAIMessage(const AZStd::string& jsonString)
    {
        rapidjson::Document json;
        json.Parse(jsonString.c_str());

        return JsonToAIMessage(json);
    }

    //! Convert text (assuming JSON format) into a message (GenAIFramework type)
    //! @param jsonString a single message in JSON format (in plain text)
    //! @return message (GenAIFramework::AIMessage format)
    inline AIMessages JsonStringToAIMessages(const AZStd::string& jsonString)
    {
        rapidjson::Document json;
        json.Parse(jsonString.c_str());

        AIMessages messages;

        for (const auto& entry : json.GetArray())
        {
            rapidjson::Document entryJson;
            entryJson.CopyFrom(entry, entryJson.GetAllocator());
            messages.push_back(JsonToAIMessage(entryJson));
        }

        return messages;
    }

    //! Convert a response message from AI Agent (GenAIFramework type) to JSON format (in plain text)
    //! @param response a response message (response) from AI Agent (GenAIFramework type)
    //! @return a response message in JSON format (in plain text)
    inline AZStd::string ModelAPIExtractedResponseToJsonString(const ModelAPIExtractedResponse& response)
    {
        rapidjson::Document responseJson;
        responseJson.SetObject();

        responseJson.AddMember("isSuccess", response.IsSuccess(), responseJson.GetAllocator());

        if (response.IsSuccess())
        {
            rapidjson::Value messageJson = AIMessageToJsonValue(response.GetValue(), responseJson.GetAllocator());
            responseJson.AddMember("message", messageJson, responseJson.GetAllocator());
        }
        else
        {
            rapidjson::Value errorValue;
            errorValue.SetString(response.GetError().c_str(), responseJson.GetAllocator());
            responseJson.AddMember("error", errorValue, responseJson.GetAllocator());
        }

        rapidjson::StringBuffer buffer;
        rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
        responseJson.Accept(writer);

        return buffer.GetString();
    }

} // namespace GenAIFramework::JsonUtils
