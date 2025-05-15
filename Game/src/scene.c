#include "scene.h"
#include <stdio.h>
#include <obj/load.h>
#include <obj/draw.h>
#include <time.h>
#include <math.h>



float light = 1.0;
void init_scene(Scene* scene)
{
   // load_model(&(scene->cube), "assets/models/cube.obj");
    //scene->texture_id = load_texture("assets/textures/grass.png");
    
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
    scene->tank.pos = (vec3){0.0f, 0.0f, 0.0f};
    scene->tank.rot = (vec3){0.0f, 0.0f, 0.0f};
    scene->tank.scale = (vec3){0.3f, 0.3f, 0.3f};

    scene->tank_speed = 0.04f;       // Set tank speed
    scene->tank_rotation = 0.0f;   // Initial rotation angle

    // Initialize tank2
    init_Object(&(scene->tank2), "assets/models/14079_WWII_Tank_UK_Cromwell_v1_L2.obj", 
                "assets/textures/14079_WWII_Tank_UK_Cromwell_hull_diff.jpg");
    scene->tank2.pos = (vec3){5.0f, 0.0f, 0.0f};
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
        printf("Projectile %d initialized: active=%d\n", i, scene->projectiles[i].active);
    }

     // Initialize the ground
     init_Object(&(scene->ground), "assets/models/plane.obj", 
     "assets/textures/ground_texture.jpg");
    scene->ground.pos = (vec3){0.0f, -0.5f, 0.0f};  // Slightly below the tank
    scene->ground.rot = (vec3){0.0f, 0.0f, 0.0f};   // No rotation
    scene->ground.scale = (vec3){10.0f, 10.0f, 1.0f}; // Large scale for the ground
    
}

void set_lighting()
{
    float ambient_light[] = { 0.0f, 0.0f, 0.0f, 1.0f };
    float diffuse_light[] = { 1.0f, 1.0f, 1.0, 1.0f };
    float specular_light[] = { 0.0f, 0.0f, 0.0f, 1.0f };
    float position[] = { 0.0f, 0.0f, 10.0f, 1.0f };

    glLightfv(GL_LIGHT0, GL_AMBIENT, ambient_light);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse_light);
    glLightfv(GL_LIGHT0, GL_SPECULAR, specular_light);
    glLightfv(GL_LIGHT0, GL_POSITION, position);
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
            printf("Updating projectile %d: pos=(%.2f, %.2f, %.2f), dir=(%.2f, %.2f, %.2f)\n",
                   i,
                   scene->projectiles[i].pos.x, scene->projectiles[i].pos.y, scene->projectiles[i].pos.z,
                   scene->projectiles[i].dir.x, scene->projectiles[i].dir.y, scene->projectiles[i].dir.z);

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

    // Render the tank
    glPushMatrix();
    glTranslatef(scene->tank.pos.x, scene->tank.pos.y, scene->tank.pos.z);
    glRotatef(scene->tank.rot.z * (180.0f / M_PI), 0.0f, 0.0f, 1.0f); // Convert radians to degrees
    glScalef(scene->tank.scale.x, scene->tank.scale.y, scene->tank.scale.z);

    glBindTexture(GL_TEXTURE_2D, scene->tank.texture_id);
    draw_model(&(scene->tank.model)); // Render the entire model with a single texture

    glPopMatrix();

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
        printf("Rendering projectile %d at position: x=%.2f, y=%.2f, z=%.2f\n",
               i, scene->projectiles[i].pos.x, scene->projectiles[i].pos.y, scene->projectiles[i].pos.z);

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
