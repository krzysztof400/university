package Programming_course.third_list.third_task;

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

        if(parameters.length == 1)
        {
            Figures.One_par fig1 = null;
            switch (args[0])
            {
            case "c":
                fig1 = Figures.One_par.SQUARE;
                break;
            case "o":
                fig1 = Figures.One_par.CIRCLE;
                break;
            case "p":
                fig1 = Figures.One_par.PENTAGON;
                break;
            case "s":
                fig1 = Figures.One_par.HEXAGON;
                break;
            default:
                throw new IllegalArgumentException("Figure type must be c, o, s, k, r or p");
            }
            fig1.set_parameter(parameters[0]);
            System.out.println("Figure type: " + fig1.name());
            System.out.println("Area: " + fig1.calculate_area());
            System.out.println("Peremiter: " + fig1.calculate_peremiter());
        }
        else if (parameters.length == 2)
        {
            Figures.Two_par fig2 = null;
            switch(args[0])
            {
            case "r":
                fig2 = Figures.Two_par.RHOMBUS;
                break;
            case "c":
                fig2 = Figures.Two_par.RECKTANGLE;
                break;
            default:
                throw new IllegalArgumentException("Figure type must be c, o, s, k, r or p");
            }
            fig2.set_parameter(parameters[0], parameters[1]);
            System.out.println("Figure type: " + fig2.name());
            System.out.println("Area: " + fig2.calculate_area());
            System.out.println("Peremiter: " + fig2.calculate_peremiter());
        }
        else
        {
            throw new IllegalArgumentException("Too much arguments Java Programming_course.third_list.third_task.Test <figure type> <figure property 1> <figure property 2>");
        }
    }
}