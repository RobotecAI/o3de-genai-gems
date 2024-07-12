# Scene generation feature

This feature demonstrates the use of the GenAIFramework for scene generation. The language model is used for Python code generation that manipulates prefabs through the interface available in [Python Editor Bindings](https://docs.o3de.org/docs/user-guide/editor/editor-automation/).

## Prerequisites

The feature is currently configured (with `prefabs.yaml` file) to use prefabs available in [WarehouseAssets](https://github.com/RobotecAI/o3de-extras/tree/development/Gems/WarehouseAssets). Thus, the WarehouseAssets gem must be enabled in the project.

```sh
cd <o3de-directory>
scripts/o3de.sh
scripts/o3de.sh register -gp <o3de-extras>/Gems/WarehouseAssets
scripts/o3de.sh enable-gem -pp <project name> -gp <o3de-extras>/Gems/WarehouseAssets
```

## Usage

Configure the model as described in the [Getting Started](../../../../../docs/gettingStarted.md) guide. In 12. step select the "O3DE Scene Generation Assistant (Python)" feature.

**Example questions**

- *Create a warehouse with several rows of various racks*
- *Remove small racks*
- *Move the racks closer to the wall*
- *Create the same warehouse next to the existing one*

>NOTE: The available [system prompt](prompt_templates/layout_designer_system_prompt_template.md) has been prepared for strong Claude models. It is useful to adjust the prompt for other, especially weaker models. For best results Claude Sonnet 3.5 is recommended.