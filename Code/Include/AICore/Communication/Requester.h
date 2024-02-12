
#pragma once

#include <AzCore/Memory/SystemAllocator.h>
#include <AzCore/RTTI/RTTIMacros.h>
#include <AzCore/RTTI/TypeInfo.h>
#include <AzCore/std/functional.h>
#include <AzCore/std/string/string.h>

#include <AzCore/RTTI/RTTI.h>
#include <AzCore/RTTI/TemplateInfo.h>

namespace AICore
{
    template<typename RequestType>
    class Requester
    {
    public:
        // AZ_CLASS_ALLOCATOR(Requester, AZ::SystemAllocator);

        Requester() = default;
        virtual ~Requester() = default;

        virtual void SendRequest(RequestType request, AZStd::function<void(RequestType, AZStd::optional<AZStd::string>)> callback) = 0;

        using Request = RequestType;
    };

} // namespace AICore

namespace AZ
{
    AZ_TYPE_INFO_TEMPLATE(AICore::Requester, "{0bcac4e2-09a9-4702-98dd-ef37006664a7}", AZ_TYPE_INFO_TYPENAME);
}