
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
    
    Source/Communication/JSONHttp/BasicJSONRequesterController.h
    Source/Communication/JSONHttp/BasicJSONRequesterController.cpp
    Source/Communication/JSONHttp/BasicJSONRequesterComponent.h
    Source/Communication/JSONHttp/BasicJSONRequesterComponent.cpp

    Source/RequestGenerator/ollama/OllamaContextRequestGeneratorComponent.h
    Source/RequestGenerator/ollama/OllamaContextRequestGeneratorComponent.cpp
    Source/RequestGenerator/ollama/OllamaContextRequestGeneratorController.h
    Source/RequestGenerator/ollama/OllamaContextRequestGeneratorController.cpp
    Source/RequestGenerator/ollama/OllamaBasicPromptConfiguration.h
    Source/RequestGenerator/ollama/OllamaBasicPromptConfiguration.cpp
    Source/RequestGenerator/ollama/OllamaBasicRequestGenerator.h
)
