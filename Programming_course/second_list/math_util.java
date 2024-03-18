package Programming_course.second_list;

public class math_util {
    public static int newton_binominal(int n, int k)
    {
        //add contract
        return math_util.factorial(n)/(math_util.factorial(k)*math_util.factorial(n-k));
    }

    public static int factorial(int n)
    {
        //add contract
        for(int i = n; i > 1; i--)
        {
            i = i * (i-1);
        }
        return i;
    }
}
