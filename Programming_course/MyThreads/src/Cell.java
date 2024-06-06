import javafx.scene.shape.Rectangle;
import javafx.scene.paint.Color;

public class Cell extends Rectangle{
    private boolean suspended = false;

    public Cell(int size){
       super(size, size);
       setOnMouseClicked(event -> toggleSuspended());
    }

    public Rectangle getCell(){
       return this;
    }

    public void toggleSuspended(){
       suspended = !suspended;
    }

    public Color getColor(){
        if(isSuspended()){
            return null;
        } else {
            return (Color) getFill();
        }
    }

    public boolean isSuspended(){
        return suspended;
    }
}