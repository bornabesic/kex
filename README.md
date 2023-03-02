# 🍪 Kex

> Dok si rek'o keks!

Plug-and-play 2D graphics C++ library built on top of [OpenGL ES 3.0 API](https://registry.khronos.org/OpenGL-Refpages/es3.0/).

### Features and Design Goals
- Simple and well-designed API
- Minimal dependencies
- [WebAssembly](https://developer.mozilla.org/en-US/docs/WebAssembly) support (compatible with [Emscripten](https://emscripten.org/) and [WebGL 2](https://developer.mozilla.org/en-US/docs/Web/API/WebGL_API#webgl_2))

### OpenGL Context
This library is only in charge of drawing and **does not** manage OpenGL contexts. Therefore, users are required to manually initialize an OpenGL context themselves before proceeding to use the library.

### Conventions
This project uses the following conventions:
- Layout: [pitchfork](https://github.com/vector-of-bool/pitchfork)
- Changelog: [Keep a Changelog](https://github.com/olivierlacan/keep-a-changelog)

### License
Copyright © 2023  Borna Bešić  
[GNU LGPLv3](LICENSE.txt)
