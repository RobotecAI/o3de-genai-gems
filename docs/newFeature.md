# `GenAIFramework Gem` adding a feature
The Generative AI Feature is based on the O3DE Component. It needs to be registered within the `GenAIFramework` and it needs to implement the `FeatureBase` interface. Typically, the feature uses the `ConversationBus` to get the input data from the chat widget and `AIModelAgentBus` to communicate with the AI. The latter can be skipped if you decide to connect the AI model and service provider by yourself (e.g. using the Python API and toolsets tailored for one model).

## Flow
The feature gets the message from the user using the `OnNewMessage` EBus event. This message can be then modified and sent to the agent using `AIAgentBus::Events::SendPrompt` event.  
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

The newly created feature needs to inherit from `FeatureBase` and have a constructor that takes `AZ::u64 agentId` and `AZ::u64 conversationId` as arguments. These arguments 
are the IDs of the agents and conversations that are needed for EBus communication and should be passed to the `FeatureBase` constructor. They are provided by the `GenAIFramework` during creation of the feature. The `FeatureBase` constructor connects the feature to the `ConversationBus` and `AIModelAgentBus`, however en empty constructor might be used if you do not wish to use both buses.

Two functions need to be overridden:
- `OnNewMessage` - this function is called when a new message is received from the user (chat widget).
- `OnAIResponse` - this function is called when the response from the AI agent (model and service combination) is received.

Each of the `OnNewMessage` function calls needs a `ConversationNotificationBus::Events::OnFeatureResponse` call to finish the process and allow for the next message to be received. Typically, `OnNewMessage` function calls the `AIModelAgentBus::Events::SendPrompt` request to send the message to the agent. `AIMessages`  is the argument of this bus call. It is a collection of `AIMessage` objects, consisting of the Role and the message itself. Your newly implemented feature should define the messages based on the user's prompt and some additional knowledge, e.g., it might read the current state of O3DE Editor using buses and combine them into context message for the AI.

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

#include <GenAIFramework/Communication/AIModelRequestBus.h>
#include <GenAIFramework/Feature/AIAgentBus.h>
#include <GenAIFramework/Feature/ConversationBus.h>
#include <GenAIFramework/SystemRegistrationContext/SystemRegistrationContext.h>

namespace GenAIFramework
{
    O3DEAssistantFeature::O3DEAssistantFeature(AZ::u64 agentId, AZ::u64 conversationId)
        : FeatureBase(agentId, conversationId)
    {
    }

    void O3DEAssistantFeature::Reflect(AZ::ReflectContext* context)
    {
        ...

        if (auto registrationContext = azrtti_cast<GenAIFramework::SystemRegistrationContext*>(context))
        {
            registrationContext->RegisterFeature<O3DEAssistantFeature>("O3DE Assistant");
        }
    }

    void O3DEAssistantFeature::OnNewMessage(const AZStd::string& message)
    {
        AIMessages messages;
        AIMessage newMessage = { Role::User, { AZStd::any(message) } };
        messages.push_back(newMessage);

        AIAIAgentBus::Event(m_agentId, &AIAIAgentBus::Events::SendPrompt, messages);
    }

    void O3DEAssistantFeature::OnAIResponse(ModelAPIExtractedResponse response)
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
