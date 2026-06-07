#include "Collision.h"

SDL_FRect create_Rect(Vector2D<float>& cornerPoint, std::array<float, 2>& remainderData)
{
	float tmpX = cornerPoint[0];
	float tmpY = cornerPoint[1];
	float tmpW = remainderData[0];
	float tmpH = remainderData[0];
	return SDL_FRect{ tmpX, tmpY, tmpW, tmpH };
}

void print_Rect_data(SDL_FRect& rect)
{
	std::cout << "rectangle data: x:" << rect.x << " y:" << rect.y << " h:" << rect.h << " w:" << rect.w << std::endl;
	std::cout << "middle point: " << calc_Rect_center(rect) << std::endl;
}

Vector2D<float> calc_Rect_center(SDL_FRect& rect)
{
	return Vector2D<float> {rect.x + rect.w / 2.0f, rect.y + rect.h / 2.0f };
}

Vector2D<float> safeNormalize(Vector2D<float> vec, Vector2D<float> fallback) {
	float length = vec[0] * vec[0] + vec[1] * vec[1];
	if (length < 1e-10f) {
		return fallback;
	}
	float scale = 1.f / std::sqrt(length);
	return { vec[0] * scale, vec[1] * scale };
}

//======================================
// why doesnt sdl have this?
bool compareRect(const SDL_Rect& first, const SDL_Rect& second)
{
	return first.x == second.x && first.y == second.y
		&& first.h == second.h && first.w == second.w;
}
