# o3de-genai-gems

`o3de-genai-gems` repo contains the core Gem of the Gen AI O3DE Framework, a sample Gen AI Feature Gem for assisting with O3DE related questions and a collection of Gen AI Vendor Gems.

## Context
[Open 3D Engine](https:://o3de.org) - an open-source game & simulation engine. O3DE is extendable through modules called Gems. This repository is a collection of such Gems related to one functionality.

**Gen AI O3DE Framework** is a framework that enables employing Generative AI in specific tasks in O3DE. Three main components build this framework:
1. A core framework Gem that is intended for developers to build on. It is a bridge between the user and the AI: other Gems will use it to build features powered by Generative AI.
2. A collection of vendor Gems used to connect to specific AI services or models.
3. A collection of feature Gems that execute certain functionalities in O3DE based on Gen AI responses, such as level generation/modification or character control.

In other words, a feature Gem prepares the query to the Generative AI (e.g. asks to modify the level's layout given the current layout) based on the user's input and a vendor Gem is responsible for converting this query into a meaningful format and connecting with a given AI model. _GenAIFramework_ Gem connects the two, i.e., it ensures that all vendor Gems are available for feature Gems. Importantly, it also provides the user interface for AI communication. The framework is summarized in the picture below.

![Framework Design](docs/images/GenAIFramework.png)

More details about the architecture of the system are given in the [RFC](https://github.com/o3de/sig-simulation/issues/87) document.

## Repository structure

### Folder structure
_TODO_

### `GenAIFramework Gem`
The information about `GenAIFramework Gem` can be found in the [dedicated README.md file](Gems/GenAIFramework/README.md)

### Vendor Gems
This repository contains two Gems that bundle multiple AI models and service definitions each:
- `GenAIVendorBundle` - this vendor Gem implements communication with [Anthropic](https://docs.anthropic.com/en/docs/welcome) service for running `Claude` model and [Ollama toolset](https://github.com/ollama/ollama) that allows getting up and running language models locally.
- `GenAIAmazonBedrockVendorBundle Gem` - this vendor Gem implements communication with [Amazon Bedrock service](https://aws.amazon.com/bedrock/) that offers a choice of high-performing models from leading AI companies. This gem is dependent on the `GenAIVendorBundle` Gem and allows for communication with the `Claude` model using the `Amazon Bedrock` service.
- `GenAIMock`- this mocked vendor Gem can produce predictable responses based on given input options, that will give stable, cost-effective and fast tests.

To learn more about the available vendors and find a guide for the configurations, see the [docs/vendors.md](./docs/vendors.md) file.

## Contribute
For information about contributing to O3DE, visit [https://o3de.org/docs/contributing/](https://o3de.org/docs/contributing/).

## License
For terms please see the LICENSE*.TXT files at the root of this distribution.
