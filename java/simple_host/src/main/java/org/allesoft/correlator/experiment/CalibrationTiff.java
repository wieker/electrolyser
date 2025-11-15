package org.allesoft.correlator.experiment;

import org.ejml.data.DMatrixRMaj;
import org.ejml.dense.row.CommonOps_DDRM;

import javax.imageio.ImageIO;
import java.awt.*;
import java.awt.image.BufferedImage;
import java.io.File;
import java.io.FileInputStream;
import java.io.IOException;
import java.io.InputStream;
import java.util.Arrays;
import java.util.List;

public class CalibrationTiff {
    public static void main(String[] args) throws Exception {
        List<String> tiffFiles = Arrays.asList(
                "red.tif",
                "green.tif",
                "blue.tif",
                "grey1.tif",
                "grey2.tif",
                "yellow.tif",
                "magenta.tif",
                "cyan.tif"
        );
        tiffFiles.forEach( file -> {
            File tiffFile = new File(
                    "/home/wieker/Pictures/colors4/darktable_exported/" + file
            );


            BufferedImage image = null;
            try {
                image = ImageIO.read(tiffFile);
            } catch (IOException e) {
                throw new RuntimeException(e);
            }
            if (image != null) {
                System.out.println("TIFF image read successfully.");
                // You can now process or display the BufferedImage
            } else {
                System.out.println("Could not read TIFF image with ImageIO.");
            }

            System.out.println(image.getHeight());
            System.out.println(image.getWidth());

            // WB matrix apply
            // check exposure / highlights
            // demosaic as below
            // apply XYZ calibration
            // apply gamma tone curve calibration


            BufferedImage resizedImage = new BufferedImage(image.getWidth() / 2, image.getHeight() / 2, BufferedImage.TYPE_INT_RGB);
            System.out.println(image.getType());
            DMatrixRMaj M = new DMatrixRMaj(3, 3);
            DMatrixRMaj a = new DMatrixRMaj(3, 1);
            DMatrixRMaj x = new DMatrixRMaj(3, 1);

            M.set(0, 0, 80);
            M.set(0, 1, 80);
            M.set(0, 2, 22);
            M.set(1, 0, 52);
            M.set(1, 1, 131);
            M.set(1, 2, 79);
            M.set(2, 0, 28);
            M.set(2, 1, 62);
            M.set(2, 2, 119);

            for (int i = 0; i < resizedImage.getWidth(); i++) {
                for (int j = 0; j < resizedImage.getHeight(); j++) {
                    int redVal = Short.toUnsignedInt(((short[]) image.getRaster().getDataElements(i * 2, j * 2, (Object) null))[0]) >> 8;
                    int greenVal = Short.toUnsignedInt(((short[]) image.getRaster().getDataElements(i * 2 + 1, j * 2, (Object) null))[0]) >> 8;
                    int blueVal = Short.toUnsignedInt(((short[]) image.getRaster().getDataElements(i * 2 + 1, j * 2 + 1, (Object) null))[0]) >> 8;

                    CommonOps_DDRM.solve(M, a, x);

                    a.set(0, 0, redVal);
                    a.set(1, 0, greenVal);
                    a.set(2, 0, blueVal);

                    int newRedVal = (int) (Math.pow(x.get(0, 0), 1/2.2) * 255);
                    int newGreenVal = (int) (Math.pow(x.get(1, 0), 1/2.2) * 255);
                    int newBlueVal = (int) (Math.pow(x.get(2, 0), 1/2.2) * 255);

                    if (i == 1500 && j == 1000) {
                        System.out.println("grey val = " + newRedVal + " " + newGreenVal + " " + newBlueVal);
                    }

                    if (newRedVal < 0) {
                        newRedVal = 0;
                    }

                    if (newRedVal > 255) {
                        newRedVal = 255;
                    }

                    if (newGreenVal < 0) {
                        newGreenVal = 0;
                    }

                    if (newGreenVal > 255) {
                        newGreenVal = 255;
                    }

                    if (newBlueVal < 0) {
                        newBlueVal = 0;
                    }

                    if (newBlueVal > 255) {
                        newBlueVal = 255;
                    }

                    if (newRedVal < 0 || newRedVal > 255 ||
                            newGreenVal < 0 || newGreenVal > 255 ||
                            newBlueVal < 0 || newBlueVal > 255
                    ) {
                        resizedImage.setRGB(i, j, new Color(
                                0,
                                0,
                                0)
                                .getRGB());
                    } else {
                        resizedImage.setRGB(i, j, new Color(
                                newRedVal,
                                newGreenVal,
                                newBlueVal)
                                .getRGB());
                    }
                }
            }
            boolean success = false;
            try {
                success = ImageIO.write(resizedImage, "BMP", new File(
                        "/home/wieker/calib-output-" + file + ".bmp"
                ));
            } catch (IOException e) {
                throw new RuntimeException(e);
            }
            if (success) {
                System.out.println("BMP image saved successfully.");
            } else {
                System.out.println("Failed to save BMP image. Check image type compatibility.");
            }

            //CommonOps_DDF3.invert()

            int redVal = 0;
            int greenVal = 0;
            int blueVal = 0;
            for (int i = -16; i <= 16; i += 2) {
                for (int j = -16; j <= 16; j += 2) {
                    redVal += Short.toUnsignedInt(((short[]) image.getRaster().getDataElements(3000 + i, 2000 + j, (Object) null))[0]) >> 8;
                    greenVal += Short.toUnsignedInt(((short[]) image.getRaster().getDataElements(3000 + 1 + i, 2000 + j, (Object) null))[0]) >> 8;
                    blueVal += Short.toUnsignedInt(((short[]) image.getRaster().getDataElements(3000 + 1 + i, 2000 + 1 + j, (Object) null))[0]) >> 8;
                }
            }
            redVal = redVal / 17 / 17;
            greenVal = greenVal / 17 / 17;
            blueVal = blueVal / 17 / 17;
            System.out.println("Mid point RGB = " + redVal + " " + greenVal + " " + blueVal);
            // grey 5600K:
            // Mid point RGB = 61 83 74
            // red:
            // Mid point RGB = 106 66 35
            // green:
            // Mid point RGB = 58 93 42
            // blue
            // Mid point RGB = 27 85 133
        });
    }
}
