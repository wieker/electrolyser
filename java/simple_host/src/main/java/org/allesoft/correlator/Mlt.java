package org.allesoft.correlator;

public class Mlt {
    public static void main2(String[] args) {
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


    public static void main(String[] args) {
        show2(0x01, 0x80);
        show2(0x3f, 0x40);
        show2(0x7f, 0x00);
        show2(0xbe, 0x3f);
        show2(0xfe, 0x7f);
        show2(0xc0, 0xbf);
        show2(0x80, 0xff);
        show2(0x41, 0xc0);
    }

    private static void show2(int value, int qv) {
//        System.out.println(String.format("%d", (0x80 - value) * (0x80 - value) +
//                (0x80 - qv) * (0x80 - qv)));
        System.out.println(String.format("%d", Math.abs(0x80 - value) +
                Math.abs(0x80 - qv)));
        System.out.println(String.format("%d", Math.abs(value) +
                Math.abs(qv)));
    }
}
