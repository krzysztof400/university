import java.util.Scanner;

public class CommandLineInterface<T extends Comparable<T>> {
    private final Parser<T> parser;

    public CommandLineInterface(Parser<T> parser) {
        this.parser = parser;
    }

    public void run() {
        BinarySearchTree<T> tree = new BinarySearchTree<>();
        Scanner scanner = new Scanner(System.in);

        while (true) {
            System.out.println("Enter command: ");
            String input = scanner.nextLine();
            if (input.equals("print") || input.equals("p")){
                tree.print();
                continue;
            } 

            String[] parts = input.split(":");
            if (parts.length != 2) {
                System.out.println("Invalid command format. Please use 'command: value'.");
                continue;
            }
            String command = parts[0].trim().toLowerCase();
            String value = parts[1].trim();

            try {
                if (command.equals("insert") || command.equals("i") || command.equals("add") || command.equals("a")){
                    T parsedValue = parser.parse(value);
                    tree.insert(parsedValue);
                } 
                else if (command.equals("delete") || command.equals("d") || command.equals("remove") || command.equals("r")){
                    T parsedValue = parser.parse(value);
                    tree.delete(parsedValue);
                } 
                else if (command.equals("search") || command.equals("s") || command.equals("find") || command.equals("f")){
                    T parsedValue = parser.parse(value);
                    boolean found = tree.search(parsedValue);
                    System.out.println("Search result: " + (found ? "Found" : "Not Found"));
                } 
                else {
                    System.out.println("Invalid command");
                }
            } catch (NumberFormatException e) {
                System.out.println("Invalid value format: " + value);
            }
        }
    }
}