package org.allesoft.correlator.java_c_ide;

import java.util.ArrayList;
import java.util.HashSet;
import java.util.List;
import java.util.Set;

import java.util.ArrayList;
import java.util.List;

class GaSo {


    class Ratio {
        byte[] a;
        byte[] b;
    }

    class Irratio {
        byte[] a;
    }

    /*
    1. 5 complex numerics solver with epsilon E
    2. torrelate to something
    Xcorrelate structures
    correlation
    binary stream cross correlation
    rational and algebraic values
     */
}

public class GaloisSolver {
    public static void main(String[] args) {
        System.out.println("Решение полиномиальных уравнений до 5-й степени\n");

        // Пример 1: Квадратное уравнение x² - 5x + 6 = 0 (корни 2 и 3)
        System.out.println("Пример 1: x² - 5x + 6 = 0");
        Polynomial p1 = new Polynomial(1, -5, 6);
        List<Complex> roots1 = EquationSolver.findAllRoots(p1);
        printRoots(roots1);

        // Пример 2: Кубическое уравнение x³ - 6x² + 11x - 6 = 0 (корни 1, 2, 3)
        System.out.println("\nПример 2: x³ - 6x² + 11x - 6 = 0");
        Polynomial p2 = new Polynomial(1, -6, 11, -6);
        List<Complex> roots2 = EquationSolver.findAllRoots(p2);
        printRoots(roots2);

        // Пример 3: Уравнение с комплексными корнями: x² + 1 = 0 (корни i и -i)
        System.out.println("\nПример 3: x² + 1 = 0");
        Polynomial p3 = new Polynomial(1, 0, 1);
        List<Complex> roots3 = EquationSolver.findAllRoots(p3);
        printRoots(roots3);

        // Пример 4: Уравнение 4-й степени: x⁴ - 10x² + 9 = 0
        // Корни: ±1, ±3
        System.out.println("\nПример 4: x⁴ - 10x² + 9 = 0");
        Polynomial p4 = new Polynomial(1, 0, -10, 0, 9);
        List<Complex> roots4 = EquationSolver.findAllRoots(p4);
        printRoots(roots4);

        // Пример 5: Уравнение 5-й степени с комплексными корнями
        System.out.println("\nПример 5: x⁵ - 1 = 0");
        Polynomial p5 = new Polynomial(1, 0, 0, 0, 0, -1);
        List<Complex> roots5 = EquationSolver.findAllRoots(p5);
        printRoots(roots5);
    }

    private static void printRoots(List<Complex> roots) {
        System.out.println("Найденные корни:");
        for (int i = 0; i < roots.size(); i++) {
            System.out.printf("  Корень %d: %s\n", i + 1, roots.get(i));
        }
    }
}

class EquationSolver {
    private static final double EPSILON = 1e-12;
    private static final int MAX_ITERATIONS = 100;

    // Метод Лагерра для нахождения одного корня
    public static Complex laguerreMethod(Polynomial poly, Complex initialGuess) {
        Complex z = initialGuess;
        int degree = poly.degree();

        for (int iter = 0; iter < MAX_ITERATIONS; iter++) {
            Complex f = poly.evaluate(z);
            if (f.abs() < EPSILON) break;

            Polynomial deriv = poly.derivative();
            Complex fp = deriv.evaluate(z);

            Polynomial secondDeriv = deriv.derivative();
            Complex fpp = secondDeriv.evaluate(z);

            // Вычисляем G и H
            Complex G = fp.divide(f);
            Complex H = G.multiply(G).subtract(fpp.divide(f));

            // Вычисляем дискриминант
            int n = degree;
            double temp = (n - 1) * (n * H.abs() - G.multiply(G).abs());
            Complex sqrtTerm = temp >= 0 ?
                    new Complex(Math.sqrt(temp), 0) :
                    new Complex(0, Math.sqrt(-temp));

            // Два возможных знаменателя
            Complex denominator1 = G.add(sqrtTerm);
            Complex denominator2 = G.subtract(sqrtTerm);

            // Выбираем лучший шаг
            Complex delta1 = new Complex(n, 0).divide(denominator1);
            Complex delta2 = new Complex(n, 0).divide(denominator2);

            Complex delta = delta1.abs() > delta2.abs() ? delta1 : delta2;
            z = z.subtract(delta);

            if (delta.abs() < EPSILON) break;
        }
        return z;
    }

