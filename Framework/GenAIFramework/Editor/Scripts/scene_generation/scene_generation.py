"""
* Copyright (c) Contributors to the Open 3D Engine Project.
* For complete copyright and license terms please see the LICENSE at the root of this distribution.
*
* SPDX-License-Identifier: Apache-2.0 OR MIT
*
"""

import json
import argparse
import logging
from pathlib import Path
import sys
from typing import TypeAlias
import azlmbr.bus as bus
import azlmbr.editor as editor
from azlmbr.ai import (
    ConversationNotificationBus,
    ConversationNotificationBusHandler,
    AIAgentNotificationBusHandler,
    AIAgentRequestBus,
)
from utils import extract_text_from_xml_tags, setup_prompt
from prefabs_manager import PrefabsManager
import python_interpreter

logging.basicConfig(level=logging.INFO, stream=sys.stderr)

parser = argparse.ArgumentParser(
    description="Script that creates a chat with the AI agent in o3de using LLMs"
)
parser.add_argument("agent_id", help="The agent id")
parser.add_argument("conversation_id", help="The conversation id")
args = parser.parse_args()

agent_id = int(args.agent_id)

if "scene_generation_instances" not in globals():
    scene_generation_instances = []

GenAIFrameworkMessage: TypeAlias = dict[str, list[str]]


SYSTEM_PROMPT_TEMPLATE_FILE_PATH = (
    Path(__file__).parent / "prompt_templates" / "layout_designer_system_prompt_template.md"
)
PREFABS_YAML_FILE_PATH = Path(__file__).parent / "prefabs.yml"


class SceneGeneration:
    def __init__(self, agent_id):
        self.agent_id = agent_id
        self.pm = PrefabsManager.from_yaml(PREFABS_YAML_FILE_PATH)
        self.messages: list[GenAIFrameworkMessage] = []
        self.python_output: str = ""
        self.response_with_status: dict[str, bool | GenAIFrameworkMessage] | None = None
        self.python_code: str = ""
        self.tickbus_handler = bus.NotificationHandler("TickBus")
        self.conversation_handler = ConversationNotificationBusHandler()
        self.agent_handler = AIAgentNotificationBusHandler()
        self.connect_tickbus_notification_handler()
        self.connect_conversation_notification_handler()
        self.connect_agent_notification_handler()
        python_interpreter.exec_global_scope["sg"] = self

    def connect_conversation_notification_handler(self):
        self.conversation_handler.connect(self.agent_id)
        self.conversation_handler.add_callback("OnNewMessage", self.on_new_message)

    def connect_agent_notification_handler(self):
        self.agent_handler.connect(self.agent_id)
        self.agent_handler.add_callback("OnAIResponse", self.on_ai_response)

    def connect_tickbus_notification_handler(self):
        self.tickbus_handler.connect()
        self.tickbus_handler.add_callback("OnTick", self.on_tick)

    def set_system_message(self):
        self.pm.refresh_prefabs()
        logging.debug("Available prefabs:\n%s\n", self.pm.available_prefabs_xml)
        self.prompt_parts = {
            "AVAILABLE_PREFABS_XML": self.pm.available_prefabs_xml,
        }
        self.system_prompt: str = setup_prompt(
            SYSTEM_PROMPT_TEMPLATE_FILE_PATH, self.prompt_parts
        )
        self.system_message: GenAIFrameworkMessage = {
            "role": "system",
            "content": [self.system_prompt],
        }
        self.messages.append(self.system_message)

    def handle_response(self):
        is_success: bool = self.response_with_status["isSuccess"]
        if is_success:
            message: GenAIFrameworkMessage = self.response_with_status["message"]
            self.messages.append(message)
            response_text: str = message["content"][0]
            logging.debug("\nResponse: %s\n", response_text)

            thinking_text = extract_text_from_xml_tags(response_text, tag="thinking")
            answer_text = extract_text_from_xml_tags(response_text, tag="answer")
            python_code = extract_text_from_xml_tags(response_text, tag="python")
            if thinking_text:
                ConversationNotificationBus(
                    bus.Event,
                    "OnFeatureResponse",
                    self.agent_id,
                    thinking_text,
                    [],
                )
            if python_code:
                ConversationNotificationBus(
                    bus.Event, "OnFeatureResponse", self.agent_id, "Please wait...", []
                )
                python_output = self.run_code(python_code)
                self.messages.append({"role": "user", "content": [python_output]})
                self.send_prompt()
            if answer_text:
                ConversationNotificationBus(
                    bus.Event, "OnFeatureResponse", self.agent_id, answer_text, []
                )
        else:
            failure_message = "Something went wrong. Please try again."
            ConversationNotificationBus(
                bus.Event, "OnFeatureResponse", self.agent_id, failure_message, []
            )
            self.messages.append({"role": "assistant", "content": [failure_message]})
        self.response_with_status = None

    def on_tick(self, args):
        if not self.messages:
            self.tickbus_handler.disconnect()
            self.set_system_message()
            self.connect_tickbus_notification_handler()
        if self.response_with_status:
            self.tickbus_handler.disconnect()
            self.handle_response()
            self.connect_tickbus_notification_handler()

    def on_new_message(self, request_elements: tuple[str]):
        self.prepare_prompt()
        request_text: str = request_elements[0]
        message: GenAIFrameworkMessage = {"role": "user", "content": [request_text]}
        self.messages.append(message)
        ConversationNotificationBus(
            bus.Event, "OnFeatureResponse", self.agent_id, "Please wait...", []
        )
        self.send_prompt()

    def on_ai_response(self, response_elements: tuple[str]):
        response: str = response_elements[0]
        self.response_with_status = json.loads(response)

    def send_prompt(self):
        AIAgentRequestBus(bus.Event, "SendPrompt", self.agent_id, json.dumps(self.messages))

    def run_code(self, code: str) -> str:
        editor.ToolsApplicationRequestBus(bus.Broadcast, "BeginUndoBatch", "Run Python Code")
        python_output = python_interpreter.run_code(code)
        logging.debug("Python output:\n%s\n", python_output)
        self.pm.add_dirty_entities()
        editor.ToolsApplicationRequestBus(bus.Broadcast, "EndUndoBatch")
        return python_output

    def prepare_prompt(self):
        self.pm.refresh_prefabs()
        if len(self.messages) == 1:
            if self.pm.spawned_prefabs:
                self.messages += [
                    {
                        "role": "user",
                        "content": [
                            "There are some prefabs in the scene already. Let me give you the result of spawned_prefabs_xml tool:\n"
                            + self.pm.spawned_prefabs_xml
                        ],
                    },
                    {
                        "role": "assistant",
                        "content": ["Thank you. Now I am waiting for your request."],
                    },
                ]
            else:
                self.messages += [
                    {"role": "user", "content": ["There are no prefabs in the scene yet."]},
                    {
                        "role": "assistant",
                        "content": ["Thank you. Now I am waiting for your request."],
                    },
                ]


scene_generation_instances.append(SceneGeneration(agent_id))
