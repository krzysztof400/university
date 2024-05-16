import javafx.scene.paint.Color;
import javafx.scene.shape.Rectangle;
import javafx.scene.layout.AnchorPane;

public class RectDrawer extends Drawer {
    private Rectangle outlineRectangle;  // Outline rectangle for visual feedback
    @Override public MyShape draw(double start_x, double start_y, double end_x, double end_y, Color color) {
        double width = Math.abs(end_x - start_x);
        double height = Math.abs(end_y - start_y);
        double x = Math.min(start_x, end_x);
        double y = Math.min(start_y, end_y);

        MyRectangle rect = new MyRectangle(x, y, width, height, color);
        return rect;
    }

    @Override public void creation(double start_x, double start_y, double current_x, double current_y, AnchorPane pane) {
        if (outlineRectangle == null) {
            outlineRectangle = new Rectangle();
            outlineRectangle.setFill(Color.TRANSPARENT);
            outlineRectangle.setStroke(Color.BLACK);
            outlineRectangle.setStrokeWidth(1.5);

            pane.getChildren().add(outlineRectangle);
        }

        double x = Math.min(start_x, current_x);
        double y = Math.min(start_y, current_y);
        double width = Math.abs(current_x - start_x);
        double height = Math.abs(current_y - start_y);

        outlineRectangle.setX(x);
        outlineRectangle.setY(y);
        outlineRectangle.setWidth(width);
        outlineRectangle.setHeight(height);
    }

    @Override public void clearOutline(AnchorPane pane) {
        if (outlineRectangle != null) {
            pane.getChildren().remove(outlineRectangle);
            outlineRectangle = null; 
        }
    }
}