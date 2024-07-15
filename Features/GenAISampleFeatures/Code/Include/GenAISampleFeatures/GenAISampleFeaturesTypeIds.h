
#pragma once

namespace GenAISampleFeatures
{
    // System Component TypeIds
    inline constexpr const char* GenAISampleFeaturesSystemComponentTypeId = "{AF047F2F-DE88-435A-A6BF-2CCFB820DAF5}";
    inline constexpr const char* GenAISampleFeaturesEditorSystemComponentTypeId = "{9C038999-3635-47F6-8296-74D8E1219EA4}";

    // Module derived classes TypeIds
    inline constexpr const char* GenAISampleFeaturesModuleInterfaceTypeId = "{6D290A36-299C-42D6-81F3-C309C23F6E99}";
    inline constexpr const char* GenAISampleFeaturesModuleTypeId = "{8D7B956D-477D-4544-992F-5B63A1266998}";
    // The Editor Module by default is mutually exclusive with the Client Module
    // so they use the Same TypeId
    inline constexpr const char* GenAISampleFeaturesEditorModuleTypeId = GenAISampleFeaturesModuleTypeId;

    // Interface TypeIds
    inline constexpr const char* GenAISampleFeaturesRequestsTypeId = "{6830CBD4-7998-4340-85C7-A2F1CCDE87F7}";
} // namespace GenAISampleFeatures
