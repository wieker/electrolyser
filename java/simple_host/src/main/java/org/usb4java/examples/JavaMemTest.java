package org.usb4java.examples;

import org.usb4java.Device;
import org.usb4java.DeviceHandle;
import org.usb4java.LibUsb;
import org.usb4java.LibUsbException;

import static org.usb4java.examples.SimplePing.findDevice;
import static org.usb4java.examples.SimplePing.sendCommand;

public class JavaMemTest {
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

            sendCommand(handle, 0, new byte[] { }, false);
            sendCommand(handle, 1, new byte[] { }, false);
            Thread.sleep(1000l);

            readLine(handle);
            sendCommand(handle, 7, new byte[] {'W', 0x00, 0x00, 0x00, 0x01,
                    0x00},false);
            readLine(handle);
            sendCommand(handle, 7, new byte[] {'W', 0x00, 0x00, 0x00, 0x01,
                    (byte) 0xff},false);
            readLine(handle);

            sendCommand(handle, 1, new byte[] { }, false);
            sendCommand(handle, 0, new byte[] { }, false);
        } catch (InterruptedException e) {
            e.printStackTrace();
        } finally
        {
            LibUsb.close(handle);
            LibUsb.exit(null);
        }
    }

    private static void readLine(DeviceHandle handle) throws InterruptedException {
        sendCommand(handle, 7, new byte[]{'R', 0x00, 0x00, 0x00, 0x01}, false);
        Thread.sleep(1000l);
        sendCommand(handle, 6, new byte[5], true);
    }
}
