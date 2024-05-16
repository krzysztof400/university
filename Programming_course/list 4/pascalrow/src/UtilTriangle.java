import java.util.ArrayList;

public class UtilTriangle {
    
    public static ArrayList<Integer> gen_row(int n)
    {
        ArrayList<Integer> board = new ArrayList<Integer>(n);
        board.add(1);
        for (int i = 1; i <= n; i++)
        {
            board.add(board.get(i-1) * (n - i + 1) / i);
        }
        return board;
    }
}