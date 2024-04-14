#include <iostream>

void invertSelectedBits(void* data, size_t amountBytes, void* masks) {
    size_t wideCount = amountBytes / sizeof(uint64_t);
    uint64_t* dataWide = static_cast<uint64_t*>(data);
    uint64_t* masksWide = static_cast<uint64_t*>(masks);

    for (size_t i = 0; i < wideCount; ++i) {
        dataWide[i] ^= masksWide[i];
    }

    size_t shortCount = amountBytes % sizeof(uint64_t);
    unsigned char* byteData = reinterpret_cast<unsigned char*>(dataWide + wideCount);
    unsigned char* maskShort = reinterpret_cast<unsigned char*>(masksWide + wideCount);
    for (size_t i = 0; i < shortCount; ++i) {
        byteData[i] ^= maskShort[i];
    }
}


void printBinary(unsigned char value) {
    for (int i = 7; i >= 0; --i) {
        std::cout << ((value >> i) & 1);
    }
    std::cout << std::endl;
}

int main() {


    unsigned char data[] = {0b11111111, 0b00000000, 0b11110000, 0b00001111, 0b10101010, 0b01010101, 0b11111111, 0b11111111, 0b11111111};
    size_t numBytes = sizeof(data) / sizeof(data[0]);

    std::cout << "Before invert: " << std::endl;
    for (size_t i = 0; i < numBytes; ++i) {
        printBinary(data[i]);
    }

    std::cout << std::endl;

    unsigned char masks[] = {0b00000001, 0b00000010, 0b00000100, 0b00001000, 0b00010000, 0b00100000, 0b01000000, 0b10000000, 0b10101010};
    invertSelectedBits(data, numBytes, masks);

    std::cout << "After invert: " << std::endl;
    for (size_t i = 0; i < numBytes; ++i) {
        printBinary(data[i]);
    }
    std::cout << std::endl;

    int x = 255;
    char mask = 0b00000001;
    invertSelectedBits(&x, 1, &mask);
    std::cout << x << std::endl;

    return 0;
}
