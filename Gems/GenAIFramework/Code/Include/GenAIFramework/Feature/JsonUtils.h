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

namespace GenAIFramework::JsonUtils
{
    inline rapidjson::Value AIMessageToJsonValue(const AIMessage& message, rapidjson::Document::AllocatorType& allocator)
    {
        rapidjson::Value AIMessageJson(rapidjson::kObjectType);

        rapidjson::Value entryJson(rapidjson::kObjectType);

        AZStd::string role;

        switch (message.first)
        {
        case Role::User:
            role = "user";
            break;
        case Role::Assistant:
            role = "assistant";
            break;
        case Role::System:
            role = "system";
            break;
        }

        rapidjson::Value roleValue;
        roleValue.SetString(role.c_str(), allocator);
        AIMessageJson.AddMember("role", roleValue, allocator);

        rapidjson::Value contentJson(rapidjson::kArrayType);
        for (const auto& content : message.second)
        {
            rapidjson::Value contentValue;
            if (content.type() == AZ::AzTypeInfo<AZStd::string>().Uuid())
            {
                contentValue.SetString(AZStd::any_cast<AZStd::string>(content).c_str(), allocator);
            }

            contentJson.PushBack(contentValue, allocator);
        }

        AIMessageJson.AddMember("content", contentJson, allocator);

        return AIMessageJson;
    }

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

    inline AZStd::string AIMessagesToJsonString(const AIMessages& AIMessages)
    {
        rapidjson::Document messagesDocument;
        messagesDocument.SetArray();

        for (const auto& entry : AIMessages)
        {
            rapidjson::Value entryJson = AIMessageToJsonValue(entry, messagesDocument.GetAllocator());
            messagesDocument.PushBack(entryJson, messagesDocument.GetAllocator());
        }

        rapidjson::StringBuffer buffer;
        rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
        messagesDocument.Accept(writer);

        return buffer.GetString();
    }

    inline AIMessage JsonToAIMessage(const rapidjson::Document& json)
    {
        Role role;
        AZStd::string roleString = json["role"].GetString();

        if (roleString == "user")
        {
            role = Role::User;
        }
        else if (roleString == "assistant")
        {
            role = Role::Assistant;
        }
        else if (roleString == "system")
        {
            role = Role::System;
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

    inline AIMessage JsonStringToAIMessage(const AZStd::string& jsonString)
    {
        rapidjson::Document json;
        json.Parse(jsonString.c_str());

        return JsonToAIMessage(json);
    }

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

} // namespace GenAIFramework::Internal
