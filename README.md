# SMBIOS UUID Reader

A C++ program that retrieves and displays the SMBIOS UUID of a Windows system. The SMBIOS UUID is a unique identifier assigned to your computer's hardware, useful for system identification and asset tracking.

## Table of Contents

- [Overview](#overview)
- [Prerequisites](#prerequisites)
- [Compilation](#compilation)
- [Usage](#usage)
- [How It Works](#how-it-works)
- [Troubleshooting](#troubleshooting)
- [License](#license)

## Overview

This program accesses the system's firmware table to extract SMBIOS data and parses it to find the UUID. It corrects the endianness of certain fields to conform to the SMBIOS specification and outputs the UUID in a human-readable format.

## Prerequisites

- **Operating System**: Windows (due to the use of Windows-specific APIs)
- **Compiler**: A C++ compiler supporting C++11 or higher (e.g., Microsoft Visual C++)
- **Permissions**: May require administrative privileges to access system firmware tables

## Compilation

### Using Visual Studio

1. **Create a New Project**:
   - Open Visual Studio.
   - Go to **File** > **New** > **Project**.
   - Select **Console Application** and set the project name.

2. **Add Source Code**:
   - Replace the content of the generated `smbios-uuid.cpp` with the provided code.

3. **Configure Project Settings** (if necessary):
   - Ensure the project is set to use C++11 or higher standard.

4. **Build the Project**:
   - Press **Ctrl+Shift+B** or go to **Build** > **Build Solution**.

### Using Command Line

1. **Open Developer Command Prompt**:
   - Navigate to **Start Menu** > **Visual Studio** > **Developer Command Prompt**.

2. **Navigate to Source Directory**:

   ```cmd
   cd C:\Users\user\path\to\source\code\smbios-uuid
   ```

3. **Compile the Program**:

   ```cmd
   cl /EHsc /std:c++latest smbios-uuid.cpp
   ```

## Usage

After successful compilation, run the executable from the command prompt:

```cmd
.\smbios-uuid.exe
```

The program will display the SMBIOS UUID:

```cmd
SMBIOS UUID: 123e4567-e89b-12d3-a456-426614174000
```

## How It Works

1. **Access SMBIOS Data**:
   - Uses `GetSystemFirmwareTable` with the `RSMB` signature to retrieve raw SMBIOS data.
2. **Parse SMBIOS Structures**:
   - Iterates through SMBIOS structures to find the **System Information (Type 1)** structure.
3. **Extract UUID**:
    - Validates the UUID to ensure it's not all zeros or ones.
    - Adjusts the endianness of the first three fields as per the SMBIOS specification.
4. **Format UUID**:
    - Converts the UUID bytes into a standard string representation with hyphens.

## Troubleshooting

- **Cannot access system firmware table**:
  - Ensure you have the necessary permissions.
  - Run the command prompt or IDE as an administrator.
- **UUID not found in SMBIOS data**:
  - Your system's BIOS may not provide a UUID.
  - Check for BIOS updates from your motherboard manufacturer.

## License

This project is licensed under the MIT License.

---
**Disclaimer**: Use this program responsibly and ensure compliance with your organization's policies and applicable laws when accessing hardware identifiers.
