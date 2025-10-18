import javafx.fxml.FXML;
import javafx.scene.canvas.Canvas;
import javafx.scene.control.ColorPicker;
import javafx.scene.input.MouseEvent;
import javafx.scene.input.ScrollEvent;
import javafx.scene.paint.Color;
import javafx.scene.layout.VBox;
import javafx.event.ActionEvent;
import javafx.scene.layout.AnchorPane;
import javafx.scene.Node;
import javafx.collections.ObservableList;


public class PaintController {
    private double start_x, start_y;
    private Drawer d;
    private MyProperties properties;
    @FXML private VBox root;
    @FXML private Canvas canvas;
    @FXML private ColorPicker color_picker;
    @FXML private AnchorPane pane;

    public void initialize() {
        properties = new MyProperties(Color.BLACK);
        color_picker.setValue(Color.BLACK);
    }

    @FXML private void handle_mouse_pressed(MouseEvent event) {
        start_x = event.getX();
        start_y = event.getY();
        if(event.isControlDown()){
            properties.setaction(MyProperties.Action.ROTATE);
        }
        else if(properties.get_active_shape()!=null){
            if(properties.get_active_shape().get_shape().contains(start_x, start_y)){
                properties.setaction(MyProperties.Action.MOVE);
            }
            else{
                properties.set_active_shape(null);
                properties.setaction(MyProperties.Action.CREATE);
            }
        }
        else{
            properties.setaction(MyProperties.Action.CREATE);
        }
    }

    @FXML private void hande_mouse_released(MouseEvent event) {
        double property_x = event.getX();
        double property_y = event.getY();

        if(properties.getaction()==MyProperties.Action.CREATE && d != null){
                MyShape new_shape = d.draw(start_x, start_y, property_x, property_y, properties.getcolor());
                GUImethods.creating(new_shape, properties, pane);
                properties.set_active_shape(null);
                d.clearOutline(pane);
        }
    }

    @FXML private void handle_mouse_drag(MouseEvent event) {
        double current_x = event.getX();
        double current_y = event.getY();
    
        if (properties.getaction() == MyProperties.Action.ROTATE && properties.get_active_shape()!=null) {
            properties.get_active_shape().my_rotation(current_x, current_y);
        } 
        else if (properties.getaction() == MyProperties.Action.MOVE && properties.get_active_shape()!=null) {
            properties.get_active_shape().move(current_x, current_y);
        } 
        else if (properties.getaction() == MyProperties.Action.CREATE) {
            d.creation(start_x, start_y, current_x, current_y, pane);
        }
    }

    @FXML private void handle_scroll(ScrollEvent event){
        if(properties.get_active_shape()!=null){
            properties.get_active_shape().my_resize(event.getDeltaY());
        }
    }

    @FXML private void color_picked(ActionEvent event){
        Color my_color = color_picker.getValue();
        properties.change_color(my_color);
        if(properties.get_active_shape()!=null){
            properties.get_active_shape().get_shape().setFill(my_color);
            MyUtils.set_stroke(properties.get_active_shape());
        }
    }   
    
    @FXML private void show_info(){
        MyUtils.show_popup("INFO", "Title: Missisipi paint\nPurpose: Lacks, we live in postmodern times\nAuthor: Krzysztof Zajac\nAffiliation: Dobry Swiat corp\nCzystosc ducha: dostateczna");
    }

    @FXML private void save_it(){
        MyPaneSerializer.serializePane(pane.getChildren(), "info.txt");
    }

    @FXML
    private void load_it() {
        ObservableList<Node> deserializedNodes = MyPaneSerializer.deserializePane("info.txt");
        for (Node node : deserializedNodes) {
            if (node instanceof MyShape) {
                MyShape n = (MyShape) node;
                GUImethods.creating(n, properties, pane);
            }
            if (!pane.getChildren().contains(node)) {
                pane.getChildren().add(node);
            }
        }
    }
    

        
    @FXML private void clicked_circle_button(ActionEvent event) {
        d = new CircDrawer();
        properties.set_active_shape(null);
    }

    @FXML private void clicked_rectangle_button(ActionEvent event) {
        d = new RectDrawer();
        properties.set_active_shape(null);
    }

    @FXML private void clicked_triangle_button(ActionEvent event) {
        d = new TriangleDrawer();
        properties.set_active_shape(null);
    }
}