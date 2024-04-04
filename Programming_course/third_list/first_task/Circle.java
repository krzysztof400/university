package Programming_course.third_list.first_task;

class Circle implements Figure
{
    protected double radius;
    
    public Circle(double radius)
    {
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
        return "Circle";
    }
}