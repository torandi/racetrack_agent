#ifndef CONFIG_H_
#define CONFIG_H_

#include <map>
#include <string>

class Config {
	std::map<std::string, std::string> _attr;
	std::string _file;

	bool parse();
	public:
		Config();
		Config(std::string file);
		bool open(std::string file);

		std::string &operator[](const std::string &attr);

		int get_int(const std::string &attr);
		float get_float(const std::string &attr);

		
};

#endif
