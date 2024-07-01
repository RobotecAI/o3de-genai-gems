"""
* Copyright (c) Contributors to the Open 3D Engine Project.
* For complete copyright and license terms please see the LICENSE at the root of this distribution.
*
* SPDX-License-Identifier: Apache-2.0 OR MIT
*
"""

import json
import argparse
import azlmbr.bus as bus
import azlmbr.editor as editor
from azlmbr.ai import (
    ConversationNotificationBus,
    ConversationNotificationBusHandler,
    AIAgentNotificationBusHandler,
    AIAgentRequestBus,
)

parser = argparse.ArgumentParser(
    description="Script that creates a chat with the AI agent in o3de using LLMs"
)
parser.add_argument("agent_id", help="The agent id")
parser.add_argument("conversation_id", help="The conversation id")
args = parser.parse_args()

connectionId = int(args.agent_id)

if "assistants" not in globals():
    assistants = []


class Assistant:
    def __init__(self, id):
        self.connectionId = id

        self.handlerConversation = ConversationNotificationBusHandler()
        self.handlerConversation.connect(self.connectionId)
        self.handlerConversation.add_callback("OnNewMessage", self.OnNewMessage)

        self.handlerAgent = AIAgentNotificationBusHandler()
        self.handlerAgent.connect(self.connectionId)
        self.handlerAgent.add_callback("OnAIResponse", self.OnAIResponse)

        self.systemMessage = [
            {
                "role": "system",
                "content": [
                    "You are an AI assistant for the Open 3D Engine. You are here to help you with any questions regarding the "
                    + "workings of the engine. Guidance on using specific features, troubleshooting issues, or understanding "
                    + "the best practices are some things that you will help with."
                ],
            }
        ]

    def OnNewMessage(self, message):
        history = AIAgentRequestBus(bus.Event, "GetHistory", connectionId)

        prompt = [{"role": "user", "content": [message[0]]}]
        jsonHistory = json.loads(history)

        if len(jsonHistory) <= 0:
            fullPrompt = self.systemMessage + prompt
        else:
            fullPrompt = jsonHistory + prompt

        fullJsonStringPrompt = json.dumps(fullPrompt)
        AIAgentRequestBus(
            bus.Event, "SendPrompt", self.connectionId, fullJsonStringPrompt
        )

    def OnAIResponse(self, message):
        jsonMessage = json.loads(message[0])
        if jsonMessage["isSuccess"] == True:
            if jsonMessage["message"]["role"] == "assistant":
                for elem in jsonMessage["message"]["content"]:
                    ConversationNotificationBus(
                        bus.Event, "OnFeatureResponse", self.connectionId, elem, []
                    )


assistants.append(Assistant(connectionId))
