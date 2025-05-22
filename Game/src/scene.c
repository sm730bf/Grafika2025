#include "scene.h"
#include <stdio.h>
#include <obj/load.h>
#include <obj/draw.h>
#include <time.h>
#include <math.h>



float light = 1.0f;
void init_scene(Scene* scene)
{
   // load_model(&(scene->cube), "assets/models/cube.obj");
    //scene->texture_id = load_texture("assets/textures/grass.png");
    scene->light_position = (vec3){5.0f, 5.0f, 5.0f};
    glBindTexture(GL_TEXTURE_2D, scene->texture_id);

    scene->material.ambient.red = 0.0;
    scene->material.ambient.green = 0.0;
    scene->material.ambient.blue = 0.0;

    scene->material.diffuse.red = 1.0;
    scene->material.diffuse.green = 1.0;
    scene->material.diffuse.blue = 0.0;

    scene->material.specular.red = 0.0;
    scene->material.specular.green = 0.0;
    scene->material.specular.blue = 0.0;
    
    scene->material.shininess = 0.0; 

    // Initialize the tank
    init_Object(&(scene->tank), "assets/models/14079_WWII_Tank_UK_Cromwell_v1_L2.obj", 
                "assets/textures/14079_WWII_Tank_UK_Cromwell_hull_diff.jpg");
    scene->tank.pos = (vec3){0.0f, 0.0f, -0.25f};
    scene->tank.rot = (vec3){0.0f, 0.0f, 0.0f};
    scene->tank.scale = (vec3){0.3f, 0.3f, 0.3f};

    scene->tank_speed = 0.04f;       // Set tank speed
    scene->tank_rotation = 0.0f;   // Initial rotation angle

    // Initialize tank2
    init_Object(&(scene->tank2), "assets/models/14079_WWII_Tank_UK_Cromwell_v1_L2.obj", 
                "assets/textures/14079_WWII_Tank_UK_Cromwell_hull_diff.jpg");
    scene->tank2.pos = (vec3){5.0f, 0.0f, -0.25f};
    scene->tank2.rot = (vec3){0.0f, 0.0f, 0.0f};
    scene->tank2.scale = (vec3){0.3f, 0.3f, 0.3f};

    // Initialize hit points
    scene->tank_hp = 3;
    scene->tank2_hp = 3;

    // Initialize the projectile model
    init_Object(&(scene->projectile_model), "assets/models/projectile.obj", 
                "assets/textures/grass.png");
    printf("Projectile model loaded successfully.\n");

    // Initialize projectiles
    for (int i = 0; i < 10; i++) {
        scene->projectiles[i].active = 0;
        //printf("Projectile %d initialized: active=%d\n", i, scene->projectiles[i].active);
    }

     // Initialize the ground
     init_Object(&(scene->ground), "assets/models/plane.obj", 
     "assets/textures/ground_texture.jpg");
    scene->ground.pos = (vec3){0.0f, 0.0f, -0.01f};  // Slightly below the tank
    scene->ground.rot = (vec3){0.0f, 0.0f, 0.0f};   // No rotation
    scene->ground.scale = (vec3){10.0f, 10.0f, 0.01f}; // Large scale for the ground

    // Initialize the cubes
    for (int i = 0; i < NUM_CUBES; i++) {
        init_Object(&(scene->cubes[i]), "assets/models/cube.obj", "assets/textures/wall_texture.jpg");

        // Randomize positions within the map boundaries
        scene->cubes[i].pos = (vec3){
            (float)(rand() % 18 - 9), // Random X position between -9 and 9
            (float)(rand() % 18 - 9), // Random Y position between -9 and 9
            0.0f                      // Z position (ground level)
        };

        scene->cubes[i].scale = (vec3){1.0f, 1.0f, 2.0f}; // Uniform scale
        scene->cubes[i].rot = (vec3){0.0f, 0.0f, 0.0f};   // No rotation
    }
    // Initialize the walls
    init_Object(&(scene->walls[0]), "assets/models/wall.obj", "assets/textures/wall_texture.jpg");
    scene->walls[0].pos = (vec3){-10.0f, 0.0f, 0.0f}; // Left wall
    scene->walls[0].scale = (vec3){1.0f, 20.0f, 1.0f}; // Scale to fit the map
    scene->walls[0].rot = (vec3){0.0f, 0.0f, 0.0f};

    init_Object(&(scene->walls[1]), "assets/models/wall.obj", "assets/textures/wall_texture.jpg");
    scene->walls[1].pos = (vec3){10.0f, 0.0f, 0.0f}; // Right wall
    scene->walls[1].scale = (vec3){1.0f, 20.0f, 1.0f};
    scene->walls[1].rot = (vec3){0.0f, 0.0f, 0.0f};

    init_Object(&(scene->walls[2]), "assets/models/wall.obj", "assets/textures/wall_texture.jpg");
    scene->walls[2].pos = (vec3){0.0f, -10.0f, 0.0f}; // Bottom wall
    scene->walls[2].scale = (vec3){20.0f, 1.0f, 1.0f};
    scene->walls[2].rot = (vec3){0.0f, 0.0f, 0.0f};

    init_Object(&(scene->walls[3]), "assets/models/wall.obj", "assets/textures/wall_texture.jpg");
    scene->walls[3].pos = (vec3){0.0f, 10.0f, 0.0f}; // Top wall
    scene->walls[3].scale = (vec3){20.0f, 1.0f, 1.0f};
    scene->walls[3].rot = (vec3){0.0f, 0.0f, 0.0f};
}
void update_light() {
    float ambient_light[] = { light * 0.2f, light * 0.2f, light * 0.2f, 1.0f };
    float diffuse_light[] = { light, light, light, 1.0f };
    float specular_light[] = { light, light, light, 1.0f };
    float position[] = { 5.0f, 5.0f, 5.0f, 1.0f }; // Example light position

    glLightfv(GL_LIGHT0, GL_AMBIENT, ambient_light);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse_light);
    glLightfv(GL_LIGHT0, GL_SPECULAR, specular_light);
    glLightfv(GL_LIGHT0, GL_POSITION, position);

    printf("Light updated: intensity=%f\n", light);
}

