//
// Created by 21553 on 2023/6/22.
//

#ifndef RW_CPPS_UTILS_H
#define RW_CPPS_UTILS_H
#include <random>
#include <cinttypes>
#include <cstring>
#include <string>
#include <chrono>
#include "PicoSHA2.h"
#include "iostream"


#if defined(_WIN32)
#include <objbase.h>
#elif defined(__linux__)
#include <uuid/uuid.h>
#else
#error "uuid unsupport platform"
#endif

#define GUID_LEN 64

#include <memory>
using namespace std;

class RandomUtils {
public:
    RandomUtils() = default;
    ~RandomUtils() = default;
    //梅森旋转算法
    int32_t randomInt32(int32_t origin,int32_t bound);

    string randomBigLetter(size_t _n);

private:
    shared_ptr<mt19937 > randomEngine = make_shared<mt19937>(std::chrono::system_clock::now().time_since_epoch().count());
};


class StringUtils {
public:
    static bool startsWith(string *str, const char *c_str);

    static bool isNumeric(const char* str);
    static bool isNumeric(const string& str);

private:
    //保证至少有一个数
    static bool scanUnsignedInteger(const char **str);

    //保证+或-加上至少有一个数
    static bool scanInteger(const char **str);
};


class UUIDUtils
{
public:
    static string GenerateUUID();
};



/* Parameters of MD5. */
#define s11 7
#define s12 12
#define s13 17
#define s14 22

#define s21 5
#define s22 9
#define s23 14
#define s24 20

#define s31 4
#define s32 11
#define s33 16
#define s34 23

#define s41 6
#define s42 10
#define s43 15
#define s44 21

#define F(x, y, z) (((x) & (y)) | ((~(x)) & (z)))
#define G(x, y, z) (((x) & (z)) | ((y) & (~(z))))
#define H(x, y, z) ((x) ^ (y) ^ (z))
#define I(x, y, z) ((y) ^ ((x) | (~(z))))

#define ROTATELEFT(num, n) (((num) << (n)) | ((num) >> (32-(n))))

#define FF(a, b, c, d, x, s, ac) { \
  (a) += F ((b), (c), (d)) + (x) + (ac); \
  (a) = ROTATELEFT ((a), (s)); \
  (a) += (b); \
}
#define GG(a, b, c, d, x, s, ac) { \
  (a) += G ((b), (c), (d)) + (x) + (ac); \
  (a) = ROTATELEFT ((a), (s)); \
  (a) += (b); \
}
#define HH(a, b, c, d, x, s, ac) { \
  (a) += H ((b), (c), (d)) + (x) + (ac); \
  (a) = ROTATELEFT ((a), (s)); \
  (a) += (b); \
}
#define II(a, b, c, d, x, s, ac) { \
  (a) += I ((b), (c), (d)) + (x) + (ac); \
  (a) = ROTATELEFT ((a), (s)); \
  (a) += (b); \
}


using std::string;

/* Define of btye.*/
typedef unsigned char mdbyte;
/* Define of byte. */
typedef unsigned int bit32;


class MD5{
public:
public:
    /* Construct a MD5 object with a string. */
    explicit MD5(const string& message);

    /* Generate md5 digest. */
    const mdbyte* getDigest();

    /* Convert digest to string value */
    string toStr();

private:
    /* Initialization the md5 object, processing another message block,
     * and updating the context.*/
    void init(const mdbyte* input, size_t len);

    /* MD5 basic transformation. Transforms state based on block. */
    void transform(const mdbyte block[64]);

    /* Encodes input (usigned long) into output (byte). */
    void encode(const bit32* input, mdbyte* output, size_t length);

    /* Decodes input (byte) into output (usigned long). */
    void decode(const mdbyte* input, bit32* output, size_t length);

private:
    /* Flag for mark whether calculate finished. */
    bool finished;

    /* state (ABCD). */
    bit32 state[4]{};

    /* number of bits, low-order word first. */
    bit32 count[2]{};

    /* input buffer. */
    mdbyte buffer[64]{};

    /* message digest. */
    mdbyte digest[16]{};

    /* padding for calculate. */
    static const mdbyte PADDING[64];

    /* Hex numbers. */
    static const char HEX_NUMBERS[16];
};

#endif //RW_CPPS_UTILS_H
