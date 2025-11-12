package org.allesoft.correlator.experiment;

import mil.nga.tiff.FileDirectory;
import mil.nga.tiff.Rasters;
import mil.nga.tiff.TIFFImage;
import mil.nga.tiff.TiffReader;

import javax.imageio.ImageIO;
import java.awt.image.BufferedImage;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.InputStream;
import java.util.List;

public class Tiff2Bmp {
    public static void main(String[] args) throws Exception {
        //File input = ...
        InputStream input = new FileInputStream("/home/wieker/Downloads/darktable_exported/IMG_7030.tif");
        //byte[] input = ...
        //ByteReader input = ...
        File tiffFile = new File("/home/wieker/Downloads/darktable_exported/IMG_7030.tif");


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
    }
}
