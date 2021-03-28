package org.usb4java.examples;

import org.usb4java.Device;
import org.usb4java.DeviceDescriptor;
import org.usb4java.DeviceHandle;
import org.usb4java.DeviceList;
import org.usb4java.LibUsb;
import org.usb4java.LibUsbException;

import java.io.FileInputStream;
import java.io.InputStream;
import java.nio.ByteBuffer;
import java.nio.IntBuffer;

import static org.usb4java.examples.Ctl1.readKey;

public class SimplePing
{
    private static final short VENDOR_ID = 0x6666;

    private static final short PRODUCT_ID = 0x6666;

    private static final int TIMEOUT = 0;


    byte data_wake[] = { (byte) 0xAB, 0x00, 0x00, 0x00, 0x00, 0x00 };
    byte data[] = { (byte) 0x9F, 0x00, 0x00, 0x00, 0x00, 0x00 };

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

    public static void send(DeviceHandle handle, byte[] message) {
        ByteBuffer buffer = ByteBuffer.allocateDirect(message.length);
        buffer.put(message);
        buffer.rewind();
        IntBuffer transferred = IntBuffer.allocate(1);
        printBytes(message);
        int err = LibUsb.interruptTransfer(handle, (byte) 2, buffer, transferred, TIMEOUT);
        if (err < 0) {
            throw new LibUsbException("Control transfer failed", err);
        }
        if (transferred.get() != message.length) {
            throw new RuntimeException("Not all data was sent to device");
        }
    }

    private static byte[] recv(DeviceHandle handle, int length) {
        ByteBuffer buffer = ByteBuffer.allocateDirect(64);
        IntBuffer transferred = IntBuffer.allocate(1);
        int transfered = LibUsb.interruptTransfer(handle, (byte) 0x81, buffer, transferred, TIMEOUT);
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
        printBytes(bytes);

        return bytes;
    }

    private static void printBytes(byte[] bytes) {
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
        sendCommand(handle, 4, data_we);
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
        sendCommand(handle, 4, buffer);
        spi_desel(handle);
    }

    public static void flash_read(DeviceHandle handle, int addr) {
        byte buffer[] = new byte[4 + 32];
        buffer[0] = 0x0B;
        buffer[1] = (byte) ((addr >> 16) & 0xFF);
        buffer[2] = (byte) ((addr >> 8) & 0xFF);
        buffer[3] = (byte) ((addr >> 0) & 0xFF);
        spi_select(handle);
        sendCommand(handle, 4, buffer);
        spi_desel(handle);
    }

    public static void flash_erase(DeviceHandle handle) {
        byte data[] = { (byte) 0xC7 };
        spi_select(handle);
        sendCommand(handle, 4, data);
        spi_desel(handle);
    }

    public static void flash_wait(DeviceHandle handle, int cond) {
        byte data_wait[] = { 0x05, 0x00 };
        byte[] result;
        do {
            spi_select(handle);
            result = sendCommand(handle, 4, data_wait);
            spi_desel(handle);
        } while (result[1] != cond);
    }

    public static byte[] sendCommand(DeviceHandle handle, int command, byte[] cmd) {
        byte[] message = new byte[64];
        message[0] = (byte) command;
        message[1] = (byte) cmd.length;
        System.arraycopy(cmd, 0, message, 2, cmd.length);
        send(handle, message);
        return recv(handle, cmd.length);
    }

    public static void main(String[] args) throws Exception {
        int result = LibUsb.init(null);
        if (result != LibUsb.SUCCESS) {
            throw new LibUsbException("Unable to initialize libusb", result);
        }

        Device device = findDevice();
        if (device == null) {
            System.err.println("Missile launcher not found.");
            System.exit(1);
        }

        DeviceHandle handle = new DeviceHandle();
        result = LibUsb.open(device, handle);
        if (result != LibUsb.SUCCESS) {
            throw new LibUsbException("Unable to open USB device", result);
        } try {
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


            System.out.println("WADX = Move, S = Stop, F = Fire, Q = Exit");
            boolean exit = false;
            while (!exit) {
                System.out.print("> ");
                char key = readKey();
                switch (key) {

                    case 'l':
                        sendCommand(handle, 0, new byte[] { });
                        break;
                    case 'f':
                        sendCommand(handle, 1, new byte[] { });
                        break;
                    case 's':
                        flash_wakeup(handle);
                        flash_id(handle);
                        flash_we(handle);
                        flash_wait(handle, 0x02);
                        flash_erase(handle);
                        flash_wait(handle, 0x00);

                        InputStream inputStream = new FileInputStream(
                                "/home/wieker/Projects/linux/hackrf/hackrf/firmware/ice40/hx1k-6502/icestorm/icestick_6502_top.bin");
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
                        break;
                    case 'r':
                        flash_wakeup(handle);
                        flash_id(handle);

                        for (addr = 0; addr < 32220; addr += 16) {
                            flash_read(handle, addr);
                        }
                        break;
                    case 'u':
                        byte[] ch = sendCommand(handle, 6, new byte[]{1});
                        System.out.println((char) ch[0]);
                        break;

                    case 'q':
                        exit = true;
                        break;

                    default:
                }
            }
        }
        finally
        {
            LibUsb.close(handle);
            LibUsb.exit(null);
        }
    }

    private static void flash_id(DeviceHandle handle) {
        spi_select(handle);
        sendCommand(handle, 4, new byte[] { (byte) 0x9F, 0x00, 0x00, 0x00, 0x00, 0x00 });
        spi_desel(handle);
    }

    private static void flash_wakeup(DeviceHandle handle) {
        spi_select(handle);
        sendCommand(handle, 4, new byte[] { (byte) 0xAB, 0x00, 0x00, 0x00, 0x00, 0x00 });
        spi_desel(handle);
    }

    private static void spi_desel(DeviceHandle handle) {
        sendCommand(handle, 5, new byte[] { });
    }

    private static void spi_select(DeviceHandle handle) {
        sendCommand(handle, 3, new byte[] { });
    }
}
