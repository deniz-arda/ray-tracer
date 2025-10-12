#include <SDL2/SDL.h>
#include <iostream>
#include <vector>
#include <cmath>
#include <limits>
#include <algorithm>
#include <omp.h>
#include <chrono>

// 3D vector structure for geometric operations
struct Vec3 {
    double x, y, z;
    
    Vec3(double x = 0, double y = 0, double z = 0) : x(x), y(y), z(z) {}
    
    Vec3 operator+(const Vec3& v) const { return Vec3(x + v.x, y + v.y, z + v.z); }
    Vec3 operator-(const Vec3& v) const { return Vec3(x - v.x, y - v.y, z - v.z); }
    Vec3 operator*(double t) const { return Vec3(x * t, y * t, z * t); }
    Vec3 operator/(double t) const { return Vec3(x / t, y / t, z / t); }
    
    double dot(const Vec3& v) const { return x * v.x + y * v.y + z * v.z; }
    
    Vec3 cross(const Vec3& v) const {
        return Vec3(y * v.z - z * v.y, z * v.x - x * v.z, x * v.y - y * v.x);
    }
    
    double length() const { return sqrt(x * x + y * y + z * z); }
    
    Vec3 normalize() const {
        double len = length();
        return len > 0 ? *this / len : Vec3(0, 0, 0);
    }
    
    Vec3 reflect(const Vec3& normal) const {
        return *this - normal * 2 * this->dot(normal);
    }
};

using Color = Vec3;

// Ray structure with origin and direction
struct Ray {
    Vec3 origin, direction;
    
    Ray(const Vec3& o, const Vec3& d) : origin(o), direction(d.normalize()) {}
    
    Vec3 at(double t) const { return origin + direction * t; }
};

// Material properties for Phong shading
struct Material {
    Color color;
    double ambient, diffuse, specular, shininess, reflectivity;
    
    Material(const Color& c = Color(1, 1, 1), double amb = 0.1, double diff = 0.7, 
             double spec = 0.6, double shin = 32, double refl = 0.3)
        : color(c), ambient(amb), diffuse(diff), specular(spec), shininess(shin), reflectivity(refl) {}
};

// Sphere primitive with ray intersection
struct Sphere {
    Vec3 center;
    double radius;
    Material material;
    
    Sphere(const Vec3& c, double r, const Material& m) : center(c), radius(r), material(m) {}
    
    bool intersect(const Ray& ray, double& t) const {
        Vec3 oc = ray.origin - center;
        double a = ray.direction.dot(ray.direction);
        double b = 2.0 * oc.dot(ray.direction);
        double c = oc.dot(oc) - radius * radius;
        double discriminant = b * b - 4 * a * c;
        
        if (discriminant < 0) return false;
        
        double t1 = (-b - sqrt(discriminant)) / (2.0 * a);
        double t2 = (-b + sqrt(discriminant)) / (2.0 * a);
        
        if (t1 > 0.001) {
            t = t1;
            return true;
        }
        if (t2 > 0.001) {
            t = t2;
            return true;
        }
        return false;
    }
    
    Vec3 getNormal(const Vec3& point) const {
        return (point - center).normalize();
    }
};

// Point light source
struct Light {
    Vec3 position;
    Color color;
    double intensity;
    
    Light(const Vec3& p, const Color& c, double i = 1.0) 
        : position(p), color(c), intensity(i) {}
};

// Scene containing geometry and lighting
class Scene {
public:
    std::vector<Sphere> spheres;
    std::vector<Light> lights;
    Color background;
    
    Scene() : background(0.1, 0.1, 0.15) {}
    
    void addSphere(const Sphere& sphere) { spheres.push_back(sphere); }
    void addLight(const Light& light) { lights.push_back(light); }
    
    bool intersect(const Ray& ray, double& closest_t, int& hit_idx) const {
        closest_t = std::numeric_limits<double>::max();
        hit_idx = -1;
        
        for (size_t i = 0; i < spheres.size(); i++) {
            double t;
            if (spheres[i].intersect(ray, t) && t < closest_t) {
                closest_t = t;
                hit_idx = i;
            }
        }
        return hit_idx != -1;
    }
    
