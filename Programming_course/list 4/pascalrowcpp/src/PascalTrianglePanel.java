import javax.swing.*;
import java.awt.*;
import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;

public class PascalTrianglePanel extends JPanel {

    private JTextField inputField;
    private JTextArea outputArea;

    public PascalTrianglePanel() {
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
            if (numRows < 0 || numRows > 30) {
                JOptionPane.showMessageDialog(this, "Enter a number from 0 to 30.", "Error", JOptionPane.ERROR_MESSAGE);
                return;
            }

            Process process = Runtime.getRuntime().exec("./pascal_triangle_cpp " + numRows);
            BufferedReader reader = new BufferedReader(new InputStreamReader(process.getInputStream()));
            String line;
            StringBuilder sb = new StringBuilder();
            while ((line = reader.readLine()) != null) {
                sb.append(line).append("\n");
            }
            
            outputArea.setText(sb.toString());
        } catch (NumberFormatException ex) {
            JOptionPane.showMessageDialog(this, "Enter a valid number.", "Error", JOptionPane.ERROR_MESSAGE);
        } catch (IOException e) {
            JOptionPane.showMessageDialog(this, "Error executing C++ program.", "Error", JOptionPane.ERROR_MESSAGE);
        }
    }
}

    // private void displayTriangle(List<List<Integer>> triangle) {
    //     StringBuilder sb = new StringBuilder();
    //     for (List<Integer> row : triangle) {
    //         for (Integer num : row) {
    //             sb.append(num).append(" ");
    //         }
    //         sb.append("\n");
    //     }
    //     outputArea.setText(sb.toString());
    // }
