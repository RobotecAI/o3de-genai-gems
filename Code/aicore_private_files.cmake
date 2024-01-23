
set(FILES
    Include/AICore/RequestGenerator/ollama/OllamaBasicRequestGenerator.h
    Include/AICore/RequestGenerator/ollama/OllamaBasicRequestConfiguration.h
    Include/AICore/Prompter/BasicOllamaPrompter.h
    Include/AICore/Communication/JSONHttp/BasicJSONRequester.h
    
    Source/Communication/JSONHttp/BasicJSONRequester.cpp
    Source/Prompter/BasicOllamaPrompter.cpp
    Source/RequestGenerator/ollama/OllamaBasicRequestGenerator.cpp

    Source/AICoreModuleInterface.cpp
    Source/AICoreModuleInterface.h
    Source/Clients/AICoreSystemComponent.cpp
    Source/Clients/AICoreSystemComponent.h

)
