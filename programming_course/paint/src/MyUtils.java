import javafx.scene.paint.Color;
import javafx.scene.paint.Paint;
import java.lang.Math;
import javafx.scene.shape.Shape;
import javafx.scene.control.Alert;
import javafx.scene.control.Alert.AlertType;

public class MyUtils {

    public static double distance_two_point(double x1, double x2, double y1, double y2){
        double result = Math.sqrt((y2 - y1) * (y2 - y1) + (x2 - x1) * (x2 - x1));
        return result;
    }

    public static void set_stroke(MyShape shape) {
        if (shape != null) {
            Paint fill = shape.get_shape().getFill();
            if (fill instanceof Color) {
                Color fillColor = (Color) fill;
                double brightness = fillColor.getBrightness();

                if (brightness < 0.5) {
                    shape.get_shape().setStroke(Color.RED);
                } 
                else {
                    shape.get_shape().setStroke(Color.BLUE);
                }

                shape.get_shape().setStrokeWidth(5);
            }
        }
    }

    public static void remove_stroke(MyShape shape) {
        shape.get_shape().setStroke(Color.TRANSPARENT);
    }

    public static double value_of_angle(double x1, double y1, double x2, double y2, double x3, double y3){
        double one = distance_two_point(x2, x3, y2, y3);
        double two = distance_two_point(x1, x3, y1, y3);
        double result;

        result = one/two;
        result = Math.toDegrees(Math.atan(result));
        return result;
    }


    public static void rotation_for_poly (Shape shape, double mouse_x, double mouse_y) {
        double center_x = shape.getBoundsInParent().getCenterX();
        double center_y = shape.getBoundsInParent().getCenterY();
        double angle_rad = Math.atan2(mouse_y - center_y, mouse_x - center_x);
        double angle_deg = Math.toDegrees(angle_rad);
        shape.setRotate(angle_deg);
    }


    public static void show_popup(String title, String message) {
        Alert alert = new Alert(AlertType.INFORMATION);
        alert.setTitle(title);
        alert.setHeaderText(null);
        alert.setContentText(message);

        alert.showAndWait();
    }
}