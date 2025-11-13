package org.allesoft.correlator.experiment;

import mil.nga.tiff.FileDirectory;
import mil.nga.tiff.Rasters;
import mil.nga.tiff.TIFFImage;
import mil.nga.tiff.TiffReader;

import javax.imageio.ImageIO;
import java.awt.*;
import java.awt.image.BufferedImage;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.InputStream;
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
        File tiffFile = new File(
                //"/home/wieker/Downloads/darktable_exported/IMG_7030.tif"
                "/home/wieker/Pictures/colors/darktable_exported/grey_5600K.tif"
        );


        BufferedImage image = ImageIO.read(tiffFile);
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
        for (int i = 0; i < resizedImage.getWidth(); i ++) {
            for (int j = 0; j < resizedImage.getHeight(); j ++) {
                int redVal = Short.toUnsignedInt(((short[]) image.getRaster().getDataElements(i * 2, j * 2, (Object) null))[0]) >> 8;
                int greenVal = Short.toUnsignedInt(((short[]) image.getRaster().getDataElements(i * 2 + 1, j * 2, (Object) null))[0]) >> 8;
                int blueVal = Short.toUnsignedInt(((short[]) image.getRaster().getDataElements(i * 2 + 1, j * 2 + 1, (Object) null))[0]) >> 8;
                resizedImage.setRGB(i, j, new Color(
                        redVal,
                        greenVal,
                        blueVal)
                        .getRGB());
            }
        }
        boolean success = ImageIO.write(resizedImage, "BMP", new File(
                "/home/wieker/output-5600K.bmp"
        ));
        if (success) {
            System.out.println("BMP image saved successfully.");
        } else {
            System.out.println("Failed to save BMP image. Check image type compatibility.");
        }
    }
}
