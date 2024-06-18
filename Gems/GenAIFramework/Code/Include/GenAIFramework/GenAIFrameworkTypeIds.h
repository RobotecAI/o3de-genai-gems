/*
 * Copyright (c) Contributors to the Open 3D Engine Project.
 * For complete copyright and license terms please see the LICENSE at the root of this distribution.
 *
 * SPDX-License-Identifier: Apache-2.0 OR MIT
 *
 */

#pragma once

namespace GenAIFramework
{
    // System Component TypeIds
    inline constexpr const char* GenAIFrameworkSystemComponentTypeId = "{C1447E9E-C102-4FDA-8D62-34BEE2DD4D31}";
    inline constexpr const char* GenAIFrameworkEditorSystemComponentTypeId = "{CD496C8B-7D2A-447B-A1F9-B449A9BFEC04}";
    inline constexpr const char* GenAIFrameworkSystemComponentConfigurationTypeId = "{6AB6E636-60DC-4377-BD45-2326CF6A0069}";

    inline constexpr const char* GenAIAsyncRequestSystemComponentTypeId = "{D2D195A2-C720-4961-9AEF-C00FCB0AA70D}";
    inline constexpr const char* GenAIAsyncRequestEditorSystemComponentTypeId = "{018E7A45-43AE-7E69-8BD9-0751B9170BF5}";

    // Communication TypeIds
    inline constexpr const char* SystemRegistrationContextTypeId = "{E162937C-0177-4CED-87B3-3B037A44C394}";
    inline constexpr const char* AsyncRequestsTypeId = "{018E7A12-5DB2-7DAA-A764-78D7703C8EFA}";
    inline constexpr const char* AIServiceProviderRequestsTypeId = "{0BCAC4E2-09A9-4702-98DD-EF37006664A7}";
    inline constexpr const char* AIModelRequestTypeId = "{8C140E10-E3A2-478B-9886-C7C24EB16816}";
    inline constexpr const char* AIModelAgentRequestsTypeId = "{E66D5979-457B-4DFD-A749-AE722101649C}";
    inline constexpr const char* AIModelAgentNotificationsTypeId = "{C317848A-4D75-4E3D-A3A0-E86E621DB350}";
    inline constexpr const char* ConversationNotificationsTypeId = "{B7B4645A-30F4-4BBF-92A6-1328C671B15F}";

    // Module derived classes TypeIds
    inline constexpr const char* GenAIFrameworkModuleInterfaceTypeId = "{4ACED336-C28C-4163-996C-4645A0EF6510}";
    inline constexpr const char* GenAIFrameworkModuleTypeId = "{C448193F-A197-481A-9CF5-73BBAD978E3A}";
    // The Editor Module by default is mutually exclusive with the Client Module
    // so they use the Same TypeId
    inline constexpr const char* GenAIFrameworkEditorModuleTypeId = GenAIFrameworkModuleTypeId;

    // Interface TypeIds
    inline constexpr const char* GenAIFrameworkRequestsTypeId = "{3C39C889-9AA7-43F9-820A-DAF53095BAAD}";
    inline constexpr const char* GenAIFrameworkActionRequestsTypeId = "{D2D195A2-C720-4961-9AEF-C00FCB0AA70D}";
    inline constexpr const char* GenAIFrameworkEditorRequestsTypeId = "{B20BA166-2237-4F0C-AFCD-FB17B63A9CF6}";
    inline constexpr const char* FeatureBaseTypeId = "{4050040A-31C3-4949-8CE0-52FAD7562F4D}";

    // Feature TypeIds
    inline constexpr const char* O3DEAssistantFeatureTypeId = "{2C753228-4D1C-4632-9182-2F2E39970D9B}";
} // namespace GenAIFramework
