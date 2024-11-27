#include <stdio.h>

float integer_to_float(int integer);
int float_to_integer(float floating_point);
char* float_to_binary(float f);
float binary_to_float(const char* bits);

int main(){
    printf("integer_to_float(5) = %f\n", integer_to_float(5));
    printf("float_to_integer(5.6) = %d\n", float_to_integer(5.6));
    printf("float_to_binary(5.6) = %s\n", float_to_binary(5.6));
    printf("binary_to_float(5.6) = %f\n", binary_to_float(float_to_binary(5.6)));
    return 0;
}

float integer_to_float(int integer) {
    return (float)integer;
}
// we dont lose precision when converting from integer to float

int float_to_integer(float floating_point) {
    return (int)floating_point;
}
// we lose precision when converting from float to integer

char* float_to_binary(float f) {
    static char binary[33];
    unsigned int bits = *(unsigned int*)&f;  // Reinterpret the float as an unsigned int
    for (int i = 31; i >= 0; i--) {
        binary[31 - i] = ((bits >> i) & 1) ? '1' : '0';
    }
    binary[32] = '\0';  // Null-terminate the string
    return binary;
}


float binary_to_float(const char* bits) {
    union ufloat {
        float f;
        unsigned int u;
    } u;
    
    u.u = 0;
    for (int i = 0; i < 32; i++) {
        u.u = (u.u << 1) | (bits[i] - '0');
    }
    return u.f;
}