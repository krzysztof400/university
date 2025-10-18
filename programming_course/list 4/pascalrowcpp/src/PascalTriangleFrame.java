import javax.swing.*;

public class PascalTriangleFrame extends JFrame {

    public PascalTriangleFrame() {
        setTitle("Pascal's Triangle Generator");
        setSize(600, 400);
        setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        setLocationRelativeTo(null);
        setResizable(true);

        PascalTrianglePanel trianglePanel = new PascalTrianglePanel();

        add(trianglePanel);
    }
}
