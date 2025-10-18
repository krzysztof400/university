import javafx.scene.paint.Color;
import javafx.scene.shape.Polygon;
import javafx.scene.shape.Shape;
import java.io.IOException;
import java.io.ObjectInputStream;
import java.io.ObjectOutputStream;

public class MyTriangle extends Polygon implements MyShape {
    private double startX;
    private double startY;

    public MyTriangle(double startX, double startY, double end_x, double end_y, Color fill) {
        super(startX, startY,
                end_x, startY,
                    Math.abs((startX + end_x)/2), end_y);

        this.startX = startX;
        this.startY = startY;
        setFill(fill);
    }

    @Override
    public void my_resize(double factor) {
        factor*=0.002;
        double currentCentroidX = (getPoints().get(0) + getPoints().get(2) + getPoints().get(4)) / 3.0;
        double currentCentroidY = (getPoints().get(1) + getPoints().get(3) + getPoints().get(5)) / 3.0;

        for (int i = 0; i < getPoints().size(); i += 2) {
            double x = getPoints().get(i);
            double y = getPoints().get(i + 1);
            double deltaX = x - currentCentroidX;
            double deltaY = y - currentCentroidY;
            double newX = currentCentroidX + deltaX * (1 + factor);
            double newY = currentCentroidY + deltaY * (1 + factor);
            getPoints().set(i, newX);
            getPoints().set(i + 1, newY);
        }
    }


    @Override
    public void move(double x, double y) {
        double currentCentroidX = (getPoints().get(0) + getPoints().get(2) + getPoints().get(4)) / 3.0;
        double currentCentroidY = (getPoints().get(1) + getPoints().get(3) + getPoints().get(5)) / 3.0;

        double deltaX = x - currentCentroidX;
        double deltaY = y - currentCentroidY;

        for (int i = 0; i < getPoints().size(); i += 2) {
            double newX = getPoints().get(i) + deltaX;
            double newY = getPoints().get(i + 1) + deltaY;
            getPoints().set(i, newX);
            getPoints().set(i + 1, newY);
        }
    }

    @Override
    public void my_rotation(double x, double y) {
        MyUtils.rotation_for_poly(this, x, y);
    }

    @Override
    public void show_stats() {
        System.out.println("Color: " + getFill() +
                "; Start X: " + startX +
                "; Start Y: " + startY);
    }

    @Override
    public Color getcolor() {
        return (Color) getFill();
    }

    @Override
    public Shape get_shape() {
        return this;
    }

    private void writeObject(ObjectOutputStream s) throws IOException {
        s.defaultWriteObject();
        s.writeDouble(getPoints().get(0));
        s.writeDouble(getPoints().get(1));
        s.writeDouble(getPoints().get(2));
        s.writeDouble(getPoints().get(3));
        s.writeDouble(getPoints().get(4));
        s.writeDouble(getPoints().get(5));
        s.writeDouble(getRotate());
        s.writeDouble(((Color) getFill()).getRed());
        s.writeDouble(((Color) getFill()).getGreen());
        s.writeDouble(((Color) getFill()).getBlue());
    }

    private void readObject(ObjectInputStream s) throws IOException, ClassNotFoundException {
        s.defaultReadObject();
        startX = s.readDouble();
        startY = s.readDouble();
        double endX = s.readDouble();
        double endY = s.readDouble();
        double apexX = s.readDouble();
        double apexY = s.readDouble();
        double rotation = s.readDouble();
        double red = s.readDouble();
        double green = s.readDouble();
        double blue = s.readDouble();

        getPoints().setAll(
                startX, startY,
                endX, endY,
                apexX, apexY
        );
        setFill(new Color(red, green, blue, 1.0));
        setRotate(rotation);
    }
}