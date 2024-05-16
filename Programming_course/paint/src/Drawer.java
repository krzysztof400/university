import javafx.scene.layout.AnchorPane;
import javafx.scene.paint.Color;

public abstract class Drawer {
    public abstract MyShape draw(double x, double y, double property_x, double property_y, Color color);
    public abstract void creation(double start_x, double start_y, double current_x, double current_y, AnchorPane pane);
    public abstract void clearOutline(AnchorPane pane);
}