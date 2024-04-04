package Programming_course.third_list.first_task;

public class Test 
{
    public static void main(String[] args)
    {
        if(args.length < 2)
        {
            throw new IllegalArgumentException("Too few arguments, Java Programming_course.third_list.Test <figure type> <figure property 1> <figure property 2>...");
        }
        double[] parameters = new double[args.length - 1];
        for(int i = 1; i < args.length; i++)
        {
            try
            {
                parameters[i-1] = Double.parseDouble(args[i]);
            }
            catch(NumberFormatException ex)
            {
                throw new IllegalArgumentException("figure properties must be doubles");
            }
        }

        Figure fig = null;
        switch (args[0])
        {
            case "c":
                if(parameters.length != 5)
                {
                    throw new IllegalArgumentException("To create quadrilateral you need 5 arguments");
                }
                if(parameters[0] == parameters[1] && parameters[1] == parameters[2] && parameters[2] == parameters[3])
                {
                    if(parameters[4] == 90)
                    {
                        fig = new Square(parameters[0]);
                    }
                    else
                    {
                        fig = new Rhombus(parameters[0], parameters[4]);
                    }
                }
                else if (parameters[4] == 90)
                {
                    if(parameters[0] == parameters[1] && parameters[2] == parameters[3])
                    {
                        fig = new Rectangle(parameters[0], parameters[2]);
                    }
                    else if(parameters[0] == parameters[2] && parameters[1] == parameters[3])
                    {
                        fig = new Rectangle(parameters[0], parameters[2]);
                    }
                    else if(parameters[0] == parameters[3] && parameters[1] == parameters[2])
                    {
                        fig = new Rectangle(parameters[0], parameters[1]);
                    }
                    else
                    {
                        throw new IllegalArgumentException("Sides of the 'Recktangle' arent pairwise equal, therefor we cant crete any of available figures");
                    }
                }
                else
                {
                    throw new IllegalArgumentException("Sides arent equall and angle isnt 90 degrees");
                }
                break;

            case "o":
                fig = new Circle(parameters[0]);
                break;
            case "p":
                fig = new Pentagon(parameters[0]);
                break;
            case "s":
                fig = new Hexagon(parameters[0]);
                break;
            
            default:
                throw new IllegalArgumentException("Figure type must be c, o, s or p");
        }

        System.out.println("Figure type: " + fig.name());
        System.out.println("Area: " + fig.calculate_area());
        System.out.println("Peremiter: " + fig.calculate_peremiter());
    }
}