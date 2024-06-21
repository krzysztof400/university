import javafx.application.Application;
import javafx.stage.Stage;
import java.util.Scanner;

public class App extends Application {

    private static int RECT_SIZE = 100;
    public static int N;
    public static int M;
    public static int K;
    public static double P;
    private final Cell[][] cells = new Cell[N][M];

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
        Object locker = new Object();
        MyUtils.prepereCells(cells, N, M, locker, RECT_SIZE);
        MyUtils.addAllNeighbours(cells, N, M);

        new GUI(primaryStage, N, M, RECT_SIZE, K, P, cells, locker);
    }

    // @Override
    // public void stop() {
    //     MyGUIUtils.stopAllThreads();
    // }
}