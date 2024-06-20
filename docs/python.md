# `GenAIFramework Gem` asynchronous AI API for Python usage

The API allows you to get responses from AI services in an asynchronous way. 
It is based on the `AZ::EBus` and `AZ::BehaviorContext`. 

Selecting an available AI service and model configuration can be done by first getting all available models:
```python
import azlmbr.ai
import azlmbr.bus as bus
serviceProvidersNames = azlmbr.ai.GenAIFrameworkRequestBus(bus.Broadcast, 'GetServiceProviderNames')
modelConfigurationsNames = azlmbr.ai.GenAIFrameworkRequestBus(bus.Broadcast, 'GetModelConfigurationNames')
```

And after getting the available methods the models can be selected:
```python
result = azlmbr.ai.asyncRequestBus(bus.Broadcast, 'SetServiceProviderByName', 'example_name')
result = azlmbr.ai.asyncRequestBus(bus.Broadcast, 'SetModelConfigurationByName', 'example_name')
```
The result is true if the model configuration or service provider is found and set. False otherwise. It is also possible to verify which model configuration or service provider is currently set by calling the following:
```python
providerName = azlmbr.ai.asyncRequestBus(bus.Broadcast, 'GetActiveServiceProviderName')
modelConfiguration = azlmbr.ai.asyncRequestBus(bus.Broadcast, 'GetActiveModelConfigurationName')
```
The output name might be empty in the case of a service provider or model configuration respectively being not set.

Sending a prompt example:
```python
ticket = azlmbr.ai.asyncRequestBus(bus.Broadcast, 'SendPromptToLLM', "Hello World")
```
The ticket is a random identifier used to identify the response.

You can ask if the response is ready using the following example:
```python
is_ready = azlmbr.ai.asyncRequestBus(bus.Broadcast, 'IsResponseReady', ticket)
print(is_ready)
```

Use the subsequent code snippet to get the response (when available):
```python
response = azlmbr.ai.asyncRequestBus(bus.Broadcast, 'GetResponse', ticket)
print(response)
```

To get a registered name of the currently used model configuration or service provider use:
```python
registered_name = azlmbr.ai.asyncRequestBus(azlmbr.bus.Broadcast, "GetModelConfigurationTypename")
registered_name = azlmbr.ai.asyncRequestBus(azlmbr.bus.Broadcast, "GetServiceProviderTypename")
``` 

To reset the model history, use the following code snippet:
```python
azlmbr.ai.asyncRequestBus(bus.Broadcast, 'ResetModelHistory')
```

Changing the model configurations can be done by calling the following:
```python
result = azlmbr.ai.asyncRequestBus(bus.Broadcast, "SetModelParameter", "systemMessage", "This is a test system message")
```
The result will be an empty string if the model parameter is set successfully. Otherwise, it will contain an error message.

To enable or disable the model history, use the following code snippet:
```python
# Enable model history
azlmbr.ai.asyncRequestBus(bus.Broadcast, 'EnableModelHistory', True)
# Disable model history
azlmbr.ai.asyncRequestBus(bus.Broadcast, 'EnableModelHistory', False)
```