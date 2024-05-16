import java.util.ArrayList;
import java.util.List;

public class PascalTriangle {

    public List<List<Integer>> generateTriangle(int numRows) {
        List<List<Integer>> triangle = new ArrayList<>();
        for (int i = 0; i < numRows; i++) {
            List<Integer> row = generateRow(i);
            triangle.add(row);
        }
        return triangle;
    }

    private List<Integer> generateRow(int rowNum) {
        List<Integer> board = new ArrayList<>();
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