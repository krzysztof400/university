import javax.swing.*;

public class PascalTriangleFrame extends JFrame {

    public PascalTriangleFrame() {
        setTitle("Pascal's Triangle Generator");
        setSize(400, 300);
        setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        setLocationRelativeTo(null);

        PascalTrianglePanel trianglePanel = new PascalTrianglePanel(new PascalTriangle());

        add(trianglePanel);
    }
}
