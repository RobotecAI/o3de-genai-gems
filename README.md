# o3de-genai-gems

`o3de-genai-gems` repo contains the core Gem of the Gen AI O3DE Framework, a sample Gen AI Feature Gem for assisting with O3DE related questions and a collection of Gen AI Vendor Gems.

## Context
[Open 3D Engine](https:://o3de.org) - an open-source game & simulation engine. O3DE is extendable through modules called Gems. This repository is a collection of such Gems related to one functionality.

**Gen AI O3DE Framework** is a framework that enables employing Generative AI in specific tasks in O3DE. Three main components build this framework:
1. A core framework Gem that is intended for developers to build on. It is a bridge between the user, O3DE and the AI: other Gems will use it to build features powered by Generative AI.
2. A collection of vendor Gems used to connect to specific AI services or models.
3. A collection of feature Gems that execute certain functionalities in O3DE based on Gen AI responses, such as level generation/modification or character control.

In other words, a feature Gem prepares the query to the Generative AI (e.g. asks to modify the level's layout given the current layout) based on the user's input and a vendor Gem is responsible for converting this query into a meaningful format and connecting with a given AI model. _GenAIFramework_ Gem connects the two, i.e., it ensures that all vendor Gems are available for feature Gems. Importantly, it also provides the user interface for AI communication. The framework is summarized in the picture below.

![Framework Design](docs/images/GenAIFramework.png)

More details about the architecture of the system are given in the [RFC](https://github.com/o3de/sig-simulation/issues/87) document.

## Repository structure

### Folder structure
_TODO_

### `GenAIFramework Gem`

`GenAIFramework Gem` provides a centralized mechanism for integrating users with a diverse array of AI tools. To facilitate this connection, each AI tool - including vendor Gems and feature Gems - must first be registered within the O3DE ecosystem using the framework's standardized interface. Once registered, these AI tools become accessible via a user interface.

The O3DE framework provides a set of Python interfaces that enable seamless integration of existing AI tools with the O3DE interface. This allows developers to leverage their existing knowledge and expertise in AI tool development in Python language while still benefiting from the O3DE ecosystem. To integrate an AI tool into O3DE, developers can implement a feature Gem entirely in Python, with minimal C++ effort required for registering the Gem. This approach provides maximum flexibility and ease of implementation.

Once the Gem is implemented and registered, developers have two options to connect with AI service providers. First, the developers can utilize the existing pipeline to connect with AI service providers via C++ vendor Gems. This allows the design of a feature Gem in Python language while providing the flexibility of the O3DE's user interface in connecting the feature to a wide range of AI services without needing to implement this connection themselves. Alternatively, developers can use the Python API to connect directly with a given AI vendor without interfering with C++ Gems. This approach provides a more streamlined and Python-centric integration experience and allows for feature development tailored to a specific model's interface.

As part of the framework, the chat widget is available for creating conversations using a selected set of AI components, including features, AI models, and AI service providers' APIs. This unified interface enables users to easily discover, select, and utilize various AI capabilities within their application.

The information about `GenAIFramework Gem` can be found in dedicated documentation pages. This includes a detailed description of [all interfaces](./docs/interfaces.md) available in `GenAIFramework Gem` connecting feature Gems with vendor Gems, the details about available [Python support](./docs/python.md) in `GenAIFramework Gem` and the tutorials for creating a [new feature](./docs/newFeature.md) or adding support for a [new vendor](./docs/newVendor.md). Finally, there is a page describing [the UI](./docs/ui.md) with the sample use.

### Vendor Gems
This repository contains two Gems that bundle multiple AI models and service definitions each:
- `GenAIVendorBundle` - this vendor Gem implements communication with [Anthropic](https://docs.anthropic.com/en/docs/welcome) service for running `Claude` model and [Ollama toolset](https://github.com/ollama/ollama) that allows getting up and running language models locally.
- `GenAIAmazonBedrockVendorBundle Gem` - this vendor Gem implements communication with [Amazon Bedrock service](https://aws.amazon.com/bedrock/) that offers a choice of high-performing models from leading AI companies. This gem is dependent on the `GenAIVendorBundle` Gem and allows for communication with the `Claude` model using the `Amazon Bedrock` service.
- `GenAIMock`- this mocked vendor Gem can produce predictable responses based on given input options, that will give stable, cost-effective and fast tests.

To learn more about the available vendors and find a guide for the configurations, see the [docs/vendors.md](./docs/vendors.md) file.

## Setting GenAIFrameworkGem
`GenAIFrameworkGem` needs to be registered in the system so that O3DE can find the requested Gems. The same rule applies to vendor Gems and feature Gems. From the O3DE repo folder, register some or all objects using the `o3de register` command. After registering all Gems in the system and in the project, rebuild your project and start using the AI capabilities with a dedicated [user interface](./docs/ui.md).

The following command allows to register a single Gem (`GenAIFramework` Gem in this example):
```bash
scripts\o3de.bat register --gem-path <o3de-genai-gem>/Gems/GenAIFramework
```
The following command adds a registered Gem to the O3DE project:
```bash
scripts\o3de.bat enable-gem --gem-name GenAIFrameworkGem --project-name <project name>
```

For a complete tutorial on project configuration, see [Creating Projects Using the Command Line Interface](https://o3de.org/docs/welcome-guide/create/creating-projects-using-cli/) in the documentation.

## Contribute
For information about contributing to O3DE, visit [https://o3de.org/docs/contributing/](https://o3de.org/docs/contributing/).

## License
For terms please see the LICENSE*.TXT files at the root of this distribution.
