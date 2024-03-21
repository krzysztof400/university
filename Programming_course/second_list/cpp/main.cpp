#include <iostream>
#include <vector>
#include <stdexcept>
#include <cstdint>

class MathUtil 
{
public:
    static uint64_t newton_binominal(int n, int k) 
    {
        return factorial(n) / (factorial(k) * factorial(n - k));
    }

    static uint64_t factorial(int n) 
    {
        if (n < 0) 
        {
            throw std::invalid_argument("Factorial input must be non-negative");
        }
        uint64_t result = 1;
        for (int i = 2; i <= n; i++) 
        {
            result *= i;
        }
        return result;
    }
};

class pascal_triangle_row 
{
private:
    std::vector<uint64_t> row;

    std::vector<uint64_t> gen_row(int n) 
    {
        std::vector<uint64_t> board(n + 1);
        for (int i = 0; i <= n; i++) 
        {
            board[i] = MathUtil::newton_binominal(n, i);
        }
        return board;
    }

public:
    pascal_triangle_row(int n) 
    {
        row = gen_row(n);
    }

    ~pascal_triangle_row()
    {
        std::cout << "Destructor called for pascal_triangle_row" << std::endl;
    }

    uint64_t number(int m) 
    {
        if (m >= static_cast<int>(row.size()) || m < 0) 
        {
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

    int n;
    try 
    {
        n = std::stoi(argv[1]);
        if (n < 0) 
        {
            std::cout << argv[1] << " - Invalid row number" << std::endl;
            return 1;
        }

        pascal_triangle_row row(n);
        for (int i = 2; i < argc; i++) 
        {
            try 
            {
                int m = std::stoi(argv[i]);
                uint64_t value = row.number(m);
                if (value != static_cast<uint64_t>(-1)) 
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
                std::cerr << argv[i] << " - Invalid input" << std::endl;
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