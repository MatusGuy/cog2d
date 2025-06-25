// SPDX-License-Identifier: LGPL-3.0-only

#include "config.hpp"

#include <fstream>

#include "cog2d/program.hpp"

COG2D_NAMESPACE_BEGIN_IMPL

Config::Config()
{
}

void Config::init(const ProgramSettings* prgsettings)
{
	using ValueType = toml::node_type;

	std::filesystem::path cfgpath = get_config_path(prgsettings);

	if (std::filesystem::exists(cfgpath)) {
		std::ifstream cfgfile(cfgpath);
		toml::table config = toml::parse(cfgfile);

#define COG2D_READ_SETTING_CASE(e, t)                                 \
	case e:                                                           \
		(*std::get<t*>(var)) = setting->value_or(*std::get<t*>(var)); \
		break;

		for (auto& [name, var] : m_settings) {
			toml::node* setting = config.get(name);

			if (!setting) {
				// Uhhh.....
				continue;
			}

			switch (setting->type()) {
				COG2D_READ_SETTING_CASE(ValueType::integer, std::int64_t)
				COG2D_READ_SETTING_CASE(ValueType::floating_point, double)
				COG2D_READ_SETTING_CASE(ValueType::string, std::string)
				COG2D_READ_SETTING_CASE(ValueType::boolean, bool)
			default:
				break;
			}
		}
	} else {
		std::filesystem::create_directories(cfgpath.parent_path());
		std::ofstream cfgfile(cfgpath);
		save(cfgfile);
		cfgfile.close();
	}
}

std::filesystem::path Config::get_config_path(const ProgramSettings* prgsettings)
{
	std::filesystem::path prefpath = SDL_GetPrefPath(prgsettings->org_name.c_str(),
	                                                 prgsettings->app_name.c_str());
	return prefpath / "config.toml";
}

void Config::save(std::ofstream& cfgfile)
{
#define COG2D_WRITE_SETTING_CASE(i, t)                 \
	case i:                                            \
		config.at(name).ref<t>() = *std::get<t*>(var); \
		break

	toml::table config;
	for (auto& [name, var] : m_settings) {
		switch (var.index()) {
			COG2D_WRITE_SETTING_CASE(0, std::int64_t);
			COG2D_WRITE_SETTING_CASE(1, double);
			COG2D_WRITE_SETTING_CASE(2, std::string);
			COG2D_WRITE_SETTING_CASE(3, bool);
		}
	}

	cfgfile << config << std::endl;
}

COG2D_NAMESPACE_END_IMPL
