
#include <GenAIMock/GenAIMockTypeIds.h>
#include <GenAIMockModuleInterface.h>
#include "GenAIMockSystemComponent.h"

namespace GenAIMock
{
    class GenAIMockModule
        : public GenAIMockModuleInterface
    {
    public:
        AZ_RTTI(GenAIMockModule, GenAIMockModuleTypeId, GenAIMockModuleInterface);
        AZ_CLASS_ALLOCATOR(GenAIMockModule, AZ::SystemAllocator);
    };
}// namespace GenAIMock

#if defined(O3DE_GEM_NAME)
AZ_DECLARE_MODULE_CLASS(AZ_JOIN(Gem_, O3DE_GEM_NAME), GenAIMock::GenAIMockModule)
#else
AZ_DECLARE_MODULE_CLASS(Gem_GenAIMock, GenAIMock::GenAIMockModule)
#endif
