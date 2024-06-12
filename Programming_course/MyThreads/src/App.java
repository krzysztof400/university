import javafx.application.Application;
import javafx.stage.Stage;
import java.util.Random;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;
import java.util.concurrent.TimeUnit;
import java.util.Scanner;

public class App extends Application {

    private static int RECT_SIZE = 100;
    private static final Random RANDOM = new Random();
    public static int N;
    public static int M;
    public static int K;
    public static double P;
    private final Cell[][] cells = new Cell[N][M];
    private final ExecutorService executor = Executors.newFixedThreadPool(N * M);

    public static void main(String[] args) {
        Scanner scanner = new Scanner(System.in);

        System.out.print("Enter height: ");
        N = Validators.validateHeight(scanner.next());
        System.out.print("Enter width: ");
        M = Validators.validateWidth(scanner.next());
        System.out.print("Enter speed (in ms): ");
        K = Validators.validateSpeed(scanner.next());
        System.out.print("Enter probability: ");
        P = Validators.validateProbability(scanner.next());

        RECT_SIZE = Math.round(Math.min(1000f / N, 1850f / M));
        if (RECT_SIZE < 1) {
            RECT_SIZE = 1;
        }

        scanner.close();
        launch(args);
    }


    @Override
    public void start(Stage primaryStage) {
        new GUI(primaryStage, N, M, RECT_SIZE, K, P, RANDOM, cells, executor);
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