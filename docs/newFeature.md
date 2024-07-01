# `GenAIFramework Gem` adding a feature
The Generative AI Feature is based on the O3DE Component. It needs to be registered within the `GenAIFramework` and it needs to implement the `FeatureBase` interface.

## Flow
Typically, the feature uses the `ConversationBus` to get the input data from the chat widget and `AIAgentBus` to communicate with the AI. In particular, the feature gets the message from the user using the `OnNewMessage` EBus event. This message can be then modified and sent to the agent using `SendPrompt` event from `AIAgentBus`.

The response from the agent is then received using the `OnAIResponse` EBus event. This response can be further processed, sent back to the agent or the process can finish by calling
the `ConversationNotificationBus::Events::OnFeatureResponse` event.

Please see the [dedicated documentation](./interfaces.md) for more details on interfaces. Visit the [O3DE Component documentation](https://www.docs.o3de.org/docs/user-guide/programming/components/) for more information on creating a new component.

## C++ and Python features
The feature can be implemented in either C++ or Python. The Python features require minimal code in C++. The C++ features inherit from the `CppFeatureBase` and the Python features inherit from the `PythonFeatureBase`. The main difference between these interfaces is that the C++ features automatically connect to the `AIAgentBus` and `ConversationBus` while the Python features must do this manually.

## Implementing the C++ feature
The sample header file for a generative AI C++ feature might look as follows:
```cpp
#include <GenAIFramework/Feature/CppFeatureBase.h>

class MyFeature : public CppFeatureBase
{
public:
    AZ_RTTI(MyFeature, "{00000000-0000-0000-0000-000000000000}", CppFeatureBase);
    AZ_CLASS_ALLOCATOR(MyFeature, AZ::SystemAllocator, 0);
    MyFeature() = default;
    MyFeature(AZ::u64 agentId, AZ::u64 conversationId);
    ~MyFeature() = default;
    static void Reflect(AZ::ReflectContext* context);
    // CppFeatureBase overrides
    void OnNewMessage(const AZStd::string& message) override;
    void OnAIResponse(ModelAPIExtractedResponse response) override;
};
```

The newly created feature needs to inherit from `CppFeatureBase` and have a constructor that takes `AZ::u64 agentId` and `AZ::u64 conversationId` as arguments. These arguments are the IDs of the agents and conversations that are needed for EBus communication and should be passed to the `CppFeatureBase` constructor. They are provided by the `GenAIFramework` during creation of the feature. The `CppFeatureBase` constructor connects the feature to the `ConversationBus` and `AIAgentBus`. An empty constructor of the `CppFeatureBase` class might be used if you do not wish to connect to `AIAgentBus`.

> _Note_: The `AIAgentBus` handling and event calling can be skipped if you decide to connect with the AI model and service provider directly from the feature. This can happen if you want to use the Python API to implement a feature together with toolsets tailored for one model only.

Two functions need to be overridden:
- `OnNewMessage` - this function is called when a new message is received from the user (chat widget).
- `OnAIResponse` - this function is called when the response from the AI agent (model and service combination) is received.

The first function, `OnNewMessage`, implements the feature's reactions to the user's prompt. Depending on the functionality of the given feature, it creates the context for the user's input to give more information to the AI. This means calling some O3DE buses to get the state of the O3DE Editor, read some information about the level, etc. All data is packed into `AIMessages` object and sent to AI Agent using `SendPrompt` event of the `AIAgentBus`.

The response from the agent is received based on `OnAIResponse` notification. This function implements the `ConversationNotificationBus::Events::OnFeatureResponse` call to notify the UI of the response. Before it happens, your feature might verify the correctness of the AI answer, execute some additional commands in O3DE or continue the conversation with the AI. The arguments required for the `OnFeatureResponse` are the response message and a summary of the response. The user will see the response message in a conversation bubble. The role of the summary is to provide additional context of what the feature did. This can be a log of all the messages received and sent by the feature, error messages, or any other information that the user should know.

## Implementing the Python feature
The sample header for a Python feature might look as follows:
```cpp
#include <GenAIFramework/Feature/PythonFeatureBase.h>

class MyFeature : public PythonFeatureBase
{
public:
    AZ_RTTI(MyFeature, "{00000000-0000-0000-0000-000000000000}", PythonFeatureBase);
    AZ_CLASS_ALLOCATOR(MyFeature, AZ::SystemAllocator, 0);

    MyFeature() = default;
    MyFeature(AZ::u64 agentId, AZ::u64 conversationId);
    ~MyFeature() = default;

    static void Reflect(AZ::ReflectContext* context);
};
```
The header file of the Python feature is very similar to the C++ feature. Additionally the Python feature needs to call a Python script in the source file this can be done in the Constructor of this class.
```cpp
MyFeature::MyFeature(AZ::u64 agentId, AZ::u64 conversationId)
        : PythonFeatureBase(agentId, conversationId)
    {
        AZ::SystemTickBus::QueueFunction(
            [this]()
            {
                AZStd::string agentIdString = AZStd::to_string(m_conversationId);
                AZStd::string conversationIdString = AZStd::to_string(m_conversationId);

                AZStd::vector<AZStd::string_view> params = { agentIdString, conversationIdString };

                bool isOk = false;
                AzToolsFramework::EditorPythonRunnerRequestBus::BroadcastResult(
                    isOk,
                    &AzToolsFramework::EditorPythonRunnerRequestBus::Events::ExecuteByFilenameWithArgs,
                    AZStd::string_view("path/to/python/script.py"),
                    params);
            });
    }
```
The call to the Python script is done in the `SystemTickBus` to ensure that the Python script is called after the feature is fully initialized and run in the main thread.  

The script needs to implement the `OnNewMessage` and `OnAIResponse` functions. The `OnNewMessage` function is called when a new message is received from the user (chat widget). The `OnAIResponse` function is called when the response from the AI agent is received. After each `OnNewMessage` function call the `OnFeatureResponse` EBus call should be made to notify the UI that the next message can be sent.

```python
import json
import argparse
import azlmbr.bus as bus
from azlmbr.ai import (
    ConversationNotificationBus,
    ConversationNotificationBusHandler,
    AIAgentNotificationBusHandler,
    AIAgentRequestBus,
)

parser = argparse.ArgumentParser(
    description="Example of creating a Python feature for the GenAIFramework Gem."
)
parser.add_argument("agent_id", help="The agent id")
parser.add_argument("conversation_id", help="The conversation id")
args = parser.parse_args()

agentId = int(args.agent_id)
conversationId = int(args.conversation_id)

if "myFeatures" not in globals():
    myFeatures = []

class MyFeature:
    def __init__(self, agentId, conversationId):
        self.agentId = agentId
        self.conversationId = conversationId

        # Create and connect the Conversation handler and add the OnNewMessage callback.
        self.handlerConversation = ConversationNotificationBusHandler()
        self.handlerConversation.connect(self.connectionId)
        self.handlerConversation.add_callback("OnNewMessage", self.OnNewMessage)

        # Create and connect the Agent handler and add the OnAIResponse callback.
        self.handlerAgent = AIAgentNotificationBusHandler()
        self.handlerAgent.connect(self.connectionId)
        self.handlerAgent.add_callback("OnAIResponse", self.OnAIResponse)

    # This function is called when a new message is received from the user (chat widget).
    def OnNewMessage(self, message):
        # The message is a list of length, where the first element is the message from the user.
        prompt = [{"role": "user", "content": [message[0]]}]

        # A call to the agent can be made to get the history of the conversation.
        # The history is a json formatted string consisting of a list of messages with roles and content.
        history = AIAgentRequestBus(bus.Event, "GetHistory", self.agentId)
        jsonHistory = json.loads(history)

        # The prompt can be modified and sent to the agent. The format of the prompt is a list of messages with roles and content.
        fullJsonStringPrompt = "..."
        AIAgentRequestBus(bus.Event, "SendPrompt", self.agentId, fullJsonStringPrompt)

    # This function is called when the response from the AI agent is received.
    def OnAIResponse(self, message):
        # The message is a list of length 1, where the first element is the response from the AI agent.
        jsonMessage = json.loads(message[0])

        response = "..."
        summary = ["...", "...", ...]

        # The response can be further processed, sent back to the agent or the process can finish by calling the OnFeatureResponse.
        # This function notifies the UI that the response is ready to be displayed and a new message can be sent. This always needs to be called.
        ConversationNotificationBus(bus.Event, "OnFeatureResponse", self.connectionId, response, summary)

# It is important to add each feature as a separate instance to avoid redefinitions of the same feature and mismatching of the ids used for busses.
myFeatures.append(MyFeature(agentId, conversationId))
```
It is important to create the feature as a class and store its objects in some kind of data structure (like the `myFeatures` list). This script is called each time a new conversation is created with this feature and creating a class object prohibits overriding the previously declared function. This is done because of a limitation of the Python interpreter in O3DE. The interpreter is global and all scripts are executed in the same context.

More information on the available API and data formats which can be used in Python can be found in the [dedicated documentation](./docs/python.md).

## Registering the feature

The feature needs to be registered within the `GenAIFramework` using the `GenAIFramework::SystemRegistrationContext` bus. This should be done in the `Reflect` function of the feature. The registration is done using the `RegisterFeature` function, where the argument is the name of the feature. A sample implementation of the `Reflect` function is given below. This is the same for C++ and Python features.
```cpp
void MyFeature::Reflect(AZ::ReflectContext* context)
{
    ...

    if (auto registrationContext = azrtti_cast<GenAIFramework::SystemRegistrationContext*>(context))
    {
        registrationContext->RegisterFeature<MyFeature>("MyFeature");
    }
}
```
This Reflect function should be called in the system component of the Gem implementing the feature.


## Complete C++ feature implementation
A complete implementation of a sample feature is given below:
```cpp
#include "MyFeature.h"

#include <GenAIFramework/Communication/AIModelRequestBus.h>
#include <GenAIFramework/Feature/AIAgentBus.h>
#include <GenAIFramework/Feature/ConversationBus.h>
#include <GenAIFramework/SystemRegistrationContext/SystemRegistrationContext.h>

namespace GenAIFramework
{
    MyFeature::MyFeature(AZ::u64 agentId, AZ::u64 conversationId)
        : FeatureBase(agentId, conversationId)
    {
    }

    void MyFeature::Reflect(AZ::ReflectContext* context)
    {
        ...

        if (auto registrationContext = azrtti_cast<GenAIFramework::SystemRegistrationContext*>(context))
        {
            registrationContext->RegisterFeature<MyFeature>("MyFeature");
        }
    }

    void MyFeature::OnNewMessage(const AZStd::string& message)
    {
        AIMessages messages;
        AIMessage newMessage = { Role::User, { AZStd::any(message) } };
        messages.push_back(newMessage);

        AIAgentBus::Event(m_agentId, &AIAgentBus::Events::SendPrompt, messages);
    }

    void MyFeature::OnAIResponse(ModelAPIExtractedResponse response)
    {
        if (!response.IsSuccess())
        {
            AZStd::string errorMessage = response.GetError().c_str();
            AZStd::vector<AZStd::string> summary = {};
            ConversationNotificationBus::Event(
                m_conversationId, &ConversationNotificationBus::Events::OnFeatureResponse, errorMessage, summary);
            return;
        }

        for (const auto& responseItem : response.GetValue().second)
        {
            if (responseItem.is<AZStd::string>())
            {
                AZStd::string responseString = AZStd::any_cast<AZStd::string>(responseItem);
                AZStd::vector<AZStd::string> summary = {};
                ConversationNotificationBus::Event(
                    m_conversationId, &ConversationNotificationBus::Events::OnFeatureResponse, responseString, summary);
            }
        }
    }

} // namespace GenAIFramework
```
