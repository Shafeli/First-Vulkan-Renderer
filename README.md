# 🔧 Vulkan Renderer Prototype

*A throwaway prototype built to explore Vulkan rendering, debug tooling, and UI integration. Not production-ready — just a personal testbed to shake out ideas before building a full engine.*

---

## 🧪 What It Does

- Initializes a Vulkan renderer from scratch
- Renders basic 3D geometry (mesh loading via Assimp)
- Integrates **ImGui** for real-time parameter tweaking and debug overlays
- Lays the groundwork for more complex systems (e.g., pipeline abstractions, GPU resource tracking)

---

## 💭 Why I Built This

Before committing to a full engine architecture, I needed a space to prototype and stress-test Vulkan’s quirks. This repo serves as a sandbox for:

- Validating Vulkan abstraction layers (buffers, pipelines, descriptors, etc.)
- Getting **UI workflows** in early (ImGui + Vulkan backend)
- Building a renderer that explains itself through **debug visuals** and overlays
- Exploring **data-oriented layout and memory management patterns**

---

## 📦 Tech Stack

- **Vulkan SDK 1.3+**
- **ImGui** — UI/debug overlay
- **GLM** — math
- **STB** — image loading
- **Assimp** — mesh import/export

Tested on **Windows (MSVC 2022)**, should be portable to Linux with minor tweaks.\
Requires **CMake 3.20+** and the Vulkan SDK installed.

---

## ⚠️ Known Limitations

- ❌ Hardcoded asset paths
- 🛑 Minimal error checking/logging
- 🧽 No custom memory allocator yet —
