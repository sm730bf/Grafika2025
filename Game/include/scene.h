#ifndef SCENE_H
#define SCENE_H

#include "camera.h"
#include "texture.h"
#include "object.h"
#include <obj/model.h>


#define NUM_CUBES 20

typedef struct Scene
{
    //Model cube;
    Object tank;
    Object tank2;
    Object projectile_model;
    Projectile projectiles[10];
    Object cubes[NUM_CUBES];
    int tank_hp;
    int tank2_hp;
    Object ground;
    Material material;
    Object walls[4]; // Array for the 4 walls
    GLuint texture_id;
    bool is_menu_visible; // Whether the menu cube is currently visible
    GLuint menu_texture;  // Texture for the menu
    Object menu_cube;     // Cube object for the menu
    
    vec3 light_position; // Light source position
    float tank_speed;      // Speed of the tank
    float tank_rotation;   // Rotation angle of the tank
} Scene;

/**
 * Initialize the scene by loading models.
 */
void init_scene(Scene* scene);

/**
 * Set the lighting of the scene.
 */
void set_lighting();

/**
 * Set the current material.
 */
void set_material(const Material* material);

/**
 * Update the scene.
 */
void update_scene(Scene* scene);

/**
 * Render the scene objects.
 */
void render_scene(const Scene* scene);

/**
 * Draw the origin of the world coordinate system.
 */
void draw_origin();

void draw_string(const Scene* scene, const char* s);

void render_obj();

void destroy_scene();

void keeper_movement(Object* keeper);

void render_menu_texture(GLuint texture);

void draw_score(const Scene* scene);

void set_light_plus();
void set_light_minus();

void render_shadow_volume(const Object* object, vec3 light_position);
#endif /* SCENE_H */
