#pragma once

#include <SDL2/SDL.h>

class Timer {
public:
	static Timer* instance();
	static void release();

	void tick();

	static float get_fixed_step() {return fixed_step;}
	float get_accumulator() {return accumulator;}
	float get_delta() {return dt;}
	
	void update_accumulator();
private:
	static Timer* timer;

	// calculate dt between prev and current frame
	unsigned int prev_ticks;
	unsigned int current_ticks;
	float accumulator;
	float dt;

	static float fixed_step;

	Timer();
	~Timer();
};
