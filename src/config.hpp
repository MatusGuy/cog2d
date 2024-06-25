#ifndef CONFIG_HPP
#define CONFIG_HPP

#include <filesystem>
#include <vector>
#include <variant>

#include <nlohmann/json.hpp>

#include "types.hpp"

struct ProgramSettings;

using ConfigValue = std::variant<int*, float*, std::string*, bool*>;

struct ConfigSetting {
	std::string name;
	ConfigValue ptr;
};

#define COG2D_USE_CONFIG COG2D_USING(Config, config)
class Config : public Singleton<Config>
{
public:
	Config();

	void init(const ProgramSettings& prgsettings);

	/// WARNING: your pointers are mine now
	template<typename T>
	void register_setting(const std::string& name, T default_value, T* ptr = nullptr);

	template<typename T>
	void set(const std::string& name, T value);

	template<typename T>
	const T* at(const std::string& name);

	std::filesystem::path get_config_path(const ProgramSettings &prgsettings);
	void save(std::ofstream& cfgfile);

private:
	std::map<std::string, ConfigValue> m_settings;
};

#endif // CONFIG_HPP
