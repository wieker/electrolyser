package org.allesoft.correlator.experiment;

import javax.swing.*;
import java.awt.*;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.util.Arrays;

import static javax.swing.WindowConstants.EXIT_ON_CLOSE;

class JavaBridgeDraw {
    //application main entry point
    public static void main(String[] args) {

        //invoke non-static print method
        new JavaBridge().print();

        JFrame frame = new JFrame("SPI dumper");


        DrawSigPanel panel = new DrawSigPanel();
        panel.array = new byte[2048];
        frame.add(panel);
        panel.setLayout(null);

        JButton testButton = new JButton("Read dump");
        panel.add(testButton);
        testButton.setBounds(900, 0, 100, 100);
        testButton.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent actionEvent) {
                byte[] array = new byte[2048];
                array[0] = -127;
                byte[] copy = Arrays.copyOf(array, array.length);
                new JavaBridge().spi(copy);
                panel.array = copy;
                panel.repaint();
            }
        });

        JButton flashButton = new JButton("Flash");
        panel.add(flashButton);
        flashButton.setBounds(1000, 0, 100, 100);
        flashButton.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent actionEvent) {
                new JavaBridge().print();
            }
        });

        JButton runLVDSButton = new JButton("Run LVDS");
        panel.add(runLVDSButton);
        runLVDSButton.setBounds(1100, 0, 100, 100);
        runLVDSButton.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent actionEvent) {
                byte[] array = new byte[6];
                array[0] = -127;
                array[2] = (byte) 0x01;
                array[3] = 4;
                array[4] = (byte) 0x01;
                array[5] = (byte) 0x1a;
                byte[] copy = Arrays.copyOf(array, array.length);
                new JavaBridge().spi(copy);
            }
        });

        frame.setMinimumSize(new Dimension(1920, 1080));
        frame.setVisible(true);
        frame.setDefaultCloseOperation(EXIT_ON_CLOSE);
    }
}

class DrawSigPanel extends JPanel {
    public byte[] array;

    @Override
    protected void paintComponent(Graphics g) {
        super.paintComponent(g); // Call super to ensure proper painting
        Graphics2D g2d = (Graphics2D) g;

        for (int i = 0; i < 20; i ++) {
            for (int j = 0; j < 20; j ++) {
                int t = Byte.toUnsignedInt(array[i * 20 + j]);
                for (int k = 0; k < 8; k ++) {
                    if ((t & 0x01) == 1) {
                        g2d.setColor(Color.GREEN);
                    } else {
                        g2d.setColor(Color.BLACK);
                    }
                    g2d.fillRect((j * 8 + k) * 5, i * 5, 5, 5);
                    t = t >> 1;
                }
            }
        }
    }
}