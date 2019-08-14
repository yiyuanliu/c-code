// Write a function that takes an integer as input, and returns the number of bits 
// that are equal to one in the binary representation of that number. 
// You can guarantee that input is non-negative.

// Example: The binary representation of 1234 is 10011010010, 
// so the function should return 5 in this case

#include <stddef.h>

#define VERSION         3

#if VERSION==0
// simple version
size_t countBits(unsigned value) {
        size_t ans = 0;
        while (value) {
                ans += value & 0x01;
                value = value >> 1;
        }

        return ans;
}

#elif VERSION==1 

// () is need because bit because operator precedence
size_t countBits(unsigned value) {
        // assume unsigned is 32bit
        value = (value & 0x55555555) + ((value >> 1) & 0x55555555); // b01...
        value = (value & 0x33333333) + ((value >> 2) & 0x33333333); // b0011...
        value = (value & 0x0F0F0F0F) + ((value >> 4) & 0x0F0F0F0F); // b00001111...
        value = (value & 0x00FF00FF) + ((value >> 8) & 0x00FF00FF);
        value = (value & 0x0000FFFF) + ((value >> 16) & 0x0000FFFF);
        return value;
}

#elif VERSION==2

// more efficient, may use hardware instruction
size_t countBits(unsigned value) {
        return __builtin_popcount(value);
}

#elif VERSION==3
// similar to version 2
// think about three bit num abc, abc = 4a+2b+c, but we need a+b+c
// abc>>1 = 0ab = 2a+b, abc>>2 = a, abc-(abc>>1)-(abc>>2) = (4a+2b+c)-(2a+b)-a=a+b+c
// but we have 32bit, can add 0 at left, 33=11*3, when shift, the most left bit in 3 bits should be cleared
// sum by add and moudlar, this is hard to understand.
// how to expand this program to 64 bit? 4 bit, module 1024, because 63<64
size_t countBits(unsigned value) {
        value = value - ((value>>1) & 033333333333) - ((value>>2) & 011111111111);
        value = ((value + (value>>3)) & 030707070707) % 0111111;

        return value;
}
#endif