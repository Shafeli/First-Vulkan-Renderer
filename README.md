🔧 Vulkan Renderer Prototype

"A throwaway Vulkan renderer prototype I built to explore rendering pipelines, debug tooling, and UI integration. This is not production code — just a testbed before building the actual engine."

🧪 What It Does

Sets up a Vulkan renderer from scratch

Integrates ImGui for live parameter tweaking

Renders basic 3D geometry with debug overlays

💭 Why I Built This

I'm prototyping architecture, workflow, and Vulkan pain points before committing to a full engine. This repo is a sandbox for:

Validating Vulkan abstractions (buffers, pipelines, etc.)

Integrating UI workflows early (ImGui with Vulkan)

Building a renderer that explains itself in real-time via debug visuals

📦 Stuff It Uses

Vulkan SDK (1.3+)

ImGui

GLM

STB

Assimp (for basic mesh import)

Tested on Windows + MSVC 2022, should work on Linux with minimal tweaks. You’ll need Vulkan SDK and CMake 3.20+.

⚠️ Known Issues

Hardcoded asset paths

Minimal error handling

Lifetime management is rough

No memory allocator yet (just wrapper allocations)

🧺 Disclaimer

This is not clean, optimized, or even particularly safe code. It’s a stepping stone for the real engine.
