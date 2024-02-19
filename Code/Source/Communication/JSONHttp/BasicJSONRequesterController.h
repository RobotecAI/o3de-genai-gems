
#pragma once

#include <AICore/Communication/RequesterBus.h>
#include <AzCore/Component/ComponentBus.h>
#include <AzCore/RTTI/RTTIMacros.h>
#include <aws/core/utils/json/JsonSerializer.h>

namespace AZ
{
    AZ_TYPE_INFO_SPECIALIZE(Aws::Utils::Json::JsonValue, "{61466e55-f27a-4dd5-87e0-d3de11c187ae}");
}

namespace AICore
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

    class BasicJSONRequesterController : public RequesterBus<Aws::Utils::Json::JsonValue>::Handler
    {
    public:
        AZ_RTTI(BasicJSONRequesterController, "{111b48a7-126c-4796-92cb-650b121a7590}");

        BasicJSONRequesterController() = default;
        explicit BasicJSONRequesterController(const BasicJSONRequesterConfiguration& config);

        static void Reflect(AZ::ReflectContext* context);

        //////////////////////////////////////////////////////////////////////////
        // Controller component
        void Init();
        void Activate(AZ::EntityId entityId);
        void Deactivate();
        void SetConfiguration(const BasicJSONRequesterConfiguration& config);
        const BasicJSONRequesterConfiguration& GetConfiguration() const;
        //////////////////////////////////////////////////////////////////////////

        //////////////////////////////////////////////////////////////////////////
        // RequesterBus<Aws::Utils::Json::JsonValue> overrides
        void SendRequest(
            Aws::Utils::Json::JsonValue request,
            AZStd::function<void(Aws::Utils::Json::JsonValue, Aws::Http::HttpResponseCode)> callback) override;
        //////////////////////////////////////////////////////////////////////////

    private:
        BasicJSONRequesterConfiguration m_configuration;
    };

} // namespace AICore