void set_lighting() {
    glEnable(GL_LIGHT0); // Enable light 0
    glEnable(GL_LIGHTING); // Enable lighting
    update_light(); // Initialize the light properties
}
void compute_shadow_matrix(float shadow_mat[16], vec3 light_pos) {
    float a = 0, b = 0, c = 1, d = 0; // Plane: Z = 0 → ax + by + cz + d = 0

    float lx = light_pos.x;
    float ly = light_pos.y;
    float lz = light_pos.z;

    float dot = a * lx + b * ly + c * lz + d;

    shadow_mat[0]  = dot - lx * a;
    shadow_mat[4]  = -lx * b;
    shadow_mat[8]  = -lx * c;
    shadow_mat[12] = -lx * d;

    shadow_mat[1]  = -ly * a;
    shadow_mat[5]  = dot - ly * b;
    shadow_mat[9]  = -ly * c;
    shadow_mat[13] = -ly * d;

    shadow_mat[2]  = -lz * a;
    shadow_mat[6]  = -lz * b;
    shadow_mat[10] = dot - lz * c;
    shadow_mat[14] = -lz * d;

    shadow_mat[3]  = -a;
    shadow_mat[7]  = -b;
    shadow_mat[11] = -c;
    shadow_mat[15] = dot - d;
}

void render_shadows(const Scene* scene) {
    glDisable(GL_LIGHTING);
    glColor4f(0.0f, 0.0f, 0.0f, 0.5f); // Semi-transparent shadow

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // Render shadows for all objects
    for (int i = 0; i < NUM_CUBES; i++) {
        render_shadow_volume(&scene->cubes[i], scene->light_position);
    }
    render_shadow_volume(&scene->tank, scene->light_position);
    render_shadow_volume(&scene->tank2, scene->light_position);

    glDisable(GL_BLEND);
    glEnable(GL_LIGHTING);
}

void render_shadow_volume(const Object* obj, vec3 light_pos) {
    float shadow_mat[16];
    compute_shadow_matrix(shadow_mat, light_pos);

    glPushMatrix();
        glMultMatrixf(shadow_mat);
        glTranslatef(obj->pos.x, obj->pos.y, obj->pos.z);
        glScalef(obj->scale.x, obj->scale.y, obj->scale.z);
        draw_model(&obj->model);
    glPopMatrix();
}

void set_material(const Material* material)
{
    float ambient_material_color[] = {
        material->ambient.red,
        material->ambient.green,
        material->ambient.blue
    };

    float diffuse_material_color[] = {
        material->diffuse.red,
        material->diffuse.green,
        material->diffuse.blue
    };

    float specular_material_color[] = {
        material->specular.red,
        material->specular.green,
        material->specular.blue
    };

    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, ambient_material_color);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, diffuse_material_color);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specular_material_color);

    glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, &(material->shininess));
}

void update_scene(Scene* scene)
{
    for (int i = 0; i < 10; i++) {
        if (scene->projectiles[i].active) {
            /*printf("Updating projectile %d: pos=(%.2f, %.2f, %.2f), dir=(%.2f, %.2f, %.2f)\n",
                   i,
                   scene->projectiles[i].pos.x, scene->projectiles[i].pos.y, scene->projectiles[i].pos.z,
                   scene->projectiles[i].dir.x, scene->projectiles[i].dir.y, scene->projectiles[i].dir.z);
                */
            scene->projectiles[i].pos.x += scene->projectiles[i].dir.x * scene->projectiles[i].speed;
            scene->projectiles[i].pos.y += scene->projectiles[i].dir.y * scene->projectiles[i].speed;

            // Deactivate projectile if it goes out of bounds
            if (scene->projectiles[i].pos.x > 20.0f || scene->projectiles[i].pos.x < -20.0f ||
                scene->projectiles[i].pos.y > 20.0f || scene->projectiles[i].pos.y < -20.0f) {
                printf("Deactivating projectile %d (out of bounds)\n", i);
                scene->projectiles[i].active = 0;
            }
        }
    }
}

