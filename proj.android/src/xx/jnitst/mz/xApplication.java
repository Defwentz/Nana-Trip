package xx.jnitst.mz;

import com.meizu.gamecenter.sdk.MzGameCenterPlatform;

import android.app.Application;
import android.content.Context;

public class xApplication extends Application{
	private static Context context;
	
	@Override
	public void onCreate() {
		//MzGameCenterPlatform.init(this, "2777400", "00ae413d850d495fa151221ca4c68ab8");
		context = this.getApplicationContext();
	}
	
	public static Context getContxt() {
		return context;
	}
}
