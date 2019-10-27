#pragma once

#include <string>

std::string encrypt(
	const std::string& modulus,
	const std::string& exponent,
	const std::string& plain);
