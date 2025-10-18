import javafx.scene.paint.Color;
import javafx.scene.shape.Rectangle;

public class GUICell extends Rectangle {
    private Cell cell;
    GUICell(int size, Color color, Cell cell) {
        super(size, size, color);
        this.cell = cell;
    }

    public void setGUIColor(Color color) {
        setFill(color);
        setOnMouseClicked(event -> MyUtils.toggleSuspended(getCell()));
    }

    private Cell getCell() {
        return cell;
    }
}