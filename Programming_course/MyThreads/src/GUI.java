import javafx.scene.Scene;
import javafx.scene.layout.GridPane;
import javafx.stage.Stage;
import java.util.Random;
import java.util.concurrent.ExecutorService;

public class GUI {
    public GUI(Stage primaryStage, int N, int M, int RECT_SIZE, int K, double P, Random RANDOM, Cell[][] cells, ExecutorService executor) {
        GridPane grid = new GridPane();
        Object locker = new Object();
        
        MyGUIUtils.addCellsToGrid(grid, cells, N, M, locker, RANDOM);
        MyGUIUtils.prepereCells(cells, N, M, locker, RANDOM, K, P, executor);

        Scene scene = new Scene(grid, M * RECT_SIZE, N * RECT_SIZE);
        primaryStage.setScene(scene);
        primaryStage.show();
    }
}