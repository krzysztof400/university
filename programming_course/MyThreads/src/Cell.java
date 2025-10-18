import javafx.scene.paint.Color;
import java.util.ArrayList;

public class Cell{
    private boolean suspended = false;
    private Color color;
    private ArrayList<Cell> neighbours;
    private Object locker;
    private GUICell guiCell;

    public Cell(int size, Object locker, Color color){
       this.guiCell = new GUICell(size, color, this);
       this.color = color;
       this.neighbours = new ArrayList<>();
       this.locker = locker;
    }

    public synchronized void toggleSuspended(){
       suspended = !suspended;
    }

    public synchronized Color getColor(){
        return color;
    }

    public void setNeighbours(ArrayList<Cell> neighbours){
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

    public Cell getNeighbour(int index){
        return neighbours.get(index);
    }

    public GUICell getGUICell(){
        return guiCell;
    }

    public synchronized boolean isSuspended(){
        return suspended;
    }

    public void updateColor(int i, int j, Object locker, int K, double P, Cell[][] cells, int N, int M) {
        while (!Thread.currentThread().isInterrupted()) {
            try {
                Thread.sleep(MyRandom.getSpeed(K));
            } catch (InterruptedException e) {
                Thread.currentThread().interrupt();
                break;
            }

            synchronized (locker) {
                if (MyRandom.getProbability(P)) {
                    setColor(MyUtils.getRandomColor()); 
                } else {
                    setColor(MyUtils.getNeighbourColor(this));
                }
                MyGUIUtils.setNewColor(this);

                System.out.println("Start: " + (i * M + j));

                // try {
                //     Thread.sleep(1000);
                // } catch (InterruptedException e) {
                //     Thread.currentThread().interrupt();
                //     break;
                // }

                System.out.println("End: " + (i * M + j));
            }
        }
    }
}