    // Нахождение всех корней полинома
    public static List<Complex> findAllRoots(Polynomial poly) {
        List<Complex> roots = new ArrayList<>();
        Polynomial currentPoly = poly;

        int degree = poly.degree();
        for (int i = 0; i < degree; i++) {
            // Начальное приближение — случайная точка в ограниченной области
            Complex initialGuess = new Complex(
                    (Math.random() - 0.5) * 4,
                    (Math.random() - 0.5) * 4
            );

            Complex root = laguerreMethod(currentPoly, initialGuess);

            // Проверяем, не нашли ли мы уже этот корень
            boolean isNewRoot = true;
            for (Complex existingRoot : roots) {
                if (root.subtract(existingRoot).abs() < EPSILON * 100) {
                    isNewRoot = false;
                    break;
                }
            }

            if (isNewRoot) {
                roots.add(root);
                // Делим полином на (x - root)
                currentPoly = deflatePolynomial(currentPoly, root);
            }
        }

        return roots;
    }

    // Синтетическое деление полинома на (x - root) с учётом комплексных корней
    private static Polynomial deflatePolynomial(Polynomial poly, Complex root) {
        double[] coeffs = poly.getCoefficients();
        int n = coeffs.length;
        Complex[] newCoeffs = new Complex[n - 1];

        // Синтетическое деление для комплексных чисел
        newCoeffs[0] = Complex.fromReal(coeffs[0]);
        for (int i = 1; i < n - 1; i++) {
            newCoeffs[i] = Complex.fromReal(coeffs[i]).add(newCoeffs[i - 1].multiply(root));
        }

        // Преобразуем обратно в вещественные коэффициенты (если возможно)
        double[] resultCoeffs = new double[n - 1];
        for (int i = 0; i < n - 1; i++) {
            resultCoeffs[i] = newCoeffs[i].getReal();
        }

        return new Polynomial(resultCoeffs);
    }
}

class Polynomial {
    private final double[] coefficients;

    public Polynomial(double... coeffs) {
        this.coefficients = coeffs.clone();
    }

    // Вычисление значения полинома в точке (схема Горнера)
    public Complex evaluate(Complex z) {
        Complex result = Complex.fromReal(coefficients[0]);
        for (int i = 1; i < coefficients.length; i++) {
            result = result.multiply(z).add(Complex.fromReal(coefficients[i]));
        }
        return result;
    }

    // Производная полинома
    public Polynomial derivative() {
        int n = coefficients.length - 1;
        if (n == 0) return new Polynomial(0);

        double[] derivCoeffs = new double[n];
        for (int i = 0; i < n; i++) {
            derivCoeffs[i] = coefficients[i] * (n - i);
        }
        return new Polynomial(derivCoeffs);
    }

    public int degree() {
        return coefficients.length - 1;
    }

    public double[] getCoefficients() {
        return coefficients;
    }
}

class Complex {
    private final double real;
    private final double imag;

    public Complex(double real, double imag) {
        this.real = real;
        this.imag = imag;
    }

    public static Complex fromReal(double real) {
        return new Complex(real, 0);
    }

    public double getReal() { return real; }
    public double getImag() { return imag; }

    public Complex add(Complex other) {
        return new Complex(real + other.real, imag + other.imag);
    }

    public Complex subtract(Complex other) {
        return new Complex(real - other.real, imag - other.imag);
    }

    public Complex multiply(Complex other) {
        double newReal = real * other.real - imag * other.imag;
        double newImag = real * other.imag + imag * other.real;
        return new Complex(newReal, newImag);
    }

    public Complex divide(Complex other) {
        double denom = other.real * other.real + other.imag * other.imag;
        if (denom == 0) throw new ArithmeticException("Деление на ноль");
        double newReal = (real * other.real + imag * other.imag) / denom;
        double newImag = (imag * other.real - real * other.imag) / denom;
        return new Complex(newReal, newImag);
    }

    public double abs() {
        return Math.sqrt(real * real + imag * imag);
    }

    @Override
    public String toString() {
        if (Math.abs(imag) < 1e-10) return String.format("%.6f", real);
        if (Math.abs(real) < 1e-10) return String.format("%.6fi", imag);
        String sign = imag < 0 ? "-" : "+";
        return String.format("%.6f %s %.6fi", real, sign, Math.abs(imag));
    }
}

