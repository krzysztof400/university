import javafx.scene.paint.Color;
import javafx.scene.shape.Circle;
import javafx.scene.shape.Shape;
import java.io.IOException;
import java.io.ObjectInputStream;
import java.io.ObjectOutputStream;

public class MyCircle extends Circle implements MyShape {
    public MyCircle(double start_x, double start_y, double radius, Color fill_color) {
        super(start_x, start_y, radius);
        setFill(fill_color);
    }

    @Override
    public void my_resize(double x) {
        x*=0.2;
        setRadius(x+getRadius());
    }

    @Override public void move(double x, double y) {
        setCenterX(x);
        setCenterY(y);
    }

    @Override public void my_rotation(double x, double y) {}


    @Override public void show_stats(){
        System.out.println("Color: "+getFill()+"; start X: "+getLayoutX()+"; startY: "+getLayoutY()+"; radius: "+getRadius());
    }

    @Override public Color getcolor() {
        Color my_color = (Color) getFill();
        return my_color;
    }

    @Override
    public Shape get_shape() {
        return this;
    }

    private void writeObject(ObjectOutputStream s) throws IOException {
        s.defaultWriteObject();
        s.writeDouble(getCenterX());
        s.writeDouble(getCenterY());
        s.writeDouble(getRadius());
        s.writeDouble(((Color) getFill()).getRed());
        s.writeDouble(((Color) getFill()).getGreen());
        s.writeDouble(((Color) getFill()).getBlue());
        s.writeDouble(((Color) getFill()).getOpacity());
    }

    private void readObject(ObjectInputStream s) throws IOException, ClassNotFoundException {
        s.defaultReadObject();
        double centerX = s.readDouble();
        double centerY = s.readDouble();
        double radius = s.readDouble();
        double red = s.readDouble();
        double green = s.readDouble();
        double blue = s.readDouble();
        double opacity = s.readDouble();

        setCenterX(centerX);
        setCenterY(centerY);
        setRadius(radius);
        setFill(new Color(red, green, blue, opacity));
    }
}