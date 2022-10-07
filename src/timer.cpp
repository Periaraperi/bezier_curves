#include "timer.hpp"

Timer* Timer::timer = nullptr;
float Timer::fixed_step = 1.0f/60.0f;

Timer* Timer::instance()
{
	if(timer==nullptr) {
		timer = new Timer();
	}
	return timer;
}

void Timer::release()
{
	delete timer;
	timer = nullptr;
}

Timer::Timer()
	:prev_ticks{0},current_ticks{0},
	accumulator{0.0f}, dt{0.0f}
{
	prev_ticks = SDL_GetTicks();
}

Timer::~Timer() 
{}

void Timer::tick()
{
	current_ticks = SDL_GetTicks();

	dt = (current_ticks - prev_ticks)*0.001f; // between frames
	accumulator += dt; // for fixed time game loop need to accumulate dt
	prev_ticks = current_ticks;

}

void Timer::update_accumulator()
{
	accumulator -= fixed_step;
}
