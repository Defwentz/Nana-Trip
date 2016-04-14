package org.cocos2dx.mz;

import android.app.Activity;
import android.content.Context;
import android.content.SharedPreferences;

public class SimpleData {
	
	final String PREFERENCES_TAG = "NT_PREFERENCES";
	private SharedPreferences.Editor editor;
	private SharedPreferences preferences;
	
	public SimpleData(Context context){
		preferences = context
				.getSharedPreferences(PREFERENCES_TAG, Activity.MODE_PRIVATE);
		
		editor = preferences.edit();
	}
	
	public void putBoolean(String tag, boolean data){
		editor.putBoolean(tag, data);
		editor.apply();
	}
	
	public boolean getBoolean(String tag){
		return preferences.getBoolean(tag, false);
	}
	
	public void putString(String tag, String data){
		editor.putString(tag, data);
		editor.apply();
	}
	
	public String getString(String tag){
		return preferences.getString(tag, "");
	}
	
	public void putInt(String tag, int data){
		editor.putInt(tag, data);
		editor.apply();
	}
	public int getInt(String tag){
		return preferences.getInt(tag, 0);
	}
}
