import javafx.scene.shape.Rectangle;
import javafx.scene.paint.Color;
import java.util.Random;
import java.util.ArrayList;

public class Cell extends Rectangle{
    private boolean suspended = false;
    private Color color;
    private ArrayList<Cell> neighbours;
    private Object locker;
    private int i;
    private int j;

    public Cell(int size, Object locker, Color color){
       super(size, size);
       this.color = color;
       this.neighbours = new ArrayList<>();
       this.locker = locker;
       setOnMouseClicked(event -> toggleSuspended());
    }

    public Rectangle getCell(){
       return this;
    }

    public synchronized void toggleSuspended(){
       suspended = !suspended;
    }

    public Color getColor(){
        return color;
    }

    public synchronized void setNeighbours(ArrayList<Cell> neighbours){
        this.neighbours = neighbours;
    }

    public synchronized void setColor(Color color){
        synchronized(locker){
            synchronized(neighbours.get(0)){
                synchronized(neighbours.get(1)){
                    synchronized(neighbours.get(2)){
                        synchronized(neighbours.get(3)){
                            this.color = color;
                        }
                    }
                }
            }
        }
    }

    public synchronized Cell getNeighbour(int index){
        return neighbours.get(index);
    }

    public boolean isSuspended(){
        return suspended;
    }

    public void updateColor(int i, int j, Object locker, int K, double P, Random RANDOM, Cell[][] cells, int N, int M) {
        while (true) {
            try {
                Thread.sleep((long) (0.5 * K + RANDOM.nextDouble() * K));
            } catch (InterruptedException e) {
                Thread.currentThread().interrupt();
                break;
            }

            synchronized (locker) {
                // if(cells[i][j].isSuspended()) {
                //     continue;
                // }
            
                double probability = RANDOM.nextDouble();
                if (probability < P) {
                    setColor(MyUtils.getRandomColor(RANDOM)); 
                } else {
                    setColor(MyUtils.getNeighbourColor(this));
                }
                MyGUIUtils.setNewColor(this);


                System.out.println("Start: " + (i * M + j));
                // try{
                //     Thread.sleep(1000);
                // }catch(InterruptedException e){
                //     Thread.currentThread().interrupt();
                //     break;
                // }
                System.out.println("End: " + (i * M + j));
            }
        }
    }
}