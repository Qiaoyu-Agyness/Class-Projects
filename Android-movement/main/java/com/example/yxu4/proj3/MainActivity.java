package com.example.yxu4.proj3;

import android.Manifest;
import android.app.PendingIntent;
import android.content.BroadcastReceiver;
import android.content.ContentValues;
import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;
import android.content.pm.PackageManager;
import android.database.sqlite.SQLiteDatabase;
import android.location.Location;
import android.location.LocationListener;
import android.location.LocationManager;
import android.media.MediaPlayer;
import android.support.v4.app.ActivityCompat;
import android.support.v4.content.ContextCompat;
import android.support.v4.content.LocalBroadcastManager;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.util.Log;
import android.view.Menu;
import android.view.MenuItem;
import android.widget.ImageView;
import android.widget.TextView;
import android.widget.Toast;

import java.text.ParseException;
import java.text.SimpleDateFormat;
import java.util.Date;
import java.util.concurrent.TimeUnit;

import com.google.android.gms.common.ConnectionResult;
import com.google.android.gms.common.api.GoogleApiClient;
import com.google.android.gms.location.ActivityRecognition;
import com.google.android.gms.maps.CameraUpdate;
import com.google.android.gms.maps.CameraUpdateFactory;
import com.google.android.gms.maps.GoogleMap;
import com.google.android.gms.maps.MapFragment;
import com.google.android.gms.maps.MapView;
import com.google.android.gms.maps.OnMapReadyCallback;
import com.google.android.gms.maps.SupportMapFragment;
import com.google.android.gms.maps.model.LatLng;
import com.google.android.gms.maps.model.MarkerOptions;

