#include <iostream>
#include <program.hpp>

class Test : public Program {
public:
	Font* m_font;
	Texture* m_text;

public:

	Test(): Program() {
		m_settings->systems ^= System::SYSTEM_CONFIG;
	}

	void init() override {
		COG2D_USE_GRAPHICSENGINE;
		COG2D_USE_ASSETMANAGER;

		m_font = assetmanager.load_font("assets/fonts/DejaVuSans.ttf", 10);
		m_text = graphicsengine.create_text(m_font, "Hello World!");
	}

	void update() override {}

	void draw() override {
		COG2D_USE_GRAPHICSENGINE;

		graphicsengine.draw_texture({{0,0}, {-1,-1}}, m_text);
	}

	bool event(SDL_Event* ev) override { return true; }
};

int main(int argc, char* argv[]) {
	Test test;
	return test.run(argc, argv);
}
