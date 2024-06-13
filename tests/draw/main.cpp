#include <iostream>
#include <program.hpp>

class Test : public Program {
public:
	void init() override {}

	void update() override {}

	void draw() override {
		Painter::get().draw_rect({10, 10, 100, 100}, true);
		Painter::get().draw_rect({120, 10, 100, 100}, true, 0xFF0000FF);
		Painter::get().draw_rect({240, 10, 100, 100}, true, 0x00FFFFFF);

		Painter::get().draw_circle({60, 170}, 50, true);

		// Marty
		Painter::get().draw_rect({300, 300, 100, 200}, true, 0xFF0000FF);
		Painter::get().draw_rect({400, 300, 100, 200}, true, 0xFFFFFFFF);
		Painter::get().draw_circle({350, 400}, 25, true, 0xFFFFFFFF);
		Painter::get().draw_circle({450, 400}, 25, true, 0xFF0000FF);
	}

	bool event(SDL_Event* ev) override { return true; }
};

int main(int argc, char* argv[]) {
	Test test;
	return test.run(argc, argv);
}
