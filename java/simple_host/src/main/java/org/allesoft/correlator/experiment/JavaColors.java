package org.allesoft.correlator.experiment;

import javax.swing.*;

import java.awt.*;

import static javax.swing.WindowConstants.EXIT_ON_CLOSE;

public class JavaColors {
    public static void main(String[] args) {
        JFrame frame = new JFrame("Power dumper");

        frame.add(new DrawingPanel());

        frame.setMinimumSize(new Dimension(1920, 1080));
        frame.setVisible(true);
        frame.setDefaultCloseOperation(EXIT_ON_CLOSE);
    }
}

class DrawingPanel extends JPanel {

    @Override
    protected void paintComponent(Graphics g) {
        super.paintComponent(g); // Call super to ensure proper painting

        // Cast Graphics to Graphics2D for more advanced drawing features
        Graphics2D g2d = (Graphics2D) g;

        // Example: Draw a blue rectangle
        g2d.setColor(Color.BLUE);
        g2d.fillRect(50, 50, 100, 75);

        // Example: Draw a red oval
        g2d.setColor(Color.RED);
        g2d.drawOval(150, 100, 80, 60);

        // Example: Draw a string
        g2d.setColor(Color.BLACK);
        g2d.setFont(new Font("Arial", Font.BOLD, 20));
        g2d.drawString("Hello Swing Drawing!", 20, 200);



        // Example: Draw a blue rectangle
        g2d.setColor(new Color(255, 0, 0));
        g2d.fillRect(0, 0, 1000, 500);
        // Example: Draw a blue rectangle
        g2d.setColor(new Color(0, 255, 0));
        g2d.fillRect(0, 500, 1000, 1000);
        // Example: Draw a blue rectangle
        g2d.setColor(new Color(255, 255, 0));
        g2d.fillRect(1000, 0, 2000, 500);
        // Example: Draw a blue rectangle
        g2d.setColor(new Color(255, 255, 255));
        g2d.fillRect(1000, 500, 2000, 1000);
    }
}
