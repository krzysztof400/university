package Programming_course.third_list;

class Hexagon implements Figure
{
    protected double side;
    
    Hexagon(double side)
    {
        this.side = side;
    }

    public double calculate_area()
    {
        return 3*Math.sqrt(3)*side*side*(1/2);
    }

    public double calculate_peremiter()
    {
        return 6*side;
    }
}