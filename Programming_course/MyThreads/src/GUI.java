import javafx.scene.Scene;
import javafx.scene.layout.GridPane;
import javafx.stage.Stage;

public class GUI {
    public GUI(Stage primaryStage, int N, int M, int RECT_SIZE, int K, double P, Cell[][] cells, Object locker) {
        GridPane grid = new GridPane();

        MyGUIUtils.addCellsToGrid(grid, cells, N, M, locker, K, P);
        MyUtils.runThreads(cells, locker, K, P, N, M);

        Scene scene = new Scene(grid, M * RECT_SIZE, N * RECT_SIZE);
        primaryStage.setScene(scene);
        primaryStage.show();
    }
}