# Vendor Gems

Vendor Gems provide mechanisms to communicate with certain generative AI models running on certain servers. Currently only text communication is supported.

## Available Gems
Two gems are available for vendor communication:
- `GenAIVendorBundle` - supports `Claude` and `Ollama` vendors with the ability for HTTP communication.
- `GenAIAmazonBedrockVendorBundle` - extends the `GenAIVendorBundle` with the ability to communicate with the `Amazon Bedrock` service for the `Claude` model.

The O3DE Generative AI framework also includes a mocked `GenAIMockVendor` Gem, which can generate predictable responses based on input data in JSON format. This gem serves as a cost-effective and efficient testing tool, allowing developers to quickly verify their implementations while also providing a sample implementation of a real-world vendor Gem.

Add `GenAIFramework` Gem and the appropriate vendor bundle Gems to your project to enable Generative AI in O3DE. See the [getting started document](./gettingStarted.md) for a sample configuration of a `Claude` model using `Anthropic` service provider. A quick tutorial on creating a new vendor for `GenAIFramework` is presented in [new vendor text](./newVendor.md).

### Service and model distinction
Each vendor has two main elements: the `Model component` and the `Service component`. 

The first one configures the user query. This is typically done by adding some extra data on top of the user's input to form a structure that matches the AI model requirements. The extra data can represent model's version and other parameters each model uses. The output can be a `JSON` or `XML` format that can be understood by a certain model. 

Then this request is propagated to the `Service component`, which sends it to the model and returns the response. The `Service Component` is responsible for the communication with the model running on a server or a local machine. This communication is resolved using either HTTP (in the case of `GenAIVendorBundle`), or the AWS SDK (in the case of `GenAIAmazonBedrockVendorBundle`). 

Lastly, the model's response is extracted by the `Model component`.

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

Each _model component_ can be paired with the appropriate _service provider component_ to pair an _AI Agent_. This _agent_ communicates with the model running on a given hardware (e.g. `Claude Messages API` with `Claude HTTP Service Provider` to run `Claude` model in a cloud).

### Using the `GenAIAmazonBedrockVendorBundle` Gem
Claude models can be used with the `Amazon Bedrock` service provider. To set up the `Claude` model with `Amazon Bedrock` the `Service provider` needs to be changed. Select the `Claude Amazon Bedrock` service provider instead of the `Claude HTTP` service provider and set up the fields accordingly. Additionally, `Anthropic` version in the model configuration should be changed to match the versions specified in the [Claude Amazon Bedrock documentation](https://docs.aws.amazon.com/bedrock/latest/userguide/model-parameters-anthropic-claude-messages.html#model-parameters-anthropic-claude-messages-overview) (example `bedrock-2023-05-31`).

### Using the Ollama vendor
The `Ollama` vendor configuration is similar to `Claude`. The main difference is that `Ollama` allows for the selection of prepared models. In the `Model configuration` input the appropriate
model name (e.g. `mistral`, `llama3`).

## Mock vendor Gem

The `GenAIMockVendor` Gem is a testing tool that enables developers to get predictable responses that are prepared in a in JSON format The Gem implements a `Mock Model Component` and `Mock Service Provider` that should be paired together to create a _Mock AI Agent_. The agent returns the messages read from the JSON input sequentially, ignoring the user's prompt. It is also possible to force the agent to restart the scheme or to read the data from a certain spot. 

### JSON input format
The input file is a parameter in `Mock Service Provider` configuration. This designs allows to create multiple _Mock AI Agents_ feeding the _feature_ in test with different data simultaneously. During the initialization, _Mock AI Agent_ reads the input file and looks for vector named `testData`. The elements in this vector are used as prompt's responses sequentially. One element can be either a string or a vector of strings. The latter is converted by `Mock Service Provider` into a multiline response.

A sample mock input file is [stored in the repository](./../Gems/GenAIMock/Assets/sample.json).

### User's prompt

Mock Model Component` parses the query sent by the _feature_ to keep only the text input from the user. Hence, it removes the history and the system message typically injected as a prompt's context. The mock AI Agent uses an internal counter to return the messages prepared in a JSON input document sequentially. In particular, it provides one response from counter's position for each query. 

The counter can be set to the beginning using `restart` or `reset` prompt. Additionally, it can be set to a certain position when prompt consists of only a number. E.g. prompt `0` will force the _Mock AI Agent_ to return the first response from a JSON file similarly to `reset` or `restart`.
