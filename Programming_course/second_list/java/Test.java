import java.math.BigInteger;

public class Test {
    public static void main(String[] args)
    {
        if(args.length < 2)
        {
            System.err.println("Usage: java Test <row_number> [<column_index1> <column_index2> ...]");
        }
        long n;
        try
        {
            n = Long.parseLong(args[0]);
            if(n < 0)
            {
                System.out.println(n + " - invalid row number");
            }
            else
            {
                pascal_triangle_row row = new pascal_triangle_row((int) n);
                for(int i = 1; i<args.length; i++)
                {
                    int m;
                    try
                    {
                        m=Integer.parseInt(args[i]);
                        if(row.number(m) != BigInteger.valueOf(-1))
                        {
                            System.out.print(m + " - ");
                            System.out.println(row.number(m));
                        }
                        else
                        {
                            System.out.print(m);
                            System.out.println(" - out of range");
                        }
                    }
                    catch(NumberFormatException ex)
                    {
                        System.err.print(args[i]);
                        System.out.println(" - invalid input");
                    }
                }
            }
        } 
        
        catch (NumberFormatException ex)
        {
            System.out.print(args[0]);
            System.out.println(" - invalid input");
        }
    }
}
