#ifndef __FPENCODER_H__
#define __FPENCODER_H__

#include <cstdint>
#include <vector>
#include <cfloat>

class FPEncoder {
public:
    FPEncoder(double min = -DBL_MAX, double max = DBL_MAX, int length = 8);
    static FPEncoder WithPrecision(double min, double max, double precision);

    std::vector<uint8_t> Encode(double val) const;
    double Decode(const std::vector<uint8_t>& bytes) const;

private:
    // Internal parameters
    double a{}, b{};
    double bPow{}, dPow{};
    double sF{}, sR{};
    double zOffset{};
    int fieldLength{};

    static double log2(double x);

    void preCompute(double min, double max, int length);

    // Big-endian helpers
    static void writeBE16(std::vector<uint8_t>& v, uint16_t x);
    static void writeBE32(std::vector<uint8_t>& v, uint32_t x);
    static void writeBE64(std::vector<uint8_t>& v, uint64_t x);

    static uint16_t readBE16(const std::vector<uint8_t>& v);
    static uint32_t readBE32(const std::vector<uint8_t>& v);
    static uint64_t readBE64(const std::vector<uint8_t>& v);
};

#endif // __FPENCODER_H__
