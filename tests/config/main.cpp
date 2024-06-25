#include <iostream>
#include <program.hpp>
#include <logger.hpp>

class Test : public Program {
public:
	Test(): Program() {
		s_settings.systems = System::SYSTEM_CONFIG;
		s_settings.org_name = "cog2d";
		s_settings.app_name = "config test";
	}

	void init() override {
		COG2D_USE_CONFIG;

		const std::string* value = config.at<std::string>("hello");
		if (value)
			COG2D_LOG_INFO("Config test", *value);
		else
			COG2D_LOG_ERROR("Config test", "value is null!");

		m_keep_running = false;
	}

	void update() override {}

	void draw() override {}

	bool event(SDL_Event* ev) override { return true; }

	bool register_settings() override {
		COG2D_USE_CONFIG;

		config.register_setting<std::string>("hello", "world");

		return true;
	}
};

int main(int argc, char* argv[]) {
	Test test;
	return test.run(argc, argv);
}
