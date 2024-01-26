
#include "Communication/JSONHttp/BasicJSONRequester.h"

#include <HttpRequestor/HttpRequestorBus.h>
#include <HttpRequestor/HttpTypes.h>
#include <aws/core/utils/json/JsonSerializer.h>

namespace AICore
{
    BasicJSONRequester::BasicJSONRequester(AZStd::string url)
        : m_url(url)
    {
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
