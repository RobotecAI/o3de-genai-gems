# `GenAIFramework Gem` adding a feature
Adding a new feature is similar to creating a vendor. The feature needs to be registered with the `GenAIFramework` and implement the `FeatureBase` interface. It uses the `ConversationBus` and `AIModelAgentBus` to communicate with the UI and models.

## Flow
The feature gets the message from the user using the `OnNewMessage` EBus event. This message can be then modified and sent to the agent using `AIModelAgentBus::Events::SendPrompt` event.  
The response from the agent is then received using the `OnAIResponse` EBus event. This response can be further processed, sent back to the agent or the process can finish by calling
the `ConversationNotificationBus::Events::OnFeatureResponse` event.

## Implementing the feature
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
are the IDs of the agents and conversations that are needed for EBus communication and should be passed to the `FeatureBase` constructor. They are provided by the `GenAIFramework` during creation of the feature. The `FeatureBase` constructor connects the feature to the `ConversationBus` and `AIModelAgentBus`.

Two functions need to be overridden:
- `OnNewMessage` - this function is called when a new message is received from the user.
- `OnAIResponse` - this function is called when the response from the agent is received.

After each of the `OnNewMessage` function calls there needs to to be a `ConversationNotificationBus::Events::OnFeatureResponse` call to finish the process and allow for the next message to be received.

Usually in the `OnNewMessage` function a call to the `AIModelAgentBus::Events::SendPrompt` is made to send the message to the agent. The arguments of this bus call is AIMessages, a collection of AIMessage objects, consisting of the Role and the message itself.  

In the `OnAIResponse` function the response from the agent is received. Usually this function ends with a call to the `ConversationNotificationBus::Events::OnFeatureResponse` to notify the UI of the response. The arguments required for the `OnFeatureResponse` are the response message and a summary of the response. The user will see the response message in a conversation bubble. The role of the summary is to provide additional context of what the feature did. This can be a log of all the messages received and sent by the feature, error messages, or any other information that the user should know.

## Registering the feature
The feature needs to be registered with the `GenAIFramework` using the `GenAIFramework::SystemRegistrationContext`. This should be done in the `Reflect` function of the feature.
The regestration is done using the `RegisterFeature` function, where the argument is the name of the feature.
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

## Implementation example
```cpp
#include "MyFeature.h"

#include <GenAIFramework/Feature/AIAgentBus.h>
#include <GenAIFramework/Communication/AIModelRequestBus.h>
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

        AIModelAgentBus::Event(m_agentId, &AIModelAgentBus::Events::SendPrompt, messages);
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
