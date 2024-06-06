import azlmbr
from azlmbr import bus
import time

from azlmbr.ai import *
import json

def send_text_prompt(input, toolResponse = None):
    tool = Tool('get_weather', 'Get the current weather in a given location', '''{
            "type": "object",
            "properties": {
                "location": {
                    "type": "string",
                    "description": "The city and state, e.g. San Francisco, CA"
                },
                "unit": {
                    "type": "string",
                    "enum": ["celsius", "fahrenheit"],
                    "description": "The unit of temperature, either celsius or fahrenheit"
                }
            },
            "required": ["location"]
        }''')
    message = [tool]
    for elem in input:
        message.append(elem)
    if toolResponse:
        message.append(toolResponse)

    ticket = azlmbr.ai.asyncRequestBus(bus.Broadcast, 'SendPromptToLLM', message)
    
    is_ready = azlmbr.ai.asyncRequestBus(bus.Broadcast, 'IsResponseReady', ticket)
    while not is_ready:
        is_ready = azlmbr.ai.asyncRequestBus(bus.Broadcast, 'IsResponseReady', ticket)
        time.sleep(1)
        if is_ready:
            response = azlmbr.ai.asyncRequestBus(bus.Broadcast, 'GetResponse', ticket)
            return response
        
def process_tool_response(response):
    for result in response:
        if result.typename == ToolUse().typename:
            print(result.ID)
            print(result.Name)
            print(result.Input)
            input_json = json.loads(result.Input)
            # Now you can use the input_json variable to access the parsed JSON data
            # For example, you can print the location and unit values like this:
            print(input_json['location'])
            print(input_json['unit'])

            tool_response = ToolResult()
            tool_response.ToolUseId = result.ID
            tool_response.Name = result.Name
            if input_json['unit'] == 'celsius':
                print('The temperature is 20 degrees Celsius')
                tool_response.Content = 'The temperature is 20 degrees Celsius'
            else:
                print('The temperature is 68 degrees Fahrenheit')
                tool_response.Content = 'The temperature is 68 degrees Fahrenheit'

            return tool_response            

def extract_tool_use(response):
    for result in response:
        if result.typename == ToolUse().typename:
            return result
        
def print_output(results):
    for result in results:
        print(result.typename)
        if result.typename == ToolUse().typename:
            print(result.ID)
            print(result.Name)
            print(result.Input)
        elif result.typename == Text().typename:
            print(result.Text)
        
result = (send_text_prompt([Text("What is the weather in Warsaw?")]))
tool_response = process_tool_response(result)

print_output(send_text_prompt([Text("What is the weather in Warsaw?")] + result, tool_response))

class Foo:
    pass
