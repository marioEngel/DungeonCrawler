#pragma once
#include "../Math/Vector2D.h"
#include <SDL3/SDL.h>
#include "../Game.h"
#include "../Component/Comp_Texture.h"
#include "../Component/Comp_TextureLight.h"
#include "../Component/Comp_Position.h"
#include "../System/Collision/Geometry/Rectangle.h"

void printColor();
void printSDLRect(SDL_Rect rect, const char* text, Entity ent = 0);

SDL_Rect rtnScreenRect();

Vector2D<float> rtnCenter(Position& position, Texture& texture);
Vector2D<float> rtnCenter(Position& position, TextureLight& texture);

Vector2D<float> rtnCenterEntity(Entity entity);

template <typename Func, typename Tuple, std::size_t... I>
auto apply_helper(Func&& f, Tuple&& t, std::index_sequence<I...>) {
	return f(std::get<I>(std::forward<Tuple>(t))...);
}

template <typename Func, typename Tuple>
auto my_apply(Func&& f, Tuple&& t) {
	constexpr std::size_t size = std::tuple_size<typename std::decay<Tuple>::type>::value;
	return apply_helper(std::forward<Func>(f), std::forward<Tuple>(t), std::make_index_sequence<size>{});
}