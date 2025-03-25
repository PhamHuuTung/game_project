#include "Collision.h"
#include <SDL_mixer.h>


bool Collision::checkCollision(const SDL_Rect& a, const SDL_Rect& b) {
    // Check if any of the sides from A are outside of B
    if(a.x + a.w <= b.x ||  // A is to the left of B
       a.x >= b.x + b.w ||  // A is to the right of B
       a.y + a.h <= b.y ||  // A is above B
       a.y >= b.y + b.h) {  // A is below B
        return false;
    }

    return true;
}
