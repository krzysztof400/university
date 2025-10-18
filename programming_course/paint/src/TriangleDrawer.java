import javafx.scene.paint.Color;
import javafx.scene.shape.Polygon;
import javafx.scene.layout.AnchorPane;

public class TriangleDrawer extends Drawer {
    private Polygon outlineTriangle;

    @Override
    public MyShape draw(double start_x, double start_y, double end_x, double end_y, Color color) {
        MyTriangle triangle = new MyTriangle(start_x, start_y, end_x, end_y, color);
        return triangle;
    }

    @Override
    public void creation(double start_x, double start_y, double current_x, double current_y, AnchorPane pane) {
        if (outlineTriangle == null) {
            outlineTriangle = new Polygon();
            outlineTriangle.setFill(Color.TRANSPARENT);
            outlineTriangle.setStroke(Color.BLACK);
            outlineTriangle.setStrokeWidth(1.5);

            pane.getChildren().add(outlineTriangle);
        }

        outlineTriangle.getPoints().setAll(
            start_x, start_y,
            current_x, start_y,
            Math.abs((start_x + current_x)/2), current_y
        );
    }

    @Override
    public void clearOutline(AnchorPane pane) {
        if (outlineTriangle != null) {
            pane.getChildren().remove(outlineTriangle);
            outlineTriangle = null;
        }
    }
}
