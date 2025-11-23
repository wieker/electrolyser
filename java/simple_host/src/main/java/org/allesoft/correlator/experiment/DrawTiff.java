package org.allesoft.correlator.experiment;

import org.ejml.data.DMatrixRMaj;
import org.ejml.dense.row.CommonOps_DDRM;

import javax.imageio.ImageIO;
import javax.swing.*;
import java.awt.*;
import java.awt.event.MouseAdapter;
import java.awt.event.MouseEvent;
import java.awt.image.BufferedImage;
import java.awt.image.RescaleOp;
import java.io.File;
import java.io.IOException;

import static javax.swing.WindowConstants.EXIT_ON_CLOSE;

public class DrawTiff {
    public static void main(String[] args) {
        JFrame frame = new JFrame("Power dumper");


        // 2. Create the JScrollPane with the content panel
        DrawTiffPanel panel = new DrawTiffPanel();

        MouseAdapter ma;
        ma = new MouseAdapter() {

            private Point origin;

            @Override
            public void mousePressed(MouseEvent e) {
                origin = new Point(e.getPoint());
            }

            @Override
            public void mouseReleased(MouseEvent e) {
            }

            @Override
            public void mouseDragged(MouseEvent e) {
                if (origin != null) {
                    JViewport viewPort = (JViewport) SwingUtilities.getAncestorOfClass(JViewport.class, panel);
                    if (viewPort != null) {
                        int deltaX = origin.x - e.getX();
                        int deltaY = origin.y - e.getY();

                        Rectangle view = viewPort.getViewRect();
                        view.x += deltaX;
                        view.y += deltaY;

                        panel.scrollRectToVisible(view);
                    }
                }
            }

        };

        panel.addMouseListener(ma);
        panel.addMouseMotionListener(ma);

        JScrollPane scrollPane = new JScrollPane(panel);

        // Optional: Customize scroll bar policies
        scrollPane.setVerticalScrollBarPolicy(JScrollPane.VERTICAL_SCROLLBAR_ALWAYS);
        scrollPane.setHorizontalScrollBarPolicy(JScrollPane.HORIZONTAL_SCROLLBAR_ALWAYS);


        frame.add(scrollPane);

        frame.setMinimumSize(new Dimension(1920, 1080));
        frame.setVisible(true);
        frame.setDefaultCloseOperation(EXIT_ON_CLOSE);
    }
}

class DrawTiffPanel extends JPanel {
    BufferedImage resizedImage = getImageProcessingResult();

    @Override
    protected void paintComponent(Graphics g) {
        super.paintComponent(g); // Call super to ensure proper painting
        Graphics2D g2d = (Graphics2D) g;

        // Example: Draw a blue rectangle
        g2d.drawImage(resizedImage, 0, 0, this);
        // Example: Draw a blue rectangle
    }

    private BufferedImage getImageProcessingResult() {
        int SHIFT_CONST = 6;
        double gamma = 1 / 2.2;

        setPreferredSize(new Dimension(3000, 2000));

        // Cast Graphics to Graphics2D for more advanced drawing features
        DMatrixRMaj M = new DMatrixRMaj(3, 3);

        M.set(0, 0, 80);
        M.set(0, 1, 80);
        M.set(0, 2, 22);
        M.set(1, 0, 52);
        M.set(1, 1, 131);
        M.set(1, 2, 79);
        M.set(2, 0, 28);
        M.set(2, 1, 62);
        M.set(2, 2, 119);

        File tiffFile = new File(
                //"/home/wieker/Pictures/colors4/darktable_exported/" + "girl.tif"
                //"/home/wieker/Pictures/daylight0/IMG_6344.CR3.tiff"
                //"/home/wieker/Pictures/colors4/IMG_6320.CR3.tiff"
                //"/home/wieker/Pictures/colors4/IMG_6321.CR3.tiff"
                //"/home/wieker/Pictures/colors4/IMG_6322.CR3.tiff"
                "/home/wieker/Downloads/IMG_1046.CR3.tiff"
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

        int redVal_ = 0;
        int greenVal_ = 0;
        int blueVal_ = 0;
        for (int i = -16; i <= 16; i += 2) {
            for (int j = -16; j <= 16; j += 2) {
                redVal_ += (Short.toUnsignedInt(((short[]) image.getRaster().getDataElements(3000 + i, 2000 + j, (Object) null))[0]) - 2048) >> SHIFT_CONST;
                greenVal_ += (Short.toUnsignedInt(((short[]) image.getRaster().getDataElements(3000 + 1 + i, 2000 + j, (Object) null))[0]) - 2048) >> SHIFT_CONST;
                blueVal_ += (Short.toUnsignedInt(((short[]) image.getRaster().getDataElements(3000 + 1 + i, 2000 + 1 + j, (Object) null))[0]) - 2048) >> SHIFT_CONST;
            }
        }
        redVal_ = redVal_ / 17 / 17;
        greenVal_ = greenVal_ / 17 / 17;
        blueVal_ = blueVal_ / 17 / 17;
        System.out.println("Mid point RGB = " + redVal_ + " " + greenVal_ + " " + blueVal_);
        // grey 5600K:
        // Mid point RGB = 61 83 74
        // red:
        // Mid point RGB = 106 66 35
        // green:
        // Mid point RGB = 58 93 42
        // blue
        // Mid point RGB = 27 85 133

        // WB matrix apply
        // check exposure / highlights
        // demosaic as below
        // apply XYZ calibration
        // apply gamma tone curve calibration


        BufferedImage resizedImage = new BufferedImage(image.getWidth() / 2, image.getHeight() / 2, BufferedImage.TYPE_INT_RGB);
        System.out.println(image.getType());
        DMatrixRMaj a = new DMatrixRMaj(3, 1);
        DMatrixRMaj x = new DMatrixRMaj(3, 1);

        for (int i = 0; i < resizedImage.getWidth(); i++) {
            for (int j = 0; j < resizedImage.getHeight(); j++) {
                int redVal = (Short.toUnsignedInt(((short[]) image.getRaster().getDataElements(i * 2, j * 2, (Object) null))[0]) - 2048) >> SHIFT_CONST;
                int greenVal = (Short.toUnsignedInt(((short[]) image.getRaster().getDataElements(i * 2 + 1, j * 2, (Object) null))[0]) - 2048) >> SHIFT_CONST;
                int blueVal = (Short.toUnsignedInt(((short[]) image.getRaster().getDataElements(i * 2 + 1, j * 2 + 1, (Object) null))[0]) - 2048) >> SHIFT_CONST;

                CommonOps_DDRM.solve(M, a, x);

                a.set(0, 0, redVal);
                a.set(1, 0, greenVal);
                a.set(2, 0, blueVal);

                int newRedVal = (int) (Math.pow(x.get(0, 0), gamma) * 255);
                int newGreenVal = (int) (Math.pow(x.get(1, 0), gamma) * 255);
                int newBlueVal = (int) (Math.pow(x.get(2, 0), gamma) * 255);

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
        return resizedImage;
    }
}
