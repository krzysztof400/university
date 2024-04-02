package Programming_course.third_list;

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
}
