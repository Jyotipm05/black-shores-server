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
2. When prompted, choose where to host the server:
   - Type `1` for local network (auto-detects your IP).
   - Type `2` for host machine only (localhost, default).
   - Type `3` for custom IP (enter your own IP address).
   - Press Enter to select the default (2).
3. Enter the port number when prompted (default is `5555`).
   - Press Enter to use the default port.
   - Valid ports are 1-65535. Invalid input will use the default.

**Example session:**
```
Do you want to host it on local network🛜[1] or host machine only💻[2] or custom🛠️[3]?
Type 1, 2, or 3 and press enter (default is 2): 1
Hosting on local network. Current IP: 192.168.1.100
Enter port number (default is 5555): 8080
listener added on http://192.168.1.100:8080
```

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
