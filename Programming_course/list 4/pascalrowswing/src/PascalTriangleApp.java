import javax.swing.*;

public class PascalTriangleApp {

    public static void main(String[] args) {
        SwingUtilities.invokeLater(() -> {
            PascalTriangleFrame frame = new PascalTriangleFrame();
            frame.setExtendedState(JFrame.MAXIMIZED_BOTH);
            frame.setVisible(true);
        });
    }
}