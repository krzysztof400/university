public class MyRandom {
    public static double getRandomDouble() {
        return Math.random();
    }

    public static long getSpeed(double K){
        return (long) (0.5 * K + getRandomDouble() * K);
    }

    public static boolean getProbability(double P)
    {
        return getRandomDouble() <= P;
    }

    public static int getRandomInt(int N){
        return (int) (getRandomDouble() * N);
    }
}