public class MainActivity extends AppCompatActivity implements
        LocationListener, GoogleApiClient.ConnectionCallbacks, GoogleApiClient.OnConnectionFailedListener{
    private int LOCATION_REQUEST = 1;
    private GoogleMap map;
    public GoogleApiClient apiClient;
    private MapView mapView;
    private LocationManager locationManager;
    private Date oldTime = new Date();
    private String oldState = "still";
    private SQLiteDatabase db;



    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        db = new ActivityBaseHelper(getBaseContext()).getWritableDatabase();
        setContentView(R.layout.activity_main);
        LocalBroadcastManager.getInstance(this).registerReceiver(mMessageReceiver, new IntentFilter("custom-event-name"));

        mapView = (MapView) findViewById(R.id.map);
        mapView.onCreate(savedInstanceState);
        mapView.getMapAsync(new OnMapReadyCallback() {
            @Override
            public void onMapReady(GoogleMap googleMap) {
                map = googleMap;
                mapView.onResume();
            }
        });
        locationManager = (LocationManager) getSystemService((Context.LOCATION_SERVICE));
        if (ActivityCompat.checkSelfPermission(this, Manifest.permission.ACCESS_FINE_LOCATION) != PackageManager.PERMISSION_GRANTED
                && ActivityCompat.checkSelfPermission(this, Manifest.permission.ACCESS_COARSE_LOCATION) != PackageManager.PERMISSION_GRANTED) {

            Toast.makeText(this, "Location is disabled", Toast.LENGTH_LONG).show();
            ActivityCompat.requestPermissions(this,
                    new String[]{Manifest.permission.ACCESS_FINE_LOCATION},
                    0);
            return;
        }
        locationManager.requestLocationUpdates(LocationManager.NETWORK_PROVIDER, 400, 1000, this);

        apiClient = new GoogleApiClient.Builder(this)
                .addApi(ActivityRecognition.API)
                .addConnectionCallbacks(this)
                .addOnConnectionFailedListener(this)
                .build();

        apiClient.connect();
    }


    private BroadcastReceiver mMessageReceiver = new BroadcastReceiver() {
        @Override
        public void onReceive(Context context, Intent intent) {
            ImageView image = (ImageView) findViewById(R.id.image);
            TextView text = (TextView) findViewById(R.id.text);
            MediaPlayer mediaPlayer = MediaPlayer.create(getBaseContext(), R.raw.song);
            SimpleDateFormat dateFormat = new SimpleDateFormat("HH:mm:ss");
            String currentTime;
            String time;

            String message = intent.getStringExtra("message");
            Log.d("receiver", "Got message: " + message);

            if(message.equals("in_vehicle") && message !=oldState){
                image.setImageResource(R.drawable.in_vehicle);
                text.setText("You are in a Vehicle");
                mediaPlayer.release();
                currentTime = dateFormat.format(new Date());
                db.insert(ActivityDbSchema.ActivityTable.NAME, null, getContentValues(currentTime, message));


                time = calculateDuration(oldTime);
                String result = "You have " + oldState + " for: " + time;
                CharSequence toastText = result;
                int duration = Toast.LENGTH_SHORT;
                Toast toast = Toast.makeText(getApplicationContext(), toastText, duration);
                toast.show();


                oldTime = new Date();
                oldState = message;
            }
            else if(message.equals("running") && message != oldState ){
                image.setImageResource(R.drawable.running);
                text.setText("You are Running");
                mediaPlayer.start();

                time = calculateDuration(oldTime);
                String result = "You have " + oldState + " for: " + time;
                CharSequence toastText = result;
                int duration = Toast.LENGTH_SHORT;
                Toast toast = Toast.makeText(getApplicationContext(), toastText, duration);
                toast.show();

                oldTime = new Date();
                oldState = message;


            }
            else if(message.equals("still")&& message!=oldState){
                image.setImageResource(R.drawable.still);
                text.setText("You are Stading Still");
                mediaPlayer.release();


                time = calculateDuration(oldTime);
                String result = "You have " + oldState + " for: " + time;
                CharSequence toastText = result;
                int duration = Toast.LENGTH_SHORT;
                Toast toast = Toast.makeText(getApplicationContext(), toastText, duration);
                toast.show();

                oldTime = new Date();
                oldState = message;

            }
            else if(message.equals("walking")&& message!=oldState){
                image.setImageResource(R.drawable.walking);
                text.setText("You are Walking");
                mediaPlayer.start();

                time = calculateDuration(oldTime);
                String result = "You have " + oldState + " for: " + time;
                CharSequence toastText = result;
                int duration = Toast.LENGTH_SHORT;
                Toast toast = Toast.makeText(getApplicationContext(), toastText, duration);
                toast.show();


                oldTime = new Date();
                oldState = message;

            }
            else{
                image.setImageResource(android.R.color.darker_gray);
                text.setText("Unknown Activity");

            }
        }
    };

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

    @Override
    public void onConnected(Bundle bundle) {
        Intent intent = new Intent( this, ActivityRecognizedService.class );
        PendingIntent pendingIntent = PendingIntent.getService( this, 0, intent, PendingIntent.FLAG_UPDATE_CURRENT );
        ActivityRecognition.ActivityRecognitionApi.requestActivityUpdates( apiClient, 100, pendingIntent);
    }

    @Override
    public void onConnectionSuspended(int i) {
        Log.e("Connection Suspended", "Connection Suspended");
    }

    @Override
    public void onLocationChanged(Location location) {
        LatLng latLng = new LatLng(location.getLatitude(), location.getLongitude());
        CameraUpdate cameraUpdate = CameraUpdateFactory.newLatLngZoom(latLng, 21);
        map.animateCamera(cameraUpdate);
        map.addMarker(new MarkerOptions().position(latLng).title("YOU"));
        if (ActivityCompat.checkSelfPermission(this, Manifest.permission.ACCESS_FINE_LOCATION) != PackageManager.PERMISSION_GRANTED
                && ActivityCompat.checkSelfPermission(this, Manifest.permission.ACCESS_COARSE_LOCATION) != PackageManager.PERMISSION_GRANTED) {
            // TODO: Consider calling
            ActivityCompat.requestPermissions(this,
                    new String[]{Manifest.permission.ACCESS_FINE_LOCATION},
                    0);
            return;
        }
        locationManager.removeUpdates(this);
    }

    @Override
    public void onStatusChanged(String provider, int status, Bundle extras) {

    }

    @Override
    public void onProviderEnabled(String provider) {

    }

    @Override
    public void onProviderDisabled(String provider) {

    }

    @Override
    public void onConnectionFailed(ConnectionResult connectionResult) {
        Log.e("Connection Failed", "Connection Failed");
    }

    private static ContentValues getContentValues (String time, String activity) {
         ContentValues values = new ContentValues();
            values.put(ActivityDbSchema.ActivityTable.COLUMN_TIME, time);
            values.put(ActivityDbSchema.ActivityTable.COLUMN_ACTIVITY_TYPE, activity);
            return values;
    }

    private String calculateDuration(Date lastTime) {
        long duration = System.currentTimeMillis() - lastTime.getTime();
        //String minutes = String.format("%02d min, %02d sec",
        //        TimeUnit.MILLISECONDS.toMinutes(duration),
        //        TimeUnit.MILLISECONDS.toSeconds(duration) - TimeUnit.MINUTES.toSeconds(TimeUnit.MILLISECONDS.toMinutes(duration))
        //);

        String result = "";
        String durationString = String.format("%d min, %d sec", TimeUnit.MILLISECONDS.toMinutes(duration),
                TimeUnit.MILLISECONDS.toSeconds(duration) - TimeUnit.MINUTES.toSeconds(TimeUnit.MICROSECONDS.toMinutes(duration)));
        return result + durationString;
    }
}
