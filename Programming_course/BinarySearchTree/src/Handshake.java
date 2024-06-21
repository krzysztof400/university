import java.util.Scanner;

public class Handshake {
    public static void handshake(){
        System.out.println("Enter BST type: ");
        Scanner scanner = new Scanner(System.in);
        String bstType = scanner.nextLine();

        if(bstType.equals("Integer") || bstType.equals("Int") || bstType.equals("int") || bstType.equals("integer") || bstType.equals("i") || bstType.equals("I")){
            ParserInteger parser = new ParserInteger();
            CommandLineInterface<Integer> cli = new CommandLineInterface<Integer>(parser);
            cli.run();
        } 
        else if(bstType.equals("Double") || bstType.equals("double") || bstType.equals("d") || bstType.equals("D")){
            ParserDouble parser = new ParserDouble();
            CommandLineInterface<Double> cli = new CommandLineInterface<Double>(parser);
            cli.run();
        } 
        else if(bstType.equals("String") || bstType.equals("string") || bstType.equals("s") || bstType.equals("S")){
            ParserString parser = new ParserString();
            CommandLineInterface<String> cli = new CommandLineInterface<String>(parser);
            cli.run();
        } 
        else {
            System.out.println("Invalid BST type");
        }

        scanner.close();
    }
}