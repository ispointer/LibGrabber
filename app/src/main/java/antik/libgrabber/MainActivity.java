package antik.libgrabber;

/*
 * Created by aantik
 * 3/27/2026 8:28 PM
 *
 *   ⋆    ႔ ႔
 *     ᠸ^ ^ ⸝⸝
 *       |、˜〵
 *      じしˍ,)⁐̤ᐷ
 *
 * Fox Mode 🍺
 */

import android.app.AlertDialog;
import android.content.DialogInterface;
import android.content.Intent;
import android.content.pm.PackageManager;
import android.net.Uri;
import android.os.Bundle;
import android.os.Environment;
import android.provider.Settings;
import android.text.TextUtils;
import android.view.View;
import android.widget.*;

import androidx.appcompat.app.AppCompatActivity;
import androidx.appcompat.widget.Toolbar;

import java.io.File;

public class MainActivity extends AppCompatActivity {

    private EditText in1, in2;
    private Button lever;
    private TextView stone;
    private ProgressBar proccce;

    private String[] S = {"Processing <--> ", "Invalid file", "Input required", "Allow file access", "Permission Required", "Allow", "Cancel"};


    @Override
    protected void onCreate(Bundle bundle) {
        super.onCreate(bundle);


        setContentView(R.layout.activity_main);

        Toolbar t = findViewById(R.id.tool);
        setSupportActionBar(t);

        in1 = findViewById(R.id.in1);
        in2 = findViewById(R.id.in2);
        lever = findViewById(R.id.dum);
        stone = findViewById(R.id.sta);
        proccce = findViewById(R.id.proc);

        lever.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                start();
            }
        });

        permissionsAppe();
    }

    //-- deep write
    private void permissionsAppe() {

        if (android.os.Build.VERSION.SDK_INT >= android.os.Build.VERSION_CODES.R) {
            if (!Environment.isExternalStorageManager()) {alu();}
            } else {
            if (checkSelfPermission(android.Manifest.permission.READ_EXTERNAL_STORAGE) != PackageManager.PERMISSION_GRANTED || checkSelfPermission(android.Manifest.permission.WRITE_EXTERNAL_STORAGE) != PackageManager.PERMISSION_GRANTED) {
                requestPermissions(new String[]{android.Manifest.permission.READ_EXTERNAL_STORAGE, android.Manifest.permission.WRITE_EXTERNAL_STORAGE
                }, 100);
            }
        }
    }

    private void alu() {

        AlertDialog.Builder b = new AlertDialog.Builder(MainActivity.this);

        b.setTitle(S[4]);
        b.setMessage(S[3]);

        b.setPositiveButton(S[5], new DialogInterface.OnClickListener() {
            @Override
            public void onClick(DialogInterface d, int w) {
                Intent i = new Intent(Settings.ACTION_MANAGE_APP_ALL_FILES_ACCESS_PERMISSION);
                i.setData(Uri.parse("package --> " + getPackageName()));
                startActivity(i);
                d.dismiss();
            }
        });

        b.setNegativeButton(S[6], new DialogInterface.OnClickListener() {
            @Override
            public void onClick(DialogInterface d, int w) {
            d.dismiss();
            }
        });
        AlertDialog dialog = b.create();
        dialog.show();
    }

    @Override
    public void onRequestPermissionsResult(int r, String[] p, int[] g) {
        super.onRequestPermissionsResult(r, p, g);
    }
    private void start() {

        String input = in1.getText().toString().trim();
        String out = in2.getText().toString().trim();

        if (TextUtils.isEmpty(input)) {
        stone.setText(S[2]);
        return;
        }

        if (TextUtils.isEmpty(out)) {
        out = input.replace(".so", ".hpp");
        in2.setText(out);
        }

        File inFile = new File(input);
        if (!inFile.exists()) {
        stone.setText(S[1]);
        return;
        }

        File outFile = new File(out);
        if (outFile.getParentFile() != null) {
        outFile.getParentFile().mkdirs();
        }

        proccce.setVisibility(View.VISIBLE);
        lever.setEnabled(false);
        stone.setText(S[0]);

        final String finalOut = out;

        Thread th = new Thread(new Runnable() {
            @Override
            public void run() {

                final String result = lib.dump(input, finalOut);

                runOnUiThread(new Runnable() {
                    @Override
                    public void run() {
                        proccce.setVisibility(View.GONE);
                        lever.setEnabled(true);
                        stone.setText(result);
                    }
                });
            }
        });

        th.start();
    }
}