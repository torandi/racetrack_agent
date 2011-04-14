#include "config.h"
#include <map>
#include <string>
#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <exception>
#include <fstream>
#include <istream>

bool Config::parse() {
	std::ifstream f(_file.c_str());
	if(f.is_open()) {
		std::string attr,value;
		while(!f.eof()) {
			getline(f,attr,'=');
			getline(f,value);
			if(attr.length()>0) {
				_attr[attr]=value;
			}
		}
		return true;
	} else {
		return false;
	}
}

Config::Config() { }

Config::Config(std::string file) : _file(file) {
	_attr=std::map<std::string, std::string>();
	parse();	
}

bool Config::open(std::string file) {
	_file=file;
	return parse();
}

std::string &Config::operator[](const std::string &attr) {
	std::map<std::string, std::string>::iterator it = _attr.find(attr);
	if(it!=_attr.end()) {
		return it->second;
	} else {
		std::fprintf(stderr,"Can't find attribute %s\n",attr.c_str());
		throw std::exception();
	}
}

int Config::get_int(const std::string &attr) {
	std::string content=(*this)[attr];
	return atoi(content.c_str());
}

float Config::get_float(const std::string &attr) {
	std::string content=(*this)[attr];
	return atof(content.c_str());
}
