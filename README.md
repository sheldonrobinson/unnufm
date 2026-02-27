## UNNU FilmMaker SDK

**UNNU FilmMaker (unnufm)** is a lightweight C++ SDK designed to support film‑oriented tooling, rendering workflows, and media‑centric application development. It provides a modular foundation for building custom film, animation, or visualization pipelines with a focus on clarity, portability, and extensibility.

### ✨ Key Features
- **Modern C++ architecture** — Clean header (`include/unnu_fm`) and source (`src/`) layout for easy integration into existing engines or standalone tools.   
- **CMake‑based build system** — Simple cross‑platform configuration using `CMakeLists.txt`.  
- **Lightweight and dependency‑minimal** — Designed to be embedded into larger systems without heavy external requirements.  
- **MIT‑licensed** — Fully open for commercial and non‑commercial use. 

## Dependencies
1. **UNNU TTS**, [https://github.com/sheldonrobinson/unnutts]
2. **Piper**, [https://github.com/sheldonrobinson/piper1-gpl]
3. **ONNXRuntime**, [https://github.com/sheldonrobinson/onnxruntime]

### 🚀 Getting Started
**STANDALONE**
1. Clone the repository:  
   ```bash
   git clone https://github.com/sheldonrobinson/unnufm
   ```
2. Configure and build with CMake:  
   ```bash
   cmake -B build
   cmake --build build
   ```
3. Include the `include/` directory in your project and link against the generated library.

**OR**

**EMBEDDED**
```cmake
FetchContent_Declare(
  unnufm
  GIT_REPOSITORY 		 https://github.com/sheldonrobinson/unnufm.git
  GIT_TAG        		 origin/develop
  GIT_SHALLOW 			 TRUE
  GIT_SUBMODULES_RECURSE TRUE
  
  EXCLUDE_FROM_ALL
		 
  OVERRIDE_FIND_PACKAGE
)

set(UT_BUILD_SHARED_LIBS ON CACHE BOOL "Build as shared library" Force)
set(UF_BUILD_SHARED_LIBS ON CACHE BOOL "Build as shared library" Force)

FetchContent_MakeAvailable(unnufm)
```
###
### 🧩 Use Cases
- Embedding film or animation logic into custom engines  
- Building standalone media tools or editors  
- Prototyping rendering or cinematic systems  
- Extending existing C++ applications with film‑oriented modules

### 📜 License
This project is released under the **MIT License**, allowing unrestricted use, modification, and distribution. 
