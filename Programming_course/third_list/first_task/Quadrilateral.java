package Programming_course.third_list.first_task;

abstract class Quadrilateral implements Figure
{
    protected double side1, side2, side3, side4, angle;
    
    public Quadrilateral(double side1, double side2, double side3, double side4)
    {
        this.side1 = side1;
        this.side2 = side2;
        this.side3 = side3;
        this.side4 = side4;
    }
}