# Vendor Gems

Vendor Gems provide mechanisms to communicate with certain generative AI models running on certain servers. Currently only text communication is supported.

## Available Gems
Two gems are available for vendor communication:
- `GenAIVendorBundle` - supports `Claude` and `Ollama` vendors with the ability for HTTP communication.
- `GenAIAmazonBedrockVendorBundle` - extends the `GenAIVendorBundle` with the ability to communicate with the `Amazon Bedrock` service for the `Claude` model.

The O3DE Generative AI framework also includes a mocked `GenAIMock` vendor Gem, which can generate predictable responses based on input data in JSON format. This gem serves as a cost-effective and efficient testing tool, allowing developers to quickly verify their implementations while also providing a sample implementation of a real-world vendor Gem.

Add `GenAIFramework` Gem and the appropriate vendor bundle Gems to your project to enable Generative AI in O3DE. See the [getting started document](./gettingStarted.md) for a sample configuration of a `Claude` model using `Anthropic` service provider. A quick tutorial on creating a new vendor for `GenAIFramework` is presented in [new vendor text](./newVendor.md).

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

### Using the `GenAIAmazonBedrockVendorBundle` Gem
Claude models can be used with the `Amazon Bedrock` service provider. To set up the `Claude` model with `Amazon Bedrock` the `Service provider` needs to be changed. Select the `Claude Amazon Bedrock` service provider instead of the `Claude HTTP` service provider and set up the fields accordingly. Additionally, `Anthropic` version in the model configuration should be changed to match the versions specified in the [Claude Amazon Bedrock documentation](https://docs.aws.amazon.com/bedrock/latest/userguide/model-parameters-anthropic-claude-messages.html#model-parameters-anthropic-claude-messages-overview) (example `bedrock-2023-05-31`).

### Using the Ollama vendor
The `Ollama` vendor configuration is similar to `Claude`. The main difference is that `Ollama` allows for the selection of prepared models. In the `Model configuration` input the appropriate
model name (e.g. `mistral`, `llama3`).
