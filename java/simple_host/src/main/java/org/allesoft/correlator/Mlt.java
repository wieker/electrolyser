package org.allesoft.correlator;

public class Mlt {
    public static void main(String[] args) {
        show(0x10);
        show(0x50);
        show(0x8f);
        show(0xce);
        show(0xf0);
        show(0xb0);
        show(0x71);
        show(0x32);
    }

    private static void show(int value) {
        System.out.println(String.format("%x", value * value));
    }
}
