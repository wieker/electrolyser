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
import javax.swing.JPanel;
import javax.swing.JScrollBar;
import javax.swing.JScrollPane;
import javax.swing.JTextArea;
import java.awt.Adjustable;
import java.awt.BorderLayout;
import java.awt.Color;
import java.awt.Graphics;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.event.AdjustmentEvent;
import java.awt.event.AdjustmentListener;
import java.io.FileInputStream;
import java.io.IOException;
import java.io.InputStream;
import java.nio.ByteBuffer;
import java.nio.IntBuffer;
import java.util.concurrent.locks.ReentrantLock;

import static javax.swing.WindowConstants.EXIT_ON_CLOSE;

public class TwoRxDumperLoggerXGra
{
    private static final ReentrantLock lock = new ReentrantLock();

    private static final short VENDOR_ID = 0x6666;

    private static final short PRODUCT_ID = 0x6677;
    //private static final short PRODUCT_ID = 0x6668;

    private static final int TIMEOUT = 0;
    private static MyDrawing drawArea;


    byte data_wake[] = { (byte) 0xAB, 0x00, 0x00, 0x00, 0x00, 0x00 };
    byte data[] = { (byte) 0x9F, 0x00, 0x00, 0x00, 0x00, 0x00 };
    private static JTextArea textArea;

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
        lock.lock();
        byte[] message = new byte[64];
        message[0] = (byte) command;
        message[1] = (byte) cmd.length;
        System.arraycopy(cmd, 0, message, 2, cmd.length);
        send(handle, message, print);
        byte[] recv = recv(handle, cmd.length, print);
        lock.unlock();
        return recv;
    }

    static volatile boolean running;

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

        JFrame frame = new JFrame("Power dumper");
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
                textArea.setText("");
                running = true;
                new Thread(() -> start_loop(handle)).start();
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
        rstButton.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent actionEvent) {
                sendCommand(handle, 1, new byte[]{}, true);
            }
        });
        panel.add(rstButton);
        JButton readButton = new JButton("Read Flash");
        readButton.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent actionEvent) {
                textArea.setText("");
                running = true;
                new Thread(() -> readFlashDump(handle)).start();
            }
        });
        panel.add(readButton);
        JPanel mainPanel = new JPanel();
        JScrollBar scrollBar = new JScrollBar(Adjustable.HORIZONTAL, 0, 10, 0, 256);
        mainPanel.add(scrollBar);
        scrollBar.addAdjustmentListener(adjustmentEvent -> {
            int value = adjustmentEvent.getValue();
            drawArea.setValue(spiDump[value * 4]);
            drawArea.setValue(spiDump[value * 4 + 1]);
            drawArea.setOffset(spiDump[value * 4 + 3]);
            textArea.select(value * 20 + (value - 1) / 8, value * 20 + 20 + (value - 1) / 8);
        });
        textArea = new JTextArea();
        mainPanel.setLayout(new BoxLayout(mainPanel, BoxLayout.PAGE_AXIS));
        drawArea = new MyDrawing();
        mainPanel.add(drawArea);
        mainPanel.add(new JScrollPane(textArea));
        frame.getContentPane().add(BorderLayout.CENTER, mainPanel);
        frame.setVisible(true);
        frame.setDefaultCloseOperation(EXIT_ON_CLOSE);
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
                    textArea.append(value);
                    spiDump[k] = (int) recv[q] & 0xFF;
                    if (pos % 4 > 1) {
                        drawArea.setValue((int) recv[q] & 0xFF);
                    }
                    if (pos % 4 == 1) {
                        drawArea.setOffset((int) recv[q] & 0xFF);
                    }
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
        int value;
        int save;
        int offset;
        int order = 0;

        @Override
        protected void paintComponent(Graphics graphics) {
            super.paintComponent(graphics);

            graphics.setColor(Color.RED);
            graphics.fillRect(0, 60, Math.abs(0x80 - value) * 5 + Math.abs(0x80 - save) * 5, 30);
            graphics.setColor(Color.BLACK);
            graphics.drawRect(0, 60, 256 * 5, 30);

            graphics.setColor(Color.BLUE);
            graphics.fillRect(128 * 5, 120, (0x80 - offset) * 5, 30);
            graphics.setColor(Color.RED);
            graphics.fillRect(128 * 5 - Math.abs(0x80 - offset) * 5, 120, Math.abs(0x80 - offset) * 5, 30);
            graphics.setColor(Color.BLACK);
            graphics.drawRect(0, 120, 256 * 5, 30);

            graphics.drawArc(256 * 5, 200, 150, 150, 0, 360);
            graphics.drawLine(256 * 5 + 75, 275, 256 * 5 + 75 + value - 0x80, 275 + save - 0x80);
        }

        public void setValue(int value) {
            if (order == 0) {
                save = value;
                order = 1;
                return;
            }
            order = 0;
            this.value = value;
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
            sendCommand(handle, 1, new byte[]{}, false);
            int zq = 0;
            double sum = 0;
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
                    if (pos % 4 < 2) {
                        drawArea.setValue((int) ch[i + 1] & 0xFF);
                    }
                    if (pos % 4 == 3) {
                        drawArea.setOffset((int) ch[i + 1] & 0xFF);
                    }
                    pos ++;
                    System.out.print(value);
                    if (pos % 32 == 0) {
                        System.out.println();
                        textArea.append(System.lineSeparator());
                    }
//                    sum += Math.abs(((int) ch[i + 1] & 0xFF));
//                    zq ++;
                }
//                if (zq >= 100) {
//                    System.out.println(sum / zq);
//                    zq = 0;
//                    sum = 0;
//                }
            }
            sendCommand(handle, 1, new byte[]{}, false);
        } catch (Exception e) {
            System.out.println(e);
        }
    }

    private static void start_loop_without_ctl(DeviceHandle handle) {
        try {
            long total = 0;
            long locked = 0;
            for (; running; ) {
                byte[] ch = sendCommand(handle, 6, new byte[32], false);
                for (int i = 0; i < ch[0]; i ++) {
                    System.out.print(String.format("%02x ",
                            ch[i + 1]
                    ));
                    total ++;
                    if (ch[i + 1] > 0x50) {
                        locked ++;
                    }
                }
                if (ch[0] != 0) {
                    System.out.println(String.format(" ===  %f", ((float) 100 * locked) / total));
                }
                if (total == 1000) {
                    total = 0;
                    locked = 0;
                }
                for (int i = 0; i < ch[0]; i += 3) {
                    System.out.print(String.format("%d ",
                            (((int) ch[i + 1]) - 0x20) * (((int) ch[i + 2]) - 0x20)
                    ));
                }
                if (ch[0] != 0) {
                    System.out.println(String.format(" === "));
                }
                for (int i = 0; i < ch[0]; i += 3) {
                    System.out.print(String.format("%d ",
                            (((int) ch[i + 1]) - 0x20) * (((int) ch[i + 1]) - 0x20) +
                                    (((int) ch[i + 2]) - 0x20) * (((int) ch[i + 2]) - 0x20)
                    ));
                }
                if (ch[0] != 0) {
                    System.out.println(String.format(" === "));
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
                "../../fpga/dsp/dsp_power_dumper/top.bin");
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
