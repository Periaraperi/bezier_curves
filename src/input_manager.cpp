#include "input_manager.hpp"

Input_manager* Input_manager::input_manager = nullptr;

Input_manager* Input_manager::instance()
{
	if(input_manager==nullptr) {
		input_manager = new Input_manager();
	}
	return input_manager;
}

void Input_manager::release()
{
	delete input_manager;
	input_manager = nullptr;
}

Input_manager::Input_manager()
{
	keyboard_state = SDL_GetKeyboardState(&len);
	prev_keyboard_state = new Uint8[len];
	memcpy(prev_keyboard_state,keyboard_state,len);
	mouse_state = SDL_GetMouseState(&mouse_x,&mouse_y);
	prev_mouse_state = mouse_state;
}

Input_manager::~Input_manager()
{
	delete[] prev_keyboard_state;
	prev_keyboard_state = nullptr;
}

void Input_manager::update()
{
	mouse_state = SDL_GetMouseState(&mouse_x,&mouse_y);
}

void Input_manager::update_prev_keyboard_state()
{
	memcpy(prev_keyboard_state,keyboard_state,len);
	prev_mouse_state = mouse_state;
}

Vector2f Input_manager::mouse_pos() 
{
	return Vector2f((float)mouse_x,(float)mouse_y);
}

bool Input_manager::key_down(SDL_Scancode key)
{
	return (keyboard_state[key]!=0);
}

bool Input_manager::key_pressed(SDL_Scancode key)
{
	return (keyboard_state[key]!=0 && prev_keyboard_state[key]==0);
}

bool Input_manager::key_released(SDL_Scancode key)
{
	return (keyboard_state[key]==0 && prev_keyboard_state[key]!=0);
}

bool Input_manager::mouse_down(Mouse_button button)
{
	unsigned int mask = 0;
	switch(button) {
	case Mouse_button::LEFT: 
		mask = SDL_BUTTON_LMASK;
		break;
	case Mouse_button::MID: 
		mask = SDL_BUTTON_MMASK;
		break;
	case Mouse_button::RIGHT: 
		mask = SDL_BUTTON_RMASK;
		break;
	}
	return ((mouse_state&mask)!=0);
}

bool Input_manager::mouse_pressed(Mouse_button button)
{
	unsigned int mask = 0;
	switch(button) {
	case Mouse_button::LEFT: 
		mask = SDL_BUTTON_LMASK;
		break;
	case Mouse_button::MID: 
		mask = SDL_BUTTON_MMASK;
		break;
	case Mouse_button::RIGHT: 
		mask = SDL_BUTTON_RMASK;
		break;
	}
	return ((mouse_state&mask)!=0 && (prev_mouse_state&mask)==0);
}

bool Input_manager::mouse_released(Mouse_button button)
{
	unsigned int mask = 0;
	switch(button) {
	case Mouse_button::LEFT: 
		mask = SDL_BUTTON_LMASK;
		break;
	case Mouse_button::MID: 
		mask = SDL_BUTTON_MMASK;
		break;
	case Mouse_button::RIGHT: 
		mask = SDL_BUTTON_RMASK;
		break;
	}
	return ((mouse_state&mask)==0 && (prev_mouse_state&mask)!=0);
}
