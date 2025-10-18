import javafx.scene.control.ColorPicker;
import javafx.scene.paint.Color;
import javafx.scene.layout.AnchorPane;
import javafx.scene.input.MouseButton;

public class GUImethods {
    public static void creating(MyShape new_shape, MyProperties properties, AnchorPane pane){
        new_shape.get_shape().setOnMouseClicked(e -> {
            if (e.getButton() == MouseButton.SECONDARY) {
                handleShapeRightClick(new_shape, e.getScreenX(), e.getScreenY(), properties, pane);
            } else if (e.getButton() == MouseButton.PRIMARY) {
                handleShapeClick(new_shape, properties);
            }
        });
        pane.getChildren().add(new_shape.get_shape());
        properties.set_active_shape(null);
    }

    public static void handleShapeClick(MyShape shape, MyProperties properties) {
        properties.set_active_shape(shape);
    }

    public static void handleShapeRightClick(MyShape shape, double x, double y, MyProperties properties, AnchorPane pane){
        if(shape == properties.get_active_shape()){
            show_color_picker(x, y, pane, shape);
        }
    }

    public static void show_color_picker(double screenX, double screenY, AnchorPane pane, MyShape shape) {
        ColorPicker colorPicker = new ColorPicker();
        colorPicker.setLayoutX(screenX);
        colorPicker.setLayoutY(screenY);
        double localX = pane.sceneToLocal(screenX, screenY).getX();
        double localY = pane.sceneToLocal(screenX, screenY).getY();
        colorPicker.setLayoutX(localX);
        colorPicker.setLayoutY(localY);

        colorPicker.setOnAction(e -> {
            Color newColor = colorPicker.getValue();
            shape.get_shape().setFill(newColor);
            pane.getChildren().remove(colorPicker);
        });

        pane.getChildren().add(colorPicker);
    }
}
