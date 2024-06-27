# `GenAIFramework Gem` adding a vendor

Creating a vendor in O3DE requires knowledge of the O3DE component development. Visit the [O3DE Component documentation](https://www.docs.o3de.org/docs/user-guide/programming/components/) for more information.

You need to create two components that implement `AIServiceProviderBus::Handler` and `AIModelRequestBus::Handler` in order to create a complete vendor Gem:
- `Service provider component` - responsible for the communication with the AI service.
- `Model component` - responsible for the configuration and creation of requests for the AI model.

A custom registration system is used to register the components, making them available in the `GenAIFramework` Gem.

Note, that you might want to implement only one. E.g., you might want to implement only a model component and use it with an existing AI service provider.

## Creating the components

Create two components: one for the `Service provider` and one for the `Model`. The `Service provider` component should implement the `AIServiceProviderBus::Handler` and the `Model` component should implement the `AIModelRequestBus::Handler`.

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
        // more code goes here
        if (auto editContext = serializeContext->GetEditContext())
        {
            editContext->Class<MyServiceProvider>("My Service Provider", "");
            // more code goes here
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
        // more code goes here
        if (auto editContext = serializeContext->GetEditContext())
        {
            editContext->Class<MyModel>("My Model", "");
            // more code goes here
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
