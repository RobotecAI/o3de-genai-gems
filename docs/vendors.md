# Vendor Gems
The following vendors are available:
- `Claude`
- `Ollama`  

Each of these vendors supports text communication with the models.

## Available Gems
Two gems are available for vendor communication:
- `GenAIVendorBundle` - supports `Claude` and `Ollama` vendors with the ability for HTTP communication.
- `GenAIAmazonBedrockVendorBundle` - extends the `GenAIVendorBundle` with the ability to communicate with the `Amazon Bedrock` service for the `Claude` model.

To set up the project use the `GenAIFramework` Gem and the appropriate vendor bundle Gems.

### Service and model distinction
Each vendor has two main elements: the `Model component` and the `Service component`. 

The first one configures the user query. This is typically done by adding some extra data on top of the user's input to form a structure that matches the AI model requirements. The extra data can represent model's version and other parameters each model uses. The output can be a `JSON` or `XML` format that can be understood by a certain model. 

Then this request is propagated to the `Service component`, which sends it to the model and returns the response. The `Service Component` is responsible for the communication with the model running on a server or a local machine. This communication is resolved using either HTTP (in the case of `GenAIVendorBundle`), or the AWS SDK (in the case of `GenAIAmazonBedrockVendorBundle`). 

Lastly, the model's response is extracted with the `Model component`.

## Vendor compatibilities
The `GenAIVendorBundle` provides four model components and two providers.  

Model components:
- `Claude messages API` - uses the messages API for the `Claude` model. This model supports `Claude 2.1` and all of `Claude 3` versions.
- `Claude text completions` - uses the text completions API for the `Claude` model. This model only supports the `Claude 2.1` version.
- `Ollama generate` - uses the generate API of [Ollama](https://github.com/ollama/ollama).  
- `Ollama chat` - uses the chat API of [Ollama](https://github.com/ollama/ollama).

Service components:
- `Ollama HTTP` - uses HTTP communication with `Ollama` endpoint.
- `Claude HTTP` - uses HTTP communication with `Claude` model using the `Anthropic` endpoint.
- `Claude Amazon Bedrock` - uses AWS SDK communication with `Claude` model using the `Amazon Bedrock` service (available in `GenAIAmazonBedrockVendorBundle` Gem).

Each `Model component` can be paired with the appropriate `Service component` to communicate with the model (e.g. `Claude messages API` with `Claude HTTP`).

## Setting up a new vendor
### Claude
This is an example of how to set up a vendor in O3DE. It explains how to create and use the `Claude 3` models with the `Claude HTTP` service. Other vendors can be set up similarly.
1. Add `ANTHROPIC_API_KEY` to your environment variables. This can be done using the `export ANTHROPIC_API_KEY=<your_api_key>` command, where `<your_api_key>` is the key provided by `Anthropic`.
2. Open a new project in O3DE with the `GenAIFramework` and `GenAIVendorBundle` Gems.
3. Select the `AI` icon in the upper left corner to open the `Gen AI conversations` window.
4. An `AIAssistant` window will appear.  
![AIWindow](docs/vendors/chatCreation.png)
5. Click the burger button next to the `Model` dropdown box.  
6. Using the dropdown box select the `Claude messages API` model (or any other appropriate model) give it a name and click create.
![ModelConfiguration](docs/vendors/modelConfiguration.png)
7. Select the newly created model  
![Configuration](docs/vendors/configuration.png)
8. Add the `Anthropic version` (example: `2023-06-01`) according to the [Anthropic documentation](https://docs.anthropic.com/en/api/messages). You can also change other fields but it is not necessary.
9. Select the `AI Service Providers` tab and create the `Claude HTTP service provider`.
10. Select the newly created service and fill the fields with appropriate values. You can find all available models on the [Anthropic documentation](https://docs.anthropic.com/en/docs/about-claude/models).  
**Note**: If you see a message about a missing API key, make sure to include it in your environment variables.
![ServiceConfiguration](docs/vendors/provider.png)
11. You can now close the configuration window and the created models should be available for selection.
![FinishedConfiguration](docs/vendors/finishedConfiguration.png)
12. Name the chat and select the `O3DE Assistant` feature and click create.
13. Start the chat and try out the model.  
![Chat](docs/vendors/chat.png)
14. To close the conversation click on the `X` in the top right corner. To add a new conversation click on the `+` button in the tabs.

### Using the `GenAIAmazonBedrockVendorBundle` Gem
Claude models can be used with the `Amazon Bedrock` service provider. To set up the `Claude` model with `Amazon Bedrock` the `Service provider` needs to be changed. Select the `Claude Amazon Bedrock` service provider instead of the `Claude HTTP` service provider and set up the fields accordingly. Additionally, `Anthropic` version in the model configuration should be changed to match the versions specified in the [Claude Amazon Bedrock documentation](https://docs.aws.amazon.com/bedrock/latest/userguide/model-parameters-anthropic-claude-messages.html#model-parameters-anthropic-claude-messages-overview) (example `bedrock-2023-05-31`).


### Using the Ollama vendor
The `Ollama` vendor configuration is similar to `Claude`. The main difference is that `Ollama` allows for the selection of prepared models. In the `Model configuration` input the appropriate
model name (e.g. `mistral`, `llama3`).
