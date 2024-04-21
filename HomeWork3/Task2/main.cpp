#include <iostream>
#include "OneLinkedList.h"

int main()
{
    OneLinkedList<int> ist;

    ist.push_back(3);
    ist.push_back(4);
    ist.push_back(5);
    ist.push_front(2);
    ist.push_front(1);
    ist.pop_back();
    ist.pop_front();


    for (auto it = ist.begin(); it != ist.end(); ++it)
    {
        std::cout << *it << "->";
    }

    std::cout << "nullptr" << std::endl;

    return 0;
}
