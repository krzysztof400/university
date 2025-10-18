package Programming_course.third_list.first_task;

class Hexagon implements Figure
{
    protected double side;
    
    Hexagon(double side)
    {
        if(side<=0)
        {
            throw new IllegalArgumentException("Sides of hexagon cant have nonpositive values");
        }
        this.side = side;
    }

    public double calculate_area()
    {
        return 3*Math.sqrt(3)*side*side/2;
    }

    public double calculate_peremiter()
    {
        return 6*side;
    }

    public String name()
    {
        return "Hexagon";
    }
}