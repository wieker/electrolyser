package org.allesoft.correlator.experiment;

import javax.swing.*;
import java.util.Arrays;

class JavaBridge {
    static {
        System.loadLibrary("iceprog");
        //System.loadLibrary("spihost");
    }

    public native void print();
    public native void spi(byte[] data);

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
            array[0] = -127;
            byte[] copy = Arrays.copyOf(array, array.length);
            new JavaBridge().spi(copy);

            for (int i = 18; i < array.length; i++) {
                if (array[i - 18] != copy[i]) {
                    System.out.printf("Assertion %d %x %x\n\n", i, array[i - 18], copy[i]);
                    throw new RuntimeException();
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

