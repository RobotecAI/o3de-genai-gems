/*
 * Copyright (c) Contributors to the Open 3D Engine Project.
 * For complete copyright and license terms please see the LICENSE at the root of this distribution.
 *
 * SPDX-License-Identifier: Apache-2.0 OR MIT
 *
 */

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

    // Feature TypeIds
    inline constexpr const char* O3DEAssistantFeatureTypeId = "{2C753228-4D1C-4632-9182-2F2E39970D9B}";
    inline constexpr const char* O3DEAssistantPythonFeatureTypeId = "{7C524A1B-A175-4C26-BEB1-DBEFB7F13F54}";
    inline constexpr const char* SceneGenerationPythonFeatureTypeId = "{B9E36297-8823-4544-B657-777E41E149AA}";
} // namespace GenAISampleFeatures
