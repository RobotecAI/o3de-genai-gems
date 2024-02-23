
#include "BasicJSONRequesterController.h"
#include <AzCore/RTTI/BehaviorContext.h>
#include <AzCore/Serialization/EditContext.h>
#include <AzCore/Serialization/SerializeContext.h>
#include <HttpRequestor/HttpRequestorBus.h>
#include <HttpRequestor/HttpTypes.h>

namespace AICore
{

    void BasicJSONRequesterConfiguration::Reflect(AZ::ReflectContext* context)
    {
        if (auto serializeContext = azrtti_cast<AZ::SerializeContext*>(context))
        {
            serializeContext->Class<BasicJSONRequesterConfiguration, AZ::ComponentConfig>()
                ->Version(0)
                ->Field("url", &BasicJSONRequesterConfiguration::m_url)
                ->Field("contentType", &BasicJSONRequesterConfiguration::m_contentType);

            if (auto editContext = serializeContext->GetEditContext())
            {
                editContext
                    ->Class<BasicJSONRequesterConfiguration>("Basic JSON requester configuration", "Basic JSON requester configuration")
                    ->ClassElement(AZ::Edit::ClassElements::EditorData, "")
                    ->Attribute(AZ::Edit::Attributes::Category, "AICore")
                    ->Attribute(AZ::Edit::Attributes::AppearsInAddComponentMenu, AZ_CRC_CE("Game"))
                    ->DataElement(
                        AZ::Edit::UIHandlers::Default,
                        &BasicJSONRequesterConfiguration::m_url,
                        "EndpointURL",
                        "An url with port pointing to an HTTP endpoint (format addres:port)")
                    ->DataElement(
                        AZ::Edit::UIHandlers::Default,
                        &BasicJSONRequesterConfiguration::m_contentType,
                        "Content type",
                        "Content type of the request");
            }
        }
    }

    void BasicJSONRequesterController::Reflect(AZ::ReflectContext* context)
    {
        BasicJSONRequesterConfiguration::Reflect(context);
        if (auto serializeContext = azrtti_cast<AZ::SerializeContext*>(context))
        {
            serializeContext->Class<BasicJSONRequesterController>()->Version(0)->Field(
                "Configuration", &BasicJSONRequesterController::m_configuration);

            if (auto editContext = serializeContext->GetEditContext())
            {
                editContext->Class<BasicJSONRequesterController>("Basic JSON requester controller", "")
                    ->ClassElement(AZ::Edit::ClassElements::EditorData, "")
                    ->Attribute(AZ::Edit::Attributes::Category, "AICore")
                    ->Attribute(AZ::Edit::Attributes::AppearsInAddComponentMenu, AZ_CRC_CE("Game"))
                    ->DataElement(
                        AZ::Edit::UIHandlers::Default,
                        &BasicJSONRequesterController::m_configuration,
                        "Configuration",
                        "Configuration for the basic JSON requester")
                    ->Attribute(AZ::Edit::Attributes::Visibility, AZ::Edit::PropertyVisibility::ShowChildrenOnly);
            }
        }

        if (auto behaviorContext = azrtti_cast<AZ::BehaviorContext*>(context))
        {
            behaviorContext->EBus<RequesterBus<Aws::Utils::Json::JsonValue>>("RequesterBus")
                ->Attribute(AZ::Script::Attributes::Scope, AZ::Script::Attributes::ScopeFlags::Automation)
                ->Attribute(AZ::Script::Attributes::Category, "AICore")
                ->Attribute(AZ::Script::Attributes::Module, "aicore")
                ->Event("SendRequest", &RequesterBus<Aws::Utils::Json::JsonValue>::Events::SendRequest);
        }
    }

    BasicJSONRequesterController::BasicJSONRequesterController(const BasicJSONRequesterConfiguration& config)
        : m_configuration(config)
    {
    }

    void BasicJSONRequesterController::Init()
    {
    }

    void BasicJSONRequesterController::Activate(AZ::EntityId entityId)
    {
        RequesterBus<Aws::Utils::Json::JsonValue>::Handler::BusConnect(entityId);
    }

    void BasicJSONRequesterController::Deactivate()
    {
        RequesterBus<Aws::Utils::Json::JsonValue>::Handler::BusDisconnect();
    }

    void BasicJSONRequesterController::SetConfiguration(const BasicJSONRequesterConfiguration& config)
    {
        m_configuration = config;
    }

    const BasicJSONRequesterConfiguration& BasicJSONRequesterController::GetConfiguration() const
    {
        return m_configuration;
    }

    void BasicJSONRequesterController::SendRequest(
        Aws::Utils::Json::JsonValue request, AZStd::function<void(Aws::Utils::Json::JsonValue, Aws::Http::HttpResponseCode)> callback)
    {
        HttpRequestor::Headers headers;
        headers["Content-Type"] = m_configuration.m_contentType.c_str();

        HttpRequestor::Callback innerCallback =
            [callback](const Aws::Utils::Json::JsonView& jsonView, Aws::Http::HttpResponseCode responseCode)
        {
            Aws::Utils::Json::JsonValue response(jsonView.WriteCompact());
            callback(response, responseCode);
        };

        HttpRequestor::HttpRequestorRequestBus::Broadcast(
            &HttpRequestor::HttpRequestorRequests::AddRequestWithHeadersAndBody,
            m_configuration.m_url,
            Aws::Http::HttpMethod::HTTP_POST,
            headers,
            request.View().WriteCompact().c_str(),
            innerCallback);
    }
} // namespace AICore