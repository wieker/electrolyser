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

    private static void recv(DeviceHandle handle, int length) {
        ByteBuffer buffer = ByteBuffer.allocateDirect(64);
        IntBuffer transferred = IntBuffer.allocate(1);
        int transfered = LibUsb.interruptTransfer(handle, (byte) 0x81, buffer, transferred, TIMEOUT);
        if (transfered < 0) {
            throw new LibUsbException("Control transfer failed", transfered);
        }
        if (transferred.get() != 64) {
            throw new RuntimeException("Not all data was received from device");
        }

        for (int i = 0; i < length; i ++) {
            System.out.print(String.format("%02x ", buffer.get(i)));
        }
        if (length != 0) {
            System.out.println();
        }
    }

    public static void flash_we(DeviceHandle handle) {
        byte data_we[] = { 0x06 };
        spi_select(handle);
        sendCommand(handle, 4, data_we);
        spi_desel(handle);
    }

    public static void flash_wait(DeviceHandle handle) {
        byte data_wait[] = { 0x05, 0x00 };
        spi_select(handle);
        sendCommand(handle, 4, data_wait);
        spi_desel(handle);
    }

    public static void sendCommand(DeviceHandle handle, int command, byte[] cmd) {
        byte[] message = new byte[64];
        message[0] = (byte) command;
        message[1] = (byte) cmd.length;
        System.arraycopy(cmd, 0, message, 2, cmd.length);
        send(handle, message);
        recv(handle, cmd.length);
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
                        sendCommand(handle, 0, new byte[] { });
                        break;
                    case 'f':
                        sendCommand(handle, 1, new byte[] { });
                        break;
                    case 's':
                        flash_wakeup(handle);
                        flash_id(handle);
                        flash_we(handle);
                        flash_wait(handle);
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
