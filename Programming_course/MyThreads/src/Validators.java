import java.util.Scanner;

public class Validators {

    private static Scanner scanner = new Scanner(System.in);

    public static int validateHeight(String input) {
        if(validateInteger(input, "height")>50){
            System.out.println("Height should be less than 50");
            System.out.print("Enter height: ");
            return validateHeight(scanner.next());
        } else {
            return validateInteger(input, "height");
        }
    }

    public static int validateWidth(String input) {
        if(validateInteger(input, "width")>50){
            System.out.println("Width should be less than 50");
            System.out.print("Enter width: ");
            return validateWidth(scanner.next());
        } else {
            return validateInteger(input, "width");
        }
    }

    public static int validateSpeed(String input) {
        int speed = validateInteger(input, "speed (in ms)");
        if (speed <= 100) {
            System.out.println("Speed should be greater than 100");
            System.out.print("Enter speed (in ms): ");
            return validateSpeed(scanner.next());
        } else {
            return speed;
        }
    }

    public static double validateProbability(String input) {
        return validateDouble(input, "probability");
    }

    private static int validateInteger(String input, String fieldName) {
        while (true) {
            try {
                int ans = Integer.parseInt(input);
                if(ans <= 0) {
                    System.out.print("Invalid input. Please enter a valid " + fieldName + ": ");
                    input = scanner.next();
                } else {
                    return ans;
                }
            } catch (NumberFormatException e) {
                System.out.print("Invalid input. Please enter a valid " + fieldName + ": ");
                input = scanner.next();
            }
        }
    }

    private static double validateDouble(String input, String fieldName) {
        while (true) {
            try {
                double probability = Double.parseDouble(input);
                if(probability < 0 || probability > 1) {
                    System.out.print("Invalid input. Please enter a valid " + fieldName + ": ");
                    input = scanner.next();
                } else {
                    return probability;
                }
            } catch (NumberFormatException e) {
                System.out.print("Invalid input. Please enter a valid " + fieldName + ": ");
                input = scanner.next();
            }
        }
    }
}