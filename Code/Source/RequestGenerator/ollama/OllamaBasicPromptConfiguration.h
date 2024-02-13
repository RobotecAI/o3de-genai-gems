
#pragma once

#include <AzCore/Component/ComponentBus.h>
#include <AzCore/RTTI/RTTIMacros.h>
#include <AzCore/std/string/string.h>

namespace AICore
{
    class OllamaBasicPromptConfiguration : public AZ::ComponentConfig
    {
    public:
        AZ_RTTI(OllamaBasicPromptConfiguration, "{c5fa34ff-2bbb-4df4-9aa2-c27320b57f93}");
        OllamaBasicPromptConfiguration() = default;
        ~OllamaBasicPromptConfiguration() = default;

        static void Reflect(AZ::ReflectContext* context);

        AZStd::string m_model = "";
        AZStd::string m_format = "";
        AZStd::string m_options = "";
        AZStd::string m_system = "";
        AZStd::string m_template = "";
        bool m_stream = false;
        bool m_raw = false;
        AZStd::string m_keepAlive = "5m";
    };
} // namespace AICore
