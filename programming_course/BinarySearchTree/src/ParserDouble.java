public class ParserDouble implements Parser<Double>{
    public Double parse(String input) {
        return Double.parseDouble(input);
    }
}