#include "fpencoder.h"

#include <cmath>
#include <stdexcept>

#include <iostream>

FPEncoder::FPEncoder(double min, double max, int length) {
	std::cout<<"FPEncoder("<<min<<", "<<max<<", "<<length<<")"<<std::endl;
    if (length != 1 && length != 2 && length != 4 && length != 8) {
		std::cerr<<"FPEncoder("<<min<<", "<<max<<", "<<length<<") -> Only 1, 2, 4, and 8 are valid field lengths"<<std::endl;
        throw std::invalid_argument("Only 1, 2, 4, and 8 are valid field lengths");
	}
    preCompute(min, max, length);
}

FPEncoder FPEncoder::WithPrecision(double min, double max, double precision) {
    double bits = std::ceil(log2((max - min) / precision) + 1);
    double length = std::ceil(bits / 8.0);

    if (length <= 2) return FPEncoder(min, max, (int)length);
    if (length <= 4) return FPEncoder(min, max, 4);
    if (length <= 8) return FPEncoder(min, max, 8);

    throw std::invalid_argument("Range and precision cannot be represented in 64-bit");
}

std::vector<uint8_t> FPEncoder::Encode(double val) const {
    std::vector<uint8_t> encoded(fieldLength, 0);

    if (std::isinf(val) && val > 0) {
        encoded[0] = 0xC8;
        return encoded;
    }
    if (std::isinf(val) && val < 0) {
        encoded[0] = 0xE8;
        return encoded;
    }
    if (std::isnan(val)) {
        encoded[0] = 0xD0;
        return encoded;
    }
    if (val < a || val > b)
        throw std::range_error("Value must be in range");

    double d = std::floor(sF * (val - a) + zOffset);

    switch (fieldLength) {
    case 1:
        encoded[0] = static_cast<uint8_t>(d);
        break;
    case 2:
        writeBE16(encoded, (uint16_t)d);
        break;
    case 4:
        writeBE32(encoded, (uint32_t)d);
        break;
    case 8:
        writeBE64(encoded, (uint64_t)d);
        break;
    }

    return encoded;
}

double FPEncoder::Decode(const std::vector<uint8_t>& bytes) const {
    if ((int)bytes.size() != fieldLength)
        throw std::invalid_argument("Array length mismatch");

    if (bytes[0] == 0xC8) return INFINITY;
    if (bytes[0] == 0xE8) return -INFINITY;
    if (bytes[0] == 0xD0) return NAN;

    double l = 0;

    switch (fieldLength) {
    case 1:
        l = (double)bytes[0];
        break;
    case 2:
        l = (double)readBE16(bytes);
        break;
    case 4:
        l = (double)readBE32(bytes);
        break;
    case 8:
        l = (double)readBE64(bytes);
        break;
    }

    double val = sR * (l - zOffset) + a;

    if (val < a || val > b)
        throw std::runtime_error("Decoded value out of range");

    return val;
}

double FPEncoder::log2(double x) {
    return std::log(x) / std::log(2.0);
}

void FPEncoder::preCompute(double min, double max, int length) {
	std::cout<<"preCompute("<<min<<", "<<max<<", "<<length<<")"<<std::endl;
    fieldLength = length;
    a = min;
    b = max;
    bPow = std::ceil(log2(b - a));
    dPow = (double)(8 * fieldLength - 1);
    sF = std::pow(2, dPow - bPow);
    sR = std::pow(2, bPow - dPow);

    if (a < 0 && b > 0)
        zOffset = sF * a - std::floor(sF * a);
}

//
// Big endian helpers
//

void FPEncoder::writeBE16(std::vector<uint8_t>& v, uint16_t x) {
    v[0] = (x >> 8) & 0xFF;
    v[1] = x & 0xFF;
}

void FPEncoder::writeBE32(std::vector<uint8_t>& v, uint32_t x) {
    v[0] = (x >> 24) & 0xFF;
    v[1] = (x >> 16) & 0xFF;
    v[2] = (x >> 8) & 0xFF;
    v[3] = x & 0xFF;
}

void FPEncoder::writeBE64(std::vector<uint8_t>& v, uint64_t x) {
    for (int i = 0; i < 8; i++)
        v[i] = (x >> (56 - 8 * i)) & 0xFF;
}

uint16_t FPEncoder::readBE16(const std::vector<uint8_t>& v) {
    return (uint16_t(v[0]) << 8) | uint16_t(v[1]);
}

uint32_t FPEncoder::readBE32(const std::vector<uint8_t>& v) {
    return (uint32_t(v[0]) << 24) | (uint32_t(v[1]) << 16)
         | (uint32_t(v[2]) << 8)  | uint32_t(v[3]);
}

uint64_t FPEncoder::readBE64(const std::vector<uint8_t>& v) {
    uint64_t x = 0;
    for (int i = 0; i < 8; i++)
        x = (x << 8) | v[i];
    return x;
}
