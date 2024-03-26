# AI Core Gem
Core features and interfaces enabling use of modern AI with O3DE for variety of purposes.  

# Asynchronouse AI API

It allows to get repsponses from AI services in an asynchronous way. 
It is based on the `AZ::EBus` and `AZ::BehaviorContext`. 

To send a response:
```python
import azlmbr.ai;
import azlmbr.bus as bus;
ticket = azlmbr.ai.asyncRequestBus(bus.Broadcast, 'SendPromptToLLM', "Hello,World")
```
The ticket is random identifier that is used to identify the response.
You can ask if the response is ready:
```python
is_ready = azlmbr.ai.asyncRequestBus(bus.Broadcast, 'IsResponseReady', ticket)
```

To get response:
```python
response = azlmbr.ai.asyncRequestBus(bus.Broadcast, 'GetResponse', ticket)
print(response)tus
```