package org.example.rfctl;

import android.hardware.usb.UsbDevice;
import android.hardware.usb.UsbDeviceConnection;

import java.util.concurrent.locks.ReentrantLock;

public class USBUtils {
    private static final ReentrantLock lock = new ReentrantLock();

    public static byte[] sendCommand(
            UsbDeviceConnection usbDeviceConnection, UsbDevice device,
            int command, byte[] cmd, boolean print) {
        lock.lock();
        byte[] message = new byte[64];
        message[0] = (byte) command;
        message[1] = (byte) cmd.length;
        System.arraycopy(cmd, 0, message, 2, cmd.length);
        send(usbDeviceConnection, device, message);
        byte[] recv = recv(usbDeviceConnection, device, cmd.length);
        lock.unlock();
        return recv;
    }

    public static void send(
            UsbDeviceConnection usbDeviceConnection, UsbDevice device,
            byte[] data) {

        int transferred = usbDeviceConnection.bulkTransfer(device.getInterface(0).getEndpoint(1), data, data.length, 0);

        if (transferred < 0) {
            throw new RuntimeException("Control transfer failed");
        }
        if (transferred != data.length) {
            throw new RuntimeException("Not all data was sent to device");
        }
    }

    private static byte[] recv(
            UsbDeviceConnection usbDeviceConnection, UsbDevice device,
            int length) {

        byte[] data = new byte[length];
        int transfered = usbDeviceConnection.bulkTransfer(device.getInterface(0).getEndpoint(0), data, length, 0);

        if (transfered < 0) {
            throw new RuntimeException("Control transfer failed");
        }
        if (transfered != 64) {
            throw new RuntimeException("Not all data was received from device");
        }

        return data;
    }
}