    Color trace(const Ray& ray, int depth = 0) const {
        if (depth > 3) return background;
        
        double t;
        int hit_idx;
        
        if (!intersect(ray, t, hit_idx)) {
            return background;
        }
        
        const Sphere& sphere = spheres[hit_idx];
        Vec3 hit_point = ray.at(t);
        Vec3 normal = sphere.getNormal(hit_point);
        Vec3 view_dir = (ray.origin - hit_point).normalize();
        
        // Ambient component
        Color color = sphere.material.color * sphere.material.ambient;
        
        // Process each light source
        for (const Light& light : lights) {
            Vec3 light_dir = (light.position - hit_point).normalize();
            
            // Shadow test
            Ray shadow_ray(hit_point, light_dir);
            double shadow_t;
            int shadow_idx;
            bool in_shadow = intersect(shadow_ray, shadow_t, shadow_idx);
            
            if (!in_shadow) {
                // Diffuse lighting
                double diff = std::max(0.0, normal.dot(light_dir));
                Color diffuse = sphere.material.color * sphere.material.diffuse * diff * light.intensity;
                
                // Specular highlights
                Vec3 reflect_dir = (light_dir * -1).reflect(normal);
                double spec = pow(std::max(0.0, view_dir.dot(reflect_dir)), sphere.material.shininess);
                Color specular = light.color * sphere.material.specular * spec * light.intensity;
                
                color = color + (diffuse + specular);
            }
        }
        
        // Recursive reflection
        if (sphere.material.reflectivity > 0 && depth < 3) {
            Vec3 reflect_dir = (view_dir * -1).reflect(normal);
            Ray reflect_ray(hit_point, reflect_dir);
            Color reflect_color = trace(reflect_ray, depth + 1);
            color = color + reflect_color * sphere.material.reflectivity;
        }
        
        return color;
    }
};

Color clamp(const Color& c) {
    return Color(
        std::min(1.0, std::max(0.0, c.x)),
        std::min(1.0, std::max(0.0, c.y)),
        std::min(1.0, std::max(0.0, c.z))
    );
}

// Camera for ray generation
class Camera {
public:
    Vec3 position;
    Vec3 target;
    Vec3 up;
    double fov;
    
    Camera(const Vec3& pos, const Vec3& tgt, const Vec3& u = Vec3(0, 1, 0), double f = 60.0)
        : position(pos), target(tgt), up(u), fov(f) {}
    
    Ray getRay(double u, double v, double aspect_ratio) const {
        double theta = fov * M_PI / 180.0;
        double h = tan(theta / 2.0);
        double viewport_height = 2.0 * h;
        double viewport_width = aspect_ratio * viewport_height;
        
        Vec3 w = (position - target).normalize();
        Vec3 u_vec = up.cross(w).normalize();
        Vec3 v_vec = w.cross(u_vec);
        
        Vec3 horizontal = u_vec * viewport_width;
        Vec3 vertical = v_vec * viewport_height;
        Vec3 lower_left = position - horizontal / 2 - vertical / 2 - w;
        
        Vec3 direction = lower_left + horizontal * u + vertical * v - position;
        return Ray(position, direction);
    }
};

// SDL-based renderer
class Renderer {
private:
    SDL_Window* window;
    SDL_Renderer* renderer;
    SDL_Texture* texture;
    int width, height;
    Uint32* pixels;
    
public:
    Renderer(int w, int h) : width(w), height(h) {
        if (SDL_Init(SDL_INIT_VIDEO) < 0) {
            std::cerr << "SDL initialization failed: " << SDL_GetError() << std::endl;
            exit(1);
        }
        
        window = SDL_CreateWindow("Ray Tracer", 
                                   SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                                   width, height, SDL_WINDOW_SHOWN);
        if (!window) {
            std::cerr << "Window creation failed: " << SDL_GetError() << std::endl;
            exit(1);
        }
        
        renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
        if (!renderer) {
            std::cerr << "Renderer creation failed: " << SDL_GetError() << std::endl;
            exit(1);
        }
        
        texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888,
                                    SDL_TEXTUREACCESS_STREAMING, width, height);
        if (!texture) {
            std::cerr << "Texture creation failed: " << SDL_GetError() << std::endl;
            exit(1);
        }
        