void render_scene(const Scene* scene)
{
    set_material(&(scene->material));
    set_lighting();
    draw_origin();
   // draw_model(&(scene->cube));
   render_shadows(scene);

    // Render the tank
    glPushMatrix();
    glTranslatef(scene->tank.pos.x, scene->tank.pos.y, scene->tank.pos.z);
    glRotatef(scene->tank.rot.z * (180.0f / M_PI), 0.0f, 0.0f, 1.0f); // Convert radians to degrees
    glScalef(scene->tank.scale.x, scene->tank.scale.y, scene->tank.scale.z);

    glBindTexture(GL_TEXTURE_2D, scene->tank.texture_id);
    draw_model(&(scene->tank.model)); // Render the entire model with a single texture

    glPopMatrix();

    for (int i = 0; i < NUM_CUBES; i++) {
        glPushMatrix();
        glTranslatef(scene->cubes[i].pos.x, scene->cubes[i].pos.y, scene->cubes[i].pos.z);
        glScalef(scene->cubes[i].scale.x, scene->cubes[i].scale.y, scene->cubes[i].scale.z);
        glBindTexture(GL_TEXTURE_2D, scene->cubes[i].texture_id);
        draw_model(&(scene->cubes[i].model));
        glPopMatrix();
    }

     // Render the ground
     glPushMatrix();
     glTranslatef(scene->ground.pos.x, scene->ground.pos.y, scene->ground.pos.z);
     glRotatef(scene->ground.rot.y * (180.0f / M_PI), 0.0f, 1.0f, 0.0f);
     glScalef(scene->ground.scale.x, scene->ground.scale.y, scene->ground.scale.z);
 
     glBindTexture(GL_TEXTURE_2D, scene->ground.texture_id);
     draw_model(&(scene->ground.model)); // Render the ground model
 
     glPopMatrix();

    for (int i = 0; i < 10; i++) {
    if (scene->projectiles[i].active) {
       // printf("Rendering projectile %d at position: x=%.2f, y=%.2f, z=%.2f\n",
              // i, scene->projectiles[i].pos.x, scene->projectiles[i].pos.y, scene->projectiles[i].pos.z);

        glPushMatrix();
        glTranslatef(scene->projectiles[i].pos.x, scene->projectiles[i].pos.y, scene->projectiles[i].pos.z);
        glScalef(0.08f, 0.08f, 0.08f); // Scale the projectile to a smaller size
        draw_model(&(scene->projectile_model.model)); // Render the projectile model
        glPopMatrix();
        }
    }

    // Render tank2
    glPushMatrix();
    glTranslatef(scene->tank2.pos.x, scene->tank2.pos.y, scene->tank2.pos.z);
    glRotatef(scene->tank2.rot.z * (180.0f / M_PI), 0.0f, 0.0f, 1.0f); // Convert radians to degrees
    glScalef(scene->tank2.scale.x, scene->tank2.scale.y, scene->tank2.scale.z);

    glBindTexture(GL_TEXTURE_2D, scene->tank2.texture_id);
    draw_model(&(scene->tank2.model)); // Render the second tank model

    glPopMatrix();

    for (int i = 0; i < 4; i++) {
        glPushMatrix();
        glTranslatef(scene->walls[i].pos.x, scene->walls[i].pos.y, scene->walls[i].pos.z);
        glScalef(scene->walls[i].scale.x, scene->walls[i].scale.y, scene->walls[i].scale.z);
        glBindTexture(GL_TEXTURE_2D, scene->walls[i].texture_id);
        draw_model(&(scene->walls[i].model));
        glPopMatrix();
    }
}

void set_light_plus(){
    light+=0.2;
    printf("Light: %f\n", light);
}

void set_light_minus(){
    light-=0.2;
    printf("Light: %f\n", light);
}

void draw_origin()
{
    glBegin(GL_LINES);

    glColor3f(1, 0, 0);
    glVertex3f(0, 0, 0);
    glVertex3f(1, 0, 0);

    glColor3f(0, 1, 0);
    glVertex3f(0, 0, 0);
    glVertex3f(0, 1, 0);

    glColor3f(0, 0, 1);
    glVertex3f(0, 0, 0);
    glVertex3f(0, 0, 1);

    glEnd();
}
