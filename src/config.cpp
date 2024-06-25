#include "config.hpp"

#include <fstream>

#include "program.hpp"

Config::Config()
{

}

void Config::init(const ProgramSettings& prgsettings)
{
	using JsonType = nlohmann::detail::value_t;

	std::filesystem::path cfgpath = get_config_path(prgsettings);

	if (std::filesystem::exists(cfgpath)) {
		std::ifstream cfgfile(cfgpath);
		nlohmann::json jsonobj;
		cfgfile >> jsonobj;

#define COG2D_READ_SETTING_CASE(e, t) case e: (*std::get<t*>(setting)) = jsonval; break;
		for (auto& [name, setting] : m_settings) {
			auto jsonval = jsonobj[name];
			switch (jsonval.type()) {
			COG2D_READ_SETTING_CASE(JsonType::number_integer, int)
			COG2D_READ_SETTING_CASE(JsonType::number_float, float)
			COG2D_READ_SETTING_CASE(JsonType::string, std::string)
			COG2D_READ_SETTING_CASE(JsonType::boolean, bool)
			default: break;
			}
		}
	} else {
		std::filesystem::create_directories(cfgpath.parent_path());
		std::ofstream cfgfile(cfgpath);
		save(cfgfile);
		cfgfile.close();
	}
}

std::filesystem::path Config::get_config_path(const ProgramSettings& prgsettings)
{
	std::filesystem::path prefpath = SDL_GetPrefPath(prgsettings.org_name.c_str(), prgsettings.app_name.c_str());
	return prefpath/"config.json";
}

void Config::save(std::ofstream &cfgfile)
{
#define COG2D_WRITE_SETTING_CASE(i, t) case i: jsonobj[name] = *std::get<t*>(setting); break
	nlohmann::json jsonobj;
	for (auto& [name, setting] : m_settings) {
		switch (setting.index()) {
		COG2D_WRITE_SETTING_CASE(0, int);
		COG2D_WRITE_SETTING_CASE(1, float);
		COG2D_WRITE_SETTING_CASE(2, std::string);
		COG2D_WRITE_SETTING_CASE(3, bool);
		}
	}

	cfgfile << std::setw(4) << jsonobj << std::endl;
}

template<typename T>
void Config::register_setting(const std::string &name, T default_value, T *ptr) {
	if (ptr == nullptr) {
		ptr = new T;
	}

	(*ptr) = default_value;

	m_settings[name] = ptr;
}

template<typename T>
void Config::set(const std::string &name, T value) {
	if (m_settings.count(name) == -1) {
		return;
	}

	ConfigValue val = m_settings[name];

	if (!std::holds_alternative<T*>(val)) {
		return;
	}

	T* ptr = std::get<T*>(val);
	(*ptr) = value;
}


template<typename T>
const T* Config::at(const std::string &name)
{
	if (m_settings.count(name) == -1) {
		return nullptr;
	}

	ConfigValue val = m_settings[name];

	if (!std::holds_alternative<T*>(val)) {
		return nullptr;
	}

	T* ptr = std::get<T*>(val);
	return ptr;
}
