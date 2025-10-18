public class Test 
{
    public static void main(String[] args) 
    {
        if(args.length < 2)
        {
            System.err.println("Usage: java Test <limit> [<index1> <index2> ...]");
        }
        else
        {
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
                            System.out.print(m + " - ");
                            System.out.println(board.liczba(m));
                        }
                        else
                        {
                            System.out.println(m + " - out of range");
                        }
                    }
                    catch (NumberFormatException ex) {System.out.println(args[i] + " - invalid input");}
                }
            }
            else
            {
                System.out.println(n + " - out of range");
            }
        }
        
        catch (NumberFormatException ex) 
        {
            System.out.println(args[0] + " - invalid input");
        }
        }
    }
}