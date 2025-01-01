package org.allesoft.perspective;

public class PerspectiveMain {

    // a*a = b*b + c*c - 2cb*cosALPHA
    // 1: c^2 = a^2 + b^2; a1 = 2m, b1 = 7m, c1= sqrt(53)

    public static void main(String[] args) {
        double a1 = 2;
        double b1 = 7;
        double c1 = Math.sqrt(7*7 + 2*2);
        double cosALPHA = (b1 * b1 + c1*c1 - a1*a1) / 2 / c1 / b1;
        double siALPHA = 1 - cosALPHA * cosALPHA;

        // c2*c2 - 2*b2 * cosALPHA * c2 + b2 * b2 - a2 * a2 = 0
        // c2 = b2 * cosALPHA +/- sqrt(4 * b2^2 * cosALPHA ^2 - 4 * (b2^2 - a2^2)) / 2
        // c2 = b2 * cosALPHA +/- sqrt(b2^2 * (cosALPHA ^2 - 1) + a2^2)
        // c2 = b2 * cosALPHA +/- sqrt(a2^2 - b2^2 * (1 - cosALPHA ^2))
        // c2 = b2 * cosALPHA +/- sqrt(4 - b2^2 * siALPHA)

        double c2 = 1 * cosALPHA + Math.sqrt(4 - 1 * siALPHA);
        System.out.println(c2);
        System.out.println(cosALPHA);
        double cMed = 3.5 * cosALPHA + Math.sqrt(4 - 3.5*3.5 * siALPHA);
        System.out.println("cMed = " + cMed);
        System.out.println("A = " + Math.sqrt(3.5 * 3.5 + cMed * cMed - 2 * 3.5 * cMed * cosALPHA));

        // b * b = h * h + l * l
        // c * c = (h + a)^2 + l * l
        // c * c = (h + a)^2 + b * b - h * h
        // c * c = h * h + 2 * h  * a + a * a + b * b - h * h
        // c * c = 2 * h  * a + a * a + b * b
        // c * c - a * a - b * b = 2 * h  * a
        // (c * c - a * a - b * b) / 2 / a = h
        System.out.println("hMed = " + (cMed * cMed - 2 * 2 - 3.5 * 3.5) / 2 / 2);
        System.out.println("lMed = " + Math.sqrt(3.5 * 3.5 - 2.5 * 2.5));
    }
}
