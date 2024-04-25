# Raytracing in a weekend
 No Maya. No Blender. No nothin', just pure vector math. Created out of curiosity towards 
 graphics programming using the book
 <a href="https://raytracing.github.io/books/RayTracingInOneWeekend.html">
    <cite>Ray Tracing in One Weekend</cite>
</a> by Peter Shirley, Trevor David Black and Steve Hollasch. Good book, would recommend.

<b>Progress</b>
<p align="center">
 
![progress](progress_images/progress.gif)

</p>

<b>WANT TO USE THIS RAYTRACER?</b>

This is a primitive API for raytracing, but forms a solid foundation for a more robust 3D rendering program. Currently, this
program defines polygons (see "hittable.h") and places them in a collection forming the "world". This world is then rendered 
using a camera (see "camera.h") that can be positioned using a Vector3 position. Both of these tasks are completed in main.cpp, and
the output is redirected to a .ppm file using a windows .bat script.

To start printing spheres...

[1]: Download code.<br>
[2]: Using Command Prompt, navigate to downloaded folder.<br>
[3]: Run .\Make<br><br>

[!] NOTE: Windows Powershell will not correctly output .ppm data, so it cannot be used to generate images with this code.
