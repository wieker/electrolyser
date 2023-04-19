package org.allesoft.correlator;

import org.usb4java.*;

import java.nio.ByteBuffer;
import java.nio.IntBuffer;
import java.util.concurrent.locks.ReentrantLock;

public class BMMTest
{
    private static final ReentrantLock lock = new ReentrantLock();

    private static final short VENDOR_ID = 0x6666;

    private static final short PRODUCT_ID = 0x6688;

    private static final int TIMEOUT = 0;

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
        int
                    CMD_I2C_INIT = 0x00,
                    CMD_I2C_START = 0x01,
                    CMD_I2C_STOP = 0x02,
                    CMD_I2C_READ = 0x03,
                    CMD_I2C_WRITE = 0x04,
                    CMD_I2C_PINS = 0x05;
        sendCommand(handle, 8, new byte[] { }, true);
        spi_select(handle);
        sendCommand(handle, 4, new byte[] { (byte) 0x4B, 0x01, 0x00 }, true);
        Thread.sleep(100);
        spi_desel(handle);
        spi_select(handle);
        sendCommand(handle, 4, new byte[] { (byte) 0x80 + 0x40, 0x00, 0x00 }, true);
        Thread.sleep(100);
        spi_desel(handle);


    }
    private static void spi_desel(DeviceHandle handle) {
        sendCommand(handle, 5, new byte[] { }, true);
    }

    private static void spi_select(DeviceHandle handle) {
        sendCommand(handle, 3, new byte[] { }, true);
    }

}
