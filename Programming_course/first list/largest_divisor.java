public class largest_divisor
{
    public static void main (String[] args)
    {
        for (String i : args)
        {
            try 
            {
                int n = Integer.parseInt(i);
                if(n>0)
                {
                    System.out.print(n + ", ");
                    System.out.println(div(n));
                }
                else if(n ==0)
                {
                    System.out.println(n + " uwaga: dzielnikiem zera jest kazda liczba naturalna, nie ma wiec najwieszkego dzielnika zera");
                }
                else
                {
                    System.out.println(i + " nie jest liczba naturalna");
                }
            }
            catch (NumberFormatException ex)
            {
                System.out.println(i + " nie jest liczba calkowita");
            }
        }
    }

    public static int div(int n)
    {
        if(n == 0){return 0;}
        if(n==1){return 1;}

        int i = n-1;
        while (n%i != 0)
        {
            i--;
        }
        return i;
    }
}