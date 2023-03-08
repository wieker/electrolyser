package org.allesoft.correlator;

import org.usb4java.Device;
import org.usb4java.DeviceDescriptor;
import org.usb4java.DeviceHandle;
import org.usb4java.DeviceList;
import org.usb4java.LibUsb;
import org.usb4java.LibUsbException;

import javax.swing.BoxLayout;
import javax.swing.JButton;
import javax.swing.JFrame;
import javax.swing.JLabel;
import javax.swing.JPanel;
import javax.swing.JScrollBar;
import javax.swing.JScrollPane;
import javax.swing.JTextArea;
import java.awt.Adjustable;
import java.awt.BorderLayout;
import java.awt.Color;
import java.awt.Dimension;
import java.awt.Graphics;
import java.awt.TextField;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.io.FileInputStream;
import java.io.IOException;
import java.io.InputStream;
import java.nio.ByteBuffer;
import java.nio.IntBuffer;
import java.util.concurrent.locks.ReentrantLock;

import static javax.swing.WindowConstants.EXIT_ON_CLOSE;

public class LabSigXDump
{
    private static final ReentrantLock lock = new ReentrantLock();
    private static final ReentrantLock prelock = new ReentrantLock();

    private static final short VENDOR_ID = 0x6666;

    //private static final short PRODUCT_ID = 0x6677;
    private static final short PRODUCT_ID = 0x6678;
    //private static final short PRODUCT_ID = 0x6668;

    private static final int TIMEOUT = 0;
    private static MyDrawing drawArea;
    private static JLabel label;


    byte data_wake[] = { (byte) 0xAB, 0x00, 0x00, 0x00, 0x00, 0x00 };
    byte data[] = { (byte) 0x9F, 0x00, 0x00, 0x00, 0x00, 0x00 };
    private static JTextArea textArea;
    private static JTextArea sumArea;
    private static TextField offsetV;
    private static TextField ncoDen;
    private static TextField ncoNum;
    private static TextField shift;

    public static Device findDevice() {
        DeviceList list = new DeviceList();
        int result = LibUsb.getDeviceList(null, list);
        if (result < 0) {
            throw new RuntimeException("Unable to get device list. Result=" + result);
        }

        try {
            for (Device device: list) {
                DeviceDescriptor descriptor = new DeviceDescriptor();
                result = LibUsb.getDeviceDescriptor(device, descriptor);
                if (result < 0) {
                    throw new RuntimeException("Unable to read device descriptor. Result=" + result);
                }
                if (descriptor.idVendor() == VENDOR_ID && descriptor.idProduct() == PRODUCT_ID) {
                    return device;
                }
            }
        } finally {
            LibUsb.freeDeviceList(list, true);
        }

        return null;
    }

    public static void send(DeviceHandle handle, byte[] message, boolean print) {
        ByteBuffer buffer = ByteBuffer.allocateDirect(message.length);
        buffer.put(message);
        buffer.rewind();
        IntBuffer transferred = IntBuffer.allocate(1);
        if (print) {
            printBytes(message);
        }
        int err = LibUsb.bulkTransfer(handle, (byte) 2, buffer, transferred, TIMEOUT);
        if (err < 0) {
            throw new LibUsbException("Control transfer failed", err);
        }
        if (transferred.get() != message.length) {
            throw new RuntimeException("Not all data was sent to device");
        }
    }

    private static byte[] recv(DeviceHandle handle, int length, boolean print) {
        ByteBuffer buffer = ByteBuffer.allocateDirect(64);
        IntBuffer transferred = IntBuffer.allocate(1);
        int transfered = LibUsb.bulkTransfer(handle, (byte) 0x81, buffer, transferred, TIMEOUT);
        if (transfered < 0) {
            throw new LibUsbException("Control transfer failed", transfered);
        }
        if (transferred.get() != 64) {
            throw new RuntimeException("Not all data was received from device");
        }

        byte[] bytes = new byte[length];
        for (int i = 0; i < length; i ++) {
            bytes[i] = buffer.get(i);
        }
        if (print) {
            printBytes(bytes);
        }

        return bytes;
    }

    public static void printBytes(byte[] bytes) {
        for (int i = 0; i < bytes.length; i ++) {
            System.out.print(String.format("%02x ", bytes[i]));
        }
        if (bytes.length != 0) {
            System.out.println();
        }
    }

    public static void flash_we(DeviceHandle handle) {
        byte data_we[] = { 0x06 };
        spi_select(handle);
        sendCommand(handle, 4, data_we, true);
        spi_desel(handle);
    }

