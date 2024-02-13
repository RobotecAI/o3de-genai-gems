
set(FILES
    Source/AICoreModuleInterface.cpp
    Source/AICoreModuleInterface.h
    Source/Clients/AICoreSystemComponent.cpp
    Source/Clients/AICoreSystemComponent.h
    
    Include/AICore/Communication/RequesterBus.h
    Source/Communication/JSONHttp/BasicJSONRequesterController.h
    Source/Communication/JSONHttp/BasicJSONRequesterController.cpp
    Source/Communication/JSONHttp/BasicJSONRequesterComponent.h
    Source/Communication/JSONHttp/BasicJSONRequesterComponent.cpp

    Include/AICore//RequestGenerator/RequestGeneratorBus.h
    Source/RequestGenerator/ollama/OllamaContextRequestGeneratorComponent.h
    Source/RequestGenerator/ollama/OllamaContextRequestGeneratorComponent.cpp
    Source/RequestGenerator/ollama/OllamaContextRequestGeneratorController.h
    Source/RequestGenerator/ollama/OllamaContextRequestGeneratorController.cpp
    Source/RequestGenerator/ollama/OllamaBasicPromptConfiguration.h
    Source/RequestGenerator/ollama/OllamaBasicPromptConfiguration.cpp
)
