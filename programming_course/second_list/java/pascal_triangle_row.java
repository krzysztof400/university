import java.math.BigInteger;
import java.util.Vector;

public class pascal_triangle_row
{
    private Vector<BigInteger> row;

    public pascal_triangle_row(int n) {
        row = gen_row(n);
    }

    private Vector<BigInteger> gen_row(int n)
    {
        Vector<BigInteger> board = new Vector<BigInteger>();
        for (int i = 0; i <= n; i++)
        {
            board.add(math_util.newton_binominal(n, i));
        }
        return board;
    }

    public BigInteger number(int m) 
    {
        if (m >= row.size() || m < 0) 
        {
            return BigInteger.valueOf(-1);
        } 
        else 
        {
            return row.get(m);
        }
    }
}