
#pragma once

#include "AzCore/std/string/string.h"
namespace AICore
{
    struct OllamaBasicRequestConfiguration
    {
        AZStd::string m_model;
        AZStd::optional<AZStd::string> m_format = {};
        AZStd::optional<AZStd::string> m_options = {};
        AZStd::optional<AZStd::string> m_system = {};
        AZStd::optional<AZStd::string> m_template = {};
        AZStd::optional<bool> m_stream = { false };
        AZStd::optional<bool> m_raw = { false };
    };
} // namespace AICore