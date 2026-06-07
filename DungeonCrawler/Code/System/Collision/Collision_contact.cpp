#include "Collision.h"
#include "../../Misc/MiscFunctions.h"

Contact check_RectVsRect_contact(SDL_FRect& rect1, SDL_FRect& rect2)
{
    // compute overlaps
    float overlapX = std::min(rect1.x + rect1.w, rect2.x + rect2.w) - std::max(rect1.x, rect2.x);
    float overlapY = std::min(rect1.y + rect1.h, rect2.y + rect2.h) - std::max(rect1.y, rect2.y);

    // no collision
    if (overlapX <= 0.f || overlapY <= 0.f)
        return Contact{ {0.f, 0.f}, 0.f, false };

    // pick shallowest axis
    if (overlapX < overlapY)
    {
        // push along X — sign depends on which side A is on
        float sign = (rect1.x + rect1.w * 0.5f) < (rect2.x + rect2.w * 0.5f) ? -1.f : 1.f;
        return Contact{ {sign, 0.f}, overlapX, true };
    }
    else
    {
        // push along Y
        float sign = (rect1.y + rect1.h * 0.5f) < (rect2.y + rect2.h * 0.5f) ? -1.f : 1.f;
        return Contact{ {0.f, sign}, overlapY, true };
    }
}

Contact check_CircVsCirc_contact(
    Vector2D<float>& posA, float radiusA,
    Vector2D<float>& posB, float radiusB)
{
    Vector2D<float> delta = posB - posA;
    float dist2 = delta[0] * delta[0] + delta[1] * delta[1];
    float radSum = radiusA + radiusB;

    // early out — cheaper than sqrt
    if (dist2 >= radSum * radSum)
        return Contact{ {0.f, 0.f}, 0.f, false };

    float dist = std::sqrt(dist2);
    float penetration = radSum - dist;

    // safeNormalize handles the dist==0 case (same position)
    Vector2D<float> normal = safeNormalize(delta);

    return Contact{ normal, penetration, true };
}

Contact check_CircleVsRect_contact(
    Vector2D<float>& circlePos, float radius, SDL_FRect& rect, bool flipPenetration)
{
    float sign = 1.0;
    if (flipPenetration)
    {
        sign = -1.0f;
    }

    // find closest point on rect to circle center
    float closestX = clamp(circlePos[0], rect.x, rect.x + rect.w);
    float closestY = clamp(circlePos[1], rect.y, rect.y + rect.h);

    float dx = circlePos[0] - closestX;
    float dy = circlePos[1] - closestY;
    float dist2 = dx * dx + dy * dy;

    if (dist2 >= radius * radius)
        return Contact{ {0.f, 0.f}, 0.f, false };

    // --- circle center is OUTSIDE the rect ---
    if (dist2 > 0.f)
    {
        
        float dist = std::sqrt(dist2);
        return Contact{ {dx / dist, dy / dist},sign * (radius - dist), true };
    }
    // --- circle center is INSIDE the rect ---
    // find which edge is closest and push out through it
    float dLeft = circlePos[0] - rect.x;
    float dRight = (rect.x + rect.w) - circlePos[0];
    float dTop = circlePos[1] - rect.y;
    float dBottom = (rect.y + rect.h) - circlePos[1];

    float minDist = std::min({ dLeft, dRight, dTop, dBottom });

    if (minDist == dLeft)        
        return Contact{ {-1.f,  0.f}, sign * (radius + dLeft),   true };
    else if (minDist == dRight)  
        return Contact{ { 1.f,  0.f}, sign * (radius + dRight),  true };
    else if (minDist == dTop)    
        return Contact{ { 0.f, -1.f}, sign * (radius + dTop),    true };
    else                         
        return Contact{ { 0.f,  1.f}, sign * (radius + dBottom), true };
}