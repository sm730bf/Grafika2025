#include "app.h"
#include <math.h>
#include <stdio.h>

#include <SDL2/SDL_image.h>

int tank_forward = 0, tank_backward = 0, tank_left_turn = 0, tank_right_turn = 0;

void init_app(App* app, int width, int height)
{
    int error_code;
    int inited_loaders;

    app->is_running = false;

    error_code = SDL_Init(SDL_INIT_EVERYTHING);
    if (error_code != 0) {
        printf("[ERROR] SDL initialization error: %s\n", SDL_GetError());
        return;
    }

    app->window = SDL_CreateWindow(
        "Cube!",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        width, height,
        SDL_WINDOW_OPENGL);
    if (app->window == NULL) {
        printf("[ERROR] Unable to create the application window!\n");
        return;
    }

    inited_loaders = IMG_Init(IMG_INIT_PNG);
    if (inited_loaders == 0) {
        printf("[ERROR] IMG initialization error: %s\n", IMG_GetError());
        return;
    }

    app->gl_context = SDL_GL_CreateContext(app->window);
    if (app->gl_context == NULL) {
        printf("[ERROR] Unable to create the OpenGL context!\n");
        return;
    }

    init_opengl();
    reshape(width, height);

    init_camera(&(app->camera));
    init_scene(&(app->scene));

    // Set the cursor to the middle of the window
    SDL_WarpMouseInWindow(app->window, width / 2, height / 2);

    app->is_running = true;
}

void init_opengl()
{
    glShadeModel(GL_SMOOTH);

    glEnable(GL_NORMALIZE);
    glEnable(GL_AUTO_NORMAL);

    glClearColor(0.1, 0.1, 0.1, 1.0);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glEnable(GL_DEPTH_TEST);

    glClearDepth(1.0);

    glEnable(GL_TEXTURE_2D);

    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
}

