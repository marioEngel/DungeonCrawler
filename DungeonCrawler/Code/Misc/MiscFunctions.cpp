#include "MiscFunctions.h"
#include "../ECS/Coordinator.h"
#include "../Component/Comp_Position.h"
#include "../Component/Comp_Texture.h"

extern Coordinator gCoordinator;

void printSDLRect(SDL_Rect rect, const char* text, Entity ent)
{

	std::cout << "Entity: " << int(ent) << '\n';
	
	std::cout << "\033[1;32m" << "SDL Rect: " << "\033[0m" << text << '\n'
		<< "\033[1;32m" << "SDL Rect x: " << "\033[0m" << rect.x << '\n'
		<< "\033[1;32m" << "SDL Rect y: " << "\033[0m" << rect.y << '\n'
		<< "\033[1;32m" << "SDL Rect w: " << "\033[0m" << rect.w << '\n'
		<< "\033[1;32m" << "SDL Rect h: " << "\033[0m" << rect.h << '\n' << '\n';
}

SDL_Rect rtnScreenRect()
{
	return SDL_Rect{ 0, 0, Game::screenWidth, Game::screenHeight };
}

Vector2D<float> rtnCenter(Position& position, Texture& texture)
{
	return position.pos + Vector2D<float>{float(texture.width) / 2.0f, float(texture.height) / 2.0f};
}

Vector2D<float> rtnCenter(Position& position, TextureLight& texture)
{
	return position.pos + Vector2D<float>{float(texture.width) / 2.0f, float(texture.height) / 2.0f};
}

Vector2D<float> rtnCenterEntity(Entity entity)
{
	auto position = gCoordinator.GetComponent<Position>(entity);
	auto textur = gCoordinator.GetComponent<Texture>(entity);

	return position.pos + Vector2D<float>{
		float(textur.width) / 2.0f * float(textur.scale), 
		float(textur.height) / 2.0f * float(textur.scale)
	};
}

void printColor()
{
	std::cout
		<< "text color \n"  
		<< "\033[30m\033[47m" << "Color Test text what color ist that" << "\033[0m" << '\n'		// black
		<< "\033[1;30m" << "Color Test text what color ist that" << "\033[0m" << '\n'			// grey
		<< "\033[31m" << "Color Test text what color ist that" << "\033[0m" << '\n'				// red
		<< "\033[1;31m" << "Color Test text what color ist that" << "\033[0m" << '\n'			// light red
		<< "\033[32m" << "Color Test text what color ist that" << "\033[0m" << '\n'				// green
		<< "\033[1;32m" << "Color Test text what color ist that" << "\033[0m" << '\n'			// light green
		<< "\033[33m" << "Color Test text what color ist that" << "\033[0m" << '\n'				// orange
		<< "\033[1;33m" << "Color Test text what color ist that" << "\033[0m" << '\n'			// yellow
		<< "\033[34m" << "Color Test text what color ist that" << "\033[0m" << '\n'				// dark blue
		<< "\033[1;34m" << "Color Test text what color ist that" << "\033[0m" << '\n'			// blue
		<< "\033[35m" << "Color Test text what color ist that" << "\033[0m" << '\n'				// purple 
		<< "\033[1;35m" << "Color Test text what color ist that" << "\033[0m" << '\n'			// pink
		<< "\033[36m" << "Color Test text what color ist that" << "\033[0m" << '\n'				// light blue
		<< "\033[1;36m" << "Color Test text what color ist that" << "\033[0m" << '\n'			// cyan
		<< "\033[37m" << "Color Test text what color ist that" << "\033[0m" << '\n'				// light grey
		<< "\033[1;37m" << "Color Test text what color ist that" << "\033[0m" << '\n';			// white

	std::cout
		<< "background color \n"
		<< "\033[40m" << "Color Test text what color ist that" << "\033[0m" << '\n'		// black
		<< "\033[1;40m" << "Color Test text what color ist that" << "\033[0m" << '\n'			// grey
		<< "\033[41m" << "Color Test text what color ist that" << "\033[0m" << '\n'				// red
		<< "\033[1;41m" << "Color Test text what color ist that" << "\033[0m" << '\n'			// light red
		<< "\033[42m" << "Color Test text what color ist that" << "\033[0m" << '\n'				// green
		<< "\033[1;42m" << "Color Test text what color ist that" << "\033[0m" << '\n'			// light green
		<< "\033[43m" << "Color Test text what color ist that" << "\033[0m" << '\n'				// orange
		<< "\033[1;43m" << "Color Test text what color ist that" << "\033[0m" << '\n'			// yellow
		<< "\033[44m" << "Color Test text what color ist that" << "\033[0m" << '\n'				// dark blue
		<< "\033[1;44m" << "Color Test text what color ist that" << "\033[0m" << '\n'			// blue
		<< "\033[45m" << "Color Test text what color ist that" << "\033[0m" << '\n'				// purple 
		<< "\033[1;45m" << "Color Test text what color ist that" << "\033[0m" << '\n'			// pink
		<< "\033[46m" << "Color Test text what color ist that" << "\033[0m" << '\n'				// light blue
		<< "\033[1;46m" << "Color Test text what color ist that" << "\033[0m" << '\n'			// cyan
		<< "\033[47m" << "Color Test text what color ist that" << "\033[0m" << '\n'				// light grey
		<< "\033[1;47m" << "Color Test text what color ist that" << "\033[0m" << '\n';			// white
}
