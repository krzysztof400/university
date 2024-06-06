import javafx.application.Application;
import javafx.scene.Scene;
import javafx.scene.layout.GridPane;
import javafx.scene.paint.Color;
import javafx.stage.Stage;

import java.util.Random;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;
import java.util.concurrent.TimeUnit;

public class App extends Application {

    private static final int RECT_SIZE = 100;
    private static final int N = 5; // liczba wierszy
    private static final int M = 5; // liczba kolumn
    private static final int K = 1000; // szybkość działania
    private static final double P = 0.25; // prawdopodobieństwo zmiany koloru
    private static final Random RANDOM = new Random();
    private final Color[][] colors = new Color[N][M];
    private final Cell[][] cells = new Cell[N][M];
    private final ExecutorService executor = Executors.newFixedThreadPool(N * M);

    public static void main(String[] args) {
        launch(args);
    }

    @Override
    public void start(Stage primaryStage) {
        GridPane grid = new GridPane();

        // Inicjalizacja planszy
        for (int i = 0; i < N; i++) {
            for (int j = 0; j < M; j++) {
                cells[i][j] = new Cell(RECT_SIZE);
                colors[i][j] = MyUtils.getRandomColor(RANDOM);
                cells[i][j].setFill(colors[i][j]);
                grid.add(cells[i][j], j, i);

                int finalI = i;
                int finalJ = j;
                executor.submit(() -> updateColor(finalI, finalJ));
            }
        }

        Scene scene = new Scene(grid, M * RECT_SIZE, N * RECT_SIZE);
        primaryStage.setScene(scene);
        primaryStage.show();
    }

    private void updateColor(int i, int j) {
        while (true) {
            try {
                Thread.sleep((long) (0.5 * K + RANDOM.nextDouble() * K));
            } catch (InterruptedException e) {
                Thread.currentThread().interrupt();
                break;
            }

            synchronized (colors) {
                if(cells[i][j].isSuspended()) {
                    continue;
                }

                double probability = RANDOM.nextDouble();
                if (probability < P) {
                    colors[i][j] = MyUtils.getRandomColor(RANDOM);
                } else {
                    colors[i][j] = MyUtils.getAverageColor(i, j, colors, cells, N, M);
                }

                javafx.application.Platform.runLater(() -> cells[i][j].setFill(colors[i][j]));
            }

            System.out.println("Start: " + (i * M + j));
            System.out.println("End: " + (i * M + j));
        }
    }

    @Override
    public void stop() {
        executor.shutdownNow();
        try {
            if (!executor.awaitTermination(1, TimeUnit.SECONDS)) {
                executor.shutdownNow();
            }
        } catch (InterruptedException e) {
            executor.shutdownNow();
        }
    }
}