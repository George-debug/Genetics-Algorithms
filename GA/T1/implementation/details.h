#pragma once
#include <random>
#include <vector>
#include <functional>
#include <memory>

namespace details {
	static constexpr char charEveryBitOn[8] = { 1, 2, 4, 8, 16, 32, 64, -128 };
	static std::random_device s_randomDevice;

	template<typename UIntType, typename DomainType>
	using UIntConverter = std::function<DomainType(UIntType)>;

	template<typename UIntType>
	std::vector<UIntType> generateEachBit() {
		std::cout << "generateEachBit" << std::endl;
		size_t byteSize = sizeof(UIntType);
		std::vector<UIntType> everyBitOn(8 * byteSize);
		char* arr = new char[byteSize];

		for (size_t i = 0; i < byteSize; ++i)
			arr[i] = 0;

		for (size_t i = 0; i < byteSize; ++i) {
			for (size_t j = 0; j < 8; ++j) {
				arr[i] = charEveryBitOn[j];
				memcpy(&everyBitOn[i * 8 + j], arr, byteSize);
			}
			arr[i] = 0;
		}

		delete[](arr);
		return everyBitOn;
	}
}