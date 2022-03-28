package org.example.rfctl;

import androidx.appcompat.app.AppCompatActivity;

import android.app.Fragment;
import android.app.PendingIntent;
import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;
import android.hardware.usb.UsbDevice;
import android.hardware.usb.UsbDeviceConnection;
import android.hardware.usb.UsbManager;
import android.os.Bundle;
import android.util.Log;
import android.view.LayoutInflater;
import android.view.Menu;
import android.view.MenuItem;
import android.view.View;
import android.view.ViewGroup;
import android.widget.Button;
import android.widget.LinearLayout;
import android.widget.SeekBar;
import android.widget.TextView;

import java.util.HashMap;
import java.util.Iterator;

import static org.example.rfctl.USBUtils.sendCommand;
import static org.example.rfctl.USBUtils.start_loop;

public class MainActivity extends AppCompatActivity {

    UsbManager mUsbManager = null;
    IntentFilter filterAttached_and_Detached = null;
    //
    private static final String ACTION_USB_PERMISSION = "tw.g35gtwcms.android.test.list_usb_otg.USB_PERMISSION";
    //
    private final BroadcastReceiver mUsbReceiver = new BroadcastReceiver() {

        public void onReceive(Context context, Intent intent) {
            try {
                String action = intent.getAction();
                if (UsbManager.ACTION_USB_DEVICE_DETACHED.equals(action)) {
                    synchronized (this) {
                        UsbDevice device = (UsbDevice) intent.getParcelableExtra(UsbManager.EXTRA_DEVICE);

                        if (device != null) {
                            //
                            Log.d("1", "DEATTCHED-" + device);
                        }
                    }
                }
//
                if (UsbManager.ACTION_USB_DEVICE_ATTACHED.equals(action)) {
                    synchronized (this) {
                        TextView textView = (TextView) findViewById(R.id.text);
                        textView.setText("ATTACHED");
                        UsbDevice device = (UsbDevice) intent.getParcelableExtra(UsbManager.EXTRA_DEVICE);
                        if (intent.getBooleanExtra(UsbManager.EXTRA_PERMISSION_GRANTED, false)) {

                            if (device != null) {
                                //
                                textView = (TextView) findViewById(R.id.text);
                                textView.setText(device.toString());
                                Log.d("1", "ATTACHED-" + device);
                            }
                        } else {
                            PendingIntent mPermissionIntent;
                            mPermissionIntent = PendingIntent.getBroadcast(MainActivity.this, 0, new Intent(ACTION_USB_PERMISSION),
                                    PendingIntent.FLAG_ONE_SHOT | PendingIntent.FLAG_IMMUTABLE);
                            mUsbManager.requestPermission(device, mPermissionIntent);

                        }

                    }
                }
//
                if (ACTION_USB_PERMISSION.equals(action)) {
                    synchronized (this) {
                        UsbDevice device = (UsbDevice) intent.getParcelableExtra(UsbManager.EXTRA_DEVICE);
                        if (intent.getBooleanExtra(UsbManager.EXTRA_PERMISSION_GRANTED, false)) {

                            if (device != null) {
                                //
                                TextView textView = (TextView) findViewById(R.id.text);
                                textView.setText(device.toString());
                                Log.d("1", "PERMISSION-" + device);
                            }
                        }

                    }
                }
            } catch (Exception ex) {
                TextView textView = (TextView) findViewById(R.id.text);
                textView.setText(ex.toString());
            }
        }
    };
    private UsbDeviceConnection usbDeviceConnection;
    private UsbDevice device;
    public static int[] buffer = new int[64];
    public static int selection;


    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        //
        mUsbManager = (UsbManager) getSystemService(Context.USB_SERVICE);
        //
        filterAttached_and_Detached = new IntentFilter(UsbManager.ACTION_USB_ACCESSORY_DETACHED);
        filterAttached_and_Detached.addAction(UsbManager.ACTION_USB_DEVICE_ATTACHED);
        filterAttached_and_Detached.addAction(ACTION_USB_PERMISSION);
        //
        registerReceiver(mUsbReceiver, filterAttached_and_Detached);
        //

        HashMap<String, UsbDevice> deviceList = mUsbManager.getDeviceList();
        Log.d("1", deviceList.size()+" USB device(s) found");
        Iterator<UsbDevice> deviceIterator = deviceList.values().iterator();
        while(deviceIterator.hasNext()){
            UsbDevice device = deviceIterator.next();
            Log.d("1","" + device);
        }




        Button resetButton = (Button)this.findViewById(R.id.reset);
        resetButton.setOnClickListener(e -> {
            try {
                for (UsbDevice device : mUsbManager.getDeviceList().values()) {
                    if (this.device == null) {
                        this.device = device;
                    }
                    if (usbDeviceConnection == null) {
                        usbDeviceConnection = mUsbManager.openDevice(device);
                        usbDeviceConnection.claimInterface(device.getInterface(0), false);
                    }
                    sendCommand(usbDeviceConnection, device, 1, new byte[]{});
                }
            } catch (Exception ex) {
                TextView textView = (TextView) findViewById(R.id.text);
                textView.setText(ex.toString());
            }
        });

        Button loopButton = (Button)this.findViewById(R.id.loop);
        loopButton.setOnClickListener(e -> {
            TextView textView = (TextView) findViewById(R.id.text);
            textView.setText("");
            try {
                start_loop(usbDeviceConnection, device, buffer);
                for (int i = 0; i < buffer.length; i ++) {
                    String value = String.format("0x%02x ",
                            buffer[i]
                    );
                    textView.append(value);
                    if (i % 8 == 7) {
                        textView.append(System.lineSeparator());
                    }
                }
            } catch (Exception ex) {
                textView.setText(ex.toString());
            }
        });


        LinearLayout layout = (LinearLayout) this.findViewById(R.id.layout_id);
        RfView rfView = new RfView(getApplicationContext());
        layout.addView(rfView);

        SeekBar seekBar = (SeekBar)this.findViewById(R.id.seekBar);
        seekBar.setMax(0);
        seekBar.setMax(15);
        seekBar.setOnSeekBarChangeListener(new SeekBar.OnSeekBarChangeListener() {
            @Override
            public void onProgressChanged(SeekBar seekBar, int progress, boolean fromUser) {
                selection = progress;
                rfView.invalidate();
            }

            @Override
            public void onStartTrackingTouch(SeekBar seekBar) {

            }

            @Override
            public void onStopTrackingTouch(SeekBar seekBar) {

            }
        });
    }

    @Override
    protected void onDestroy() {
        super.onDestroy();
        unregisterReceiver(mUsbReceiver);
    }
}