    public static void flash_write(DeviceHandle handle, int addr, byte[] data) {
        byte buffer[] = new byte[4 + data.length];
        buffer[0] = 0x02;
        buffer[1] = (byte) ((addr >> 16) & 0xFF);
        buffer[2] = (byte) ((addr >> 8) & 0xFF);
        buffer[3] = (byte) ((addr >> 0) & 0xFF);
        System.arraycopy(data, 0, buffer, 4, data.length);
        spi_select(handle);
        sendCommand(handle, 4, buffer, true);
        spi_desel(handle);
    }

    public static byte[] flash_read(DeviceHandle handle, int addr) {
        byte buffer[] = new byte[4 + 33];
        buffer[0] = 0x0B;
        buffer[1] = (byte) ((addr >> 16) & 0xFF);
        buffer[2] = (byte) ((addr >> 8) & 0xFF);
        buffer[3] = (byte) ((addr >> 0) & 0xFF);
        spi_select(handle);
        byte[] recv = sendCommand(handle, 4, buffer, true);
        spi_desel(handle);
        return recv;
    }

    public static void flash_erase(DeviceHandle handle) {
        byte data[] = { (byte) 0xC7 };
        spi_select(handle);
        sendCommand(handle, 4, data, true);
        spi_desel(handle);
    }

    public static void flash_wait(DeviceHandle handle, int cond) {
        byte data_wait[] = { 0x05, 0x00 };
        byte[] result;
        do {
            spi_select(handle);
            result = sendCommand(handle, 4, data_wait, true);
            spi_desel(handle);
        } while (result[1] != cond);
    }

    public static byte[] sendCommand(DeviceHandle handle, int command, byte[] cmd, boolean print) {
        prelock.lock();
        lock.lock();
        prelock.unlock();
        try {
            byte[] message = new byte[64];
            message[0] = (byte) command;
            message[1] = (byte) cmd.length;
            System.arraycopy(cmd, 0, message, 2, cmd.length);
            send(handle, message, print);
            byte[] recv = recv(handle, cmd.length, print);
            return recv;
        } finally {
            lock.unlock();
        }
    }

    static volatile boolean running;
    static JScrollBar sendVal = new JScrollBar(Adjustable.HORIZONTAL, 128, 0, 0, 255);

