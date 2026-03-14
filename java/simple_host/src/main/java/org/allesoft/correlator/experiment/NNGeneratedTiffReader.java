package org.allesoft.correlator.experiment;

import org.ejml.data.DMatrixRMaj;
import org.ejml.dense.row.CommonOps_DDRM;

import javax.imageio.ImageIO;
import java.awt.*;
import java.awt.image.BufferedImage;
import java.io.File;
import java.io.IOException;

/**
 * Converts a RAW camera image (saved as 16-bit TIFF with Bayer pattern)
 * to a standard 8-bit RGB bitmap.
 *
 * Based on the example from DrawTiff.java by wieker/electrolyser.
 * Assumes RGGB Bayer pattern and uses simple nearest-neighbor demosaicing.
 */

/*
Based on the example code you provided, I'll help you create a Java program to process a RAW camera image saved as a TIFF file and convert it to a standard bitmap (like BMP or PNG). The key logic is in DrawTiff.java, which loads a 16-bit Bayer-pattern TIFF, performs simple demosaicing, applies a color correction matrix, gamma correction, and saves/views the result.

Below is a complete, self-contained Java class that encapsulates this process. It uses the EJML library for matrix operations (as in your example). You can easily adjust parameters like black level, white balance, color matrix, gamma, and output size.
Explanation of the Processing Steps

    Read TIFF – Uses ImageIO.read() which supports many TIFF variants. For 16-bit raw data, the raster typically contains short[] pixels.

    Demosaicing – Assumes an RGGB Bayer pattern and uses the simplest nearest-neighbor method:

        Red = top-left of 2x2 block

        Green = top-right

        Blue = bottom-right
        (The second green is ignored, halving the image dimensions.)

    Black Level Subtraction & Bit Shift – Subtracts a black level (e.g., 2048) and right-shifts to reduce noise/compress highlights (adjust SHIFT_CONST to your needs).

    Color Correction – Applies a 3×3 matrix to convert camera-native RGB to a standard color space (e.g., sRGB). In the example, it's a diagonal placeholder—you must calibrate this for your specific camera (or set to identity if not needed).

    Gamma Correction – Maps linear values to a display-ready curve (standard gamma ≈ 2.2).

    Clamping & Output – Ensures values stay in 0–255 range and writes the result as a standard bitmap.
 */
public class NNGeneratedTiffReader {

    // Default processing parameters (adjust as needed)
    private static final int BLACK_LEVEL = 2048;          // typical for many cameras
    private static final int SHIFT_CONST = 6;              // reduces bit depth before scaling (like exposure adjustment)
    private static final double GAMMA = 1.0 / 2.2;         // standard gamma correction

    // Color correction matrix (example: 3x3 diagonal scaling). Replace with your camera's matrix.
    // Here it's a placeholder – you should calibrate this for your specific camera.
    private static final double[][] COLOR_MATRIX = {
            {120.0, 0.0, 0.0},
            {0.0, 120.0, 0.0},
            {0.0, 0.0, 120.0}
    };

