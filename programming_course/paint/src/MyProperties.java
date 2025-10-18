import javafx.scene.paint.Color;

public class MyProperties {
    private Color fill_color;
    private MyShape active_shape;
    private Action action;

    public MyProperties(Color fill_color){
        this.fill_color = fill_color;
    }

    public Color getcolor(){
        return fill_color;
    }

    public MyShape get_active_shape(){
        return active_shape;
    }

    public void change_color(Color fill_color){
        this.fill_color = fill_color;
    }
    public void set_active_shape(MyShape shape){
        if(get_active_shape()!=null){
            MyUtils.remove_stroke(get_active_shape());
        }
        this.active_shape = shape;
        MyUtils.set_stroke(shape);
    }
    public void setaction(Action action){
        this.action = action;
    }
    public Action getaction(){
        return action;
    }

    public enum Action{
        MOVE,
        ROTATE,
        CREATE;
    }
}