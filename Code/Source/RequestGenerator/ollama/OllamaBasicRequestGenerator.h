
#pragma once

#include "AzCore/std/any.h"
#include "AzCore/std/string/string_view.h"
#include "RequestGenerator/RequestGenerator.h"

#include <AzCore/std/string/string.h>
#include <aws/core/utils/json/JsonSerializer.h>

namespace AICore
{
    class OllamaBasicRequestGenerator : public RequestGenerator<Aws::Utils::Json::JsonView, AZStd::string>
    {
    public:
        OllamaBasicRequestGenerator() = default;
        ~OllamaBasicRequestGenerator() = default;

        Aws::Utils::Json::JsonView EncodePrompt(const AZStd::string& prompt) override;

        AZStd::string DecodePrompt(const Aws::Utils::Json::JsonView& request) override;
    };
} // namespace AICore