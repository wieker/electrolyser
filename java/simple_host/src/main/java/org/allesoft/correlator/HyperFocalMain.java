package org.allesoft.correlator;

public class HyperFocalMain {

    // a*a = b*b + c*c - 2cb*cosALPHA
    // 1: c^2 = a^2 + b^2; a1 = 2m, b1 = 7m, c1= sqrt(53)

    public static void main(String[] args) {
        double focalLength = 50.0;
        double apertureFnumber = 8;
        double circleCoC = 0.03;
        double hfocal = focalLength * focalLength / apertureFnumber / circleCoC + focalLength;

        for (int i = 0; i < 10; i ++) {
            if (i == 0) {
                System.out.println("focus at inf = inf");
                System.out.println("from H/" + (i + 1) + " = " + hfocal / 1 + " to inf");
                System.out.println();
            } else if (i == 1) {
                System.out.println("focus at H/1 = " + hfocal / 1);
                System.out.println("from H/" + (i + 1) + " = " + hfocal / 2 + " to inf");
                System.out.println();
            } else {
                System.out.println("focus at H/" + i + "= " + hfocal / i);
                System.out.println("from H/" + (i + 1) + " = " + hfocal / (i + 1) + " to " + hfocal / (i - 1));
                System.out.println();
            }
        }

        double angle = Math.toRadians(40);
        double height = 1.8;
        double c = height / Math.sin(angle);
        double b = height / Math.tan(angle);
        System.out.println("b = " + b);
        System.out.println("c = " + c);
        System.out.println("diff = " + (c - b));
    }
}
