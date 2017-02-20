#pragma once

#include <random>

class RandEngine {
private:
	std::minstd_rand re;
	std::uniform_int_distribution<> ui;
	std::uniform_real_distribution<> ur;
	std::normal_distribution<> nr;
public:
	int int_() { return ui(re); }
	double unif() { return ur(re); }
	double norm() { return nr(re); }
};
