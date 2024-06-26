#include <iostream>
#include <program.hpp>

class Test : public Program {
public:
	Texture* m_logo = nullptr;

public:
	Test(): Program() {
		m_settings->systems ^= System::SYSTEM_CONFIG;
	}

	void init() override {
		// FIXME
		m_logo = AssetManager::get().load_image("assets/images/cog2d-512x512.png");
		//if (m_logo == nullptr) {
		//	return 1;
		//}
	}

	void update() override {}

	void draw() override {
		Painter::get().draw_texture({50,50, m_logo->get_size().x, m_logo->get_size().y}, m_logo);
	}

	bool event(SDL_Event* ev) override { return true; }
};

int main(int argc, char* argv[]) {
	Test test;
	return test.run(argc, argv);
}
