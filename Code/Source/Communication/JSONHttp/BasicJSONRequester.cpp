
#include "Communication/JSONHttp/BasicJSONRequester.h"
#include "AzCore/Serialization/EditContextConstants.inl"

#include <AzCore/Serialization/EditContext.h>
#include <HttpRequestor/HttpRequestorBus.h>
#include <HttpRequestor/HttpTypes.h>
#include <aws/core/utils/json/JsonSerializer.h>

namespace AICore
{
    BasicJSONRequester::BasicJSONRequester()
        : m_url("")
    {
        std::cout << "Requester created with empty URL\n";
    }

    BasicJSONRequester::BasicJSONRequester(AZStd::string url)
        : m_url(url)
    {
        std::cout << "Requester created with URL: " << m_url.c_str() << "\n";
    }

    void BasicJSONRequester::Reflect(AZ::ReflectContext* context)
    {
        if (auto serializeContext = azrtti_cast<AZ::SerializeContext*>(context))
        {
            serializeContext->Class<BasicJSONRequester, Requester<Aws::Utils::Json::JsonValue>>()->Version(0)->Field(
                "url", &BasicJSONRequester::m_url);

            if (auto* editContext = serializeContext->GetEditContext())
            {
                editContext->Class<BasicJSONRequester>("Basic JSON requester", "Basic JSON requester")
                    ->ClassElement(AZ::Edit::ClassElements::EditorData, "")
                    ->Attribute(AZ::Edit::Attributes::Category, "AICore")
                    ->DataElement(AZ::Edit::UIHandlers::Default, &BasicJSONRequester::m_url, "EndpointURL", "");
            }
        }
    }

    void BasicJSONRequester::SendRequest(
        Aws::Utils::Json::JsonValue request, AZStd::function<void(Aws::Utils::Json::JsonValue, AZStd::optional<AZStd::string>)> callback)
    {
        HttpRequestor::Headers headers;
        headers["Content-Type"] = "application/json";

        HttpRequestor::Callback innerCallback =
            [callback](const Aws::Utils::Json::JsonView& jsonView, Aws::Http::HttpResponseCode responseCode)
        {
            Aws::Utils::Json::JsonValue response(jsonView.WriteCompact());
            if (responseCode != Aws::Http::HttpResponseCode::OK)
            {
                callback(response, AZStd::string("Request failed"));
            }
            else
            {
                callback(response, AZStd::optional<AZStd::string>());
            }
        };


        HttpRequestor::HttpRequestorRequestBus::Broadcast(
            &HttpRequestor::HttpRequestorRequests::AddRequestWithHeadersAndBody,
            m_url,
            Aws::Http::HttpMethod::HTTP_POST,
            headers,
            request.View().WriteCompact().c_str(),
            innerCallback);
    };
} // namespace AICore
