# Black Shores Server

Copyright (c) 2025 jmbab

## Project Structure

```
`black-shores-server/`
├── `CMakeLists.txt`
├── `README.md`
├── `ssl-certificates_writeup.md`
├── `vcpkg.json`
├── `build/`
├── `config/`
│   ├── `ca-key.pem`
│   ├── `ca.pem`
│   ├── `ca.srl`
│   ├── `cert-key.pem`
│   ├── `cert.csr`
│   ├── `cert.pem`
│   ├── `config1.json`
│   └── `extfile.cnf`
├── `logs/`
├── `src/`
│   ├── `create_cert.cpp`
│   ├── `create_cert.hpp`
│   ├── `dotenv.hpp`
│   ├── `IpCollector.hpp`
│   ├── `main.cpp`
│   ├── `pathFinder.hpp`
│   ├── `controllers/`
│   │   ├── `CritExit.cpp`
│   │   ├── `CritExit.hpp`
│   │   ├── `home.cpp`
│   │   ├── `home.hpp`
│   │   ├── `PathManager.hpp`
│   │   ├── `RootCtrl.cpp`
│   │   └── `RootCtrl.hpp`
│   ├── `Filters/`
│   ├── `UI/`
│   │   ├── `components.hpp`
│   │   ├── `nav.hpp`
│   │   └── `style.hpp`
│   └── `WebSockets/`
│       ├── `ChatWS.cpp`
│       └── `ChatWS.h`
├── `webapp/`
│   └── `root/`
│       ├── `index.html`
│       ├── `vite.svg`
│       └── `assets/`
│           ├── `_required_.js`
│           ├── `_required_.css`
│           └── ...
└── `...`

```

## Build Instructions

1. **Install vcpkg and Drogon:**
   - Follow the [vcpkg installation guide](https://github.com/microsoft/vcpkg#quick-start).
   - Install Drogon: `vcpkg install drogon`

2. **Configure CMake with vcpkg toolchain:**
   - Find your vcpkg installation path (e.g., `D:/vcpkg`).
   - Run:
     ```bash
     cmake -S . -B build -DCMAKE_TOOLCHAIN_FILE="D:/vcpkg/scripts/buildsystems/vcpkg.cmake"
     ```
   - Build:
     ```bash
     cmake --build build
     ```

## Usage

Run the server executable:
```bash
build/black-shores-server.exe
```

When prompted:
- Type `1` for local network (auto-detects your IP).
- Type `2` for host machine only (localhost, default).
- Type `3` for custom IP (enter your own IP address).
- Press Enter to select the default (2).

Enter the port number when prompted (default is `5555`).
- Press Enter to use the default port.
- Valid ports are 1-65535. Invalid input will use the default.

**Example session:**
```
Do you want to host it on local network🛜[1] or host machine only💻[2] or custom🛠️[3]?
Type 1, 2, or 3 and press enter (default is 2): 1
Hosting on local network. Current IP: 192.168.1.100
Enter port number (default is 5555): 8080
listener added on http://192.168.1.100:8080
listener added on https://192.168.1.100:8080
```

## Drogon HTML File Location

Place your static HTML files in:
```
webapp/root/
```
For example, `webapp/root/index.html` will be served as the root page.

## HTTPS & Custom IP

- The server supports HTTPS using certificates from `config/localhost.crt` and `config/localhost.key` by default.
- You can specify custom certificate paths and document root via environment variables in `.env`:
  - `CRT_PATH`, `KEY_PATH`, `ROOT_DIR`
- The IP address is chosen interactively at startup.

## Troubleshooting

- **Runtime abort (abort() has been called):**
  - Ensure you enter a valid port (1-65535).
  - Drogon must be installed and found by CMake.
  - If you see header errors, check your vcpkg toolchain path and Drogon installation.

- **Cannot find Drogon headers:**
  - Make sure you use the correct toolchain file in your CMake command.
  - Example: `-DCMAKE_TOOLCHAIN_FILE="D:/vcpkg/scripts/buildsystems/vcpkg.cmake"`
- **Event loop error:**
  - Ensure `app().run()` is called only from the main thread.
  - **Port errors:**
      - Use ports in the range 1-65535.
- **Missing files:**
    - Ensure certificate and HTML files exist at the specified paths.

## Example Output
```
listener added on 127.0.0.1:8080
```

## Additional Resources
- [Drogon Documentation](https://github.com/drogonframework/drogon)
- [vcpkg Documentation](https://github.com/microsoft/vcpkg)
