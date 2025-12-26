package org.allesoft.correlator.experiment;

import java.util.Arrays;

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
        byte[] array = new byte[1024];
        for (int i = 0; i < array.length; i ++) {
            array[i] = (byte)(Math.random() * 256 - 128);
        }
        byte[] copy = Arrays.copyOf(array, 1024);
        new JavaBridge().spi(copy);

        for (int i = 8; i < array.length; i ++) {
            assert array[i - 8] == copy[i];
        }

        System.out.println("Arrays are identical");
    }
}