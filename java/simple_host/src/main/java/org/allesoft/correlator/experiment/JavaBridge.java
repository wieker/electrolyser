package org.allesoft.correlator.experiment;

import javax.swing.*;
import java.awt.*;
import java.awt.image.BufferedImage;
import java.util.Arrays;

import static javax.swing.WindowConstants.EXIT_ON_CLOSE;

class JavaBridge {
    static {
        System.loadLibrary("iceprog");
        //System.loadLibrary("spihost");
    }

    private native void print();
    private native void spi(byte[] data);

    //application main entry point
    public static void main(String[] args) {

        //invoke non-static print method
        new JavaBridge().print();

        JFrame frame = new JFrame("SPI dumper");

        byte[] array = new byte[2048];
        outer: while (true) {
            for (int i = 0; i < array.length; i++) {
                array[i] = (byte) (Math.random() * 256 - 128);
            }
            byte[] copy = Arrays.copyOf(array, array.length);
            new JavaBridge().spi(copy);

            for (int i = 2; i < array.length; i++) {
                if (array[i - 2] != copy[i]) {
                    System.out.printf("Assertion %d %x %x\n\n", i, array[i - 2], copy[i]);
                    continue outer;
                }
            }

            System.out.println("Arrays are identical");
        }

        /*DrawSigPanel panel = new DrawSigPanel();
        panel.array = copy;
        frame.add(panel);

        frame.setMinimumSize(new Dimension(1920, 1080));
        frame.setVisible(true);
        frame.setDefaultCloseOperation(EXIT_ON_CLOSE);*/
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