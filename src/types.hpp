#ifndef TYPES_H
#define TYPES_H

#include <cstdint>
#include <type_traits>
#include <fmt/format.h>
#include <memory>

#define COG2D_CONTAINER(container, t, tname) \
	using tname = container<t>; \
	container<t>

#define COG2D_CONTAINER_PLURAL(container, t) COG2D_CONTAINER(container, t, t##s)

#define COG2D_NUMERIC_TEMPLATE \
	template< \
		typename T = float, \
		typename = typename std::enable_if<std::is_arithmetic<T>::value, T>::type \
	>

#if __has_include(<SDL2/SDL.h>)
#include <SDL2/SDL.h>
#else
struct SDL_Color { uint8_t r, g, b, a; };
struct SDL_Point { int x, y; };
struct SDL_FPoint { float x, y; };
struct SDL_Rect { int x, y, w, h; };
struct SDL_FRect { float x, y, w, h; };
#endif

class Color : public SDL_Color {
public:
	Color(): SDL_Color {0,0,0,0} {}
	Color(uint8_t r, uint8_t g, uint8_t b, uint8_t a): SDL_Color{r,g,b,a} {}

	/**
	 * @param abgr The color in \bold abgr format (e.g. 0xFF0000FF for red)
	 */
	Color(uint32_t abgr): SDL_Color{} {
		// wow
		*(uint32_t*) this = abgr;
	}

	inline bool operator==(const Color& other) {
		// TODO: Big endian
		return r == other.r &&
			   g == other.g &&
			   b == other.b &&
			   a == other.a;
	}
};

#define COG2D_USING(c, n) c& n = c::get()

/**
 *   A 'Currenton' allows access to the currently active instance of a
 *   class via the static current() function. It is kind of like a
 *   singleton, but without handling the object construction itself or
 *   in other words its a glorified global variable that points to the
 *   current instance of a class.
 */
template<class C>
class Currenton
{
public:
	static Currenton<C>* s_current;

protected:
	virtual ~Currenton() {
		if (s_current == this)
		{
			s_current = nullptr;
		}
	}

public:
	static C& get() { return *static_cast<C*>(s_current); }
};

template<class C>
Currenton<C>* Currenton<C>::s_current = nullptr;

#define COG2D_SINGLETON(CLASS) \
public: \
	static std::unique_ptr<CLASS> s_instance; \
	\
public: \
	static CLASS& get() { \
		return *CLASS::s_instance; \
	} \




/*
	CLASS(const CLASS&) = delete; \
	CLASS& operator=(const CLASS&) = delete; \
	CLASS(CLASS&&) = delete; \
	CLASS& operator=(CLASS&&) = delete;
	*/
/*
// TODO: Delete me!
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
*/

COG2D_NUMERIC_TEMPLATE
class Vector_t {
public:
	T x = 0;
	T y = 0;

	Vector_t(T _x, T _y)
	{
		x = _x;
		y = _y;
	}

	Vector_t() {}

	inline Vector_t<T> abs() { return { std::abs(x), std::abs(y) }; }

	/// Average between two vectors
	inline Vector_t<T> avg(const Vector_t<T>& other) { return { (x + other.x) / 2, (y + other.y) / 2 }; }

	void operator=(const Vector_t<T>& other) {
		x = other.x;
		y = other.y;
	}

	inline SDL_Point to_sdl_point() { return {static_cast<int>(x), static_cast<int>(y)}; }
	inline SDL_FPoint to_sdl_fpoint() { return {static_cast<float>(x), static_cast<float>(y)}; }

	explicit operator Vector_t<float>() const { return {static_cast<float>(x), static_cast<float>(y)}; }

	Vector_t<T> operator+(Vector_t<T>& other) {
		return Vector_t<T>(x + other.x, y + other.y);
	}

	void operator+=(Vector_t<T>& other) {
		x += other.x;
		y += other.y;
	}

	Vector_t operator-(Vector_t<T>& other) {
		return Vector_t<T>(x - other.x, y - other.y);
	}

	void operator-=(Vector_t<T>& other) {
		x -= other.x;
		y -= other.y;
	}

	Vector_t<T> operator*(Vector_t<T>& other) {
		return Vector_t(x * other.x, y * other.y);
	}

	Vector_t<T> operator*(T& other) {
		return Vector_t(x * other, y * other);
	}

	void operator*=(Vector_t<T>& other) {
		x *= other.x;
		y *= other.y;
	}

	void operator*=(T& other) {
		x *= other;
		y *= other;
	}

	Vector_t<T> operator/(Vector_t<T>& other) {
		return Vector_t(x / other.x, y / other.y);
	}

