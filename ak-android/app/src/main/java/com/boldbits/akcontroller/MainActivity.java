package com.boldbits.akcontroller;

import android.app.Activity;
import android.app.Fragment;
import android.graphics.Color;
import android.os.StrictMode;
import android.util.Log;
import android.graphics.Typeface;
import android.os.Bundle;
import android.view.LayoutInflater;
import android.view.Menu;
import android.view.MenuItem;
import android.view.MotionEvent;
import android.view.View;
import android.view.ViewGroup;
import android.widget.Button;

import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.net.Socket;


public class MainActivity extends Activity {

    private static final int SERVER_PORT = 2000;
    private static final String SERVER_IP = "1.2.3.4";
    private static final int MAX_MOTOR_WIRE_VAL = 254;

    private static final String TAG = MainActivity.class.getName();

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        if (savedInstanceState == null) {
            getFragmentManager().beginTransaction()
                    .add(R.id.container, new PlaceholderFragment())
                    .commit();
        }
    }


    @Override
    public boolean onCreateOptionsMenu(Menu menu) {
        // Inflate the menu; this adds items to the action bar if it is present.
        getMenuInflater().inflate(R.menu.menu_main, menu);
        return true;
    }

    @Override
    public boolean onOptionsItemSelected(MenuItem item) {
        // Handle action bar item clicks here. The action bar will
        // automatically handle clicks on the Home/Up button, so long
        // as you specify a parent activity in AndroidManifest.xml.
        int id = item.getItemId();

        //noinspection SimplifiableIfStatement
        if (id == R.id.action_settings) {
            return true;
        }

        return super.onOptionsItemSelected(item);
    }

    /**
     * A placeholder fragment containing a simple view.
     */
    public static class PlaceholderFragment extends Fragment {

        private Button rotateLeftBtn;
        private Button rotateRightBtn;
        private Button forwardBtn;
        private Button reverseBtn;

        private volatile float leftMotor;
        private volatile float rightMotor;

        private volatile OutputStream sockOut;
        private volatile InputStream sockIn;

        public PlaceholderFragment() {
            StrictMode.ThreadPolicy policy = new
                    StrictMode.ThreadPolicy.Builder().permitAll().build();
            StrictMode.setThreadPolicy(policy);
        }

        @Override
        public View onCreateView(LayoutInflater inflater, ViewGroup container,
                                 Bundle savedInstanceState) {
            final View rootView = inflater.inflate(R.layout.fragment_main, container, false);

            Log.d(TAG, "Getting buttons");
            this.rotateLeftBtn = (Button)rootView.findViewById(R.id.rotateLeftBtn);
            this.rotateRightBtn = (Button)rootView.findViewById(R.id.rotateRightBtn);
            this.forwardBtn = (Button)rootView.findViewById(R.id.forwardBtn);
            this.reverseBtn = (Button)rootView.findViewById(R.id.reverseBtn);
            Log.d(TAG, "Setting text");

            Typeface tf = Typeface.createFromAsset(
                    rootView.getContext().getAssets(),
                    "fontawesome-webfont.ttf");

            // unicode ANTICLOCKWISE GAPPED CIRCLE ARROW
            this.rotateLeftBtn.setText("\u27F2");
            this.rotateLeftBtn.setTextSize(100);
            this.rotateLeftBtn.setTypeface(tf);

            // unicode CLOCKWISE GAPPED CIRCLE ARROW
            this.rotateRightBtn.setText("\u27F3");
            this.rotateRightBtn.setTextSize(100);
            this.rotateRightBtn.setTypeface(tf);

            // unicode UPWARD ARROW
            this.forwardBtn.setText("\u2191");
            this.forwardBtn.setTextSize(100);
            this.forwardBtn.setTypeface(tf);

            // unicode DOWNWARD ARROW
            this.reverseBtn.setText("\u2193");
            this.reverseBtn.setTextSize(100);
            this.reverseBtn.setTypeface(tf);

            this.rotateLeftBtn.setOnTouchListener(new View.OnTouchListener() {
                @Override
                public boolean onTouch(View v, MotionEvent event) {
                    switch (event.getAction()) {
                        case MotionEvent.ACTION_DOWN:
                            Log.d(TAG, "======= LEFT ACTION DOWN ======");
                            driveMotor(-0.2f, 0.2f);
                            break;
                        case MotionEvent.ACTION_UP:
                            Log.d(TAG, "=======  LEFT ACTION UP  ======");
                            driveMotor(0.0f, 0.0f);
                            break;
                    }
                    return false;
                }
            });

            this.rotateRightBtn.setOnTouchListener(new View.OnTouchListener() {
                @Override
                public boolean onTouch(View v, MotionEvent event) {
                    switch (event.getAction()) {
                        case MotionEvent.ACTION_DOWN:
                            Log.d(TAG, "======= RIGHT ACTION DOWN ======");
                            driveMotor(0.2f, -0.2f);
                            break;
                        case MotionEvent.ACTION_UP:
                            Log.d(TAG, "=======  RIGHT ACTION UP  ======");
                            driveMotor(0.0f, 0.0f);
                            break;
                    }
                    return false;
                }
            });

            this.forwardBtn.setOnTouchListener(new View.OnTouchListener() {
                @Override
                public boolean onTouch(View v, MotionEvent event) {
                    switch (event.getAction()) {
                        case MotionEvent.ACTION_DOWN:
                            Log.d(TAG, "======= FORWARD ACTION DOWN ======");
                            driveMotor(0.5f, 0.5f);
                            break;
                        case MotionEvent.ACTION_UP:
                            Log.d(TAG, "=======  FORWARD ACTION UP  ======");
                            driveMotor(0.0f, 0.0f);
                            break;
                    }
                    return false;
                }
            });

            this.reverseBtn.setOnTouchListener(new View.OnTouchListener() {
                @Override
                public boolean onTouch(View v, MotionEvent event) {
                    switch (event.getAction()) {
                        case MotionEvent.ACTION_DOWN:
                            Log.d(TAG, "======= REVERSE ACTION DOWN ======");
                            driveMotor(-0.5f, -0.5f);
                            break;
                        case MotionEvent.ACTION_UP:
                            Log.d(TAG, "=======  REVERSE ACTION UP  ======");
                            driveMotor(0.0f, 0.0f);
                            break;
                    }
                    return false;
                }
            });

            rootView.setOnTouchListener(new View.OnTouchListener() {
                @Override
                public boolean onTouch(View v, MotionEvent event) {
                    switch (event.getAction()) {
                        case MotionEvent.ACTION_DOWN:
                            Log.d(TAG, "======= VIEW ACTION DOWN ======");
                            //movedTo(event.getX(), event.getY());
                            break;
                        case MotionEvent.ACTION_MOVE:
                            Log.d(TAG, "=======  VIEW ACTION MOVE  ======");
                            //movedTo(event.getX(), event.getY());
                            break;
                        case MotionEvent.ACTION_UP:
                            Log.d(TAG, "=======  VIEW ACTION UP  ======");
                            driveMotor(0.0f, 0.0f);
                            break;
                    }
                    return true;
                }
            });

            new Thread(new Runnable() {
                @Override
                public void run() {
                    while (sockIn == null) {
                        try {
                            Socket socket = new Socket(SERVER_IP, SERVER_PORT);
                            sockOut = socket.getOutputStream();
                            sockIn = socket.getInputStream();

                            if(sockIn == null) {
                                Thread.currentThread().sleep(2000);
                            }
                        } catch (IOException e) {
                            Log.e(TAG, "failed to initialize socket", e);
                        } catch (InterruptedException e) {
                            Log.e(TAG, "failed to sleep thread", e);
                        }
                    }

                    rootView.post(new Runnable() {
                        @Override
                        public void run() {
                            rootView.setBackgroundColor(Color.GREEN);
                        }
                    });

                    try {
                        // sending 0, 0 to the motors is an initialization signal that tells AK
                        // that it should start listening to the data stream
                        driveMotor(0.0f, 0.0f);

                        new Thread(new Runnable() {
                            @Override
                            public void run() {
                                try {
                                    while(sockIn.read() != -1) {
                                        Log.d(TAG, "read byte from server");
                                    }
                                } catch (IOException e) {
                                    Log.e(TAG, "socket read failed", e);
                                }
                            }
                        }).start();

                        new Thread(new Runnable() {
                            @Override
                            public void run() {
                                try {
                                    // sending halfMax, halfMax to the motors is an initialization signal that tells AK
                                    // that it should start listening to the data stream
                                    int halfMax = MAX_MOTOR_WIRE_VAL / 2;
                                    sockOut.write(new byte[]{(byte)halfMax, (byte)halfMax});

                                    while(true) {
                                        Thread.currentThread().sleep(50);
                                        byte leftValByte = (byte) Math.round((leftMotor + 1.0f) * halfMax);
                                        byte rightValByte = (byte) Math.round((rightMotor + 1.0f) * halfMax);
                                        sockOut.write(new byte[]{leftValByte, rightValByte});
                                    }
                                } catch (Exception e) {
                                    Log.e(TAG, "failed to write drive motor message", e);
                                }
                            }
                        }).start();

//                        Thread.currentThread().sleep(5000);
//                        driveMotor(0.1f, 0.0f);

                    } catch (Exception e) {
                        Log.e(TAG, "network communication failed", e);
                        //throw new RuntimeException(e);
                    }
                }
            }).start();

            return rootView;
        }

        private void movedTo(float x, float y) {
            Log.d(TAG, "x = " + x + ", y = " + y);

            // normalize on a -1.0-1.0 range
            int height = this.getView().getHeight();
            int width = this.getView().getWidth();
            x = (x / width) * 2.0f - 1.0f;
            y = ((height - y) / height) * 2.0f - 1.0f;

            float leftMotor = y - (x / 10.0f);
            if(leftMotor > 1.0f) leftMotor = 1.0f;
            if(leftMotor < -1.0f) leftMotor = -1.0f;

            float rightMotor = y + (x / 10.0f);
            if(rightMotor > 1.0f) rightMotor = 1.0f;
            if(rightMotor < -1.0f) rightMotor = -1.0f;

            driveMotor(leftMotor, rightMotor);
        }

        private void driveMotor(float leftVal, float rightVal) {
            this.leftMotor = leftVal;
            this.rightMotor = rightVal;
        }
    }
}
