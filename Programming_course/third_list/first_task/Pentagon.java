package Programming_course.third_list.first_task;

class Pentagon implements Figure
{
    protected double side;

    public Pentagon(double side)
    {
        this.side = side;
    }

    public double calculate_area()
    {
        return 0.25 * Math.sqrt(5 * (5 + 2 * Math.sqrt(5))) * side * side;
    }

    public double calculate_peremiter()
    {
        return 5*side;
    }

    public String name()
    {
        return "Pentagon";
    }
}