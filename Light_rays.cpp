#include<iostream>
#include<SDL3/SDL.h>
#include<math.h>

#define WIDTH 900
#define HEIGHT 600
#define COLOUR_WHITE 0xffffffff
#define COLOUR_BLACK 0x00000000
#define RAYS_NUMBER 100

struct Circle{
    double x;
    double y;
    double radius;
};

struct Ray{
    double x_start, y_start;
    double angle;
};

void FillCircle(SDL_Surface *surface, struct Circle circle, Uint32 colour){
    double radius_squared = pow(circle.radius, 2);
    for(double x = circle.x - circle.radius; x<= circle.x + circle.radius;x++){
        for(double y = circle.y - circle.radius; y <= circle.y + circle.radius; y++){
            double distance_squared = pow(x - circle.x, 2) + pow(y - circle.y, 2);
            if (distance_squared < radius_squared){
                SDL_Rect pixel = (SDL_Rect){x,y,1,1};
                SDL_FillSurfaceRect(surface, &pixel, colour);
            }
        }
    }
}

void generate_rays(struct Circle circle, struct Ray rays[RAYS_NUMBER]){
    for(int i = 0; i < RAYS_NUMBER;i++){
        double angle = ((double)i /RAYS_NUMBER) * 360;
        struct Ray ray = {circle.x, circle.y, angle};
        rays[i] = ray;
    }
}

void fillrays(SDL_Surface *surface, struct Ray rays[RAYS_NUMBER], Uint32 colour, struct Circle object){
    double radius_squared = pow(object.radius,2);
    for(int i = 0; i < RAYS_NUMBER; i++){
        struct Ray ray = rays[i];

        int screen_end = 0;
        int hits_object = 0;
        double x_draw = ray.x_start;
        double y_draw = ray.y_start;
        double step = 1;
        while(!screen_end && !hits_object){
            x_draw += step * cos(ray.angle);
            y_draw += step * sin(ray.angle);

            SDL_Rect pixel = (SDL_Rect){x_draw, y_draw, 1,1};
            SDL_FillSurfaceRect(surface, &pixel, colour);

            if(x_draw < 0 || x_draw > WIDTH)
                screen_end = 1;
            if(y_draw < 0 || y_draw > HEIGHT)
                screen_end = 1;

            double distance_squared = pow(x_draw - object.x,2) + pow(y_draw - object.y, 2);
            if(distance_squared < radius_squared){
                break;
            }
        }
    }
}

int main(int argc, char* argv[]){
    SDL_Init(SDL_INIT_VIDEO);
    SDL_Window *window = SDL_CreateWindow("light ray simulation",WIDTH,HEIGHT,0);
    SDL_Surface *surface = SDL_GetWindowSurface(window);
    SDL_Rect erase_rect = (SDL_Rect){0,0,WIDTH,HEIGHT};

    struct Circle shadow_circle = {650,300,140};
    struct Circle circle = {200,200,80};
    struct Ray rays[RAYS_NUMBER];
    generate_rays(circle, rays);

    int simulation_running = 1;
    SDL_Event event;

    while(simulation_running){
        while(SDL_PollEvent(&event)){
            if(event.type == SDL_EVENT_QUIT){
                simulation_running = 0;
            }
            if(event.type == SDL_EVENT_MOUSE_MOTION && event.motion.state != 0){
                circle.x = event.motion.x;
                circle.y = event.motion.y;
                generate_rays(circle, rays);
            }
        }
        SDL_FillSurfaceRect(surface, &erase_rect, COLOUR_BLACK);
        FillCircle(surface, shadow_circle, COLOUR_WHITE);
        FillCircle(surface, circle, COLOUR_WHITE);
        fillrays(surface, rays, COLOUR_WHITE, shadow_circle);
        //draw rects from memory
        SDL_UpdateWindowSurface(window);
        SDL_Delay(10);
    }
    return 0; 

}
