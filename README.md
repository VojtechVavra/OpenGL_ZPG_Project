# OpenGL_ZPG_Project
ZPG - Základy počítačové grafiky - OpenGL

How to run in Windows:
1. Install Assimp through Visual Studio
2. Import to the project and then build with these static libraries: glew-2.1.0, glfw-3.3.2, glm, soil
   * Property -> C/C++ General -> Additional Include Directories:  (select folder with .h files) "libraries\"  "soil_texture_loader\soil\include\"
   * Property -> Linker -> General -> Additional Library Directories (select folder with .lib files): "libraries\" 
   * Property -> Linker -> Input -> Additional Dependencies (write these names): 
   * opengl32.lib
   * glu32.lib
   * glew32.lib
   * glfw3.lib
   * SOIL.lib
4. Attach to the final compiled project: Assimp32.dll, glew32.dll (place there where .exe with final program is)
5. Have proper files in folders: models, shaders, textures
6. .obj 3D models not included
