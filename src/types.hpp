#ifndef TYPES_H
#define TYPES_H

#include <SDL2/SDL.h>

typedef SDL_FPoint Vector;
typedef SDL_FRect Rect;
struct Size { float w, h; };

class Color : public SDL_Color {
public:
    Color(): SDL_Color {0,0,0,0} {}
    Color(uint8_t r, uint8_t g, uint8_t b, uint8_t a): SDL_Color{r,g,b,a} {}

    Color(uint32_t rgba): SDL_Color{} {
        // wow
        *(uint32_t*) this = rgba;
    }
};

template <typename T>
class Singleton {
public:
    static T& get() {
        static T instance{};
        return instance;
    }

protected:
    Singleton() = default;
    ~Singleton() = default;
    Singleton(const Singleton&) = delete;
    Singleton& operator=(const Singleton&) = delete;
};

#endif // TYPES_H
