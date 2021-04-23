package org.usb4java.examples;

import org.usb4java.Device;
import org.usb4java.DeviceHandle;
import org.usb4java.LibUsb;
import org.usb4java.LibUsbException;

import static org.usb4java.examples.SimplePing.findDevice;
import static org.usb4java.examples.SimplePing.sendCommand;

public class JavaSWD {
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

            sendCommand(handle, 0, new byte[] { }, true);
            sendCommand(handle, 1, new byte[] { }, true);
            Thread.sleep(1000l);

            sendCommand(handle, 7, new byte[] {
                    0x01, (byte) 0x00,
                    0x02, (byte) 0x00,
                    0x03, (byte) 0x00,
                    0x04, (byte) 0x05,
                    0x05, (byte) 0x80,
                    0x06, (byte) 'C',
            }, true);
            Thread.sleep(10000l);

            for (int i = 0; i < 5; i ++) {
                sendCommand(handle, 7, new byte[]{
                        0x01, (byte) 0x00,
                        0x02, (byte) 0x00,
                        0x03, (byte) 0x00,
                        0x04, (byte) (0x05 + 20 * i),
                        0x05, (byte) 0x20,
                        0x06, (byte) 'R',
                }, true);
                Thread.sleep(1000l);
                sendCommand(handle, 6, new byte[40], true);
            }

            sendCommand(handle, 1, new byte[] { }, true);
            sendCommand(handle, 0, new byte[] { }, true);
        } catch (InterruptedException e) {
            e.printStackTrace();
        } finally
        {
            LibUsb.close(handle);
            LibUsb.exit(null);
        }
    }
}
