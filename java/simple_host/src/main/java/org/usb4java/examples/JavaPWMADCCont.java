package org.usb4java.examples;

import org.usb4java.Device;
import org.usb4java.DeviceHandle;
import org.usb4java.LibUsb;
import org.usb4java.LibUsbException;

import static org.usb4java.examples.Ctl1.readKey;
import static org.usb4java.examples.SimplePing.findDevice;
import static org.usb4java.examples.SimplePing.sendCommand;

public class JavaPWMADCCont {
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


            while (true) {
                System.out.print("> ");
                sendCommand(handle, 2, new byte[]{ 0x00, (byte) 0x08 }, true);
                Thread.sleep(1000l);
                sendCommand(handle, 2, new byte[]{ 0x00, (byte) 0x10 }, true);
                Thread.sleep(1000l);
                sendCommand(handle, 2, new byte[]{ 0x00, (byte) 0x20 }, true);
                Thread.sleep(1000l);
                sendCommand(handle, 2, new byte[]{ 0x00, (byte) 0x40 }, true);
                Thread.sleep(1000l);
                sendCommand(handle, 2, new byte[]{ 0x00, (byte) 0x80 }, true);
                Thread.sleep(1000l);
                sendCommand(handle, 2, new byte[]{ 0x01, (byte) 0x00 }, true);
                Thread.sleep(1000l);
                sendCommand(handle, 2, new byte[]{ 0x02, (byte) 0x00 }, true);
                Thread.sleep(1000l);
                sendCommand(handle, 2, new byte[]{ 0x04, (byte) 0x00 }, true);
                Thread.sleep(1000l);
                Thread.sleep(1000l);
                }
        } catch (Exception e) {
            e.printStackTrace();
        } finally
        {
            LibUsb.close(handle);
            LibUsb.exit(null);
        }
    }
}
