# `GenAIFramework Gem` interfaces

## Registration system

The framework features a robust registration system, which enables seamless integration of various generative AI components. This mechanism allows developers to register:
* **Vendor Gems**: Specialized gems that provide specific generative AI models or services.
* **Feature Gems**: Gems that introduce novel features or capabilities to the O3DE-Generative AI ecosystem.

The registered components are stored in a centralized repository, making it available when creating new conversations with the AI. 

### Registering new AI components
To register the components in `GenAIFramework`, use the `GenAIFramework::SystemRegistrationContext` in the `Reflect` function of the components. There are multiple methods to use, depending on the component you want to register, please see the C++ code snippet below:

```cpp
template<class C>
void RegisterServiceProvider();

template<class C>
void RegisterModelConfiguration();

template<class C>
void RegisterFeature(const AZStd::string& name);
```

The first two methods allows to register a new AI service provider and AI model configuration respectively. You might want to use these methods when implementing a vendor Gem. An exact description on how to use them is available in a [new vendor tutorial](./newVendor.md) documentation. Each of the methods will register the component in the `GenAIFramework` Gem, with the name specified in the `AZ::EditContext`. If the component is not registered in the `EditContext`, the class name will be used.

The last method registers the feature Gem under a name passed as a parameter. There is a tutorial for creating a [new feature Gem](./newFeature.md) with more details available.

### Getting the list of registered AI components
The list of registered AI components is read by the chat widget, which allows you to create a conversation with any feature, using any vendor components. There should be no need to get the list of registered AI components. Nevertheless, the `GenAIFramework::SystemRegistrationContext` bus provides the following C++ API:

```cpp
AZStd::unordered_set<AZ::Uuid> GetRegisteredServiceProviders() const;
AZStd::unordered_set<AZ::Uuid> GetRegisteredModelConfigurations() const;
AZStd::unordered_map<AZStd::string, AZ::Uuid> GetFeatureNamesAndUuids() const;
```

In which the methods return the lists of registered service providers, model configurations and pairs of features with names respectively. All AI components are kept as class unique ids.