	Vector_t<T> operator/(T& other) {
		return Vector_t(x / other, y / other);
	}

	void operator/=(Vector_t<T>& other) {
		x /= other.x;
		y /= other.y;
	}

	void operator/=(T& other) {
		x /= other;
		y /= other;
	}

	// It makes sense, trust me
	bool operator<(Vector_t<T>& other) {
		return x < other.x && y < other.y;
	}

};
using Vector = Vector_t<>;

// FIXME: This will cause bugs! I don't know how any
// of this wizardry works! I just want my std::format to
// workie.
template<typename T>
struct fmt::formatter<Vector_t<T>, char>
{
	constexpr auto parse(auto& ctx)
	{
		auto it = ctx.begin();
		if (it == ctx.end())
			return it;

		if (it != ctx.end() && *it != '}')
			throw fmt::format_error("Invalid format args for Vector.");

		return it;
	}

	auto format(Vector_t<T> vec, auto& ctx) const
	{
		return fmt::format_to(ctx.out(), "({}, {})", vec.x, vec.y);
	}
};

COG2D_NUMERIC_TEMPLATE
class Rect_t {
public:
	Vector_t<T> pos;
	Vector_t<T> size;

	Rect_t(T x, T y, T w, T h)
	{
		pos = {x, y};
		size = {w, h};
	}

	Rect_t(const Vector_t<T>& _pos, const Vector_t<T>& _size)
	{
		pos = _pos;
		size = _size;
	}

	Rect_t() {}

	void operator=(const Rect_t<T>& other) {
		pos = other.pos;
		size = other.size;
	}

	inline SDL_Rect to_sdl_rect() {
		return {static_cast<int>(pos.x), static_cast<int>(pos.y), static_cast<int>(size.x), static_cast<int>(size.y)};
	}

	inline SDL_FRect to_sdl_frect() {
		return {static_cast<float>(pos.x), static_cast<float>(pos.y), static_cast<float>(size.x), static_cast<float>(size.y)};
	}

	Rect_t<T> operator+(Rect_t<T>& other) {
		return Rect_t<T>(pos + other.pos, size + other.size);
	}

	void operator+=(Rect_t<T>& other) {
		pos += other.pos;
		size += other.size;
	}

	Rect_t<T> operator-(Rect_t<T>& other) {
		return Rect_t<T>(pos - other.pos, size - other.size);
	}

	void operator-=(Rect_t<T>& other) {
		pos -= other.pos;
		size -= other.size;
	}

	Rect_t<T> operator*(Rect_t<T>& other) {
		return Rect_t<T>(pos * other.pos, size * other.size);
	}

	Rect_t<T> operator*(T& other) {
		return Rect_t<T>(pos * other, size * other);
	}

	void operator*=(Rect_t<T>& other) {
		pos *= other.pos;
		size *= other.size;
	}

	void operator*=(T& other) {
		pos *= other;
		size *= other;
	}

	Rect_t<T> operator/(Rect_t<T>& other) {
		return Rect_t<T>(pos / other.pos, size / other.size);
	}

	Rect_t<T> operator/(T& other) {
		return Rect_t<T>(pos / other, size / other);
	}

	void operator/=(Rect_t<T>& other) {
		pos /= other.pos;
		size /= other.size;
	}

	void operator/=(T& other) {
		pos /= other;
		size /= other;
	}

	inline T get_left() const { return pos.x; }
	inline T get_top() const { return pos.y; }
	inline T get_right() const { return pos.x + size.x; }
	inline T get_bottom() const { return pos.y + size.y; }

	bool overlaps(const Rect_t<T>& other) const
	{
		if (get_right() < other.get_left() || get_left() > other.get_right())
			return false;
		if (get_bottom() < other.get_top() || get_top() > other.get_bottom())
			return false;

		return true;
	}

	bool overlaps_exc(const Rect_t<T>& other) const
	{
		if (get_right() <= other.get_left() || get_left() >= other.get_right())
			return false;
		if (get_bottom() <= other.get_top() || get_top() >= other.get_bottom())
			return false;

		return true;
	}
};
using Rect = Rect_t<>;

template<typename T>
struct fmt::formatter<Rect_t<T>, char>
{
	constexpr auto parse(auto& ctx)
	{
		auto it = ctx.begin();
		if (it == ctx.end())
			return it;

		if (it != ctx.end() && *it != '}')
			throw fmt::format_error("Invalid format args for Rect.");

		return it;
	}

	auto format(Rect_t<T> rec, auto& ctx) const
	{
		return fmt::format_to(ctx.out(), "({}, {})", rec.pos, rec.size);
	}
};

#endif // TYPES_H
