
set(FILES
    Source/AICoreModuleInterface.cpp
    Source/AICoreModuleInterface.h
    
    Source/Clients/AICoreSystemComponent.cpp
    Source/Clients/AICoreSystemComponent.h
    Source/Test/AICoreTestComponent.cpp
    Source/Test/AICoreTestComponent.h
    Source/Test/AICoreTest.cpp
    Source/BehaviorContextUtils/BehaviorContextDump.cpp
    Source/BehaviorContextUtils/BehaviorContextDump.h
    Source/Action/AICoreActionHandler.cpp
    Source/Action/AICoreActionHandler.h
    Source/Action/AICoreActionRequestHandler.cpp
    Source/Action/AICoreActionRequestHandler.h
    Source/Action/AICoreLauncherScriptExecutor.cpp
    Source/Action/AICoreLauncherScriptExecutor.h
    Source/Clients/AICoreSystemComponentConfiguration.cpp
    Source/Clients/AICoreSystemComponentConfiguration.h

    Source/AICoreSettingsRegistryManager/AICoreSettingsRegistryManager.cpp
    Source/AICoreSettingsRegistryManager/AICoreSettingsRegistryManager.h

    Source/Communication/AwsSdk/AwsSdkBedrockRequesterComponent.h
    Source/Communication/AwsSdk/AwsSdkBedrockRequesterComponent.cpp

    Source/RequestGenerator/Claude/ClaudePromptConfiguration.h
    Source/RequestGenerator/Claude/ClaudePromptConfiguration.cpp
    Source/RequestGenerator/Claude/ClaudePromptGeneratorComponent.h
    Source/RequestGenerator/Claude/ClaudePromptGeneratorComponent.cpp
)
