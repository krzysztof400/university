import javax.swing.*;
import java.awt.*;
import java.util.List;

public class PascalTrianglePanel extends JPanel {

    private JTextField inputField;
    private JTextArea outputArea;
    private PascalTriangle pascalTriangle;

    public PascalTrianglePanel(PascalTriangle pascalTriangle) {
        this.pascalTriangle = pascalTriangle;

        setLayout(new BorderLayout());

        JPanel inputPanel = new JPanel();
        JLabel inputLabel = new JLabel("Enter number of rows:");
        inputField = new JTextField(10);
        inputPanel.add(inputLabel);
        inputPanel.add(inputField);

        JButton generateButton = new JButton("Generate");
        generateButton.addActionListener(e -> generatePascalsTriangle());

        JPanel buttonPanel = new JPanel();
        buttonPanel.add(generateButton);

        outputArea = new JTextArea(50, 60);
        outputArea.setEditable(false);

        JScrollPane scrollPane = new JScrollPane(outputArea);
        scrollPane.setAlignmentX(CENTER_ALIGNMENT);

        add(inputPanel, BorderLayout.NORTH);
        add(buttonPanel, BorderLayout.CENTER);
        add(scrollPane, BorderLayout.SOUTH);
    }

    private void generatePascalsTriangle() {
        try {
            int numRows = Integer.parseInt(inputField.getText());
            if (numRows < 0 || numRows>30) {
                JOptionPane.showMessageDialog(this, "Enter a number from 0 to 30.", "Error", JOptionPane.ERROR_MESSAGE);
                return;
            }

            List<List<Integer>> triangle = pascalTriangle.generateTriangle(numRows);
            displayTriangle(triangle);
        } catch (NumberFormatException ex) {
            JOptionPane.showMessageDialog(this, "Enter a valid number.", "Error", JOptionPane.ERROR_MESSAGE);
        }
    }

    private void displayTriangle(List<List<Integer>> triangle) {
        StringBuilder sb = new StringBuilder();
        for (List<Integer> row : triangle) {
            for (Integer num : row) {
                sb.append(num).append(" ");
            }
            sb.append("\n");
        }
        outputArea.setText(sb.toString());
    }
}
