package org.usb4java.examples;

import org.usb4java.Device;
import org.usb4java.DeviceHandle;
import org.usb4java.LibUsb;
import org.usb4java.LibUsbException;

import java.io.FileInputStream;
import java.io.InputStream;

import static org.usb4java.examples.Ctl1.readKey;
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
            new Thread(() -> {
                for (;;) {
                    try {
                        Thread.sleep(10l);
                        byte[] ch = sendCommand(handle, 6, new byte[]{1, 1}, false);
                        if (ch[0] > 0 && ch[1] != 0) {
                            System.out.append((char) ch[1]);
                        }
                    } catch (InterruptedException e) {
                        e.printStackTrace();
                    }
                }
            }).start();
            sendCommand(handle, 0, new byte[] { }, true);
            sendCommand(handle, 1, new byte[] { }, true);
            Thread.sleep(1000l);
            sendCommand(handle, 7, new byte[] {'R', 0x00, 0x00, 0x00, 0x05}, true);
            Thread.sleep(1000l);
            sendCommand(handle, 7, new byte[] {'R', 0x00, 0x04, 0x00, 0x05}, true);
            Thread.sleep(1000l);
            sendCommand(handle, 7, new byte[] {'W', 0x00, 0x00, 0x00, 0x05, 'a', 'b',
                    'c', 'd', 'Y'},true);
            sendCommand(handle, 7, new byte[] {'W', 0x00, 0x04, 0x00, 0x05, '1', '3',
                    '8', 'D', 'f'},true);
            sendCommand(handle, 7, new byte[] {'R', 0x00, 0x00, 0x00, 0x05}, true);
            Thread.sleep(1000l);
            sendCommand(handle, 7, new byte[] {'R', 0x00, 0x04, 0x00, 0x05}, true);
            Thread.sleep(1000l);
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
