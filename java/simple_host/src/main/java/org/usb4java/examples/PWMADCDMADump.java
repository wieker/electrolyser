package org.usb4java.examples;

import org.usb4java.Device;
import org.usb4java.DeviceHandle;
import org.usb4java.LibUsb;
import org.usb4java.LibUsbException;

import static org.usb4java.examples.SimplePing.findDevice;
import static org.usb4java.examples.SimplePing.printBytes;
import static org.usb4java.examples.SimplePing.sendCommand;

public class PWMADCDMADump {
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


            sendCommand(handle, 2, new byte[]{
                    (byte) 0x00, (byte) 0x80,
                    (byte) 0x00, (byte) 0x00,
                    (byte) 0x00, (byte) 0x40,
                    (byte) 0x00, (byte) 0x00,
            }, true);
            Thread.sleep(1000l);
            byte[] adc;
            int i = 2;

            adc = sendCommand(handle, 3, new byte[32], false);
            System.out.print("ADC: " + i + " ");
            printBytes(adc);
            Thread.sleep(50l);

            while (true) {
                sendCommand(handle, 5, new byte[]{
                        (byte) 0x00, (byte) 0x00,
                        (byte) i, (byte) 0x00,
                        (byte) 0x00, (byte) 0x00,
                        (byte) 0x00, (byte) 0x00,
                        (byte) 0x00, (byte) 0x00,
                        (byte) 0x00, (byte) 0x00,
                }, false);
                Thread.sleep(50l);

                adc = sendCommand(handle, 3, new byte[32], false);
                System.out.print("ADC: " + i + " ");
                printBytes(adc);
                Thread.sleep(50l);

                adc = sendCommand(handle, 3, new byte[32], false);
                System.out.print("ADC: " + i + " ");
                printBytes(adc);
                Thread.sleep(50l);

                i = (4 == i) ? 0 : i + 1;
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
