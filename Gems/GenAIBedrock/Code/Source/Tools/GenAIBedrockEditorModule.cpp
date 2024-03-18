
#include <GenAIBedrock/GenAIBedrockTypeIds.h>
#include <GenAIBedrockModuleInterface.h>
#include "GenAIBedrockEditorSystemComponent.h"

namespace GenAIBedrock
{
    class GenAIBedrockEditorModule
        : public GenAIBedrockModuleInterface
    {
    public:
        AZ_RTTI(GenAIBedrockEditorModule, GenAIBedrockEditorModuleTypeId, GenAIBedrockModuleInterface);
        AZ_CLASS_ALLOCATOR(GenAIBedrockEditorModule, AZ::SystemAllocator);

        GenAIBedrockEditorModule()
        {
            // Push results of [MyComponent]::CreateDescriptor() into m_descriptors here.
            // Add ALL components descriptors associated with this gem to m_descriptors.
            // This will associate the AzTypeInfo information for the components with the the SerializeContext, BehaviorContext and EditContext.
            // This happens through the [MyComponent]::Reflect() function.
            m_descriptors.insert(m_descriptors.end(), {
                GenAIBedrockEditorSystemComponent::CreateDescriptor(),
            });
        }

        /**
         * Add required SystemComponents to the SystemEntity.
         * Non-SystemComponents should not be added here
         */
        AZ::ComponentTypeList GetRequiredSystemComponents() const override
        {
            return AZ::ComponentTypeList {
                azrtti_typeid<GenAIBedrockEditorSystemComponent>(),
            };
        }
    };
}// namespace GenAIBedrock

#if defined(O3DE_GEM_NAME)
AZ_DECLARE_MODULE_CLASS(AZ_JOIN(Gem_, O3DE_GEM_NAME, _Editor), GenAIBedrock::GenAIBedrockEditorModule)
#else
AZ_DECLARE_MODULE_CLASS(Gem_GenAIBedrock_Editor, GenAIBedrock::GenAIBedrockEditorModule)
#endif
