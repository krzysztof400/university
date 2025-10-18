import javafx.scene.paint.Color;
import java.util.ArrayList;

public class MyUtils {
    public static Color getRandomColor() {
        return Color.rgb(MyRandom.getRandomInt(256), MyRandom.getRandomInt(256), MyRandom.getRandomInt(256));
    }

    public static Color getNeighbourColor(Cell cell) {
        ArrayList<Color> colors = new ArrayList<>();
        
        for(int i = 0; i<4; i++){
            if (!cell.getNeighbour(i).isSuspended()) {
                //
                colors.add(cell.getNeighbour(i).getColor());
            }
        }

        if(colors.isEmpty()){
            return cell.getColor();
        }
        else{
            return averageColor(colors);
        }
    }

    public static Color averageColor(ArrayList<Color> colors) {
        int length = colors.size();

        if (length == 0) {
            return Color.BLACK;
        }
        else{
            double red = 0, green = 0, blue = 0;
            for (int i = 0; i < length; i++) {
                red += colors.get(i).getRed();
                green += colors.get(i).getGreen();
                blue += colors.get(i).getBlue();
            }
            return Color.color(red / length, green / length, blue / length);
        }
    }

    public static void detectNeighbours(int i, int j, Cell[][] cells, int N, int M) {
        ArrayList<Cell> neighbours = new ArrayList<>();
        neighbours.add(cells[i][(j - 1 + M) % M]);
        neighbours.add(cells[i][(j + 1) % M]);
        neighbours.add(cells[(i - 1 + N) % N][j]);
        neighbours.add(cells[(i + 1) % N][j]);
        
        cells[i][j].setNeighbours(neighbours);
    }

    public static void addAllNeighbours(Cell[][] cells, int N, int M){
        for (int i = 0; i < N; i++) {
            for (int j = 0; j < M; j++) {
                MyUtils.detectNeighbours(i, j, cells, N, M);
            }
        }
    }
    
    public static void prepereCells(Cell[][] cells, int N, int M, Object locker, int RECT_SIZE){
        for (int i = 0; i < N; i++) {
            for (int j = 0; j < M; j++) {
                cells[i][j] = new Cell(RECT_SIZE, locker, getRandomColor());
            }
        }
    }

    public static void runThreads(Cell[][] cells, Object locker, int K, double P, int N, int M){
        for(int i = 0; i < N; i++){
            for(int j = 0; j < M; j++){
                int finalI = i;
                int finalJ = j;
                
                Thread cellThread = new Thread(() -> cells[finalI][finalJ].updateColor(finalI, finalJ, locker, K, P, cells, N, M));
                cellThread.setDaemon(true);
                cellThread.start();
            }
        }
    }

    public static void toggleSuspended(Cell cell){
        cell.toggleSuspended();
    }

    public static int calculateRECT_SIZE(int N, int M){
        int RectSize;
        RectSize = Math.round(Math.min(1000f / N, 1850f / M));
        if (RectSize < 1) {
            return 1;
        }
        return RectSize;
    }
}