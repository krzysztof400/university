import javafx.scene.paint.Color;
import javafx.scene.shape.Circle;
import java.lang.Math;
import javafx.scene.layout.AnchorPane;

public class CircDrawer extends Drawer {
    private Circle outlineCircle;
    @Override
    public MyShape draw(double x, double y, double property_x, double property_y, Color color) {
        double radius = MyUtils.distance_two_point(x, property_x, y, property_y);
        MyCircle circ = new MyCircle(x, y, radius, color);
        return circ;
    }

    public void creation(double start_x, double start_y, double current_x, double current_y, AnchorPane pane) {
        if (outlineCircle == null) {
            outlineCircle = new Circle();
            outlineCircle.setFill(Color.TRANSPARENT);
            outlineCircle.setStroke(Color.BLACK);
            outlineCircle.setStrokeWidth(1.5);
            pane.getChildren().add(outlineCircle);
        }

        double radius = Math.sqrt(Math.pow(current_x - start_x, 2) + Math.pow(current_y - start_y, 2));
        outlineCircle.setCenterX(start_x);
        outlineCircle.setCenterY(start_y);
        outlineCircle.setRadius(radius);
    }

    @Override public void clearOutline(AnchorPane pane) {
        if (outlineCircle != null) {
            pane.getChildren().remove(outlineCircle);
            outlineCircle = null;
        }
    }
}