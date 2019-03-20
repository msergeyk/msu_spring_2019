#include<iostream>
#include<cstdlib>
#include<math.h>
#include "numbers.dat"

int prime_number_test(int n);

int prime_number_amount(const int* Data, const int left, const int right);

int main(int argc, char* argv[])
{
    if ((argc == 1) || (argc % 2 == 0))
        return -1;

    for (int i = 1; i < argc; i = i + 2)
    {
        int bord1 = std::atoi(argv[i]);
        int bord2 = std::atoi(argv[i + 1]);

        if ((bord2 < 0) || (bord1 < 0))
            return -1;

        if (bord2 < bord1)
        {
            std::cout << 0 << std::endl;
            continue;
        }

        int left = -1;

        for (int i = 0; i < Size; i ++)
        {
            if (Data[i] == bord1)
            {
                left = i;
                break;
            }
        }

        if (left == -1)
        {
            std::cout << 0 << std::endl;
            continue;
        }

        int right = -1;

        for (int i = left; i < Size; i ++)
        {
            if ((Data[i] == bord2) && ((i == Size - 1) || (Data[i + 1] != bord2)))
            {
                right = i;
                break;
            }
        }

        if (right == -1)
        {
            std::cout << 0 << std::endl;
            continue;
        }

        int k = prime_number_amount(Data, left, right);
        std::cout << k << std::endl;
    }
    return 0;
}

int prime_number_test(int n)
{
    if ((n == 1) || (n == 0))
        return 0;
    for (int i = 2; i*i <= n; i ++)
    {
        if (n % i == 0)
            return 0;
    }

    return 1;
}

int prime_number_amount(const int* Data, const int left, const int right)
{
    int num = 0;
    for (int i = left; i <= right; i++)
    {
        num = num + prime_number_test(Data[i]);
    }
    return num;
}
