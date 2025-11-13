package org.allesoft.correlator.experiment;

import mil.nga.tiff.FileDirectory;
import mil.nga.tiff.Rasters;
import mil.nga.tiff.TIFFImage;
import mil.nga.tiff.TiffReader;
import org.ejml.data.DMatrix3;
import org.ejml.data.DMatrix3x3;
import org.ejml.data.DMatrixRMaj;
import org.ejml.dense.fixed.CommonOps_DDF3;
import org.ejml.dense.row.CommonOps_DDRM;

import javax.imageio.ImageIO;
import java.awt.*;
import java.awt.image.BufferedImage;
import java.io.*;
import java.util.Arrays;
import java.util.List;

public class Tiff2Bmp {
    public static void main(String[] args) throws Exception {
        //File input = ...
        InputStream input = new FileInputStream(
                //"/home/wieker/Downloads/darktable_exported/IMG_7030.tif"
                "/home/wieker/Pictures/colors/darktable_exported/red.tif"
        );
        //byte[] input = ...
        //ByteReader input = ...
        List<String> tiffFiles = Arrays.asList(
                "red.tif",
                "green.tif",
                "blue.tif",
                "t1.tif",
                "t2.tif",
                "t3.tif"
        );
        tiffFiles.forEach( file -> {
            File tiffFile = new File(
                    //"/home/wieker/Downloads/darktable_exported/IMG_7030.tif"
                    //"/home/wieker/Pictures/colors/darktable_exported/grey_5600K.tif"
                    //"/home/wieker/Pictures/colors/darktable_exported/red.tif"
                    //"/home/wieker/Pictures/colors/darktable_exported/green.tif"
                    //"/home/wieker/Pictures/colors/darktable_exported/blue.tif"
                    "/home/wieker/Pictures/colors2/darktable_exported/" + file
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

//        TIFFImage tiffImage = TiffReader.readTiff(input);
//        List<FileDirectory> directories = tiffImage.getFileDirectories();
//        FileDirectory directory = directories.get(0);
//        Rasters rasters = directory.readRasters();

            System.out.println(image.getHeight());
            System.out.println(image.getWidth());


            System.out.println("Supported Read Formats:");
            String[] readerFormats = ImageIO.getReaderFormatNames();
            for (String format : readerFormats) {
                System.out.println("- " + format);
            }

            System.out.println("\nSupported Write Formats:");
            String[] writerFormats = ImageIO.getWriterFormatNames();
            for (String format : writerFormats) {
                System.out.println("- " + format);
            }

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

            M.set(0, 0, 60);
            M.set(0, 1, 56);
            M.set(0, 2, 14);
            M.set(1, 0, 34);
            M.set(1, 1, 91);
            M.set(1, 2, 55);
            M.set(2, 0, 19);
            M.set(2, 1, 45);
            M.set(2, 2, 86);

            for (int i = 0; i < resizedImage.getWidth(); i++) {
                for (int j = 0; j < resizedImage.getHeight(); j++) {
                    int redVal = Short.toUnsignedInt(((short[]) image.getRaster().getDataElements(i * 2, j * 2, (Object) null))[0]) >> 8;
                    int greenVal = Short.toUnsignedInt(((short[]) image.getRaster().getDataElements(i * 2 + 1, j * 2, (Object) null))[0]) >> 8;
                    int blueVal = Short.toUnsignedInt(((short[]) image.getRaster().getDataElements(i * 2 + 1, j * 2 + 1, (Object) null))[0]) >> 8;

                    CommonOps_DDRM.solve(M, a, x);

                    a.set(0, 0, redVal);
                    a.set(1, 0, greenVal);
                    a.set(2, 0, blueVal);

                    int newRedVal = (int) (x.get(0, 0) * 255);
                    int newGreenVal = (int) (x.get(1, 0) * 255);
                    int newBlueVal = (int) (x.get(2, 0) * 255);

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
                        "/home/wieker/output-" + file + ".bmp"
                        //"/home/wieker/output-red.bmp"
                        //"/home/wieker/output-green.bmp"
                        //"/home/wieker/output-blue.bmp"
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


            int redVal = Short.toUnsignedInt(((short[]) image.getRaster().getDataElements(3000, 2000, (Object) null))[0]) >> 8;
            int greenVal = Short.toUnsignedInt(((short[]) image.getRaster().getDataElements(3000 + 1, 2000, (Object) null))[0]) >> 8;
            int blueVal = Short.toUnsignedInt(((short[]) image.getRaster().getDataElements(3000 + 1, 2000 + 1, (Object) null))[0]) >> 8;
            System.out.println("Mid point RGB = " + redVal + " " + greenVal + " " + blueVal);
            // grey 5600K:
            // Mid point RGB = 61 83 74
            // red:
            // Mid point RGB = 106 66 35
            // green:
            // Mid point RGB = 58 93 42
            // blue
            // Mid point RGB = 27 85 133

            System.out.println(27 / 4 + 58 / 2 + 106 / 6);
            System.out.println(85 / 4 + 93 / 2 + 66 / 6);
            System.out.println(133 / 4 + 42 / 2 + 35 / 6);
        });
    }
}
