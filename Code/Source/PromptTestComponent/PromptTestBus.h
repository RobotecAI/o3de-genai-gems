
#pragma once

#include "AzCore/Component/ComponentBus.h"
#include "AzCore/RTTI/RTTIMacros.h"

namespace AICore
{
    class PromptTestRequests : public AZ::ComponentBus
    {
    public:
        AZ_RTTI(PromptTestRequests, "{377dd175-4560-47d9-93ad-a5066dfef267}");

        static const AZ::EBusHandlerPolicy HandlerPolicy = AZ::EBusHandlerPolicy::Single;
        static const AZ::EBusAddressPolicy AddressPolicy = AZ::EBusAddressPolicy::ById;
        using BusIdType = AZ::EntityId;

        virtual void PromptExecute(AZStd::string prompt) = 0;
        virtual void SetSaveContext(bool saveContext) = 0;
        virtual void ResetContext() = 0;
        virtual void PromptNoExecute(AZStd::string prompt) = 0;
        virtual void Execute(AZStd::string pythonScript) = 0;

        void virtual TestBus() = 0;
    };

    using PromptTestBus = AZ::EBus<PromptTestRequests>;
} // namespace AICore