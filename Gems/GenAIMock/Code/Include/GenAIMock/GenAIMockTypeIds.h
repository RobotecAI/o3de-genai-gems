
#pragma once

namespace GenAIMock
{
    // System Component TypeIds
    inline constexpr const char* GenAIMockSystemComponentTypeId = "{302345A1-F4C2-481E-B7A5-F86A49976DFF}";
    inline constexpr const char* GenAIMockEditorSystemComponentTypeId = "{60230B38-1389-4642-A658-163171D639DE}";

    // Module derived classes TypeIds
    inline constexpr const char* GenAIMockModuleInterfaceTypeId = "{6655F958-25D8-48C4-B51E-3FA4E3DF1BC9}";
    inline constexpr const char* GenAIMockModuleTypeId = "{BBEA3144-EF92-4818-856E-09F9DE65858D}";
    // The Editor Module by default is mutually exclusive with the Client Module
    // so they use the Same TypeId
    inline constexpr const char* GenAIMockEditorModuleTypeId = GenAIMockModuleTypeId;

    // Interface TypeIds
    inline constexpr const char* GenAIMockRequestsTypeId = "{F51BC484-EC06-4B28-A2EB-745E1B948EE0}";
} // namespace GenAIMock