    public static void main(String[] args) throws Exception {
        int result = LibUsb.init(null);
        if (result != LibUsb.SUCCESS) {
            throw new LibUsbException("Unable to initialize libusb", result);
        }

        Device device = findDevice();
        if (device == null) {
            throw new RuntimeException("Device is not found");
        }

        DeviceHandle handle = new DeviceHandle();
        result = LibUsb.open(device, handle);
        if (result != LibUsb.SUCCESS) {
            throw new LibUsbException("Unable to open USB device", result);
        }
        int attached = LibUsb.kernelDriverActive(handle, 1);
        if (attached < 0) {
            throw new LibUsbException( "Unable to check kernel driver active", result);
        }

        result = LibUsb.detachKernelDriver(handle, 0);
        if (result != LibUsb.SUCCESS && result != LibUsb.ERROR_NOT_SUPPORTED && result != LibUsb.ERROR_NOT_FOUND) {
            throw new LibUsbException("Unable to detach kernel driver", result);
        }

        result = LibUsb.claimInterface(handle, 0);
        if (result != LibUsb.SUCCESS) {
            throw new LibUsbException("Unable to claim interface", result);
        }

        JFrame frame = new JFrame("Power dumper UP5K Phase");
        JPanel panel = new JPanel();
        frame.getContentPane().add(BorderLayout.NORTH, panel);
        JButton flashButton = new JButton("Flash");
        flashButton.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent actionEvent) {
                try {
                    flash(handle);
                } catch (IOException e) {
                    throw new RuntimeException(e);
                }
            }
        });
        panel.add(flashButton);
        JButton loopButton = new JButton("Loop");
        loopButton.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent actionEvent) {
                running = true;
                new Thread(() -> {
                    while (running) {
                        try {
                            Thread.sleep(500L);
                        } catch (InterruptedException e) {
                            e.printStackTrace();
                        }
                        //send78(handle, sendVal);
                        start_loop(handle);
                        calcIQ();
                    }
                }).start();
            }
        });
        panel.add(loopButton);
        JButton stopButton = new JButton("Stop");
        stopButton.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent actionEvent) {
                running = false;
            }
        });
        panel.add(stopButton);
        JButton rstButton = new JButton("Switch RST");
        rstButton.addActionListener(actionEvent -> sendCommand(handle, 1, new byte[]{}, true));
        panel.add(rstButton);
        JButton readButton = new JButton("Read Flash");
        readButton.addActionListener(actionEvent -> {
            textArea.setText("");
            sendCommand(handle, 8, new byte[] { }, true);
            spi_select(handle);
            byte[] bytes = new byte[32];
            bytes[0] = 0x55;
            bytes[1] = (byte) 0x0a;
            //for (int i = 0; i < 16; i ++) {
                sendCommand(handle, 4, bytes, true);
            //}
            spi_desel(handle);
            sendCommand(handle, 9, new byte[] { }, true);
//running = true;
            //new Thread(() -> readFlashDump(handle)).start();
        });
        panel.add(readButton);
        JButton drawButton = new JButton("Redraw");
        drawButton.addActionListener(actionEvent -> drawArea.repaint());
        panel.add(drawButton);
        JButton calcButton = new JButton("Calc");
        calcButton.addActionListener(actionEvent -> {
            calcIQ();
        });
        panel.add(calcButton);
        JButton send78Button = new JButton("Send0x78");
        send78Button.addActionListener(actionEvent -> {
            send78(handle, sendVal);
        });
        panel.add(send78Button);
        offsetV = new TextField();
        offsetV.setText("0000000");
        offsetV.setMinimumSize(new Dimension(100, 20));
        ncoNum = new TextField();
        ncoNum.setText("00000001");
        ncoNum.setMinimumSize(new Dimension(100, 20));
        ncoDen = new TextField();
        ncoDen.setText("00000001");
        ncoDen.setMinimumSize(new Dimension(100, 20));
        shift = new TextField();
        shift.setText("0.00000000");
        panel.add(offsetV);
        panel.add(ncoNum);
        panel.add(ncoDen);
        panel.add(shift);
        JPanel mainPanel = new JPanel();
        mainPanel.add(sendVal);
        JScrollBar scrollBar = new JScrollBar(Adjustable.HORIZONTAL, 0, 10, 0, 256);
        mainPanel.add(scrollBar);
        scrollBar.addAdjustmentListener(adjustmentEvent -> {
            int value = adjustmentEvent.getValue();
            drawArea.setOffset(value);
            textArea.select(value * 10, value * 10 + 10);
        });
        label = new JLabel();
        mainPanel.add(label);
        textArea = new JTextArea();
        textArea.setRows(15);
        sumArea = new JTextArea();
        sumArea.setRows(10);
        mainPanel.setLayout(new BoxLayout(mainPanel, BoxLayout.PAGE_AXIS));
        drawArea = new MyDrawing();
        Dimension minimumSize = new Dimension();
        minimumSize.setSize(1900, 500);
        drawArea.setMinimumSize(minimumSize);
        mainPanel.add(drawArea);
        mainPanel.add(new JScrollPane(textArea));
        frame.getContentPane().add(BorderLayout.CENTER, mainPanel);
        frame.setVisible(true);
        frame.setDefaultCloseOperation(EXIT_ON_CLOSE);
    }

    private static void send78(DeviceHandle handle, JScrollBar sendVal) {
        sendCommand(handle, 7, new byte[] { (byte) sendVal.getValue() }, true);
    }

    private static void calcIQ() {
        sumArea.setText("");
        for (int i = 0; i < 2; i ++) {
            int t = 0;
            int accI = 0;
            int accQ = 0;
            for (int k = 0; k < 256; k ++) {
                int value = spiDump[i * 256 + k];
                for (int j = 7; j >= 0; j--) {
                    int bit = ((value >> j) & 0x01);
                    int sinphase = t / 1024;
                    int quad = (t + 512) % 2048 / 1024;
                    accI += sinphase == bit ? 1 : 0;
                    accQ += quad == bit ? 1 : 0;
                    t = (t + 1) % 2048;
                }
            }
            accI -= 1024;
            accQ -= 1024;
            sumArea.append("Values: I: " + accI + " Q: " + accQ + " Power: " + (Math.abs(accI) + Math.abs(accQ)) + System.lineSeparator());
        }
        drawArea.repaint();
    }

    private static int[] spiDump = new int[32 * 32];

    private static void readFlashDump(DeviceHandle handle) {
        sendCommand(handle, 8, new byte[] { }, true);
        flash_wakeup(handle);
        flash_id(handle);
        int k = 0;
        for (int i = 0; i < 4; i ++) {
            for (int j = 0; j < 8; j ++) {
                byte[] recv = flash_read(handle, 0x100000 + i * 256 + j * 32);

                int pos = 0;
                for (int q = 5; q < recv.length; q ++) {
                    // FIXME: no MT safe
                    String value = String.format("0x%02x ",
                            (int) recv[q] & 0xFF
                    );
                    // FIXME: no MT safe
                    spiDump[k] = (int) recv[q] & 0xFF;
                    pos ++;
                    k ++;
                }
                textArea.append(System.lineSeparator());

                if (!running) {
                    return;
                }
            }
        }
        sendCommand(handle, 9, new byte[] { }, true);
    }

    static class MyDrawing extends JPanel {
        int offset;

        @Override
        protected void paintComponent(Graphics graphics) {
            super.paintComponent(graphics);

            int z = 0, o = 0;

            for (int i = 0; i < 8; i ++) {
                for (int k = 0; k < 64; k ++) {
                    int value = spiDump[i * 64 + k];
                    for (int j = 7; j >= 0; j--) {
                        int bit = ((value >> (7 - j)) & 0x01);
                        if (bit == 1) {
                            graphics.setColor(Color.RED);
                            graphics.fillRect(k * 24 + j * 3, 10 + 25 * i, 5, 5);
                            o ++;
                        } else {
                            graphics.setColor(Color.BLACK);
                            graphics.fillRect(k * 24 + j * 3, 15 + 25 * i, 5, 5);
                            z ++;
                        }
                    }
                }
            }
            label.setText("zero / one: " + z + " / " + o);
        }

        private void drawArcForSample(Graphics graphics, int sample, int pos) {
            drawArc(graphics, spiDump[sample * 2], spiDump[sample * 2 + 1], pos);
        }

        private void drawArc(Graphics graphics, int Icomp, int Qcomp, int pos) {
            graphics.drawArc(256 * pos, 200, 150, 150, 0, 360);
            graphics.drawLine(256 * pos + 75, 275, 256 * pos + 75 + Icomp - 0x80, 275 + Qcomp - 0x80);
        }

        public void setOffset(int value) {
            this.offset = value;
            repaint();
        }
    }

    private static void start_loop(DeviceHandle handle) {
        try {
            sendCommand(handle, 6, new byte[32], false);
            sendCommand(handle, 6, new byte[32], false);
            sendCommand(handle, 6, new byte[32], false);
            sendCommand(handle, 6, new byte[32], false);
            int pos = 0;
            for (; running; ) {
                byte[] ch = sendCommand(handle, 6, new byte[32], false);
                for (int i = 0; i < ch[0]; i ++) {
                    String value = String.format("0x%02x ",
                            (int) ch[i + 1] & 0xFF
                    );
                    spiDump[pos] = (int) ch[i + 1] & 0xFF;
                    // FIXME: no MT safe
                    textArea.append(value);
                    pos ++;
                    System.out.print(value);
                    if (pos % 32 == 0) {
                        System.out.println();
                        textArea.append(System.lineSeparator());
                    }
                }
                if (pos == 510) {
                    break;
                }
            }
        } catch (Exception e) {
            System.out.println(e);
        }
    }

    private static int unsigned(byte signed) {
        return signed < 0 ? 256 - signed : signed;
    }

    private static void flash_id(DeviceHandle handle) {
        spi_select(handle);
        sendCommand(handle, 4, new byte[] { (byte) 0x9F, 0x00, 0x00, 0x00, 0x00, 0x00 }, true);
        spi_desel(handle);
    }

    private static void flash_wakeup(DeviceHandle handle) {
        spi_select(handle);
        sendCommand(handle, 4, new byte[] { (byte) 0xAB, 0x00, 0x00, 0x00, 0x00, 0x00 }, true);
        spi_desel(handle);
    }

    private static void spi_desel(DeviceHandle handle) {
        sendCommand(handle, 5, new byte[] { }, true);
    }

    private static void spi_select(DeviceHandle handle) {
        sendCommand(handle, 3, new byte[] { }, true);
    }

    private static void flash(DeviceHandle handle) throws IOException {
        sendCommand(handle, 8, new byte[] { }, true);
        flash_wakeup(handle);
        flash_id(handle);
        flash_we(handle);
        flash_wait(handle, 0x02);
        flash_erase(handle);
        flash_wait(handle, 0x00);

        InputStream inputStream = new FileInputStream(
                //"../../fpga/dsp/dsp_tx_android/top.bin");
                //"../../fpga/dsp/dsp_phase_dumper/top-up5k.bin");
        //"../../fpga/phase_dsp/phase_2xRx/top-up5k.bin");
        "../../fpga/phase_dsp/phase_2xRx_bitdump/top-up5k.bin");
        int addr = 0;
        byte[] buf = new byte[16];
        for (;;) {
            int size = inputStream.read(buf);
            if (size == -1) {
                break;
            }

            flash_we(handle);
            flash_wait(handle, 0x02);
            flash_write(handle, addr, buf);
            flash_wait(handle, 0x00);

            flash_read(handle, addr);
            addr += buf.length;
        }
        sendCommand(handle, 9, new byte[] { }, true);
    }
}
