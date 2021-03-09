/*
 * Copyright (C) 2014 Klaus Reimer <k@ailis.de>
 * See LICENSE.txt for licensing information.
 */

package org.usb4java.examples;

import org.usb4java.Device;
import org.usb4java.DeviceDescriptor;
import org.usb4java.DeviceHandle;
import org.usb4java.DeviceList;
import org.usb4java.LibUsb;
import org.usb4java.LibUsbException;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.nio.ByteBuffer;
import java.nio.IntBuffer;

import static org.usb4java.examples.Ctl1.readKey;

/**
 * Controls a USB missile launcher (Only compatible with Vendor/Product
 * 1130:0202).
 * 
 * @author Klaus Reimer <k@ailis.de>
 */
public class SimplePing
{
    /** The vendor ID of the missile launcher. */
    private static final short VENDOR_ID = 0x6666;

    /** The product ID of the missile launcher. */
    private static final short PRODUCT_ID = 0x6666;

    /** The USB communication timeout. */
    private static final int TIMEOUT = 0;

    /**
     * Searches for the missile launcher device and returns it. If there are
     * multiple missile launchers attached then this simple demo only returns
     * the first one.
     * 
     * @return The missile launcher USB device or null if not found.
     */
    public static Device findMissileLauncher()
    {
        // Read the USB device list
        DeviceList list = new DeviceList();
        int result = LibUsb.getDeviceList(null, list);
        if (result < 0)
        {
            throw new RuntimeException(
                "Unable to get device list. Result=" + result);
        }

        try
        {
            // Iterate over all devices and scan for the missile launcher
            for (Device device: list)
            {
                DeviceDescriptor descriptor = new DeviceDescriptor();
                result = LibUsb.getDeviceDescriptor(device, descriptor);
                if (result < 0)
                {
                    throw new RuntimeException(
                        "Unable to read device descriptor. Result=" + result);
                }
                if (descriptor.idVendor() == VENDOR_ID
                    && descriptor.idProduct() == PRODUCT_ID) return device;
            }
        }
        finally
        {
            // Ensure the allocated device list is freed
            LibUsb.freeDeviceList(list, true);
        }

        // No missile launcher found
        return null;
    }

    /**
     * Sends a message to the missile launcher.
     * 
     * @param handle
     *            The USB device handle.
     * @param message
     *            The message to send.
     */
    public static void sendMessage(DeviceHandle handle, byte[] message)
    {
        ByteBuffer buffer = ByteBuffer.allocateDirect(message.length);
        buffer.put(message);
        buffer.rewind();
        IntBuffer transferred = IntBuffer.allocate(19);
        int transfered = LibUsb.interruptTransfer(handle, (byte) 2, buffer,
                transferred, TIMEOUT);
        if (transfered < 0)
            throw new LibUsbException("Control transfer failed", transfered);
        if (transferred.get() != message.length)
            throw new RuntimeException("Not all data was sent to device");
    }

    /**
     * Sends a command to the missile launcher.
     * 
     * @param handle
     *            The USB device handle.
     * @param command
     *            The command to send.
     */
    public static void sendCommand(DeviceHandle handle, int command)
    {
        byte[] message = new byte[64];
        message[0] = (byte) command;
        message[1] = 0;
        message[2] = 0;
        message[3] = 1;
        message[4] = 0;
        message[5] = 0;
        message[6] = 0;
        message[7] = 0;
        sendMessage(handle, message);

        receiveData(handle);
    }

    private static void receiveData(DeviceHandle handle) {
            ByteBuffer buffer = ByteBuffer.allocateDirect(64);
            IntBuffer transferred = IntBuffer.allocate(1);
            int transfered = LibUsb.interruptTransfer(handle, (byte) 0x81, buffer,
                    transferred, TIMEOUT);
            if (transfered < 0)
                throw new LibUsbException("Control transfer failed", transfered);
            if (transferred.get() != 64)
                throw new RuntimeException("Not all data was received from device");

        System.out.println(String.format("%x", buffer.get(0)));
        System.out.println(String.format("%x", buffer.get(1)));
        System.out.println(String.format("%x", buffer.getShort()));
        int a = 0;
        System.out.println(String.format("%x", a = buffer.getInt(4)));
        System.out.println("====");
        if (a == 0) {
            throw new RuntimeException("a = 0");
        }
    }

    /**
     * Main method.
     * 
     * @param args
     *            Command-line arguments (Ignored)
     */
    public static void main(String[] args)
    {
        // Initialize the libusb context
        int result = LibUsb.init(null);
        if (result != LibUsb.SUCCESS)
        {
            throw new LibUsbException("Unable to initialize libusb", result);
        }

        // Search for the missile launcher USB device and stop when not found
        Device device = findMissileLauncher();
        if (device == null)
        {
            System.err.println("Missile launcher not found.");
            System.exit(1);
        }

        // Open the device
        DeviceHandle handle = new DeviceHandle();
        result = LibUsb.open(device, handle);
        if (result != LibUsb.SUCCESS)
        {
            throw new LibUsbException("Unable to open USB device", result);
        }
        try
        {
            // Check if kernel driver is attached to the interface
            int attached = LibUsb.kernelDriverActive(handle, 1);
            if (attached < 0)
            {
                throw new LibUsbException(
                    "Unable to check kernel driver active", result);
            }

            // Detach kernel driver from interface 0 and 1. This can fail if
            // kernel is not attached to the device or operating system
            // doesn't support this operation. These cases are ignored here.
            result = LibUsb.detachKernelDriver(handle, 0);
            if (result != LibUsb.SUCCESS &&
                result != LibUsb.ERROR_NOT_SUPPORTED &&
                result != LibUsb.ERROR_NOT_FOUND)
            {
                throw new LibUsbException("Unable to detach kernel driver",
                    result);
            }

            // Claim interface
            result = LibUsb.claimInterface(handle, 0);
            if (result != LibUsb.SUCCESS)
            {
                throw new LibUsbException("Unable to claim interface", result);
            }


            // Read commands and execute them
            System.out.println("WADX = Move, S = Stop, F = Fire, Q = Exit");
            boolean exit = false;
            while (!exit)
            {
                System.out.print("> ");
                char key = readKey();
                switch (key)
                {

                    case 'l':
                        sendCommand(handle, 0);
                        break;
                    case 'f':
                        sendCommand(handle, 1);
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
}
