## Scene Generation feature

This feature allows to ask the model to generate scenes using available prefabs.

The configured language model is used for Python code generation that manipulates prefabs through the interface available in [Python Editor Bindings](https://docs.o3de.org/docs/user-guide/editor/editor-automation/).

>NOTE: The available [system prompt](prompt_templates/layout_designer_system_prompt_template.md) has been prepared for strong Claude models. It is useful to adjust the prompt (e.g. add examples) for other, especially weaker models. For best results Claude Sonnet 3.5 is recommended.

### Setting up a chat with the Scene Generation feature

The setup procedure is the same as described in [this section](../../../../../docs/gettingStarted.md#setting-up-a-new-vendor-and-a-chat-with-o3de-assistant-feature). Select `Scene Generation (Python)` feature in the 12. step.

For this feature, it is also necessary to make some adjustments:
-  in the 8. step increase the `Max tokens to sample` parameter to a large value as the responses containing code can be quite long. For example, for Claude models use `Max tokens to sample=4096`.
- _(if you are using Anthropic API)_:  in the 10. step (AI Service Provider configuration) increase `Request timeout (ms)` to a large value, e.g. 30000ms (30s). Large models like Opus (which are recommended for this feature) can sometimes response quite slowly.

### Prerequisites

The feature is currently configured (with `prefabs.yaml` file) to use prefabs available in [WarehouseAssets](https://github.com/RobotecAI/o3de-extras/tree/development/Gems/WarehouseAssets). Thus, the WarehouseAssets gem must be enabled in the project.

```sh
cd <o3de-directory>
scripts/o3de.sh
scripts/o3de.sh register -gp <o3de-extras>/Gems/WarehouseAssets
scripts/o3de.sh enable-gem -pp <project name> -gp <o3de-extras>/Gems/WarehouseAssets
```

### Usage

**Example questions**

- *Create a warehouse with several rows of various racks*
- *Remove small racks*
- *Move the racks closer to the wall*
- *Create the same warehouse next to the existing one*

>NOTE: The more specifc the request is, the better the results will be. For example, instead of asking to create a warehouse, ask to create a warehouse with a specific number of rows and racks.

### Limitations

- Only prefabs from the `WarehouseAssets` gem are available
- Currently the model can not rotate objects in the scene
- Prompts for smaller/weaker models may need to be adjusted
