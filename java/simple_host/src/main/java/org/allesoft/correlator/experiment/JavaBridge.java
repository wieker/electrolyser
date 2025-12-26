package org.allesoft.correlator.experiment;

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
        new JavaBridge().spi("".getBytes());
        new JavaBridge().spi("".getBytes());
    }
}