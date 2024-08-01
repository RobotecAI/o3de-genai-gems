
set(FILES
    Source/GenAIVendorBundleModuleInterface.cpp
    Source/GenAIVendorBundleModuleInterface.h

    Source/Providers/Ollama/OllamaHttpProvider.cpp
    Source/Providers/Ollama/OllamaHttpProvider.h
    Source/Models/Ollama/OllamaGenerateModel.cpp
    Source/Models/Ollama/OllamaGenerateModel.h
    Source/Models/Ollama/OllamaChatModel.cpp
    Source/Models/Ollama/OllamaChatModel.h

    Source/Providers/Claude/ClaudeHttpProvider.cpp
    Source/Providers/Claude/ClaudeHttpProvider.h
    Source/Models/Claude/ClaudeModelMessagesAPI.cpp
    Source/Models/Claude/ClaudeModelMessagesAPI.h
    Source/Models/Claude/ClaudeModelTextCompletions.cpp
    Source/Models/Claude/ClaudeModelTextCompletions.h
    Source/Models/Claude/ClaudeModelConfiguration.cpp
    Source/Models/Claude/ClaudeModelConfiguration.h

    Source/Utils/StringManipulation.h
)
