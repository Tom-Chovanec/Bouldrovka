package com.bouldrovka.app;

import android.Manifest;
import android.annotation.TargetApi;
import android.content.Intent;
import android.content.pm.PackageManager;
import android.database.Cursor;
import android.net.Uri;
import android.os.Build;
import android.provider.MediaStore;
import android.util.Log;

import org.libsdl.app.SDLActivity;

import java.io.File;
import java.io.IOException;

public class Bouldrovka extends SDLActivity {

    private static final int REQUEST_READ_STORAGE = 1;
    private static final int OPEN_GALLERY = 2;

    @TargetApi(Build.VERSION_CODES.TIRAMISU)
    public void openImagePicker() {
        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.M) {
            if (checkSelfPermission(Manifest.permission.READ_EXTERNAL_STORAGE) != PackageManager.PERMISSION_GRANTED) {
                requestPermissions(new String[]{Manifest.permission.READ_MEDIA_IMAGES}, REQUEST_READ_STORAGE);
            } else {
                //open gallery
                Intent intenta = new Intent(Intent.ACTION_PICK, MediaStore.Images.Media.EXTERNAL_CONTENT_URI);
                startActivityForResult(intenta, OPEN_GALLERY);
            }
        }
    }

    //c++ function
    public native void onImagePicked();


    private String getPathFromUri(Uri uri) {
        String[] projection = {MediaStore.Images.Media.DATA};
        Cursor cursor = getContentResolver().query(uri, projection, null, null, null);
        if (cursor != null) {
            int columnIndex = cursor.getColumnIndexOrThrow(MediaStore.Images.Media.DATA);
            cursor.moveToFirst();
            String path = cursor.getString(columnIndex);
            cursor.close();
            return path;
        }
        return null;
    }

    @Override
    protected void onActivityResult(int requestCode, int resultCode, Intent data) {
        super.onActivityResult(requestCode, resultCode, data);

        //opening gallery and handeling the selected image
        if (requestCode == OPEN_GALLERY && resultCode == RESULT_OK && data != null) {
            Uri selectedImage = data.getData();
            assert selectedImage != null;

            String imagePath = getPathFromUri(selectedImage);

            File destinationDir = new File(getFilesDir(), "boulder");
            if (!destinationDir.exists()) {
                destinationDir.mkdirs();
            }
            File destinationFile = new File(destinationDir, "background.jpg");
            Log.i("imagepath", destinationFile.getAbsolutePath());
            try {
                copyFile(new File(imagePath), destinationFile);
            } catch (IOException e) {
                throw new RuntimeException(e);
            }

            //send info to c++ that a new image has been selected
            onImagePicked();
        }
    }

/*
    @Override
    public void onRequestPermissionsResult(int requestCode, String[] permissions, int[] grantResults) {
        if (grantResults.length > 0 && grantResults[0] == PackageManager.PERMISSION_GRANTED) {
            Intent intent = new Intent(Intent.ACTION_PICK, MediaStore.Images.Media.EXTERNAL_CONTENT_URI);
            startActivityForResult(intent, OPEN_GALLERY);
        } else {
        }

    }
*/
}
