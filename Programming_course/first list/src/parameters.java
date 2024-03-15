public class parameters
{
    public static void main(String[] args)
    {
        // check if parameters are good
        //     check number of given parameters
        //     check unusual input eg. non latin alphabet
        //     check when input is " """"""""""""""""""""""" "
        for (String i : args)
        {
            System.out.println(i);
        }
    }
}