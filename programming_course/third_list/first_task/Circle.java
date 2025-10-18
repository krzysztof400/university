package Programming_course.third_list.first_task;

class Circle implements Figure
{
    protected double radius;
    protected String type = "Circle";
    
    public Circle(double radius)
    {
        if(radius<=0)
        {
            throw new IllegalArgumentException("Radius cant have nonpositive value");
        }
        this.radius = radius;
    }

    public double calculate_area()
    {
        return radius*radius*Math.PI;
    }

    public double calculate_peremiter()
    {
        return radius*2*Math.PI;
    }
    
    public String name()
    {
        return type;
    }
}