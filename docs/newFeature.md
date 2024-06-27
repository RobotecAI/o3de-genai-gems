# `GenAIFramework Gem` adding a feature
The Generative AI Feature is based on the O3DE Component. It needs to be registered within the `GenAIFramework` and it needs to implement the `FeatureBase` interface.

## Flow
Typically, the feature uses the `ConversationBus` to get the input data from the chat widget and `AIAgentBus` to communicate with the AI. In particular, the feature gets the message from the user using the `OnNewMessage` EBus event. This message can be then modified and sent to the agent using `SendPrompt` event from `AIAgentBus`.

The response from the agent is then received using the `OnAIResponse` EBus event. This response can be further processed, sent back to the agent or the process can finish by calling
the `ConversationNotificationBus::Events::OnFeatureResponse` event.

Please see the [dedicated documentation](./interfaces.md) for more details on interfaces. Visit the [O3DE Component documentation](https://www.docs.o3de.org/docs/user-guide/programming/components/) for more information on creating a new component.

## Implementing the feature
The sample header file for a generative AI feature might look as follows:
```cpp
#include <GenAIFramework/Feature/FeatureBase.h>

class MyFeature : public FeatureBase
{
public:
    AZ_RTTI(MyFeature, "{00000000-0000-0000-0000-000000000000}", FeatureBase);
    AZ_CLASS_ALLOCATOR(MyFeature, AZ::SystemAllocator, 0);
    MyFeature() = default;
    MyFeature(AZ::u64 agentId, AZ::u64 conversationId);
    ~MyFeature() = default;
    static void Reflect(AZ::ReflectContext* context);
    // FeatureBase overrides
    void OnNewMessage(const AZStd::string& message) override;
    void OnAIResponse(ModelAPIExtractedResponse response) override;
};
```

The newly created feature needs to inherit from `FeatureBase` and have a constructor that takes `AZ::u64 agentId` and `AZ::u64 conversationId` as arguments. These arguments are the IDs of the agents and conversations that are needed for EBus communication and should be passed to the `FeatureBase` constructor. They are provided by the `GenAIFramework` during creation of the feature. The `FeatureBase` constructor connects the feature to the `ConversationBus` and `AIAgentBus`. An empty constructor of the `FeatureBase` class might be used if you do not wish to connect to `AIAgentBus`.

> _Note_: The `AIAgentBus` handling and event calling can be skipped if you decide to connect with the AI model and service provider directly from the feature. This can happen if you want to use the Python API to implement a feature together with toolsets tailored for one model only.

Two functions need to be overridden:
- `OnNewMessage` - this function is called when a new message is received from the user (chat widget).
- `OnAIResponse` - this function is called when the response from the AI agent (model and service combination) is received.

The first function, `OnNewMessage`, implements the feature's reactions to the user's prompt. Depending on the functionality of the given feature, it creates the context for the user's input to give more information to the AI. This means calling some O3DE buses to get the state of the O3DE Editor, read some information about the level, etc. All data is packed into `AIMessages` object and sent to AI Agent using `SendPrompt` event of the `AIAgentBus`.

The response from the agent is received based on `OnAIResponse` notification. This function implements the `ConversationNotificationBus::Events::OnFeatureResponse` call to notify the UI of the response. Before it happens, your feature might verify the correctness of the AI answer, execute some additional commands in O3DE or continue the conversation with the AI. The arguments required for the `OnFeatureResponse` are the response message and a summary of the response. The user will see the response message in a conversation bubble. The role of the summary is to provide additional context of what the feature did. This can be a log of all the messages received and sent by the feature, error messages, or any other information that the user should know.

## Registering the feature

The feature needs to be registered within the `GenAIFramework` using the `GenAIFramework::SystemRegistrationContext` bus. This should be done in the `Reflect` function of the feature. The registration is done using the `RegisterFeature` function, where the argument is the name of the feature. A sample implementation of the `Reflect` function is given below.
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

A complete implementation of a sample feature is given below:
```cpp
#include "MyFeature.h"

#include <GenAIFramework/GenAIFrameworkTypes.h>
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
