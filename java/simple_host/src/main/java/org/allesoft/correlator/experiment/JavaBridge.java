package org.allesoft.correlator.experiment;

class JavaBridge {
    static {
        System.loadLibrary("iceprog");
    }

    private native void print();

    //application main entry point
    public static void main(String[] args) {

        //invoke non-static print method
        new JavaBridge().print();
    }
}