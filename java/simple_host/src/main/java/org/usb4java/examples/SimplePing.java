package org.usb4java.examples;

import org.usb4java.Device;
import org.usb4java.DeviceDescriptor;
import org.usb4java.DeviceHandle;
import org.usb4java.DeviceList;
import org.usb4java.LibUsb;
import org.usb4java.LibUsbException;

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
        int err = LibUsb.interruptTransfer(handle, (byte) 2, buffer, transferred, TIMEOUT);
        if (err < 0) {
            throw new LibUsbException("Control transfer failed", err);
        }
        if (transferred.get() != message.length) {
            throw new RuntimeException("Not all data was sent to device");
        }
    }

    private static void recv(DeviceHandle handle) {
        ByteBuffer buffer = ByteBuffer.allocateDirect(64);
        IntBuffer transferred = IntBuffer.allocate(1);
        int transfered = LibUsb.interruptTransfer(handle, (byte) 0x81, buffer, transferred, TIMEOUT);
        if (transfered < 0) {
            throw new LibUsbException("Control transfer failed", transfered);
        }
        if (transferred.get() != 64) {
            throw new RuntimeException("Not all data was received from device");
        }

        System.out.println(String.format("%x", buffer.get(0)));
        System.out.println(String.format("%x", buffer.get(1)));
        System.out.println(String.format("%x", buffer.getShort()));
        int a = 0;
        System.out.println(String.format("%x", a = buffer.getInt(4)));
        for (int i = 0; i < 32; i ++) {
            System.out.println(String.format("%x", buffer.get(i)));
        }
        System.out.println("====");
        if (a == 0) {
            throw new RuntimeException("a = 0");
        }
    }

    public static void we(DeviceHandle handle, int command, int cmd) {
        byte[] message = new byte[64];
        message[0] = (byte) command;
        message[1] = 6;
        message[2] = (byte) cmd;
        message[3] = 0;
        message[4] = 0;
        message[5] = 0;
        message[6] = 0;
        message[7] = 0;
        send(handle, message);

        recv(handle);
    }

    public static void sendCommand(DeviceHandle handle, int command, int cmd) {
        byte[] message = new byte[64];
        message[0] = (byte) command;
        message[1] = 6;
        message[2] = (byte) cmd;
        message[3] = 0;
        message[4] = 0;
        message[5] = 0;
        message[6] = 0;
        message[7] = 0;
        send(handle, message);

        recv(handle);
    }

    public static void main(String[] args) {
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
                        sendCommand(handle, 0, 0x9F);
                        break;
                    case 'f':
                        sendCommand(handle, 1, 0x9F);
                        break;
                    case 's':
                        spi_select(handle);
                        flash_wakeup(handle);
                        spi_desel(handle);
                        spi_select(handle);
                        flash_id(handle);
                        spi_desel(handle);
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
        sendCommand(handle, 4, 0x9F);
    }

    private static void flash_wakeup(DeviceHandle handle) {
        sendCommand(handle, 4, 0xAB);
    }

    private static void spi_desel(DeviceHandle handle) {
        sendCommand(handle, 5, 0x9F);
    }

    private static void spi_select(DeviceHandle handle) {
        sendCommand(handle, 3, 0x9F);
    }
}
