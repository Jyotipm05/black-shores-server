# Black Shores Server

## Build Instructions

1. **Install vcpkg and Drogon:**
   - Follow the [vcpkg installation guide](https://github.com/microsoft/vcpkg#quick-start).
   - Install Drogon: `vcpkg install drogon`

2. **Configure CMake with vcpkg toolchain:**
   - Find your vcpkg installation path (e.g., `D:/vcpkg`).
   - Run:
     ```
     cmake -S . -B build -DCMAKE_TOOLCHAIN_FILE="D:/vcpkg/scripts/buildsystems/vcpkg.cmake"
     ```
   - Build:
     ```
     cmake --build build
     ```

## Usage

1. Run the server executable:
   ```
   build/black-shores-server.exe
   ```
2. Enter the IP address and port when prompted. Example:
   ```
   127.0.0.1
   8080
   ```
   - If you want to use localhost, just press Enter for IP and enter a valid port.

## Troubleshooting

- **Runtime abort (abort() has been called):**
  - Ensure you enter a valid port (1-65535).
  - Drogon must be installed and found by CMake.
  - If you see header errors, check your vcpkg toolchain path and Drogon installation.

- **Cannot find Drogon headers:**
  - Make sure you use the correct toolchain file in your CMake command.
  - Example: `-DCMAKE_TOOLCHAIN_FILE="D:/vcpkg/scripts/buildsystems/vcpkg.cmake"`

## Example Output
```
listener added on 127.0.0.1:8080
```

## Additional Resources
- [Drogon Documentation](https://github.com/drogonframework/drogon)
- [vcpkg Documentation](https://github.com/microsoft/vcpkg)

