# assumes a level has been opened or created
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

connectionId = args.agent_id

print("Agent ID: " + connectionId)


def OnNewMessage(message):
    history = AIAgentRequestBus(bus.Event, "GetHistory", connectionId)

    prompt = [{"role": "user", "content": [message[0]]}]

    history += prompt

    jsonPrompt = json.dumps(history)

    AIAgentRequestBus(bus.Event, "SendPrompt", connectionId, jsonPrompt)

    print(message)


def OnAIResponse(message):
    jsonMessage = json.loads(message[0])
    if jsonMessage["isSuccess"] == True:
        if jsonMessage["message"]["role"] == "assistant":
            for elem in jsonMessage["message"]["content"]:
                ConversationNotificationBus(
                    bus.Event, "OnFeatureResponse", connectionId, elem, []
                )


handlerConversation = ConversationNotificationBusHandler()
handlerConversation.connect(connectionId)  # connects to a singleton bus handler
handlerConversation.add_callback("OnNewMessage", OnNewMessage)

handlerAgent = AIAgentNotificationBusHandler()
handlerAgent.connect(connectionId)
handlerAgent.add_callback("OnAIResponse", OnAIResponse)
