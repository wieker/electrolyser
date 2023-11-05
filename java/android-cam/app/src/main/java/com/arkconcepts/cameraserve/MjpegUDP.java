package com.arkconcepts.cameraserve;

import android.util.Log;

import java.net.DatagramPacket;
import java.net.DatagramSocket;
import java.net.InetAddress;

public class MjpegUDP implements Runnable {
    private DatagramSocket socket;
    private String boundary = "CameraServeDataBoundary";

    public MjpegUDP(DatagramSocket socket) {
        this.socket = socket;
        Log.w("udpSender", "run: init", null);
    }

    @Override
    public void run() {
        android.os.Process.setThreadPriority(android.os.Process.THREAD_PRIORITY_BACKGROUND);

        try {
            while(true) {
                byte[] frame = MainActivity.getJpegFrame();

                Log.w("udpSender", "run: sent", null);

                if (frame != null) {
                    DatagramPacket packet = new DatagramPacket(frame, frame.length, InetAddress.getByName("192.168.43.46"), 4477);
                    socket.send(packet);
                }
            }

        } catch (Exception e) {
            Log.w("udpSender", "run: error", e);
            e.printStackTrace();
        }
    }
}
