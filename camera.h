#ifndef CAMERA_H
#define CAMERA_H

#include"rtweekend.h"
#include"hittable.h"
#include"material.h"
//Define an Image dimensions

const char* MAX_PPM_COLOR = "255";
const char* FULL_COLOR_PPM_NUMBER = "P3";

//Camera class for handling two important jobs:
//[1] - Construct and dispatch rays into the world.
//[2] - Use the results of these rays to construct the rendered image. 
class camera {
    public:
        double  aspect_ratio        = 1.0;              //Ratio of image width over height
        int     image_width         = 100;              //Rendered image width in pixel count
        int     samples_per_pixel   = 10;               //Count of random samples for each pixel
        int     max_depth           = 10;               //Maximum number of ray bounces into scene

        double  vertical_fov        = 90;               //Vertical view angle (field of view)
        point3  lookfrom            = point3(0,0,0);    //Point camera is looking from 
        point3  lookat              = point3(0,0,-1);   //Point camera is looking at
        vec3    view_up             = vec3(0,1,0);      //Camera relative 'up' direction

        double defocus_angle = 0;   //Variation angle of rays through each pixel
        double focus_dist = 10;     //Distance from camera lookfrom point to plane of perfect focus

        //Renders 'world' parameter using camera settings defined by aspect_ration and image_width.
        void render(const hittable& world){
            initialize();
    
            //Output ppm header
            std::cout << FULL_COLOR_PPM_NUMBER << '\n' << image_width << ' ' << image_height << '\n' <<MAX_PPM_COLOR << '\n';

            //Render world pixel by pixel with raytracing
            for(int j = 0; j < image_height; j++){
                std::clog << "\rScanlines remaining: " << (image_height - j) << ' ' << std::flush;
                for(int i = 0; i < image_width; i++){
                    //Cast ray and antialias color
                    color pixel_color(0,0,0);
                    for(int sample =  0; sample < samples_per_pixel; sample++) {
                        ray r = get_ray(i, j);
                        pixel_color += ray_color(r, max_depth, world);
                    }
                    write_color(std::cout, pixel_samples_scale * pixel_color);
                }
            }
            std::clog << "\rDone.                 \n";
        }

    private:
        int     image_height;                   //Rendered image height in pixels
        double  pixel_samples_scale;            //Color scale factor for a sum of pixel samples
        point3  camera_center = point3(0,0,0);  //Camera center
        point3  pixel_00_location;              //Location of pixel 0, 0
        vec3    pixel_delta_u;                  //offset to pixel to the right
        vec3    pixel_delta_v;                  //offset to pixel below
        vec3    u, v, w;                        //Camera frame basis vectors
        vec3    defocus_disk_u;                 //Defocus disk horizontal radius
        vec3    defocus_disk_v;                 //Defocus disk vertical radius

        //Camera and viewport setup for rendering.
        void initialize(){
             //Determine image height from image width and aspect ratio
            image_height = int(image_width / aspect_ratio);
            image_height = (image_height < 1) ? 1 : image_height;

            pixel_samples_scale = 1.0 / samples_per_pixel;

            camera_center = lookfrom;

            //Determine viewport dimensions
            auto theta = degrees_to_radians(vertical_fov);
            auto h = tan(theta/2);
            auto viewport_height = 2 * h * focus_dist;    
            auto viewport_width = viewport_height * (double(image_width) / image_height);

            //Calculate the u,v,w basis vectors for the camera coordinate frame.
            w = unit_vector(lookfrom - lookat);
            u = unit_vector(cross(view_up, w));
            v = cross(w, u);

            //Compute horizontal, vertical edges for viewport
            auto viewport_u = viewport_width * u;
            auto viewport_v = viewport_height * -v;

            //Compute horizontal and vertical delta vectors from pixel to pixel
            pixel_delta_u = viewport_u / image_width;
            pixel_delta_v = viewport_v / image_height;

            //Calculate location of upper left pixel
            auto viewport_upper_left = camera_center - (focus_dist * w) - (viewport_u / 2.0) - (viewport_v / 2.0);
            pixel_00_location = viewport_upper_left + 0.5 * (pixel_delta_u + pixel_delta_v);

            //Compute the camera defocus disk basis vectors
            auto defocus_radius = focus_dist * tan(degrees_to_radians(defocus_angle));
            defocus_disk_u = u  * defocus_radius;
            defocus_disk_v = v * defocus_radius;
        }

        //Construct a camera ray originating from the defocus disk and directed at a randomly
        //sampled point around the pixel location i, j.
        ray get_ray(int i, int j) const {
            auto offset = sample_square();
            auto pixel_sample =  pixel_00_location
                                +((i + offset.x()) * pixel_delta_u)
                                +((j + offset.y()) * pixel_delta_v);
            auto ray_origin = (defocus_angle <= 0) ? camera_center : defocus_disk_sample();
            auto ray_direction = pixel_sample - ray_origin;

            return ray(ray_origin, ray_direction);
        }

        //Returns the vector to a random point in the [-.5, -.5] - [+.5, +.5] unit square.
        vec3 sample_square() const {
            return vec3(random_double() - 0.5, random_double() - 0.5, 0);
        }

        //Return a random point in the camera defocus disk
        point3 defocus_disk_sample() const {
            auto p = random_in_unit_disk();
            return camera_center + (p[0] * defocus_disk_u) + (p[1] * defocus_disk_v);
        }

        //Determines color of ray through raycasting and checking for hit objects. If no hit occurs,
        //LERPS from blue to white based on height traveled by ray r [y = low: Blue, y = high: White]
        color ray_color(const ray& r, int depth, const hittable& world){
            //If we've exceeded ray bounce limit, no more light is gathered and we return black.
            if(depth <= 0)
                return color(0,0,0);

            color blue = color(1.0, 1.0, 1.0);
            color soft_white = color(0.5, 0.7, 1.0);
            color red = color(1.0,0.0,0.0);

            //[BEGIN MATERIAL ALGORITHM]

            hit_record rec;
            //On a hit...
            if(world.hit(r, interval(0.001, infinity), rec)){
                ray scattered;
                color attenuation;
                //...If the material causes a ray scatter
                if(rec.mat->scatter(r, rec, attenuation, scattered)){
                    //...Recursively check bounces of ray.
                    return attenuation * ray_color(scattered, depth - 1, world);
                }
                //...Otherwise return black.
                return color(0,0,0);
            }

            //NOTE: This section will stop recurring when the bounce rays fail
            //      to collide with another hittable. The reflected color will then be
            //      the background, or surrounding light. You can see this if you
            //      modify the scalar in the return statement.

            //[END MATERIAL ALGORITHM]

            vec3 unit_direction = unit_vector(r.direction());
            double a = 0.5 * (unit_direction.y() + 1.0);
            return (1.0 - a) * blue + a * soft_white;
        }

};

#endif