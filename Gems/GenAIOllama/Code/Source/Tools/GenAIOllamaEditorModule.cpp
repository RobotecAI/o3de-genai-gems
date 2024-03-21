
#include "GenAIOllamaEditorSystemComponent.h"
#include <GenAIOllama/GenAIOllamaTypeIds.h>
#include <GenAIOllamaModuleInterface.h>

namespace GenAIOllama
{
    class GenAIOllamaEditorModule : public GenAIOllamaModuleInterface
    {
    public:
        AZ_RTTI(GenAIOllamaEditorModule, GenAIOllamaEditorModuleTypeId, GenAIOllamaModuleInterface);
        AZ_CLASS_ALLOCATOR(GenAIOllamaEditorModule, AZ::SystemAllocator);

        GenAIOllamaEditorModule()
        {
            // Push results of [MyComponent]::CreateDescriptor() into m_descriptors here.
            // Add ALL components descriptors associated with this gem to m_descriptors.
            // This will associate the AzTypeInfo information for the components with the the SerializeContext, BehaviorContext and
            // EditContext. This happens through the [MyComponent]::Reflect() function.
            m_descriptors.insert(
                m_descriptors.end(),
                {
                    GenAIOllamaEditorSystemComponent::CreateDescriptor(),
                });
        }

        /**
         * Add required SystemComponents to the SystemEntity.
         * Non-SystemComponents should not be added here
         */
        AZ::ComponentTypeList GetRequiredSystemComponents() const override
        {
            return AZ::ComponentTypeList{
                azrtti_typeid<GenAIOllamaEditorSystemComponent>(),
            };
        }
    };
} // namespace GenAIOllama

#if defined(O3DE_GEM_NAME)
AZ_DECLARE_MODULE_CLASS(AZ_JOIN(Gem_, O3DE_GEM_NAME, _Editor), GenAIOllama::GenAIOllamaEditorModule)
#else
AZ_DECLARE_MODULE_CLASS(Gem_GenAIOllama_Editor, GenAIOllama::GenAIOllamaEditorModule)
#endif
