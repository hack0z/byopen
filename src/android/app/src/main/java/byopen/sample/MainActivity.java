package byopen.sample;

import androidx.appcompat.app.AppCompatActivity;

import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.widget.Button;

import dyopen.lib.SystemLoader;

public class MainActivity extends AppCompatActivity {

    private static final String TAG = "byOpen";

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        Button origin_loadLibraryBtn = (Button) findViewById(R.id.origin_loadLibrary);
        origin_loadLibraryBtn.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                System.loadLibrary("chrome");
            }
        });

        Button byopen_loadLibraryBtn = (Button) findViewById(R.id.byopen_loadLibrary);
        byopen_loadLibraryBtn.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                SystemLoader.loadLibrary("chrome");
            }
        });

        Button dlopenBtn = (Button) findViewById(R.id.byopen_dlopen);
        dlopenBtn.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                NativeTest.test();
            }
        });
    }
}