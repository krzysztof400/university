import java.util.ArrayList;
import java.util.List;
import javafx.application.Platform;
import javafx.scene.layout.GridPane;


public class MyGUIUtils {
// private static List<Thread> cellThreads = new ArrayList<>();

    public static void setNewColor(Cell cell){
        Platform.runLater(() -> cell.getGUICell().setGUIColor(cell.getColor()));
    }

    public static void addCellsToGrid(GridPane grid, Cell[][] cells, int N, int M, Object locker, int K, double P){
        for (int i = 0; i < N; i++) {
            for (int j = 0; j < M; j++) {
                grid.add(cells[i][j].getGUICell(), j, i);
                cells[i][j].getGUICell().setGUIColor(cells[i][j].getColor());
            }
        }
    }

    // public static void stopAllThreads() {
    //     for (Thread thread : cellThreads) {
    //         thread.interrupt();
    //     }
    //     for (Thread thread : cellThreads) {
    //         try {
    //             thread.join();
    //         } catch (InterruptedException e) {
    //             Thread.currentThread().interrupt();
    //         }
    //     }
    // }
}