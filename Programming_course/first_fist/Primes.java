public class Primes {
    private int[] primes;

    public Primes(int n) {
        primes = GenPrimes(n);
    }

    private int[] GenPrimes(int n) {
        int[] board = new int[n];
        int count = 0;
        for (int i = 2; i <= n; i++) {
            if (MathUtil.isPrime(i)) {
                board[count] = i;
                count++;
            }
        }

        int[] result = new int[count];
        System.arraycopy(board, 0, result, 0, count);
        return result;
    }

    public int liczba(int m) {
        if (m < 0 || m >= primes.length) {
            return -1;
        }
        return primes[m];
    }
}