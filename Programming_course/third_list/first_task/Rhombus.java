package Programming_course.third_list.first_task;

class Rhombus extends Quadrilateral
{
    Rhombus(double side, double angle)
    {
        super(side, side, side, side);
        this.angle = angle;
    }

    public double calculate_area()
    {
        return Math.sin(Math.toRadians(angle))*side1*side1;
    }

    public double calculate_peremiter()
    {
        return side1*4;
    }

    public String name()
    {
        return "Rhombus";
    }
}