package Programming_course.second_list;

public class WierszTrojkatPascala
{
    private int[] row;

    public WierszTrojkatPascala(int n)
    {
        row = gen_row(n);
    }

    private int[] gen_row(int n)
    {
        int[] board = new int[n];
        for(int i = 0; i<n; i++)
        {
            board[i] = math_util.newton_binominal(n, i);
        }
        return board;
    }

    public int number(int m)
    {
        if(m<row.length || m<0)
        {
            return -1;
        }
        else
        {
            return row[m];
        }
    }
}