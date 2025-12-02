// Interesting Scene Configurations for Ray Tracer
/* HOW TO USE: 
    - copy and paste the contents of whichever scenery function that you want to use into the main 
    function of raytracer.cpp where the scenery detailing is.
*/

// SCENE 1: "Mirror Gallery" - Highly reflective spheres
void setupMirrorGallery(Scene& scene) {
    // Chrome/mirror materials with high reflectivity
    Material chrome1(Color(0.9, 0.9, 1.0), 0.05, 0.3, 1.0, 512, 0.9);
    Material chrome2(Color(1.0, 0.9, 0.9), 0.05, 0.3, 1.0, 512, 0.9);
    Material chrome3(Color(0.9, 1.0, 0.9), 0.05, 0.3, 1.0, 512, 0.9);
    Material gold_mirror(Color(1.0, 0.84, 0.0), 0.1, 0.3, 1.0, 512, 0.85);
    Material floor(Color(0.2, 0.2, 0.25), 0.1, 0.6, 0.4, 64, 0.3);
    
    // Floating mirror spheres in a circle
    scene.addSphere(Sphere(Vec3(0, 0, 0), 1.0, gold_mirror));          // Center
    scene.addSphere(Sphere(Vec3(2.5, 0, 0), 0.7, chrome1));            // Right
    scene.addSphere(Sphere(Vec3(-2.5, 0, 0), 0.7, chrome2));           // Left
    scene.addSphere(Sphere(Vec3(0, 0, 2.5), 0.7, chrome3));            // Front
    scene.addSphere(Sphere(Vec3(0, 0, -2.5), 0.7, chrome1));           // Back
    scene.addSphere(Sphere(Vec3(0, 2.0, 0), 0.5, chrome2));            // Top
    scene.addSphere(Sphere(Vec3(0, -101, 0), 100, floor));             // Floor
    
    // Dramatic lighting
    scene.addLight(Light(Vec3(5, 8, 5), Color(1, 1, 1), 1.2));
    scene.addLight(Light(Vec3(-5, 8, -5), Color(0.8, 0.9, 1.0), 0.8));
    scene.addLight(Light(Vec3(0, -3, 0), Color(1.0, 0.9, 0.8), 0.3)); // Uplight
}


// SCENE 2: "Neon Dreams" - Vibrant colors with high specularity
void setupNeonDreams(Scene& scene) {
    // Bright, glossy materials like neon glass
    Material neon_pink(Color(1.0, 0.1, 0.5), 0.15, 0.6, 1.0, 256, 0.7);
    Material neon_cyan(Color(0.0, 0.9, 1.0), 0.15, 0.6, 1.0, 256, 0.7);
    Material neon_green(Color(0.2, 1.0, 0.2), 0.15, 0.6, 1.0, 256, 0.7);
    Material neon_purple(Color(0.8, 0.2, 1.0), 0.15, 0.6, 1.0, 256, 0.7);
    Material neon_yellow(Color(1.0, 1.0, 0.1), 0.15, 0.6, 1.0, 256, 0.7);
    Material dark_floor(Color(0.05, 0.05, 0.1), 0.05, 0.3, 0.8, 128, 0.6);
    
    // Clustered arrangement
    scene.addSphere(Sphere(Vec3(-2, 0.5, 0), 1.2, neon_pink));
    scene.addSphere(Sphere(Vec3(2, 0.5, 0), 1.2, neon_cyan));
    scene.addSphere(Sphere(Vec3(0, 0.5, 2), 1.2, neon_green));
    scene.addSphere(Sphere(Vec3(0, 2.5, 0), 0.8, neon_purple));
    scene.addSphere(Sphere(Vec3(0, 0.5, -2), 1.2, neon_yellow));
    scene.addSphere(Sphere(Vec3(0, -101, 0), 100, dark_floor));
    
    // Colored lighting to enhance neon effect
    scene.addLight(Light(Vec3(-5, 5, 5), Color(1.0, 0.2, 0.8), 1.0));  // Pink
    scene.addLight(Light(Vec3(5, 5, 5), Color(0.2, 0.8, 1.0), 1.0));   // Cyan
    scene.addLight(Light(Vec3(0, 8, 0), Color(1.0, 1.0, 1.0), 0.5));   // White
}

