#pragma once
#include <random>
#include <functional>
#include <vector>


namespace details {
	static std::random_device s_randomDevice;
	static std::mt19937 _mtGenerator(s_randomDevice());
	static std::uniform_real_distribution<> getRandom01(0, 1);
}