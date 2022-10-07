#include <iostream>
#include <vector>

#include <SDL2/SDL.h>

#include "input_manager.hpp"
#include "timer.hpp"
#include "color.hpp"

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600

#define CONTROL_POINT_LEN 16
#define POINT_LEN 12

const Color cp_color = {0,0,0,255};
const Color p_color = {33,96,31,255};
const Color bg = {58,58,58,255};
const Color line_color = {30,0,0,255};
const Color curve_color = {30,30,200,255};

int sdl_check(int code)
{
    if (code<0) {
        std::cerr << "Error: " << SDL_GetError() << '\n';
        exit(1);
    }
    return code;
}

void* sdl_check_ptr(void* ptr)
{
    if (ptr==nullptr) {
        std::cerr << "Error: " << SDL_GetError() << '\n';
        exit(1);
    }
    return ptr;
}

void draw_square(SDL_Renderer* renderer, Vector2f rect_center, int len, Color c)
{
    Vector2f upper_left = rect_center-Vector2f(len*0.5f,len*0.5f);
    SDL_Rect rect = {(int)upper_left.x,(int)upper_left.y,len,len};
    sdl_check(SDL_SetRenderDrawColor(renderer,c.r,c.g,c.b,c.a));
    sdl_check(SDL_RenderFillRect(renderer,&rect));
}

void draw_lines(SDL_Renderer* renderer, const std::vector<Vector2f>& pts, Color c)
{
    if ((int)pts.size()<2) return;
    sdl_check(SDL_SetRenderDrawColor(renderer,c.r,c.g,c.b,c.a));
    for (int i=0; i<(int)pts.size()-1; ++i) {
        auto x1 = (int)pts[i].x;
        auto x2 = (int)pts[i+1].x;
        auto y1 = (int)pts[i].y;
        auto y2 = (int)pts[i+1].y;
        sdl_check(SDL_RenderDrawLine(renderer,x1,y1,x2,y2));
    }
}

Vector2f lerp(const Vector2f& a, const Vector2f& b, float t)
{
    return Vector2f(a*(1.0f-t)+b*t);
}

Vector2f bezier_point(std::vector<Vector2f> pts, float t)
{
    if ((int)pts.size()==1) return pts[0];
    std::vector<Vector2f> next_pts((int)pts.size()-1);
    for (int i=0; i<(int)pts.size()-1; ++i) {
        next_pts[i] = lerp(pts[i],pts[i+1],t);
    }
    return bezier_point(next_pts,t);
}

void gen_bezier(const std::vector<Vector2f>& control_points, float inc, std::vector<Vector2f>& out)
{
    if (!out.empty()) out.clear();
    if (control_points.empty()) return;
    for (float t=0.0f; t<=1.0f; t+=inc) {
        out.push_back(bezier_point(control_points,t));
    }
}

int get_point(const std::vector<Vector2f>& control_points, Vector2f mpos) 
{
    for (int i=0; i<(int)control_points.size(); ++i) {
        auto upper_left = control_points[i]-Vector2f(CONTROL_POINT_LEN*0.5f,CONTROL_POINT_LEN*0.5f);
        auto down_right = control_points[i]+Vector2f(CONTROL_POINT_LEN*0.5f,CONTROL_POINT_LEN*0.5f);
        if (mpos.x>=upper_left.x && mpos.x<=down_right.x && mpos.y>=upper_left.y && mpos.y<=down_right.y)
            return i;
    }
    return -1;
}


int main()
{
    // init SDL2 and get timer and input handlers
    sdl_check(SDL_Init(SDL_INIT_VIDEO));
    SDL_Window* window = (SDL_Window*)sdl_check_ptr(SDL_CreateWindow("Bezier-curves",
                                                        SDL_WINDOWPOS_CENTERED,
                                                        SDL_WINDOWPOS_CENTERED,
                                                        SCREEN_WIDTH,
                                                        SCREEN_HEIGHT,
                                                        SDL_WINDOW_SHOWN));
    SDL_Renderer* renderer = (SDL_Renderer*)sdl_check_ptr(SDL_CreateRenderer(window,-1,SDL_RENDERER_ACCELERATED));
    Input_manager* input_manager = Input_manager::instance();
    Timer* timer = Timer::instance();
    bool running = true;

    std::vector<Vector2f> control_points;
    std::vector<Vector2f> curve;
    float inc = 0.001f;
    bool moving = false; // track if we are moving control points
    int mouse_at_cp = -1; // index of control point where mouse cursor is at
    int show_lines = 0;

    while (running) { // main loop
        timer->tick();
        input_manager->update();

        SDL_Event ev;
        while (SDL_PollEvent(&ev)) {
            switch (ev.type) {
                case SDL_QUIT:
                    running = false;
                    break;
            }
        }
        
        while (timer->get_accumulator()>=timer->get_fixed_step()) { 
            // updates go here
           
            if (moving && mouse_at_cp!=-1) {
                control_points[mouse_at_cp] = input_manager->mouse_pos();
                gen_bezier(control_points,inc,curve);
            }
            if (input_manager->mouse_down(Mouse_button::LEFT)) {
                moving = true;
                mouse_at_cp = get_point(control_points,input_manager->mouse_pos());
            }
            if (input_manager->mouse_released(Mouse_button::LEFT)) {
                moving = false;
                mouse_at_cp = -1;
            }

            if (input_manager->mouse_pressed(Mouse_button::MID)) {
                control_points.push_back(input_manager->mouse_pos());
                gen_bezier(control_points,inc,curve);
            }
            if (input_manager->mouse_pressed(Mouse_button::RIGHT)) {
                if (!control_points.empty()) {
                    control_points.pop_back();
                    gen_bezier(control_points,inc,curve);
                }
            }
            if (input_manager->key_pressed(SDL_SCANCODE_D)) {
                show_lines ^= 1;
            }

            timer->update_accumulator();
            input_manager->update_prev_keyboard_state();
        }

        sdl_check(SDL_SetRenderDrawColor(renderer,bg.r,bg.g,bg.b,255));
        sdl_check(SDL_RenderClear(renderer));
        // drawing goes here
        for (const auto& cp:control_points) {
            draw_square(renderer,cp,CONTROL_POINT_LEN,cp_color);
        }
        if (show_lines) {
            draw_lines(renderer,control_points,line_color);
        }
        //for (const auto& p:curve) {
        //    draw_square(renderer,p,POINT_LEN,p_color);
        //}
        draw_lines(renderer,curve,curve_color);

        SDL_RenderPresent(renderer);
    }
    
    Timer::release();
    Input_manager::release();
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
