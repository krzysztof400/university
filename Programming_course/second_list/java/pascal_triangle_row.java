import java.math.BigInteger;

public class pascal_triangle_row
{
    private BigInteger[] row;

    public pascal_triangle_row(int n) {
        row = gen_row(n);
    }

    private BigInteger[] gen_row(int n) {
        BigInteger[] board = new BigInteger[n + 1];
        for (int i = 0; i <= n; i++) {
            board[i] = math_util.newton_binominal(n, i);
        }
        return board;
    }

    public BigInteger number(int m) {
        if (m >= row.length || m < 0) {
            return BigInteger.valueOf(-1);
        } else {
            return row[m];
        }
    }
}