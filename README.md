Setup for building yourself:<br>
Look into this guide https://learnopengl.com/Getting-started/Creating-a-window for help with the setup<br>
(1) Setup GLFW and link its includes and its lib file<br>
(2) Setup GLAD and link its includes. I chose to compile glad.c into a static library glad.lib and link it, but you can alternatively just add glad.c as one of your project files<br>
(3) Setup STB by downloading stb_image.h from https://github.com/nothings/stb/blob/master/stb_image.h and linking it<br>
(4) Setup GLM by downloading it from https://github.com/g-truc/glm and copying the "glm" subdirectory containing the header files to your include directory<br>

In the end, you should have an include structure like this:<br>
-include<br>
---glad/...<br>
---GLFW/...<br>
---glm/...<br>
---KHR/...<br>
---STB/...<br>
-lib (note that the glad.lib file below is optional, you'll need it only if you choose to compile GLAD into a static library yourself)<br>
---glad.lib<br>
---glfw3.lib<br>
