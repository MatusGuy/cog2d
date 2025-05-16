#ifndef CONFIG_HPP
#define CONFIG_HPP

#include <filesystem>
#include <vector>
#include <variant>

#include <nlohmann/json.hpp>

#include "cog2d/util/currenton.hpp"

COG2D_NAMESPACE_BEGIN_DECL

struct ProgramSettings;

using ConfigValue = std::variant<int*, float*, std::string*, bool*>;

struct ConfigSetting
{
	std::string name;
	ConfigValue ptr;
};

#define COG2D_USE_CONFIG COG2D_USING(Config, config)
class Config : public Currenton<Config>
{
public:
	Config();

	void init(const ProgramSettings* prgsettings);

	/// WARNING: your pointers are mine now
	template<typename T>
	void register_setting(const std::string& name, T default_value, T* ptr = nullptr)
	{
		if (ptr == nullptr) {
			ptr = new T;
		}

		(*ptr) = default_value;

		m_settings[name] = ptr;
	}

	template<typename T>
	void set(const std::string& name, T value)
	{
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
	const T* at(const std::string& name)
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

	std::filesystem::path get_config_path(const ProgramSettings* prgsettings);
	void save(std::ofstream& cfgfile);

private:
	std::map<std::string, ConfigValue> m_settings;
};

COG2D_NAMESPACE_END_DECL

#endif  // CONFIG_HPP
