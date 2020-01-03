package com.boyaa.glesdemo;

import android.content.res.AssetManager;
import android.os.Bundle;
import android.text.Editable;
import android.text.TextWatcher;
import android.util.Log;
import android.view.KeyEvent;
import android.view.ViewGroup;
import android.view.inputmethod.EditorInfo;
import android.widget.EditText;
import android.widget.LinearLayout;
import android.widget.RelativeLayout;
import android.widget.TextView;

import androidx.annotation.Nullable;
import androidx.appcompat.app.AppCompatActivity;

public class MainActivity extends AppCompatActivity {
    public static final String TAG = "MainActivity";

    //loadAssetFile.cpp 需要改方法
    private static AssetManager am;
    public static AssetManager getAM(){
        return am;
    }

    GLESView glesView;
    EditText editText;
    @Override
    protected void onCreate(@Nullable Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        am = getAssets();

        LinearLayout linearLayout = new LinearLayout(this);
        linearLayout.setOrientation(LinearLayout.VERTICAL);
        setContentView(linearLayout);

        glesView = new GLESView(getApplication());
        LinearLayout.LayoutParams param1 = new LinearLayout.LayoutParams(ViewGroup.LayoutParams.MATCH_PARENT,
                0);
        param1.weight = 0.8f;



        editText = new EditText(this);
        editText.setSingleLine();
        editText.setImeOptions(EditorInfo.IME_ACTION_DONE);
        LinearLayout.LayoutParams params2 = new LinearLayout.LayoutParams(ViewGroup.LayoutParams.MATCH_PARENT,
                0);
        params2.weight = 0.2f;
        linearLayout.setWeightSum(1f);
        linearLayout.addView(glesView,param1);
        linearLayout.addView(editText,params2);

        editText.setOnEditorActionListener(new TextView.OnEditorActionListener() {
            @Override
            public boolean onEditorAction(TextView v, int actionId, KeyEvent event) {
                switch (actionId){
                    case EditorInfo.IME_ACTION_DONE:
                        runCommand(editText.getText().toString());
                        return true;
                }
                return false;
            }
        });
    }

    protected void runCommand(final String cmd){
        glesView.queueEvent(new Runnable() {
            @Override
            public void run() {
                Log.d(TAG, "run: " + cmd);
                NativeLibrary.rumCmd(cmd);
            }
        });
    }


    @Override
    protected void onResume() {
        super.onResume();
        glesView.onResume();
    }

    @Override
    protected void onPause() {
        super.onPause();
        glesView.onPause();
    }
}