## Conversation system
The AI pipeline within O3DE consist of: (i) a chat widget that interfaces with the user; (ii) a feature that provides the context of the conversation to AI; and (iii) an AI Agent that specifies the model and service provider used to generate responses within the conversation. The three elements are connected with each other using a system of [buses and event handlers](https://docs.o3de.org/docs/user-guide/programming/messaging/ebus/). 

The buses and handlers are defined in `GenAIFramework Gem` as follows:
![GenAIFramework Interfaces](./images/interfaces.svg)

When a user initiates a conversation through the chat widget, the `OnNewMessage` event is triggered and sent to the designated feature, which handles this event. Upon receiving it, the feature is responsible for preparing a prompt that will be used as input for the AI Agent. This process involves sending the `SendPrompt` request, which transmits the prepared prompt to the AI Agent for processing.

The AI Agent processes a prompt and generates a response. It notifies the Feature of the outcome by triggering the `OnPromptResponse` event when ready. This event contains the extracted message from the generative AI model. The feature is responsible for handling this event and preparing a comprehensive response to be returned to the user. To achieve this, the feature prepares a summary of all actions it took, the generated response and any relevant details. Once complete, it triggers the `OnFeatureResponse` event to pass this information back to the user through the chat widget.

### ConversationNotificationBus
`ConversationNotificationBus` is designed as a two-directional communication service between the user interface and the feature. Hence, the notifications from the bus need to be handled on both ends. In particular, it is addressed using the id created while initializing a new conversation. The following interfaces are overwritten by a feature and the chat widget:
```cpp
void OnNewMessage(const AZStd::string& message);
void OnFeatureResponse(const AZStd::string& summary, const AZStd::vector<AZStd::string>& detailedResponse);
```

The chat widget produces a single message, that is transmitted as a text. It expects single text message with a summary and an optional list of detailed text messages that are displayed in the user interface when requested. This interface could be extended in the future to support other types of data, such as images.

### AIAgentRequestBus
`AIAgentRequestBus` transmits the requests from a feature to the AI Agent. Currently, two calls are supported and need to be implemented by AI Agent:

```cpp
void SendPrompt(const AIMessages& prompt);
AIHistory GetHistory();
```

The first interface is designed to send a single query from a feature to the selected AI model. This message is typically build out of the user's query and the context that is generated by the feature. The `AIMessage` form a complete message for the model. It consists of a role and a list of messages sent by that role. The roles represent the _user_, _AI assistant_, and _system_ messages. Each message should be used to create a full request that is accepted by the model. The message type is `AZStd::any`, so any data format can be supported in the future.

The second interface, `AIHistory GetHistory();`, allows the feature to retrieve the conversation history from the AI Agent. This design decision is justified by the fact that the AI Agent stores the conversation history, thereby eliminating the need for features developers to reimplement this component.

The history is defined as a list of `AIMessage` data structures, i.e., as a list of roles and messages. The C++ code definition is the following:

```cpp
enum class Role
{
    User,
    Assistant,
    System
};

using AIContent = AZStd::vector<AZStd::any>;
using AIMessage = AZStd::pair<Role, AIContent>;
using AIMessages = AZStd::vector<AIMessage>;
using AIHistory = AIMessages;
```

### AIAgentNotificationBus
`AIAgentNotificationBus` is a notification channel that connects AI Agent with a feature. When the AI service provider receives the response from the AI model, it uses the internal model definition to extract the content into a common format defined for `GenAIFramework`. Finally, it notifies the feature about the upcoming message with the bus. This means, a feature should always implement a handler for such events:

```cpp
using ModelAPIExtractedResponse = AZ::Outcome<AIMessage, AZStd::string>; 
void OnPromptResponse(ModelAPIExtractedResponse response);
```

The communication protocol between AI Agents and features supports the cases in which AI Agent failed to receive an answer from AI service (e.g., due to the timeout or missing credentials). For that reason the exchanged data is defined as a success or a failure `AZ::Outcome<>` with the corresponding message `AIMessage` or the error message in text format `AZStd::string`.

## AI Agent buses

`AI Agent` buses are the internal buses designed for the communication between AI Models and AI service providers. The reason for such structure is making the code more reusable. E.g., the `Claude` model can be run on either `Anthropic` or `Amazon Bedrock` service. This means AI model definition can be reused with different AI service providers and defined interface is necessary.

### AIModelRequestBus
`AIModelRequestBus` bus specifies functions for preparing and extracting requests to the model endpoints. I.e., it interfaces features on one side and AI service providers on the other. The snippet below presents its implementation:
```cpp
enum class Role
{
    User,
    Assistant,
    System
};

using AIContent = AZStd::vector<AZStd::any>;
using AIMessage = AZStd::pair<Role, AIContent>;
using AIMessages = AZStd::vector<AIMessage>;
using ModelAPIRequest = AZStd::string;
using ModelAPIResponse = AZ::Outcome<AZStd::string, AZStd::string>;
using ModelAPIExtractedResponse = AZ::Outcome<AIMessage, AZStd::string>;

ModelAPIRequest PrepareRequest(const AIMessages& prompt);
ModelAPIExtractedResponse ExtractResult(const ModelAPIResponse& modelAPIResponse);
AZ::Outcome<void, AZStd::string> SetModelParameter(const AZ::Name& parameterName, const AZStd::string& parameterValue);
```
A visitor pattern should be used for processing the messages. Each message should be converted into a string and added to the request. The `PrepareRequest` function creates that request. The outcome of this method is then sent by AI service provider to the server. The `ExtractResult` function extracts the response from the model and returns it as an `AIMessage` type, that can be sent back to the feature.

The `SetModelParameter` function is used to set the model parameters. This function allows modification of the model parameters programmatically.

### AIServiceProviderBus
`AIServiceProviderBus` bus is an interface for implementing API of the AI service provider. It specifies the `SendRequest` function as follows:
```cpp
using ModelAPIRequest = AZStd::string;
using ModelAPIResponse = AZ::Outcome<AZStd::string, AZStd::string>;
void SendRequest(const ModelAPIRequest& request, AZStd::function<void(ModelAPIResponse)> callback);
```

The `ModelAPIRequest` is a string. The callback should always be called after the request is processed: it typically triggers an event on `AIModelRequestBus` to extract the models outcome and even triggers an event on `AIModelAgentNotificationBus` to inform the feature about the AI response.

The `ModelAPIResponse` is an `AZ::Outcome<>` corresponding to the success or failure of the request. On success, the _outcome_ contains the response, on failure it contains an error message.
