import javafx.scene.paint.Color;
import java.util.Random;

public class MyUtils {
    public static Color getRandomColor(Random RANDOM) {
        return Color.rgb(RANDOM.nextInt(256), RANDOM.nextInt(256), RANDOM.nextInt(256));
    }

    public static Color getAverageColor(int i, int j, Color[][] colors, Cell[][] cells, int N, int M) {
        Color[] neighbors = new Color[4];

        neighbors[0] = cells[(i - 1 + N) % N][j].getColor(); // top neighbor
        neighbors[1] = cells[(i + 1) % N][j].getColor(); // bottom neighbor
        neighbors[2] = cells[i][(j - 1 + M) % M].getColor(); // left neighbor
        neighbors[3] = cells[i][(j + 1) % M].getColor(); // right neighbor

        double red = 0, green = 0, blue = 0;
        for (Color color : neighbors) {
            red += color.getRed();
            green += color.getGreen();
            blue += color.getBlue();
        }

        return Color.color(red / 4, green / 4, blue / 4);
    }
}