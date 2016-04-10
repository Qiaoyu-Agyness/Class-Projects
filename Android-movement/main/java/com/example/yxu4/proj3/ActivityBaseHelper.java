package com.example.yxu4.proj3;

import android.content.Context;
import android.database.sqlite.SQLiteDatabase;
import android.database.sqlite.SQLiteOpenHelper;
import android.content.ContentValues;


/**
 * Created by JoeXu on 4/8/16.
 */

public class ActivityBaseHelper extends SQLiteOpenHelper{
    private static final String DB_NAME = "ActivityDB.db";
    private static final int DB_VERSION = 1;

    private static final String COLUMN_ID = "id";
    private static final String COLUMN_TIME = "time";
    private static final String COLUMN_ACTIVITY_TYPE = "activityType";
    private static ActivityBaseHelper me = null;


    public ActivityBaseHelper ( Context context ) {
        super(context, DB_NAME, null, DB_VERSION);
    }

    @Override
    public void onCreate(SQLiteDatabase db) {
        db.execSQL("CREATE TABLE " + ActivityDbSchema.ActivityTable.NAME + " ("
                + ActivityDbSchema.ActivityTable.COLUMN_TIME + " TEXT NOT NULL, "
                + ActivityDbSchema.ActivityTable.COLUMN_ACTIVITY_TYPE + " TEXT NOT NULL"
                + ");");
    }

    @Override
    public void onUpgrade(SQLiteDatabase db, int oldVersion, int newVersion) {
        db.execSQL("DROP TABLE IF EXISTS " + DB_NAME + ";");
        onCreate(db);
    }

    public void addActivity(String time, String type) {
        ContentValues values = new ContentValues();
        values.put(COLUMN_TIME, time);
        values.put(COLUMN_ACTIVITY_TYPE, type);

        SQLiteDatabase db = getWritableDatabase();
        db.insert(DB_NAME, null, values);
        db.close();
    }
}
