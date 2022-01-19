#pragma once
#include <gtest/gtest.h>

int add(int a, int b)
{
    return a + b;
}

int Factorial(int n)
{
    int result = 1;
    for (int i = 1; i <= n; i++)
    {
        result *= i;
    }

    return result;
}
