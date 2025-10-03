package org.allesoft.correlator;

public class HyperFocalMain {

    // a*a = b*b + c*c - 2cb*cosALPHA
    // 1: c^2 = a^2 + b^2; a1 = 2m, b1 = 7m, c1= sqrt(53)

    public static void main(String[] args) {
        double focalLength = 50.0;
        double apertureFnumber = 2.8;
        double circleCoC = 0.03;
        double hfocal = focalLength * focalLength / apertureFnumber / circleCoC + focalLength;

        for (int i = 0; i < 20; i ++) {
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
                System.out.println("depth = " + ( - hfocal / (i + 1) + hfocal / (i - 1)) / 1000);
                System.out.println();
            }
            if ((hfocal / (i + 1)) / 1000 < 2.145156466669578) {
                break;
            }
        }

        double angle = Math.toRadians(40);
        double height = 1.8;
        double c = height / Math.sin(angle);
        double b = height / Math.tan(angle);
        System.out.println("b = " + b);
        System.out.println("c = " + c);
        System.out.println("diff = " + (c - b));

        angle = Math.toRadians(20);
        height = 0.9;
        c = height / Math.sin(angle);
        b = height / Math.tan(angle);
        System.out.println("b = " + b);
        System.out.println("c = " + c);
        System.out.println("diff = " + (c - b));

        System.out.println("Blur point calculation");
        double subjectDistance = 2400;
        double focusDistance = 2500;

        double cocCalculated = Math.abs((subjectDistance - focusDistance) / subjectDistance) *
                (focalLength * focalLength / (apertureFnumber * (focusDistance - focalLength)));
        double cocReCalculated = Math.abs(1.0 / focusDistance - 1.0 / subjectDistance) *
                (focalLength * focalLength / apertureFnumber);
        System.out.println("Calculated CoC: " + cocCalculated);
        System.out.println("Canon R8 Pixel size = " + (36.0 / 6000));
        System.out.println("cocReCalculated CoC: " + cocReCalculated);

        focalLength = 4.5;
        apertureFnumber = 2.2;
        cocCalculated = Math.abs((subjectDistance - focusDistance) / subjectDistance) *
                (focalLength * focalLength / (apertureFnumber * (focusDistance - focalLength)));
        System.out.println("Calculated CoC: " + cocCalculated);
        System.out.println("Honor 8 Pixel size = " + (6.0 / 4000));

        focalLength = 50.0;
        double canonR8pixelSize = (36.0 / 6000);
        double reverseFnumber = Math.abs((subjectDistance - focusDistance) / subjectDistance) *
                (focalLength * focalLength / (canonR8pixelSize * (focusDistance - focalLength)));
        System.out.println("Calculated f-number: " + reverseFnumber);

        focalLength = 50.0 / 6;
        double honor8pixelSize = (6.0 / 4000);
        reverseFnumber = Math.abs((subjectDistance - focusDistance) / subjectDistance) *
                (focalLength * focalLength / (honor8pixelSize * (focusDistance - focalLength)));
        System.out.println("Calculated f-number: " + reverseFnumber);
    }
}
