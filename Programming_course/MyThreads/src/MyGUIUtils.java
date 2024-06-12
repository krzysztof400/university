import javafx.scene.layout.GridPane;
import java.util.Random;
import java.util.concurrent.ExecutorService;

public class MyGUIUtils {
    public static void setNewColor(Cell cell){
        javafx.application.Platform.runLater(() -> cell.setFill(cell.getColor()));
    }

    public static void addCellsToGrid(GridPane grid, Cell[][] cells, int N, int M, Object locker, Random RANDOM){
        for (int i = 0; i < N; i++) {
            for (int j = 0; j < M; j++) {
                cells[i][j] = new Cell(MyUtils.calculateRECT_SIZE(N, M), locker, MyUtils.getRandomColor(RANDOM));
                cells[i][j].setFill(cells[i][j].getColor());
                grid.add(cells[i][j], j, i);
            }
        }
    }

    public static void prepereCells(Cell[][] cells, int N, int M, Object locker, Random RANDOM, int K, double P, ExecutorService executor){
        for (int i = 0; i < N; i++) {
            for (int j = 0; j < M; j++) {
                MyUtils.addAllNeighbours(i, j, cells, N, M);

                int finalI = i;
                int finalJ = j;
                executor.submit(() -> cells[finalI][finalJ].updateColor(finalI, finalJ, locker, K, P, RANDOM, cells, N, M));
            }
        }
    }
}