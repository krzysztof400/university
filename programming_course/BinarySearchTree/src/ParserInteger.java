public class ParserInteger implements Parser<Integer>{
    public Integer parse(String input) {
        return Integer.parseInt(input);
    }
}