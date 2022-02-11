package org.example.rfctl;

import android.hardware.usb.UsbDevice;
import android.hardware.usb.UsbDeviceConnection;
import android.widget.TextView;

import java.util.concurrent.locks.ReentrantLock;

public class USBUtils {
    private static final ReentrantLock lock = new ReentrantLock();

    public static void start_loop(
            UsbDeviceConnection usbDeviceConnection, UsbDevice device,
            TextView textView) {
        try {
            sendCommand(usbDeviceConnection, device, 6, new byte[32]);
            sendCommand(usbDeviceConnection, device, 6, new byte[32]);
            sendCommand(usbDeviceConnection, device, 6, new byte[32]);
            sendCommand(usbDeviceConnection, device, 6, new byte[32]);
            sendCommand(usbDeviceConnection, device, 1, new byte[]{});
            int pos = 0;
            for (; true; ) {
                byte[] ch = sendCommand(usbDeviceConnection, device, 6, new byte[32]);
                for (int i = 0; i < ch[0]; i ++) {
                    String value = String.format("0x%02x ",
                            (int) ch[i + 1] & 0xFF
                    );
                    //spiDump[pos] = (int) ch[i + 1] & 0xFF;
                    // FIXME: no MT safe
                    textView.append(value);
                    if (pos % 4 < 2) {
                        //drawArea.setValue((int) ch[i + 1] & 0xFF);
                    }
                    if (pos % 4 == 3) {
                        //drawArea.setOffset((int) ch[i + 1] & 0xFF);
                    }
                    pos ++;
                    System.out.print(value);
                    if (pos % 32 == 0) {
                        System.out.println();
                        textView.append(System.lineSeparator());
                    }
                }
            }
            //sendCommand(usbDeviceConnection, device, 1, new byte[]{});
        } catch (Exception e) {
            System.out.println(e);
        }
    }

    public static byte[] sendCommand(
            UsbDeviceConnection usbDeviceConnection, UsbDevice device,
            int command, byte[] cmd) {
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
