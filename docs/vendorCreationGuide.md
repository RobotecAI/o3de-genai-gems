# Creating your vendor
Creating a vendor in O3DE requires knowledge of the O3DE component development. Visit the [O3DE Component documentation](https://www.docs.o3de.org/docs/user-guide/programming/components/) for more information.

## Required components
To create a vendor, two components need to be created:
- `Service provider component` - responsible for the communication with the model.
- `Model component` - responsible for the configuration and creation of requests for the model.

These components implement the `AIServiceProviderBus::Handler` and `AIModelRequestBus::Handler` respectively.  

A custom registration system is used to register the components, making them available in the `GenAIFramework` Gem.

### AIServiceProviderBus
This bus specifies the `SendRequest` function
```cpp
using ModelAPIRequest = AZStd::string;
using ModelAPIResponse = AZ::Outcome<AZStd::string, AZStd::string>;
void SendRequest(const ModelAPIRequest& request, AZStd::function<void(ModelAPIResponse)> callback);
```
The `ModelAPIRequest` is a string. The callback should always be called after the request is processed.
The `ModelAPIResponse` is an `AZ::Outcome` corresponding to the success or failure of the request. On success, the Outcome contains the response, on failure it contains an error message.
The response should not be processed by the `Service Provider` component. The `Model` component is responsible for extracting the response.

### AIModelRequestBus
This bus specifies functions for preparing and extracting requests to the model endpoints.
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
The `AIContent` and `AIMessage` form a complete message for the model. It consists of a role and a list of messages sent by that role. The roles represent the user, assistant, and system messages.
Each message should be used to create a full request that is accepted by the model. The message type is `AZStd::any`. A visitor pattern should be used for processing the messages. Each message should be converted into a string and added to the request.
The `PrepareRequest` function creates that request.  
The `ExtractResult` function extracts the response from the model and returns it as an `AIMessage`.

The `SetModelParameter` function is used to set the model parameters. This function allows modification of the model parameters programmatically.

### SystemRegistrationContext
The `SystemRegistrationContext` is used to register the components. It is used to register the `Service provider` and the `Model` components. The `SystemRegistrationContext` is a part of the `GenAIFramework` Gem.  
This context provides two methods:
- `RegisterServiceProvider` - registers the `Service provider` component.
- `RegisterModelConfiguration` - registers the `Model` component.  

Each of the methods will register the component in the `GenAIFramework` Gem, with the name specified in the `AZ::EditContext`. If the component is not registered in the `EditContext`,
the class name will be used.

## Creating the components
Create two components: one for the `Service provider` and one for the `Model`. The `Service provider` component should implement the `AIServiceProviderBus` and the `Model` component should implement the `AIModelRequestBus`.
### Service provider component
```cpp
#include <GenAIFramework/Communication/AIModelRequestBus.h>
#include <AzCore/Component/Component.h>

class MyServiceProvider 
 : public AZ::Component
    , public GenAIFramework::AIServiceProviderBus::Handler
    {
    public:
        AZ_COMPONENT(MyServiceProvider, "{00000000-0000-0000-0000-000000000000}");

        static void Reflect(AZ::ReflectContext* context);
        
        // Override the AIServiceProviderBus functions
        void SendRequest(const ModelAPIRequest& request, AZStd::function<void(ModelAPIResponse)> callback) override;
    };
```
This component should reflect parameters such as the model's `URI` and any other parameter required for communication. The `SendRequest` function sends the request to an endpoint and calls the callback with the response.

### Model component

```cpp
#include <GenAIFramework/Communication/AIModelRequestBus.h>
#include <AzCore/Component/Component.h>

class MyModel 
 : public AZ::Component
    , public GenAIFramework::AIModelRequestBus::Handler
    {
    public:
        AZ_COMPONENT(MyModel, "{00000000-0000-0000-0000-000000000000}");

        static void Reflect(AZ::ReflectContext* context);
        
        // Override the AIModelRequestBus functions
        ModelAPIRequest PrepareRequest(const AIMessages& prompt) override;
        ModelAPIExtractedResponse ExtractResult(const ModelAPIResponse& modelAPIResponse) override;
        AZ::Outcome<void, AZStd::string> SetModelParameter(const AZ::Name& parameterName, const AZStd::string& parameterValue) override;
    };
```
This component should reflect the model's parameters such as `temperature`, `system messages`, and others. The `PrepareRequest` function should create a request that is accepted by the model. The `ExtractResult` function should extract the response from the model. The `SetModelParameter` function should set the model parameters, based on the string value and the name of the parameter.

## Registering the components
To register the components in `GenAIFramework`, use the `GenAIFramework::SystemRegistrationContext` in the `Reflect` function of the components.
### Service provider component
```cpp
#include <GenAIFramework/SystemRegistrationContext/SystemRegistrationContext.h>
#include <AzCore/Serialization/SerializeContext.h>

void MyServiceProvider::Reflect(AZ::ReflectContext* context) {
    if (auto serializeContext = azrtti_cast<AZ::SerializeContext*>(context))
    {
        serializeContext->Class<MyServiceProvider, AZ::Component>()->Version(0);
        ...
        if (auto editContext = serializeContext->GetEditContext())
        {
            editContext->Class<MyServiceProvider>("My Service Provider", "");
            ...
        }
    }

    // Registration in the GenAIFramework
    if (auto registrationContext = azrtti_cast<GenAIFramework::SystemRegistrationContext*>(context))
    {
        registrationContext->RegisterServiceProvider<MyServiceProvider>();
    }
}
```

### Model component
```cpp
#include <GenAIFramework/SystemRegistrationContext/SystemRegistrationContext.h>
#include <AzCore/Serialization/SerializeContext.h>

void MyModel::Reflect(AZ::ReflectContext* context) {
    if (auto serializeContext = azrtti_cast<AZ::SerializeContext*>(context))
    {
        serializeContext->Class<MyModel, AZ::Component>()->Version(0);
        ...
        if (auto editContext = serializeContext->GetEditContext())
        {
            editContext->Class<MyModel>("My Model", "");
            ...
        }
    }

    // Registration in the GenAIFramework
    if (auto registrationContext = azrtti_cast<GenAIFramework::SystemRegistrationContext*>(context))
    {
        registrationContext->RegisterModelConfiguration<MyModel>();
    }
}
```

## Registering the components with O3DE
The components should be registered in the `ModuleInterface` file of the Gem, where the components are defined. See the [O3DE Component registration documentation](https://www.docs.o3de.org/docs/user-guide/programming/components/create-component/#register-the-component) for more information.
