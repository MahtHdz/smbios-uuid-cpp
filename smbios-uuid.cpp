#include <windows.h>

#include <array>
#include <vector>
#include <string>
#include <sstream>
#include <iomanip>
#include <iostream>
#include <stdexcept>
#include <algorithm>

// Ensure structures are packed to match SMBIOS data alignment
#pragma warning(disable : 4200)
#pragma pack(push, 1)
struct RawSMBIOSData {
    BYTE  Used20CallingMethod;
    BYTE  SMBIOSMajorVersion;
    BYTE  SMBIOSMinorVersion;
    BYTE  DmiRevision;
    DWORD Length;
    BYTE  SMBIOSTableData[];
};

struct DmiHeader {
    BYTE type;
    BYTE length;
    WORD handle;
};
#pragma pack(pop)

static std::array<unsigned char, 16> GetSMBIOSUUIDBytes() {
    constexpr DWORD kSMBiosSignature = 'RSMB';  // Firmware Table Provider Signature (RSMB)
    DWORD smBiosDataSize = GetSystemFirmwareTable(kSMBiosSignature, 0, nullptr, 0);
    if (smBiosDataSize == 0) {
        throw std::runtime_error("Error: Cannot access system firmware table.");
    }

    // Allocate buffer for SMBIOS data
    std::vector<BYTE> buffer(smBiosDataSize);
    auto smBiosData = reinterpret_cast<RawSMBIOSData*>(buffer.data());

    // Retrieve SMBIOS table
    if (GetSystemFirmwareTable(kSMBiosSignature, 0, smBiosData, smBiosDataSize) == 0) {
        throw std::runtime_error("Error: Cannot access system firmware table.");
    }

    BYTE* data = smBiosData->SMBIOSTableData;
    BYTE* dataEnd = data + smBiosData->Length;

    while (data + sizeof(DmiHeader) <= dataEnd) {
        auto h = reinterpret_cast<DmiHeader*>(data);

        if (h->length < sizeof(DmiHeader) || data + h->length > dataEnd) {
            break;
        }

        // Search for System Information structure (Type 1)
        if (h->type == 0x01 && h->length >= 0x19) {
            BYTE* uuidData = data + 0x08;  // UUID offset

            // Check if UUID is valid (not all 0x00 or all 0xFF)
            bool allZero = std::all_of(uuidData, uuidData + 16, [](BYTE b) { return b == 0x00; });
            bool allOne = std::all_of(uuidData, uuidData + 16, [](BYTE b) { return b == 0xFF; });
            if (!allZero && !allOne) {
                std::array<unsigned char, 16> uuid;

                // Correct endianness for the first three fields
                uuid[0] = uuidData[3];
                uuid[1] = uuidData[2];
                uuid[2] = uuidData[1];
                uuid[3] = uuidData[0];
                uuid[4] = uuidData[5];
                uuid[5] = uuidData[4];
                uuid[6] = uuidData[7];
                uuid[7] = uuidData[6];
                std::copy(uuidData + 8, uuidData + 16, uuid.begin() + 8);

                return uuid;
            }
            break;
        }

        // Move to the next structure
        BYTE* next = data + h->length;
        while (next + 1 < dataEnd && (next[0] != 0 || next[1] != 0)) {
            next++;
        }
        if (next + 1 >= dataEnd) {
            break;
        }
        data = next + 2;  // Skip double null terminator
    }

    throw std::runtime_error("Error: UUID not found in SMBIOS data.");
}

std::string GetSMBIOSUUID() {
    const auto uuidBytes = GetSMBIOSUUIDBytes();

    // Convert UUID bytes to string format
    std::ostringstream oss;
    oss << std::hex << std::setfill('0');

    for (size_t i = 0; i < uuidBytes.size(); ++i) {
        oss << std::setw(2) << static_cast<int>(uuidBytes[i]);
        if (i == 3 || i == 5 || i == 7 || i == 9) {
            oss << '-';
        }
    }

    return oss.str();
}


int main()
{
    try {
		std::string uuid = GetSMBIOSUUID();
		std::cout << "SMBIOS UUID: " << uuid << std::endl;
	}
    catch (const std::exception& e) {
		std::cerr << e.what() << std::endl;
	}

	return 0;
}
