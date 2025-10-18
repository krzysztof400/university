import java.util.ArrayList;
import java.util.List;

public class PascalTriangle {

    public List<List<Integer>> generateTriangle(int numRows) {
        List<List<Integer>> triangle = new ArrayList<>();
        for (int i = 0; i < numRows; i++) {
            List<Integer> row = UtilsTriangle.generateRow(i);
            triangle.add(row);
        }
        return triangle;
    }
}