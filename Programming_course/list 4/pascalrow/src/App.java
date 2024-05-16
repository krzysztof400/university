import javafx.application.Application;
import javafx.fxml.FXMLLoader;
import javafx.scene.Parent;
import javafx.scene.Scene;
import javafx.stage.Stage;

public class App extends Application {
    public static void main(String[] args) {
        launch(args);
    }

    @Override
    public void start(Stage stage) throws Exception {
        FXMLLoader loader = new FXMLLoader(getClass().getResource("/Main.fxml"));
        Parent root = loader.load();
        Scene scene = new Scene(root);
        stage.setTitle("Row in a Pascal triangle");
        stage.setScene(scene);
        
        Controler controller = loader.getController();
        controller.set_stage(stage);
        
        stage.show();
    }
}
