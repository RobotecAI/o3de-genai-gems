# `GenAIFramework Gem` user interface

_TODO_ This is an old version of the UI; the instruction needs to be rewritten.

When loaded, the system component of this Gem should appear in O3DE toolbar and the _Tools__ section of the menu bar. It automatically detects available Gen AI _vendor_ Gems and allows adding them through the provided UI. The widget is meant to configure the model and the AI service that runs this model. After any of the two is added to the framework, it can be configured via the widget and used by the Gen AI _feature_ Gems. The widget stores the configuration in a _setreg_ file, so it can be reloaded and shared if needed. Finally, it allows the removal of the configurations that were added earlier.

A sample screenshot of adding a new model configuration besides the previously added _mock_ configuration is given below.

![Widget: Model Configuration](./../Gems/GenAIFramework/docs/images/ModelConfiguration.png)

A sample screenshot of AI Service Configuration named _basic JSON_ follows (note the _mock_ configuration alongside).

![Widget: AI Service Configuration](./../Gems/GenAIFramework/docs/images/AIServiceConfiguration.png)