public class Test {
    public static void main(String[] args) {
        int n;
        try
        {
            n=Integer.parseInt(args[0]);
            if(n>1)
            {
                Primes board = new Primes(n);
                for (int i = 1; i < args.length; i++) 
                { 
                    int m;
                    try 
                    {
                        m=Integer.parseInt(args[i]);
                        if(board.liczba(m) != -1)
                        {
                            System.out.println(board.liczba(m));
                        }
                        else{
                            System.out.println(m + " -liczba spoza zakresu");
                        }
                    }
                    catch (NumberFormatException ex) {System.out.println(args[i] + " -nieprawidlowa dana");}
                }
            }
            else
            {
                System.out.println(n + " -nieprawidlowy zakres");
            }
        }
        
        catch (NumberFormatException ex) {System.out.println(args[0] + " -nieprawidlowa dana");}
    }
}