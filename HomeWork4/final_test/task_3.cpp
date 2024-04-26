#include <iostream>

int main() {
    int array[10] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    int *ptr = array;

    ptr += 3;

    std::cout << "array[3]: " << *ptr << std::endl;

    return 0;
}
