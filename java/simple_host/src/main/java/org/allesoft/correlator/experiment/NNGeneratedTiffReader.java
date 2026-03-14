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

    /*
    Hmm, the user previously got Java code to process RAW TIFFs and now wants to know how to actually get that color correction matrix - specifically asking if they can use their LCD display showing R,G,B patches. This is a practical calibration question that moves from theory to implementation.

Let me plan my answer structure. First, I need to validate whether their LCD approach works - and explain why it's actually a great idea. Then I need to provide step-by-step instructions with formulas they can implement. I'll organize this logically: concept explanation, prerequisites, capture process, matrix calculation steps, and finally integration into their Java code.

Looking at the search results, I find several highly relevant sources. A patent document explains the mathematical foundation of color correction matrices and how RGB values relate to XYZ tristimulus values
. This is authoritative and gives me the core theory. The academic paper from KCI specifically describes using polynomial regression with color chart digital values and standard tristimulus data for camera characterization

- this directly supports using display-generated colors as targets.

The MATLAB discussions are particularly useful - one user discusses the importance of linear data for matrix calculation
, which matches what the Imatest reference emphasizes about linearity. Another MATLAB thread mentions applying gamma after correction , confirming the correct order of operations. The Google Patents document about display calibration using cameras

is highly relevant as it describes the same concept of capturing test patterns from displays.

Some search results are less useful - the Magic Lantern forum discussions about LCD calibration are about adjusting displays, not camera calibration. The MatRaw and ColorChecker extraction toolboxes are relevant but more about MATLAB implementation than the core concept.

For the practical steps, I'll combine the matrix calculation theory from
with the implementation advice from about working in linear space. The LCD approach is actually validated by

which discusses using cameras to calibrate displays - the reverse process works just as well.

I need to provide clear mathematical formulas - the least squares solution for M where M * cameraRGB = targetXYZ. I'll explain that solving M = targetXYZ * pinv(cameraRGB) gives the 3x3 matrix. Then I'll show how to convert XYZ to sRGB for validation, with formulas for gamma correction and clipping.

For integration, I'll reference back to their previous code structure - replacing the placeholder diagonal matrix with the calculated one, and emphasize the importance of working on linear data before gamma.

Yes, you can absolutely use your LCD display showing pure red, green, and blue patches to calibrate a color correction matrix for your camera. This is actually a clever DIY approach to what's formally known as camera characterization

. The core idea is to establish a mathematical relationship between the RGB values your camera records and the known, "perfect" RGB values your display intended to show.

Here's the key insight: You're using your display as a known light source

. By photographing its primaries, you can calculate how to transform your camera's device-specific RGB into a standard, device-independent color space (like sRGB).
📸 Step-by-Step Calibration Process

Follow these steps to capture your data and calculate the matrix.
1. Prerequisites & Preparation

    Stable Setup: Mount your camera on a tripod facing your LCD display in a dark room. You want zero ambient light interfering.

    Linear Capture: This is critical. Configure your camera to save RAW files and ensure you convert them to TIFFs without any automatic white balance, color, or gamma correction. Your RAW processor must output linear, un-gamma-corrected data

    . Your provided DrawTiff.java code does this by subtracting a black level and working with the raw pixel values.

    Known Target Values: You need to know the exact RGB values your display is sending to its pixels. For an ideal calibration, assume the display primaries are the standard sRGB primaries:

        Red: (255, 0, 0)

        Green: (0, 255, 0)

        Blue: (0, 0, 255)

        (Optional but recommended) Gray: (255, 255, 255) and (128, 128, 128). These help with white balance and verifying linearity.

2. Capture Your Test Images

    On your LCD, display a full-screen, pure red patch (R=255, G=0, B=0).

    Take a photo with your camera, saving the RAW file.

    Repeat this process for the pure green and pure blue patches. Ensure your camera settings (aperture, shutter speed, ISO) are identical for all shots. You want the only variable to be the color on the screen.

3. Extract Linear RGB Values from RAW TIFFs

Use your Java code (or a modified version of it) to process the three TIFF files and extract the average raw pixel values from the center of each image. Remember, you must skip the gamma correction and black level subtraction steps for this calibration data.

Let's say you get these average linear values from a small patch in the center of each image:

    From the Red patch photo: Your camera records (r_cam, g_cam, b_cam) as (12000, 3000, 500)

    From the Green patch photo: Your camera records (r_cam, g_cam, b_cam) as (3500, 11000, 4000)

    From the Blue patch photo: Your camera records (r_cam, g_cam, b_cam) as (800, 4000, 13000)

These numbers are the "Response" of your camera's sensor to the known stimuli.
4. Calculate the Color Correction Matrix (CCM)

You are looking for a 3x3 matrix M that, when multiplied with your camera's response values, gives you the target display values. The equation is:

M * Camera_RGB = Target_RGB

You can solve for M using linear least squares. Here's a way to think about it with the data from step 3. We'll first normalize the target RGB values to a 0-1 range by dividing by 255. We'll also normalize the camera responses so the white balance is roughly correct. A more robust method is to use the gray patches to find scaling factors for each channel, but for a first matrix, we can work directly with the primary colors.

A simpler approach to get a first matrix is to assume your goal is to make the camera's recorded primaries match the ideal ones as closely as possible. This can be done by setting up a system of equations.

For example, for the red patch, we want:
M * [12000, 3000, 500]^T = [1, 0, 0]^T (after normalizing target red to 1)

Doing this for all three primaries gives you 9 equations (3 equations per patch). Solving this system will yield your 3x3 matrix M. This is a well-known technique in color science

.

For a practical implementation, you would assemble the camera responses into a 3x3 matrix C (where each column is the RGB response for a patch) and the target values into a 3x3 matrix T. You then solve M * C = T for M, which is M = T * inv(C).
5. Apply and Validate the Matrix

Once you have your 3x3 matrix, you can integrate it into your RawTiffToBitmap.java code. Replace the placeholder diagonal matrix with your newly calculated one. The critical step is to apply this matrix to the linear camera RGB values (after black level subtraction but before gamma correction)

.

Your processing pipeline in Java would then look like this:

    Read RAW TIFF: Get the 16-bit linear sensor data.

    Subtract Black Level: As you do with (pixel - BLACK_LEVEL).

    Apply your Calibrated Color Correction Matrix: Multiply the linear RGB values by your new 3x3 matrix M.

    Apply Gamma Correction: Finally, apply the gamma curve (e.g., value^0.45) to map the linear values to a display-ready, non-linear sRGB space

    .

💡 Tips for a Better Result

    Include Gray: Photographing a neutral gray (like 128,128,128) alongside the primaries helps you verify if your white balance is correct after applying the matrix. The R, G, and B values for the gray patch should be equal.

    LCD Limitations: Be aware that LCDs are not perfect light sources. Their color output can vary with viewing angle and brightness

. Shoot from a direct, perpendicular angle.

Polynomial Model: For even higher accuracy, you can use a polynomial model instead of a simple linear 3x3 matrix

    . This can account for some of the non-linearities in the camera and display, but a 3x3 matrix is an excellent and standard starting point.

To sum up, using your LCD to display red, green, and blue is a totally valid way to get a calibration matrix. The key is to keep everything linear, capture the data carefully, and solve for the 3x3 transformation. It's the same principle used in professional display calibration, just in reverse

.

Would you like help with the mathematical details of solving the system of equations to get your 3x3 matrix?
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