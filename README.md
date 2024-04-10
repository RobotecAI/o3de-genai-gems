# o3de-genai-gems

`o3de-genai-gems` repo contains the core Gem of the Gen AI O3DE Framework and a collection of Gen AI Vendor Gems.

## Context
[Open 3D Engine](https:://o3de.org) - an open-source game & simulation engine. O3DE is extendable through modules called Gems. This repository is a collection of such Gems related to one functionality.

**Gen AI O3DE Framework** is a framework that enables employing Generative AI in specific tasks in O3DE. Three main components build this framework:
1. **Gen AI Framework Gem** is a core framework Gem intended for developers to build on. As such, it does not, by itself, do anything useful that is user-facing. Other Gems will use it to build features powered by Generative AI.
2. **Gen AI Vendor Gems** is a collection of separate Gems used to connect to specific AI services or models. This repo contains multiple examples of such Gems.
3. **Gen AI Feature Gems** is a collection of separate Gems that execute certain functionalities in O3DE based on Gen AI responses, such as level generation/modification or character control.

In other words, a feature Gem prepares the query to the Generative AI (e.g. asks to modify the level's layout given the current layout) and a vendor Gem is responsible for converting this query into a meaningful format and connecting with a given AI model. _GenAIFramework_ Gem connects the two, i.e., it ensures that all vendor Gems are available for feature Gems. The framework is summarized in the picture below.

![Framework Design](docs/images/GenAIFramework.png)

More details about the architecture of the system are given in the [RFC](https://github.com/o3de/sig-simulation/issues/87) document. The information about `GenAIFramework Gem` can be found in the [dedicated README.md file](Gems/GenAIFramework/README.md)

## Available vendor Gems
The following Gems are available in this repository:
- `GenAIBedrock Gem` - this vendor Gem implements communication with [Amazon Bedrock service](https://aws.amazon.com/bedrock/) that offers a choice of high-performing models from leading AI companies.
- `GenAIMock`- this mocked vendor Gem can produce predictable responses based on given input options, that will give stable, cost-effective and fast tests.
- `GenAIOllama` - this vendor Gem implements communication with [Ollama toolset](https://github.com/ollama/ollama) that allows getting up and running language models locally.

## Contribute
For information about contributing to O3DE, visit [https://o3de.org/docs/contributing/](https://o3de.org/docs/contributing/).

## License
For terms please see the LICENSE*.TXT files at the root of this distribution.
