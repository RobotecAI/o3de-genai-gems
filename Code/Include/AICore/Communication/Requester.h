
#pragma once

#include <AzCore/Memory/SystemAllocator.h>
#include <AzCore/RTTI/RTTIMacros.h>
#include <AzCore/RTTI/TypeInfo.h>
#include <AzCore/std/functional.h>
#include <AzCore/std/string/string.h>

namespace AICore
{
    template<typename RequestType>
    class Requester
    {
    public:
        AZ_CLASS_ALLOCATOR(Requester, AZ::SystemAllocator);

        Requester() = default;
        virtual ~Requester() = default;

        virtual void SendRequest(RequestType request, AZStd::function<void(RequestType, AZStd::optional<AZStd::string>)> callback) = 0;

        using Request = RequestType;
    };

} // namespace AICore
