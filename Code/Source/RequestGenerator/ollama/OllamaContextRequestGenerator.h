
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
        AZ_RTTI(OllamaContextRequestGenerator, "{5d67a8fb-5684-4e69-b382-9f130ae463b6}", OllamaBasicRequestGenerator);
        AZ_CLASS_ALLOCATOR(OllamaContextRequestGenerator, AZ::SystemAllocator);

        OllamaContextRequestGenerator(OllamaBasicPromptConfiguration config);
        ~OllamaContextRequestGenerator() = default;

        Aws::Utils::Json::JsonValue EncodePrompt(AZStd::string prompt);
        AZStd::string DecodeRequest(Aws::Utils::Json::JsonValue request);

    protected:
        AZStd::vector<int> m_context;
    };
} // namespace AICore