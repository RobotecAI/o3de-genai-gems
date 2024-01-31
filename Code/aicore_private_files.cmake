
set(FILES
    Source/AICoreModuleInterface.cpp
    Source/AICoreModuleInterface.h
    Source/Clients/AICoreSystemComponent.cpp
    Source/Clients/AICoreSystemComponent.h
    
    Include/AICore/Communication/Requester.h
    Include/AICore/RequestGenerator/RequestGenerator.h
    Include/AICore/Prompter/Prompter.h

    Source/Communication/JSONHttp/BasicJSONRequester.h
    Source/Communication/JSONHttp/BasicJSONRequester.cpp

    Source/RequestGenerator/ollama/OllamaBasicPromptConfiguration.h
    Source/RequestGenerator/ollama/OllamaBasicRequestGenerator.h
    Source/RequestGenerator/ollama/OllamaBasicRequestGenerator.cpp
    Source/RequestGenerator/ollama/OllamaContextRequestGenerator.h
    Source/RequestGenerator/ollama/OllamaContextRequestGenerator.cpp

)
