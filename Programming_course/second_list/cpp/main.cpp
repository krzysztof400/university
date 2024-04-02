#include <iostream>
#include <vector>
#include <stdexcept>
#include <cstdint>

class pascal_triangle_row 
{
private:
    std::vector<long long> row;

    std::vector<long long> gen_row(long long n) 
    {
        std::vector<long long> board(n + 1);
        board[0] = 1;
        for (long long i = 1; i <= n; i++)
        {
            board[i] = board[i-1] * (n - i + 1) / i;
        }
        return board;
    }


public:
    pascal_triangle_row(long long n) 
    {
        row = gen_row(n);
    }

    ~pascal_triangle_row()
    {
        std::cout << "Destructor called for pascal_triangle_row" << std::endl;
    }

    long long number(long long m) 
    {
        if (m >= static_cast<long long>(row.size()) || m < 0) 
        {
            throw std::out_of_range(std::string("ala ma ") + std::to_string(m));
            return -1;
        } 
        else 
        {
            return row[m];
        }
    }
};

int main(int argc, char* argv[]) 
{
    if (argc < 2) 
    {
        std::cout << "Usage: ./main <row_number> [<column_index1> <column_index2> ...]" << std::endl;
        return 1;
    }

    long long n;
    try 
    {
        n = std::stoi(argv[1]);
        if (n < 0) 
        {
            std::cout << argv[1] << " - Invalid row number" << std::endl;
            return 1;
        }

        pascal_triangle_row row(n);
        for (long long i = 2; i < argc; i++) 
        {
            try 
            {
                long long m = std::stoi(argv[i]);
                long long value = row.number(m);
                if (value != static_cast<long long>(-1)) 
                {
                    std::cout << m << " - " << value << std::endl;
                } 
                else 
                {
                    std::cout << m << " - Out of range" << std::endl;
                }
            } 
            catch (const std::invalid_argument& ex) 
            {
                std::cerr << argv[i] << " - Invalid input" << ex.what() << std::endl;
            }
        }
    } 
    catch (const std::invalid_argument& ex) 
    {
        std::cerr << argv[1] << " - Invalid input" << std::endl;
        return 1;
    }

    return 0;
}


// class MathUtil 
// {
// public:
//     static long long newton_binominal(long long n, long long k)
//     {
//         long long result;
//         for(int i = 1; i <= k; i++)
//         {
//             result
//         }
//         return (factorial(n) / factorial(k) )/ factorial(n - k);
//     }

//     static long long factorial(long long n) 
//     {
//         if (n < 0) 
//         {
//             throw std::invalid_argument("Factorial input must be non-negative");
//         }
//         long long result = 1;
//         for (long long i = 2; i <= n; i++) 
//         {
//             result *= i;
//         }
//         return result;
//     }
// };