    /**
     * Processes a RAW TIFF file and returns an 8-bit RGB BufferedImage.
     *
     * @param tiffFile input TIFF file (16-bit, Bayer pattern)
     * @return processed BufferedImage (TYPE_INT_RGB)
     * @throws IOException if file cannot be read
     */
    public static BufferedImage processRawTiff(File tiffFile) throws IOException {
        // 1. Read the TIFF using ImageIO
        BufferedImage rawImage = ImageIO.read(tiffFile);
        if (rawImage == null) {
            throw new IOException("Could not read TIFF image. Ensure it's a valid 16-bit TIFF.");
        }

        int width = rawImage.getWidth();
        int height = rawImage.getHeight();

        // Ensure dimensions are even (required for Bayer pattern)
        if (width % 2 != 0 || height % 2 != 0) {
            throw new IllegalArgumentException("Image dimensions must be even for Bayer demosaicing.");
        }

        // 2. Prepare color correction matrix
        DMatrixRMaj M = new DMatrixRMaj(COLOR_MATRIX);
        DMatrixRMaj rgbVector = new DMatrixRMaj(3, 1);
        DMatrixRMaj correctedVector = new DMatrixRMaj(3, 1);

        // 3. Create output image (half size due to demosaicing)
        BufferedImage outputImage = new BufferedImage(width / 2, height / 2, BufferedImage.TYPE_INT_RGB);

        // 4. Iterate over each 2x2 Bayer block
        for (int outX = 0; outX < outputImage.getWidth(); outX++) {
            for (int outY = 0; outY < outputImage.getHeight(); outY++) {
                int inX = outX * 2;
                int inY = outY * 2;

                // Get raw pixel values (assuming RGGB pattern)
                int red   = getRawPixel(rawImage, inX,     inY);      // top-left
                int green = getRawPixel(rawImage, inX + 1, inY);      // top-right
                int blue  = getRawPixel(rawImage, inX + 1, inY + 1);  // bottom-right
                // Note: the second green (inX, inY+1) is ignored in this simple demosaic

                // Apply black level subtraction and shift
                red   = (red - BLACK_LEVEL) >> SHIFT_CONST;
                green = (green - BLACK_LEVEL) >> SHIFT_CONST;
                blue  = (blue - BLACK_LEVEL) >> SHIFT_CONST;

                // Apply color correction matrix: corrected = M^{-1} * rgb
                // In the example, they solve M * corrected = rgb. We'll do the same.
                rgbVector.set(0, 0, red);
                rgbVector.set(1, 0, green);
                rgbVector.set(2, 0, blue);
                CommonOps_DDRM.solve(M, rgbVector, correctedVector);

                // Gamma correction and scaling to 0-255
                int newRed   = gammaCorrectAndClamp(correctedVector.get(0, 0));
                int newGreen = gammaCorrectAndClamp(correctedVector.get(1, 0));
                int newBlue  = gammaCorrectAndClamp(correctedVector.get(2, 0));

                // Set pixel in output image
                outputImage.setRGB(outX, outY, new Color(newRed, newGreen, newBlue).getRGB());
            }
        }

        return outputImage;
    }

    /**
     * Extracts a 16-bit raw pixel value from a BufferedImage that contains
     * 16-bit data (likely stored as signed short in the raster).
     */
    private static int getRawPixel(BufferedImage img, int x, int y) {
        Object data = img.getRaster().getDataElements(x, y, null);
        if (data instanceof short[]) {
            // Convert signed short to unsigned int (0..65535)
            return Short.toUnsignedInt(((short[]) data)[0]);
        } else {
            throw new IllegalArgumentException("Unsupported raster data type. Expected 16-bit short array.");
        }
    }

    /**
     * Applies gamma correction and clamps the value to 0-255.
     */
    private static int gammaCorrectAndClamp(double value) {
        int result = (int) (Math.pow(value, GAMMA) * 255);
        return Math.max(0, Math.min(255, result));
    }

    // Example main method: processes a file and saves as BMP
    public static void main(String[] args) {
        if (args.length < 1) {
            System.out.println("Usage: java RawTiffToBitmap <input-tiff-file> [output-bmp-file]");
            return;
        }

        File inputFile = new File(args[0]);
        File outputFile = (args.length >= 2) ? new File(args[1]) :
                new File(inputFile.getParent(), "processed.bmp");

        try {
            BufferedImage bitmap = processRawTiff(inputFile);
            boolean saved = ImageIO.write(bitmap, "BMP", outputFile);
            if (saved) {
                System.out.println("Bitmap saved successfully to: " + outputFile.getAbsolutePath());
            } else {
                System.out.println("Failed to save bitmap (unsupported format?).");
            }
        } catch (Exception e) {
            e.printStackTrace();
        }
    }
}