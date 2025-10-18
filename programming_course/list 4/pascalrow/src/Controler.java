import javafx.event.ActionEvent;
import javafx.fxml.FXML;
import javafx.fxml.FXMLLoader;
import javafx.scene.Parent;
import javafx.scene.Scene;
import javafx.scene.control.Button;
import javafx.scene.control.TextField;
import javafx.stage.Stage;
import java.util.ArrayList;
import java.util.List;

public class Controler {
    private Stage stage;

    public void set_stage(Stage stage){
        this.stage = stage;
    }
    @FXML
    private TextField demand_rows;

    @FXML
    private Button submit;

    @FXML
    private TextField user_input;

    @FXML

    public void event(ActionEvent event) {
        int num;
        try{
            num = Integer.parseInt(user_input.getText());
            System.out.println(num);
            if(num>29 || num<0){
                throw new IllegalArgumentException("Give me a number from 0 to 29");
            }
            try{
                FXMLLoader loader = new FXMLLoader(getClass().getResource("pascal.fxml"));
                Parent root = loader.load();
                PascalController controller = loader.getController();
                controller.restart();
                List<List<Integer>> output = new ArrayList<List<Integer>>();
                for(int i = 0; i<=num; i++){
                    output.add(i, UtilTriangle.gen_row(i));
                }
                if(num>17){
                    controller.smaller_font();
                }
                if(num>20){
                    controller.extension();
                }
                controller.set_text(output);
                stage.setScene(new Scene(root));
                PascalController new_controller = loader.getController();
                new_controller.set_stage(stage);
            }
            catch(Exception c){
                c.printStackTrace();
            }
        }
        catch(NumberFormatException e){
            Utils.showErrorAlert("Input error", "Give me an integer number");
        }
        catch(IllegalArgumentException e){
            Utils.showErrorAlert("Input error", e.getMessage());
        }
    }
}