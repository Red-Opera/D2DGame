#include "stdafx.h"
#include "GUID_Generator.h"
#include <guiddef.h>
#include <iomanip>

const uint GUID_Generator::Generate()
{
    std::hash<std::string> hasher;

    return static_cast<uint>(hasher(GenerateToString()));
}

const std::string GUID_Generator::GenerateToString()
{
    GUID guid;
    HRESULT hr = CoCreateGuid(&guid);

    if (SUCCEEDED(hr))
    {
        std::stringstream sstream;

        sstream
            << std::hex << std::uppercase
            << std::setw(8) << std::setfill('0') << guid.Data1 << "-"
            << std::setw(4) << std::setfill('0') << guid.Data2 << "-"
            << std::setw(4) << std::setfill('0') << guid.Data3 << "-"
            << std::setw(2) << std::setfill('0') << static_cast<uint>(guid.Data4[0])
            << std::setw(2) << std::setfill('0') << static_cast<uint>(guid.Data4[1])
            << std::setw(2) << std::setfill('0') << static_cast<uint>(guid.Data4[2])
            << std::setw(2) << std::setfill('0') << static_cast<uint>(guid.Data4[3])
            << std::setw(2) << std::setfill('0') << static_cast<uint>(guid.Data4[4])
            << std::setw(2) << std::setfill('0') << static_cast<uint>(guid.Data4[5])
            << std::setw(2) << std::setfill('0') << static_cast<uint>(guid.Data4[6])
            << std::setw(2) << std::setfill('0') << static_cast<uint>(guid.Data4[7]);

        return sstream.str();
    }

    assert(false);
    return "N/A";
}
