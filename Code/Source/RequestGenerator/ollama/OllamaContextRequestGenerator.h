
#pragma once

#include "OllamaBasicRequestGenerator.h"
#include <AzCore/Memory/SystemAllocator.h>
#include <AzCore/RTTI/RTTI.h>
#include <AzCore/RTTI/RTTIMacros.h>
#include <AzCore/std/containers/vector.h>

namespace AICore
{
    class OllamaContextRequestGenerator : public OllamaBasicRequestGenerator
    {
    public:
        OllamaContextRequestGenerator(OllamaBasicPromptConfiguration config);
        OllamaContextRequestGenerator(const OllamaContextRequestGenerator& other) = default;

        ~OllamaContextRequestGenerator() = default;

        Aws::Utils::Json::JsonValue EncodePrompt(AZStd::string prompt);
        AZStd::string DecodeRequest(Aws::Utils::Json::JsonValue request);

    protected:
        AZStd::vector<int> m_context;
    };
} // namespace AICore