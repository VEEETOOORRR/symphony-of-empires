#ifndef NATION_H
#define NATION_H

#include <stdint.h>
#include <string>
#include "texture.hpp"

class Nation {
public:
	Nation() {};
	Nation& operator=(const Nation&) = default;

	std::string name;
	std::string ref_name;
	uint32_t color;
	Texture default_flag;
};

#endif
