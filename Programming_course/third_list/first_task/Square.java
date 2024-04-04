package Programming_course.third_list.first_task;

class Square extends Quadrilateral
{
    public Square(double side)
    {
        super(side, side, side, side);
    }

    public double calculate_area()
    {
        return side1*side1;
    }
    public double calculate_peremiter()
    {
        return side1*5;
    }
    public String name()
    {
        return "Square";
    }
}