// SCENE 3: "Planetary System" - Solar system inspired
void setupPlanetarySystem(Scene& scene) {
    // Planet-like materials
    Material sun(Color(1.0, 0.9, 0.3), 0.3, 0.7, 0.3, 16, 0.1);       // Dim/matte
    Material mercury(Color(0.7, 0.7, 0.7), 0.1, 0.6, 0.8, 128, 0.4);  // Shiny gray
    Material venus(Color(1.0, 0.8, 0.5), 0.1, 0.7, 0.6, 64, 0.3);     // Pale orange
    Material earth(Color(0.2, 0.5, 1.0), 0.1, 0.8, 0.5, 64, 0.4);     // Blue
    Material mars(Color(0.9, 0.4, 0.2), 0.1, 0.7, 0.4, 32, 0.3);      // Red
    Material jupiter(Color(0.8, 0.6, 0.4), 0.1, 0.7, 0.5, 64, 0.4);   // Brown/tan
    Material space(Color(0.01, 0.01, 0.02), 0.02, 0.2, 0.1, 8, 0.05); // Dark space
    
    // Central "sun"
    scene.addSphere(Sphere(Vec3(0, 0, -3), 1.5, sun));
    
    // Orbiting "planets" at different distances
    scene.addSphere(Sphere(Vec3(-2.5, -0.2, 0), 0.3, mercury));
    scene.addSphere(Sphere(Vec3(-1.5, 0.3, 2), 0.5, venus));
    scene.addSphere(Sphere(Vec3(2, -0.3, 1), 0.6, earth));
    scene.addSphere(Sphere(Vec3(3.5, 0.5, -1), 0.4, mars));
    scene.addSphere(Sphere(Vec3(-3, 1.0, 3), 1.0, jupiter));
    
    // "Space" floor
    scene.addSphere(Sphere(Vec3(0, -101, 0), 100, space));
    
    // Bright light from "sun" direction
    scene.addLight(Light(Vec3(-2, 3, -3), Color(1.0, 0.95, 0.8), 1.5));
    scene.addLight(Light(Vec3(5, 5, 5), Color(0.3, 0.3, 0.4), 0.3));  // Ambient fill
}

// SCENE 4: "Glass Orbs" - Transparent/translucent looking spheres
void setupGlassOrbs(Scene& scene) {
    // Glass-like materials (high specular, medium diffuse, high reflectivity)
    Material glass_clear(Color(0.95, 0.95, 1.0), 0.05, 0.2, 1.0, 512, 0.8);
    Material glass_blue(Color(0.7, 0.85, 1.0), 0.05, 0.25, 1.0, 512, 0.75);
    Material glass_amber(Color(1.0, 0.8, 0.5), 0.05, 0.25, 1.0, 512, 0.75);
    Material glass_green(Color(0.7, 1.0, 0.85), 0.05, 0.25, 1.0, 512, 0.75);
    Material glass_rose(Color(1.0, 0.8, 0.9), 0.05, 0.25, 1.0, 512, 0.75);
    Material marble_floor(Color(0.85, 0.85, 0.9), 0.15, 0.6, 0.7, 128, 0.4);
    
    // Arranged like on a display shelf
    scene.addSphere(Sphere(Vec3(0, 0, 0), 1.0, glass_clear));
    scene.addSphere(Sphere(Vec3(-2.2, -0.3, 0.5), 0.7, glass_blue));
    scene.addSphere(Sphere(Vec3(2.2, -0.3, 0.5), 0.7, glass_amber));
    scene.addSphere(Sphere(Vec3(-1.5, 1.2, 1), 0.5, glass_green));
    scene.addSphere(Sphere(Vec3(1.5, 1.2, 1), 0.5, glass_rose));
    scene.addSphere(Sphere(Vec3(0, -101, 0), 100, marble_floor));
    
    // Bright lighting to show glass effect
    scene.addLight(Light(Vec3(-5, 8, 3), Color(1, 1, 1), 1.2));
    scene.addLight(Light(Vec3(5, 8, 3), Color(1, 1, 1), 1.2));
    scene.addLight(Light(Vec3(0, 3, -5), Color(0.8, 0.8, 1.0), 0.6)); // Backlight
}