        pixels = new Uint32[width * height];
    }
    
    ~Renderer() {
        delete[] pixels;
        SDL_DestroyTexture(texture);
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
    }
    
    void render(const Scene& scene, const Camera& camera) {
        double aspect_ratio = double(width) / height;
        
        std::cout << "Rendering with " << omp_get_max_threads() << " threads..." << std::endl;
        
        auto start_time = std::chrono::high_resolution_clock::now();
        
        #pragma omp parallel for schedule(dynamic, 1)
        for (int j = 0; j < height; j++) {
            #pragma omp critical
            {
                if (j % 50 == 0) {
                    std::cout << "Progress: " << (100 * j / height) << "%\r" << std::flush;
                }
            }
            
            for (int i = 0; i < width; i++) {
                double u = double(i) / (width - 1);
                double v = double(height - 1 - j) / (height - 1);
                
                Ray ray = camera.getRay(u, v, aspect_ratio);
                Color pixel_color = scene.trace(ray);
                pixel_color = clamp(pixel_color);
                
                Uint8 r = Uint8(255.99 * pixel_color.x);
                Uint8 g = Uint8(255.99 * pixel_color.y);
                Uint8 b = Uint8(255.99 * pixel_color.z);
                
                pixels[j * width + i] = (0xFF << 24) | (r << 16) | (g << 8) | b;
            }
        }
        
        auto end_time = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time);
        
        std::cout << "Progress: 100% - Done!     " << std::endl;
        std::cout << "Render time: " << (duration.count() / 1000.0) << " seconds" << std::endl;
        
        SDL_UpdateTexture(texture, nullptr, pixels, width * sizeof(Uint32));
        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, texture, nullptr, nullptr);
        SDL_RenderPresent(renderer);
    }
    
    bool handleEvents() {
        SDL_Event e;
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) {
                return false;
            }
            if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_ESCAPE) {
                return false;
            }
        }
        return true;
    }
    
    void waitForClose() {
        std::cout << "Press ESC or close window to exit..." << std::endl;
        bool running = true;
        while (running) {
            running = handleEvents();
            SDL_Delay(16);
        }
    }
};

int main(int argc, char* argv[]) {
    Scene scene;
    
    // Define materials
    Material red(Color(1.0, 0.2, 0.2), 0.1, 0.7, 0.8, 64, 0.4);
    Material green(Color(0.2, 1.0, 0.2), 0.1, 0.8, 0.6, 32, 0.2);
    Material blue(Color(0.2, 0.2, 1.0), 0.1, 0.6, 0.9, 128, 0.6);
    Material gold(Color(1.0, 0.84, 0.0), 0.2, 0.5, 1.0, 256, 0.5);
    Material silver(Color(0.75, 0.75, 0.75), 0.1, 0.4, 1.0, 256, 0.8);
    
    // Build scene geometry
    scene.addSphere(Sphere(Vec3(0, 0, 0), 1.0, red));
    scene.addSphere(Sphere(Vec3(-2.5, 0, -1), 0.8, green));
    scene.addSphere(Sphere(Vec3(2.5, 0.5, -0.5), 1.2, blue));
    scene.addSphere(Sphere(Vec3(0, -101, 0), 100, silver));
    scene.addSphere(Sphere(Vec3(-1, 1.5, 1), 0.5, gold));
    
    // Configure lighting
    scene.addLight(Light(Vec3(-5, 5, 5), Color(1, 1, 1), 0.8));
    scene.addLight(Light(Vec3(5, 3, 3), Color(1, 1, 1), 0.6));
    
    // Setup camera
    Camera camera(Vec3(0, 1, 5), Vec3(0, 0, 0));
    
    // Initialize renderer
    int width = 800;
    int height = 600;
    Renderer display(width, height);
    
    display.render(scene, camera);
    display.waitForClose();
    
    return 0;
}