This is a basic OpenGL 3D application base template that one can use to build larger projects upon.<br>
Parts of this repo's code have been created by following the tutorial at https://learnopengl.com.<br>
It's a fantastic tutorial for starting out with OpenGL and I truly recommend checking it out.<br>

The template includes a camera implementation that allows for a free-flight camera, so pitch/yaw/rotation are all able to be modified.<br>
Also, I used quaternions for all rotations and implemented a uniform buffer object.<br>
And I wrote lots of documentation so that everyone who is just starting out with OpenGL can understand what's going on.<br>

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

Note: All dummy images were taken from Wikimedia to avoid copyright problems.<br>