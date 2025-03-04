#ifndef VEC3_H
#define VEC3_H


/*
    Vector3 Class Definition

    Description:    Class definition and functions for a 3D vector.
*/

class vec3 {
    
    public:
        double e[3];
        vec3(): e{0,0,0} {};
        vec3(double e1, double e2, double e3) : e{e1, e2, e3} {};

        double x() const { return e[0]; }
        double y() const { return e[1]; }
        double z() const { return e[2]; }

        vec3 operator -() const { return vec3(-e[0], -e[1], -e[2]); }
        double operator[](int i) const { return e[i]; }
        double& operator[](int i) { return e[i]; }

        vec3& operator +=(const vec3& v){
            e[0] += v.e[0];
            e[1] += v.e[1];
            e[2] += v.e[2];
            return *this;
        }

        vec3& operator *=(double t){
            e[0] *= t;
            e[1] *= t;
            e[2] *= t;
            return *this;
        }
        
        vec3& operator /=(double t){
            return *this *= 1/t;
        }

        double length() const{
            return sqrt(length_squared());
        }

        double length_squared() const{
            return (e[0] * e[0]) + (e[1] * e[1]) + (e[2] * e[2]);
        }

        //Return true if the vector is close to zero in all dimensions
        bool near_zero() const{
            auto s = 0.00000001;
            return (fabs(e[0]) < s) && (fabs(e[1]) < s) && (fabs(e[2] < s));
        }

        static vec3 random() {
            return vec3(random_double(), random_double(), random_double());
        }

        static vec3 random(double min, double max) {
            return vec3(random_double(min, max), random_double(min,max), random_double(min,max));
        }
};

using point3 = vec3;

/*
    Vector3 Utility Functions
*/

inline std::ostream& operator<<(std::ostream& out, const vec3 &v){
    return out << v.e[0] << ' ' << v.e[1] << ' ' << v.e[2];
}

inline vec3 operator+(const vec3 &u, const vec3 &v){
    return vec3(u.e[0] + v.e[0], u.e[1] + v.e[1], u.e[2]  + v.e[2]);
}

inline vec3 operator-(const vec3 &u, const vec3 &v){
    return vec3(u.e[0] - v.e[0], u.e[1] - v.e[1], u.e[2]  - v.e[2]);
}

inline vec3 operator*(const vec3 &u, const vec3 &v){
    return vec3(u.e[0] * v.e[0], u.e[1] * v.e[1], u.e[2]  * v.e[2]);
}

inline vec3 operator*(double t, const vec3& v) {
    return vec3(t*v.e[0], t*v.e[1], t*v.e[2]);
}

inline vec3 operator*(const vec3& v, double t) {
    return t * v;
}

inline vec3 operator/(const vec3& v, double t) {
    return (1/t) * v;
}

inline double dot(const vec3& u, const vec3& v){
    return   u.e[0] * v.e[0]
            +u.e[1] * v.e[1]
            +u.e[2] * v.e[2];
}

inline vec3 cross(const vec3& u, const vec3& v) {
    return vec3(u.e[1] * v.e[2] - u.e[2] * v.e[1],
                u.e[2] * v.e[0] - u.e[0] * v.e[2],
                u.e[0] * v.e[1] - u.e[1] * v.e[0]);
}

inline vec3 unit_vector(const vec3& v){
    return v / v.length();
}

inline vec3 random_in_unit_disk() {
    while(true) {
        auto p = vec3(random_double(-1,1), random_double(-1,1), 0);
        if(p.length_squared() < 1)
            return p;
    }
}

inline vec3 random_in_unit_sphere() {
    while (true) {
        auto p = vec3::random(-1, 1);
        if(p.length_squared() < 1)
            return p;
    }
}

inline vec3 random_unit_vector() {
    return unit_vector(random_in_unit_sphere());
}

inline vec3 random_on_hemisphere(const vec3& normal){
    vec3 on_unit_sphere = random_unit_vector();
    if(dot(on_unit_sphere, normal) > 0.0) //If in the same hemisphere as the normal...
        return on_unit_sphere;
    else
        return -on_unit_sphere; 
}

inline vec3 reflect(const vec3& v, const vec3& n){
    return v - 2 * dot(v,n) * n;
}

//Calculates the refracted ray of Ray uv using the normal vector n and the quotient of 
//refractive indices eta_over_etaPrime using Snell's Law as described in section 11_2 of Raytracing in a weekend.
inline vec3 refract(const vec3& uv, const vec3& n, double eta_over_etaPrime) {
    auto cos_theta = fmin(dot(-uv, n), 1.0);
    vec3 r_out_perp = eta_over_etaPrime * (uv + cos_theta*n);
    vec3 r_out_parallel = -sqrt(fabs(1.0 - r_out_perp.length_squared())) * n;
    return r_out_perp + r_out_parallel;
}

#endif