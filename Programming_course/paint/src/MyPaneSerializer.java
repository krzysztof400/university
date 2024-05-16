import javafx.collections.FXCollections;
import javafx.collections.ObservableList;
import javafx.scene.Node;
import java.io.*;


public class MyPaneSerializer {

    public static void serializePane(ObservableList<Node> nodes, String filename) {
        try (ObjectOutputStream oos = new ObjectOutputStream(new FileOutputStream(filename))) {
            oos.writeInt(nodes.size());
            for (Node n : nodes) {
                if (n instanceof Serializable) {
                    oos.writeObject(n);
                }
            }
        } catch (IOException e) {
            e.printStackTrace();
        }
    }

    public static ObservableList<Node> deserializePane(String filename) {
        ObservableList<Node> nodes = FXCollections.observableArrayList();
        try (ObjectInputStream ois = new ObjectInputStream(new FileInputStream(filename))) {
            int numNodes = ois.readInt();
            for (int i = 0; i < numNodes; i++) {
                MyShape n = (MyShape) ois.readObject();
                nodes.add(n.get_shape());
            }
        } catch (IOException | ClassNotFoundException e) {
            e.printStackTrace();
        }
        return nodes;
    }
}