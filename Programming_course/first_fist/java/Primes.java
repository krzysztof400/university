import java.util.Vector;

public class Primes {
    private Vector<Integer> primes;

    public Primes(int n) {
        primes = GenPrimes(n);
    }

    private Vector<Integer> GenPrimes(int n) {
        Vector<Integer> board = new Vector<Integer>();

        for (int i = 2; i <= n; i++)
        {
            if (MathUtil.isPrime(i))
            {
                board.add(i);
            }
        }
        return board;
    }

    public int liczba(int m) {
        if (m < 0 || m >= primes.size()) {
            return -1;
        }
        return primes.get(m);
    }
}