// SCENE 5: "Golden Hour" - Warm sunset-like lighting
void setupGoldenHour(Scene& scene) {
    // Warm, natural materials
    Material terracotta(Color(0.8, 0.4, 0.3), 0.15, 0.7, 0.3, 32, 0.2);
    Material sand(Color(0.9, 0.8, 0.6), 0.2, 0.7, 0.2, 16, 0.1);
    Material copper(Color(0.9, 0.6, 0.4), 0.1, 0.5, 0.9, 256, 0.6);
    Material bronze(Color(0.7, 0.5, 0.3), 0.1, 0.6, 0.8, 128, 0.5);
    Material clay(Color(0.7, 0.5, 0.4), 0.15, 0.7, 0.3, 32, 0.2);
    Material desert_floor(Color(0.8, 0.7, 0.5), 0.2, 0.7, 0.2, 16, 0.15);
    
    // Natural arrangement
    scene.addSphere(Sphere(Vec3(0, 0, 0), 1.0, copper));
    scene.addSphere(Sphere(Vec3(-2.5, -0.2, -0.5), 0.8, terracotta));
    scene.addSphere(Sphere(Vec3(2.5, 0.3, 0.5), 1.0, bronze));
    scene.addSphere(Sphere(Vec3(-1, 1.5, 1.5), 0.6, clay));
    scene.addSphere(Sphere(Vec3(1.2, 1.8, -1), 0.5, sand));
    scene.addSphere(Sphere(Vec3(0, -101, 0), 100, desert_floor));
    
    // Warm, low-angle lighting like sunset
    scene.addLight(Light(Vec3(-8, 3, 2), Color(1.0, 0.7, 0.4), 1.5));   // Warm sun
    scene.addLight(Light(Vec3(5, 8, -3), Color(0.6, 0.7, 1.0), 0.4));   // Cool sky fill
}

// SCENE 6: "Candy Land" - Playful, saturated colors
void setupCandyLand(Scene& scene) {
    // Sweet, saturated colors
    Material bubblegum(Color(1.0, 0.4, 0.7), 0.2, 0.7, 0.6, 64, 0.3);
    Material lemon(Color(1.0, 1.0, 0.3), 0.2, 0.7, 0.5, 64, 0.3);
    Material mint(Color(0.4, 1.0, 0.7), 0.2, 0.7, 0.5, 64, 0.3);
    Material grape(Color(0.6, 0.3, 1.0), 0.2, 0.7, 0.6, 64, 0.3);
    Material orange(Color(1.0, 0.6, 0.2), 0.2, 0.7, 0.5, 64, 0.3);
    Material cream(Color(1.0, 0.95, 0.85), 0.3, 0.6, 0.3, 32, 0.2);
    
    // Pile of candy spheres
    scene.addSphere(Sphere(Vec3(0, 0, 0), 1.0, bubblegum));
    scene.addSphere(Sphere(Vec3(-1.8, -0.3, 0.8), 0.8, lemon));
    scene.addSphere(Sphere(Vec3(1.8, -0.3, 0.8), 0.8, mint));
    scene.addSphere(Sphere(Vec3(-0.8, 1.3, 1.2), 0.7, grape));
    scene.addSphere(Sphere(Vec3(0.8, 1.3, 1.2), 0.7, orange));
    scene.addSphere(Sphere(Vec3(0, -101, 0), 100, cream));
    
    // Bright, cheerful lighting
    scene.addLight(Light(Vec3(-5, 8, 5), Color(1, 1, 1), 1.0));
    scene.addLight(Light(Vec3(5, 8, 5), Color(1, 1, 1), 1.0));
}

// SCENE 7: "Deep Ocean" - Cool, mysterious underwater vibe
void setupDeepOcean(Scene& scene) {
    // Underwater color palette
    Material pearl(Color(0.9, 0.95, 1.0), 0.1, 0.4, 1.0, 256, 0.7);
    Material aqua(Color(0.3, 0.7, 0.8), 0.15, 0.6, 0.6, 64, 0.4);
    Material deep_blue(Color(0.2, 0.4, 0.7), 0.15, 0.6, 0.5, 64, 0.3);
    Material teal(Color(0.2, 0.6, 0.6), 0.15, 0.6, 0.6, 64, 0.4);
    Material coral(Color(0.9, 0.5, 0.5), 0.15, 0.7, 0.4, 32, 0.2);
    Material ocean_floor(Color(0.15, 0.25, 0.35), 0.1, 0.5, 0.3, 32, 0.2);
    
    // Floating spheres like bubbles/organisms
    scene.addSphere(Sphere(Vec3(0, 0.5, 0), 1.0, pearl));
    scene.addSphere(Sphere(Vec3(-2, 0, 1), 0.7, aqua));
    scene.addSphere(Sphere(Vec3(2, 1, 0), 0.8, deep_blue));
    scene.addSphere(Sphere(Vec3(-1, 2, -1), 0.5, teal));
    scene.addSphere(Sphere(Vec3(1.5, -0.3, 2), 0.6, coral));
    scene.addSphere(Sphere(Vec3(0, -101, 0), 100, ocean_floor));
    
    // Muted, directional lighting (like underwater)
    scene.addLight(Light(Vec3(-3, 10, 0), Color(0.6, 0.8, 1.0), 0.8));
    scene.addLight(Light(Vec3(5, 5, 5), Color(0.4, 0.6, 0.8), 0.5));
}

