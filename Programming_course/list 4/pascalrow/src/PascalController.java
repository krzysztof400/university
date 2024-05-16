import java.util.List;
import javafx.event.ActionEvent;
import javafx.fxml.FXML;
import javafx.fxml.FXMLLoader;
import javafx.scene.Parent;
import javafx.scene.Scene;
import javafx.scene.control.Button;
import javafx.scene.control.TextArea;
import javafx.scene.layout.AnchorPane;
import javafx.stage.Stage;

public class PascalController {

    private Stage stage;

    public void set_stage(Stage stage){
        this.stage = stage;
    }

    @FXML
    private TextArea pascal_triangle;

    @FXML
    private Button relapse;

    @FXML
    private AnchorPane background;

    @FXML
    public void relapse_event(ActionEvent event) throws Exception {
        FXMLLoader loader = new FXMLLoader(getClass().getResource("/Main.fxml"));
        Parent root = loader.load();

        Stage stage = (Stage) pascal_triangle.getScene().getWindow();
        stage.setScene(new Scene(root));

        Controler controller = loader.getController();
        controller.set_stage(stage);
    }

    public void set_text(List<List<Integer>> triangle) {
        StringBuilder sb = new StringBuilder();
        for (List<Integer> row : triangle) {
            for (Integer num : row) {
                sb.append(num).append(" ");
            }
            sb.append("\n");
        }
        pascal_triangle.setText(sb.toString());
    }
    public void restart(){
        background.setPrefSize(1000, 1000);
        pascal_triangle.setPrefSize(1000, 850);
        relapse.setLayoutX(400);
        relapse.setLayoutY(900);
        pascal_triangle.setStyle("-fx-font: 20 System");
    }

    public void smaller_font(){
        pascal_triangle.setStyle("-fx-font: 15 System");
    }

    public void extension(){
        background.setPrefSize(2000, 1000);
        pascal_triangle.setPrefSize(2000, 850);
        relapse.setLayoutX(900);
        relapse.setLayoutY(900);
    }
}