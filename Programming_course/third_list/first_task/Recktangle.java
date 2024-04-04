package Programming_course.third_list.first_task;

class Rectangle extends Quadrilateral
{
    public Rectangle(double side1, double side2)
    {
        super(side1, side2, side1, side2);
    }

    public double calculate_area()
    {
        return side1*side2;
    }
    
    public double calculate_peremiter()
    {
        return 2*side1 + 2*side2;
    }

    public String name()
    {
        return "Recktangle";
    }
}