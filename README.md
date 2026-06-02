# DLT MCP Server Plugin

A plugin for DLT Viewer that exposes DLT log analysis capabilities via the Model Context Protocol (MCP). It allows AI agents and MCP-compatible clients to query and retrieve information from DLT log files.

## Features

- **Log Summary** – Get message count, file size, timestamp range, duration, and available CTID/APID contexts.
- **Search** – Filter log messages by CTID, APID, log level, timestamp range, and keyword, with pagination.
- **Message Retrieval** – Fetch full payload for specific message IDs.

## Requirements

- DLT Viewer 2.30.0
- CMake 3.10+
- Boost 1.74+
- Qt5/Qt6 (matching DLT Viewer's build)

## Build

This plugin must be built as part of the DLT Viewer source tree. Clone both repositories, place this plugin inside the DLT Viewer plugin directory, and add it to the parent CMake build:

```bash
# Clone DLT Viewer
git clone https://github.com/COVESA/dlt-viewer.git
cd dlt-viewer

# Clone this plugin into the plugin directory
git clone https://github.com/Ulenspiegel/dlt-mcp-server.git plugin/dlt-mcp-server

# Add a single line to `plugin/CMakeLists.txt`:
#   add_subdirectory(dlt-mcp-server)

# Build
cmake -B build -S .
cmake --build build --config Release
```

The plugin binary will be placed in `build/bin/plugins/`.

## License

Mozilla Public License 2.0 – see [LICENSE](LICENSE).
