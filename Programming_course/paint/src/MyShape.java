import javafx.scene.paint.Color;
import javafx.scene.shape.Shape;
import java.io.Serializable;


public interface MyShape extends Serializable{
    public void show_stats();
    public Color getcolor();

    public void my_resize(double x);
    public void my_rotation(double x, double y);
    public void move(double x, double y);

    public Shape get_shape();
}