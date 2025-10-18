package Programming_course.third_list.third_task;

public abstract class Figures
{
    public enum One_par implements One_interface {
        SQUARE, PENTAGON, HEXAGON, CIRCLE;
    
        private double parameter;
    
        public void set_parameter(double parameter) {
            if (parameter <= 0) {
                throw new IllegalArgumentException("Parameters can't be non-positive");
            }
            this.parameter = parameter;
        }
    
        public double calculate_area() {
            switch (this) {
                case SQUARE:
                    return parameter * parameter;
                case PENTAGON:
                    return 0.25 * Math.sqrt(5 * (5 + 2 * Math.sqrt(5))) * parameter * parameter;
                case HEXAGON:
                    return 3 * Math.sqrt(3) * parameter * parameter / 2;
                case CIRCLE:
                    return parameter * parameter * Math.PI;
                default:
                    throw new IllegalArgumentException("Unrecognized figure while calculating one parameter area");
            }
        }
    
        public double calculate_peremiter() {
            switch (this) {
                case SQUARE:
                    return parameter * 4;
                case PENTAGON:
                    return parameter * 5;
                case HEXAGON:
                    return parameter * 6;
                case CIRCLE:
                    return parameter * 2 * Math.PI;
                default:
                    throw new IllegalArgumentException("Unrecognized figure while calculating one parameter perimeter");
            }
        }
    }
    

    public enum Two_par implements Two_interface
    {
        RECKTANGLE, RHOMBUS;

        private double parameter1, parameter2;

        public void set_parameter(double parameter1, double parameter2) {
            if (parameter1 <= 0 || parameter2<=0) {
                throw new IllegalArgumentException("Parameters can't be non-positive");
            }
            if(this == RHOMBUS && parameter2>=180)
            {
                throw new IllegalArgumentException("Angle cant be larger than 180 degrees");
            }
            this.parameter1 = parameter1;
            this.parameter2 = parameter2;
        }

        public double calculate_area()
        {
            switch(this)
            {
                case RECKTANGLE:
                    return parameter1*parameter2;
                case RHOMBUS:
                    return parameter1*Math.sin(Math.toRadians(parameter2))*parameter1;
                default:
                    throw new IllegalArgumentException("Unrecognised figure while calculating two parameter area");
            }
        }

        public double calculate_peremiter()
        {
            switch(this)
            {
                case RECKTANGLE:
                    return parameter1*2 + parameter2*2;
                case RHOMBUS:
                    return parameter1*4;
                default:
                    throw new IllegalArgumentException("Unrecognised figure while calculating two parameter perimiter");
            }           
        }
    }
}