void reshape(GLsizei width, GLsizei height)
{
    int x, y, w, h;
    double ratio;

    ratio = (double)width / height;
    if (ratio > VIEWPORT_RATIO) {
        w = (int)((double)height * VIEWPORT_RATIO);
        h = height;
        x = (width - w) / 2;
        y = 0;
    }
    else {
        w = width;
        h = (int)((double)width / VIEWPORT_RATIO);
        x = 0;
        y = (height - h) / 2;
    }

    glViewport(x, y, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glFrustum(
        -.1, .1,
        -.1, .1,
        .1, 10
    );
}

void tank_movement(Scene* scene)
{
    if(tank_forward){
        scene->tank.pos.y -= scene->tank_speed * sin(scene->tank.rot.z); // Use sin for X-axis
        scene->tank.pos.x -= scene->tank_speed * cos(scene->tank.rot.z); // Use cos for Z-axis
    } if(tank_backward){
        scene->tank.pos.y += scene->tank_speed * sin(scene->tank.rot.z); // Use sin for X-axis
        scene->tank.pos.x += scene->tank_speed * cos(scene->tank.rot.z); // Use cos for Z-axis
    } if(tank_left_turn){
        scene->tank.rot.z += 1.0f * (M_PI / 180.0f); // Convert degrees to radians
    } if(tank_right_turn){
        scene->tank.rot.z -= 1.0f * (M_PI / 180.0f); // Convert degrees to radians
    }
}
void fire_projectile(Scene* scene, Object* tank)
{
    if (scene == NULL || tank == NULL) {
        printf("[ERROR] Null pointer passed to fire_projectile!\n");
        return;
    }

    for (int i = 0; i < 10; i++) {
        if (!scene->projectiles[i].active) {
            printf("Firing projectile %d from tank at position: x=%.2f, y=%.2f, z=%.2f\n",
                   i, tank->pos.x, tank->pos.y, tank->pos.z);
            printf("Tank rotation: z=%.2f radians\n", tank->rot.z);

            scene->projectiles[i].active = 1;
             // Set projectile position to tank position with a Z-axis offset
             scene->projectiles[i].pos = (vec3){
                tank->pos.x,
                tank->pos.y,
                tank->pos.z + 0.5f // Add 0.5f offset to Z-axis
            };
            scene->projectiles[i].dir = (vec3){
                -cos(tank->rot.z),
                -sin(tank->rot.z),
                0.0f  
                   
            };
            scene->projectiles[i].speed = 0.2f;
            printf("Projectile %d initialized: dir=(%.2f, %.2f, %.2f), speed=%.2f\n",
                   i, scene->projectiles[i].dir.x, scene->projectiles[i].dir.y, scene->projectiles[i].dir.z,
                   scene->projectiles[i].speed);
            break;
        }
    }
}

void handle_app_events(App* app, Scene* scene)
{
    SDL_Event event;
    static bool is_mouse_down = false;
    static int mouse_x = 0;
    static int mouse_y = 0;
    int x;
    int y;
    int window_width, window_height;
    
    SDL_GetWindowSize(app->window, &window_width, &window_height);

    while (SDL_PollEvent(&event)) {
        switch (event.type) {
        case SDL_KEYDOWN:
            switch (event.key.keysym.scancode) {
            case SDL_SCANCODE_ESCAPE:
                app->is_running = false;
                break;
                case SDL_SCANCODE_W: // Move forward
                tank_forward = 1;
                break;
            case SDL_SCANCODE_S: // Move backward
                tank_backward = 1;
                break;
            case SDL_SCANCODE_A:
                tank_left_turn = 1;
                break;
            case SDL_SCANCODE_D:
                tank_right_turn = 1;
                break;
            case SDL_SCANCODE_SPACE:
                fire_projectile(&(app->scene), &(app->scene.tank));
                break;
            case SDL_SCANCODE_KP_PLUS:
                set_light_plus();
                break;
            case SDL_SCANCODE_KP_MINUS:
                set_light_minus();
                break;
            /*case SDL_SCANCODE_F1:
                scene->isShowingMenu = !scene->isShowingMenu;
                break;*/
            default:
                break;
            }
            break;
        case SDL_KEYUP:
            switch (event.key.keysym.scancode) {
            case SDL_SCANCODE_W:
                tank_forward = 0;
                break;
            case SDL_SCANCODE_S:
                tank_backward = 0;
                break;
            case SDL_SCANCODE_A:
                tank_left_turn = 0;
                break;
            case SDL_SCANCODE_D:
                tank_right_turn = 0;
                break;
            default:
                break;
            }
            break;
        case SDL_MOUSEBUTTONDOWN:
            if (event.button.button == SDL_BUTTON_MIDDLE) {
                is_mouse_down = true;
            }
        break;
        case SDL_MOUSEMOTION:
            SDL_GetMouseState(&x, &y);
            if (is_mouse_down) {
                rotate_camera(&(app->camera), mouse_x - x, mouse_y - y);
            }
            mouse_x = x;
            mouse_y = y;
            break;
        case SDL_MOUSEBUTTONUP:
            if (event.button.button == SDL_BUTTON_MIDDLE) {
                is_mouse_down = false;
            }
        break;
        case SDL_MOUSEWHEEL:
            if (event.wheel.y > 0) {
                // Zoom in
                zoom_camera(&(app->camera), -1.0f);
            } else if (event.wheel.y < 0) {
                // Zoom out
                zoom_camera(&(app->camera), 1.0f);
            }
            break;
        case SDL_QUIT:
            app->is_running = false;
            break;
        default:
            break;
        }
    }
}

void move_camera(App* app){
    int x, y;
    SDL_GetMouseState(&x, &y);

    int window_width, window_height;
    SDL_GetWindowSize(app->window, &window_width, &window_height);

    float forward = 0.0f;
    float side = 0.0f;

    // Debugging: Print mouse and window dimensions
    printf("Mouse: (%d, %d), Window: (%d, %d)\n", x, y, window_width, window_height);

    if (x <= EDGE_THRESHOLD) {
        side = CAMERA_MOVE_SPEED; // Move left
    } else if (x >= window_width - EDGE_THRESHOLD) {
        side = -CAMERA_MOVE_SPEED; // Move right
    }

    if (y <= EDGE_THRESHOLD) {
        forward = CAMERA_MOVE_SPEED; // Move forward
    } else if (y >= window_height - EDGE_THRESHOLD) {
        forward = -CAMERA_MOVE_SPEED; // Move backward
    }

    set_camera_speed(&(app->camera), forward);
    set_camera_side_speed(&(app->camera), side);
}

void update_app(App* app)
{
    double current_time;
    double elapsed_time;

    current_time = (double)SDL_GetTicks() / 1000;
    elapsed_time = current_time - app->uptime;
    app->uptime = current_time;

    move_camera(app);
    tank_movement(&(app->scene));
    update_camera(&(app->camera), elapsed_time);
    update_scene(&(app->scene));
}

void render_app(App* app)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);

    glPushMatrix();
    set_view(&(app->camera));
    render_scene(&(app->scene));
    glPopMatrix();

    if (app->camera.is_preview_visible) {
        show_texture_preview();
    }

    SDL_GL_SwapWindow(app->window);
}

void destroy_app(App* app)
{
    if (app->gl_context != NULL) {
        SDL_GL_DeleteContext(app->gl_context);
    }

    if (app->window != NULL) {
        SDL_DestroyWindow(app->window);
    }

    SDL_Quit();
}

void check_collisions(Scene* scene)
{
    for (int i = 0; i < 10; i++) {
        if (scene->projectiles[i].active) {
            printf("Checking collision for projectile %d: pos=(%.2f, %.2f, %.2f)\n",
                   i, scene->projectiles[i].pos.x, scene->projectiles[i].pos.y, scene->projectiles[i].pos.z);

            // Check collision with tank2
            if (fabs(scene->projectiles[i].pos.x - scene->tank2.pos.x) < 0.5f &&
                fabs(scene->projectiles[i].pos.y - scene->tank2.pos.y) < 0.5f) {
                printf("Projectile %d hit tank2!\n", i);
                scene->projectiles[i].active = 0;
                scene->tank2_hp--;
                if (scene->tank2_hp <= 0) {
                    printf("Tank2 destroyed!\n");
                }
            }

            // Check collision with tank
            if (fabs(scene->projectiles[i].pos.x - scene->tank.pos.x) < 0.5f &&
                fabs(scene->projectiles[i].pos.y - scene->tank.pos.y) < 0.5f) {
                printf("Projectile %d hit tank!\n", i);
                scene->projectiles[i].active = 0;
                scene->tank_hp--;
                if (scene->tank_hp <= 0) {
                    printf("Tank destroyed!\n");
                }
            }
        }
    }
}