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
            String action = intent.getAction();
            if (UsbManager.ACTION_USB_DEVICE_DETACHED.equals(action)) {
                synchronized (this) {
                    UsbDevice device = (UsbDevice)intent.getParcelableExtra(UsbManager.EXTRA_DEVICE);

                    if(device != null){
                        //
                        Log.d("1","DEATTCHED-" + device);
                    }
                }
            }
//
            if (UsbManager.ACTION_USB_DEVICE_ATTACHED.equals(action)) {
                synchronized (this) {
                    TextView textView = (TextView) findViewById(R.id.text);
                    textView.setText("ATTACHED");
                    UsbDevice device = (UsbDevice)intent.getParcelableExtra(UsbManager.EXTRA_DEVICE);
                    if (intent.getBooleanExtra(UsbManager.EXTRA_PERMISSION_GRANTED, false)) {

                        if(device != null){
                            //
                            textView = (TextView) findViewById(R.id.text);
                            textView.setText(device.toString());
                            Log.d("1","ATTACHED-" + device);
                        }
                    }
                    else {
                        PendingIntent mPermissionIntent;
                        mPermissionIntent = PendingIntent.getBroadcast(MainActivity.this, 0, new Intent(ACTION_USB_PERMISSION), PendingIntent.FLAG_ONE_SHOT);
                        mUsbManager.requestPermission(device, mPermissionIntent);

                    }

                }
            }
//
            if (ACTION_USB_PERMISSION.equals(action)) {
                synchronized (this) {
                    UsbDevice device = (UsbDevice)intent.getParcelableExtra(UsbManager.EXTRA_DEVICE);
                    if (intent.getBooleanExtra(UsbManager.EXTRA_PERMISSION_GRANTED, false)) {

                        if(device != null){
                            //
                            TextView textView = (TextView) findViewById(R.id.text);
                            textView.setText(device.toString());
                            Log.d("1","PERMISSION-" + device);
                        }
                    }

                }
            }

        }
    };
    private UsbDeviceConnection usbDeviceConnection;
    private UsbDevice device;


    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        if (savedInstanceState == null) {
            getFragmentManager().beginTransaction()
                    .add(R.id.container, (android.app.Fragment) new PlaceholderFragment()).commit();
        }
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




        Button closeButton = (Button)this.findViewById(R.id.close);
        closeButton.setOnClickListener(e -> {
            try {
                for (UsbDevice device : mUsbManager.getDeviceList().values()) {
                    if (this.device == null) {
                        this.device = device;
                    }
                    if (usbDeviceConnection == null) {
                        usbDeviceConnection = mUsbManager.openDevice(device);
                        usbDeviceConnection.claimInterface(device.getInterface(0), false);
                    }
                    usbDeviceConnection.bulkTransfer(device.getInterface(0).getEndpoint(1), new byte[] {1, 0}, 2, 0);
                    usbDeviceConnection.bulkTransfer(device.getInterface(0).getEndpoint(0), new byte[64], 64, 0);
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
                start_loop(usbDeviceConnection, device, textView);
            } catch (Exception ex) {
                textView.setText(ex.toString());
            }
        });
    }

    @Override
    protected void onStart() {
        // TODO Auto-generated method stub
        super.onStart();
    }

    @Override
    protected void onDestroy() {
        // TODO Auto-generated method stub
        super.onDestroy();
        unregisterReceiver(mUsbReceiver);
    }

    @Override
    public boolean onCreateOptionsMenu(Menu menu) {

        // Inflate the menu; this adds items to the action bar if it is present.
        getMenuInflater().inflate(R.menu.main, menu);
        return true;
    }

    @Override
    public boolean onOptionsItemSelected(MenuItem item) {
        // Handle action bar item clicks here. The action bar will
        // automatically handle clicks on the Home/Up button, so long
        // as you specify a parent activity in AndroidManifest.xml.
        int id = item.getItemId();
        if (id == R.id.action_settings) {
            return true;
        }
        return super.onOptionsItemSelected(item);
    }

    /**
     * A placeholder fragment containing a simple view.
     */
    public static class PlaceholderFragment extends Fragment {

        public PlaceholderFragment() {
        }

        @Override
        public View onCreateView(LayoutInflater inflater, ViewGroup container,
                                 Bundle savedInstanceState) {
            View rootView = inflater.inflate(R.layout.fragment_main, container,
                    false);
            return rootView;
        }
    }
}