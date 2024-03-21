
#pragma once

#include <AzCore/Component/Component.h>
#include <AzFramework/Components/ComponentAdapter.h>
#include <GenAIFramework/Communication/AIServiceRequesterBus.h>

namespace GenAIOllama
{
    class BasicJSONRequesterConfiguration : public AZ::ComponentConfig
    {
    public:
        AZ_RTTI(BasicJSONRequesterConfiguration, "{18ddba8f-1fe5-4dde-a0f4-90dccbda5f33}");

        BasicJSONRequesterConfiguration() = default;
        ~BasicJSONRequesterConfiguration() = default;

        static void Reflect(AZ::ReflectContext* context);

        AZStd::string m_url = "";
        AZStd::string m_contentType = "application/json";
    };

    class BasicJSONRequesterComponent
        : public AZ::Component
        , public GenAIFramework::AIServiceRequesterBus::Handler

    {
    public:
        AZ_COMPONENT(BasicJSONRequesterComponent, "{aad62d35-c628-4141-b759-0d3764013b29}", AZ::Component)

        BasicJSONRequesterComponent() = default;
        BasicJSONRequesterComponent(const BasicJSONRequesterConfiguration& config);
        ~BasicJSONRequesterComponent() = default;

        static void Reflect(AZ::ReflectContext* context);

        //////////////////////////////////////////////////////////////////////////
        // Component overrides
        void Activate() override;
        void Deactivate() override;
        //////////////////////////////////////////////////////////////////////////

    private:
        //////////////////////////////////////////////////////////////////////////
        // AIServiceRequesterBus::Handler
        void SendRequest(const AZStd::string& request, AZStd::function<void(AZ::Outcome<AZStd::string, AZStd::string>)> callback) override;
        //////////////////////////////////////////////////////////////////////////

        BasicJSONRequesterConfiguration m_configuration;
    };
} // namespace GenAIOllama
