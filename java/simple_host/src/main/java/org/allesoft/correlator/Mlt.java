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

        show(0x0f);
        show(0x4e);
        show(0x8e);
        show(0xd0);
        show(0xf1);
        show(0xb2);
        show(0x72);
        show(0x30);
    }

    private static void show(int value) {
        System.out.println(String.format("%x", value * value));
    }


    public static void main2(String[] args) {
        show2(0x10, 0x0f);
        show2(0x50, 0x4e);
        show2(0x8f, 0x8e);
        show2(0xce, 0xd0);
        show2(0xf0, 0xf1);
        show2(0xb0, 0xb2);
        show2(0x71, 0x72);
        show2(0x32, 0x30);
    }

    private static void show2(int value, int qv) {
        System.out.println(String.format("%x", value * value + qv * qv));
    }
}
