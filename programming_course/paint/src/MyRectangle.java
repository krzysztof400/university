import javafx.scene.paint.Color;
import javafx.scene.shape.Rectangle;
import javafx.scene.shape.Shape;
import java.io.IOException;
import java.io.ObjectInputStream;
import java.io.ObjectOutputStream;

public class MyRectangle extends Rectangle implements MyShape{
    public MyRectangle(double start_x, double start_y, double width, double height, Color fill_color) {
        super(start_x, start_y, width, height);
        setFill(fill_color);
    }

    @Override public void my_resize(double x) {
        x*=0.2;
        if(x>0 || (getHeight()>=15 && getWidth()>=15)){
            setHeight(getHeight()+(getHeight()*x)/getWidth());
            setWidth(getWidth()+x);
        }
    }

    @Override public void move(double x, double y) {
        setX(x-getWidth()/2);
        setY(y-getHeight()/2);
    }

    @Override public void my_rotation(double x, double y) {
        MyUtils.rotation_for_poly(this, x, y);
    }

    @Override public void show_stats(){
        System.out.println("Color: "+getFill()+"; start X: "+getX()+"; startY: "+getY()+"; width: "+getWidth()+"; height: "+getHeight());
    }

    @Override public Color getcolor() {
        Color my_color = (Color) getFill();
        return my_color;
    }

    @Override public Shape get_shape() {
        return this;
    }

    private void writeObject(ObjectOutputStream s) throws IOException {
        s.defaultWriteObject();
        s.writeDouble(getX());
        s.writeDouble(getY());
        s.writeDouble(getWidth());
        s.writeDouble(getHeight());
        s.writeDouble(getRotate());
        s.writeDouble(getScaleX());
        s.writeDouble(((Color) getFill()).getRed());
        s.writeDouble(((Color) getFill()).getGreen());
        s.writeDouble(((Color) getFill()).getBlue());
    }

    private void readObject(ObjectInputStream s) throws IOException, ClassNotFoundException {
        double x = s.readDouble();
        double y = s.readDouble();
        double width = s.readDouble();
        double height = s.readDouble();
        double rotate = s.readDouble();
        double scale = s.readDouble();
        double red = s.readDouble();
        double green = s.readDouble();
        double blue = s.readDouble();

        setX(x);
        setY(y);
        setWidth(width);
        setHeight(height);
        setRotate(rotate);
        setScaleX(scale);
        setScaleY(scale);
        setFill(new Color(red, green, blue, 1));
    }
}