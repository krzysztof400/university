import java.util.ArrayList;

public class UtilsTriangle {
    public static ArrayList<Integer> generateRow(int rowNum) {
        ArrayList<Integer> board = new ArrayList<>();
        for (int i = 0; i <= rowNum; i++) {
            if (i == 0 || i == rowNum) {
                board.add(1);
            } else {
                board.add(board.get(i-1) * (rowNum - i + 1) / i);
            }
        }
        return board;
    }    
}