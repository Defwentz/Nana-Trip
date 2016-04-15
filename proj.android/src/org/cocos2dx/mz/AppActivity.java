/****************************************************************************
Copyright (c) 2008-2010 Ricardo Quesada
Copyright (c) 2010-2012 cocos2d-x.org
Copyright (c) 2011      Zynga Inc.
Copyright (c) 2013-2014 Chukong Technologies Inc.
 
http://www.cocos2d-x.org

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
****************************************************************************/
package org.cocos2dx.mz;

import java.util.List;

import org.cocos.nanatrip.R;
import org.cocos2dx.lib.Cocos2dxActivity;
import org.cocos2dx.lib.Cocos2dxGLSurfaceView;
import org.cocos2dx.plugin.PluginWrapper;

import xx.jnitst.mz.xApplication;
import android.app.AlertDialog;
import android.content.Context;
import android.content.DialogInterface;
import android.content.DialogInterface.OnDismissListener;
import android.content.Intent;
import android.net.ConnectivityManager;
import android.net.NetworkInfo;
import android.os.Bundle;
import android.os.Handler;
import android.os.Looper;
import android.os.Message;
import android.util.Log;
import android.view.WindowManager;
import android.widget.EditText;
import android.widget.Toast;
import cn.bmob.v3.Bmob;
import cn.bmob.v3.BmobObject;
import cn.bmob.v3.BmobQuery;
import cn.bmob.v3.listener.FindListener;
import cn.bmob.v3.listener.SaveListener;
import cn.bmob.v3.listener.UpdateListener;
import cn.bmob.v3.update.BmobUpdateAgent;
import cn.sharesdk.ShareSDKUtils;

import com.baidu.mobads.InterstitialAd;
import com.baidu.mobads.InterstitialAdListener;
import com.meizu.gamecenter.sdk.LoginResultCode;
import com.meizu.gamecenter.sdk.MzAccountInfo;
import com.meizu.gamecenter.sdk.MzGameBarPlatform;
import com.meizu.gamecenter.sdk.MzGameCenterPlatform;
import com.meizu.gamecenter.sdk.MzLoginListener;
import com.tencent.mm.sdk.openapi.IWXAPI;
import com.tencent.mm.sdk.openapi.WXAPIFactory;

public class AppActivity extends Cocos2dxActivity {
	// log tag
	private static String LOGTAG = "nana";
	private static void log(String content) {
		Log.e(LOGTAG, content);
	}
	public static final String HIGHEST_SCORE_OBJECT_ID = "highest_score_object_id";
	// 实例
	private static AppActivity app = null;
	public static SimpleData sdata = null;
	
	// 魅族thing
	//public static MzGameBarPlatform mzGameBarPlatform;
	public static String mUid;
	
	@Override
	protected void onCreate(Bundle savedInstanceState) {
	    super.onCreate(savedInstanceState);
	    app = this;
	    sdata = new SimpleData(app);
	    
	    // 屏幕常亮
	    getWindow().setFlags(WindowManager.LayoutParams.FLAG_KEEP_SCREEN_ON, 
	    		WindowManager.LayoutParams.FLAG_KEEP_SCREEN_ON);
	    
	    // ShareSDK初始化
	    ShareSDKUtils.prepare();
//	    ShareSDKUtils.initSDK("bc6da0554e0a", true);
	    
	    // 初始化 Bmob SDK
        // 使用时请将第二个参数Application ID替换成你在Bmob服务器端创建的Application ID
	     Bmob.initialize(this, "159c81a8e531f5e39bf6ba0eb751ee0c");
	    // 查看更新
	    // BmobUpdateAgent.update(this);
	    
		// 初始化,可以指定 Gamebar 第一次显示的位置,在游戏退出时会记住用户操作的最后一次位置,再次启动时使用上一次的位置
		// 第一次显示的位置可以指定四个方向,左上,左下,右上,右下
		// public static final int GRAVITY_LEFT_TOP = 1;
		// public static final int GRAVITY_LEFT_BOTTOM = 2;
		// public static final int GRAVITY_RIGHT_TOP = 3;
		// public static final int GRAVITY_RIGHT_BOTTOM = 4;
		//mzGameBarPlatform = new MzGameBarPlatform(this, MzGameBarPlatform.GRAVITY_RIGHT_BOTTOM);
		// 调用 onActivityCreate
		//mzGameBarPlatform.onActivityCreate();
		//mzGameBarPlatform.showGameBar();
		//meizuLogin();
	}
	 public static void showbar() {
		 log("showbar");
	 }
	 public static void hidebar() {
		 log("hidebar");
//		 if(mzGameBarPlatform != null) {
//			 mzGameBarPlatform.hideGameBar();
//			 Log.e("meizu", "hidebar");
//		 }
		 //dialogHandler.sendEmptyMessage(0);
	 }
	 // Ads
//	 private static InterstitialAd interAd = null;
//	 public static void loadBaiduAd() {
//		 log("loadBaiduAd");
//		 app.runOnUiThread(new Runnable() {
//
//			@Override
//			public void run() {
//				 interAd = new InterstitialAd(app, "2321557");
//				 interAd.setListener(new InterstitialAdListener () {
//
//					@Override
//					public void onAdClick(InterstitialAd arg0) {
//						log("ad click");
//					}
//
//					@Override
//					public void onAdDismissed() {
//						log("ad dismissed");
//						//interAd.loadAd();
//					}
//
//					@Override
//					public void onAdFailed(String arg0) {
//						log("ad failed");
//					}
//
//					@Override
//					public void onAdPresent() {
//						log("ad present");
//					}
//
//					@Override
//					public void onAdReady() {
//						log("ad ready");
//					}
//					 
//				 });
//				 interAd.loadAd();
//			}
//			 
//		 });
//	 }
//	 public static void showBaiduAd() {
//		 log("showBaiduAd");
//		 app.runOnUiThread(new Runnable() {
//
//				@Override
//				public void run() {
//					 if(interAd != null) {
//						 if(interAd.isAdReady()) {
//							 log("ad ready and show");
//							 interAd.showAd(app);
//						 } else {
//							 log("ad not ready need load");
//							 interAd.loadAd();
//						 }
//					 }
//				}
//		 });
//	 }
	 public static void meizuLogin() {
		 log("meizu login");
		// 调用登录接口。注意,该方法必须在应用的主线程中调用。 
//		 MzGameCenterPlatform.login(app, new MzLoginListener() {
//			 @Override
//			 public void onLoginResult(int code, MzAccountInfo accountInfo, String errorMsg) {
//				 // 登录结果回调。注意,该回调跑在应用主线程,不能在这里做耗时操作
//				 switch(code) {
//				 case LoginResultCode.LOGIN_SUCCESS:
//				// 登录成功,拿到uid 和 session到自己的服务器去校验session合法性
//				 mUid = accountInfo.getUid();
//				 Log.e("meizu", "登录成功!\r\n 用户名:" + accountInfo.getName() + "\r\n Uid:" +
//				 accountInfo.getUid() + "\r\n session:" + accountInfo.getSession()); 
//					 break;
//				case LoginResultCode.LOGIN_ERROR_CANCEL:
//					// 用户取消登陆操作
//					break;
//				default:
//					// 登陆失败,包含错误码和错误消息。
//				// 注意,错误消息(errorMsg)需要由游戏展示给用户,提示失败原因
//				Log.e("meizu", "登录失败 : " + errorMsg + " , code = " + code);
//							break;
//				 }
//			 }
//		 });
	 }
	 public static void logout() {
		 log("meizu logout");
//		 if(mzGameBarPlatform != null) {
//			 Log.e("meizu","do nothing showbar call succeed!");
//			 MzGameCenterPlatform.logout(AppActivity.getContext());
//		 }
	 }
	 public static boolean isNetworkAvailable(Context context) {   
	        ConnectivityManager cm = (ConnectivityManager) context   
	                .getSystemService(Context.CONNECTIVITY_SERVICE);   
	        if (cm == null) {   
	        	return false;
	        } else {
	        	return cm.getActiveNetworkInfo().isAvailable();
	        }
	 }
	 public static void nameExist(final String name, final int score) {
		 BmobQuery<HighestScore> query = new BmobQuery<HighestScore>();
		 query.addWhereEqualTo("playerName", name);
		 query.findObjects(app, new FindListener<HighestScore> () {
			@Override
			public void onError(int code, String msg) {
				log("find error: " + msg);
			}
			@Override
			public void onSuccess(List<HighestScore> objects) {
				log("find succeed: "+objects.size()+"条数据。");
				if(objects.size() == 0) {
			 		makeToast("这名儿不错。");
			 		playerName = name;
			 		reportScore(score);
				} else {
			 		makeToast("别介, 换个名儿的。");
			 		showSetName(score);
				}
			}
			 
		 });
	 }
	 public static void showSetName(final int score) {
		 app.runOnUiThread(new Runnable() {
				@Override
				public void run() {
					 final EditText inputServer = new EditText(app);
					 AlertDialog.Builder builder = new AlertDialog.Builder(app);
					 builder.setTitle("来个名儿吧").setIcon(android.R.drawable.ic_dialog_info)
					 		.setView(inputServer)
					        .setNegativeButton("Cancel", new DialogInterface.OnClickListener() {
								 public void onClick(DialogInterface dialog, int which) {
									 	makeToast("不取名儿不给存最高分。");
									 	showSetName(score);
							     	}
							 });
					 builder.setPositiveButton("Ok", new DialogInterface.OnClickListener() {
						 public void onClick(DialogInterface dialog, int which) {
							 	String name = inputServer.getText().toString();
							 	nameExist(name, score);
					     	}
					 });
					 builder.show();
				}
		 });
	 }
	 
	 private static String playerName = null;
	 public static int mscore = 0;
	 public static void reportScore(final int score) {
		 mscore = score;
		 if(!isNetworkAvailable(app)) return;
		 
		 String objectId = sdata.getString(HIGHEST_SCORE_OBJECT_ID);
		 if(objectId == ""){
			 if(playerName == null) {
				 showSetName(score);
			 }
			 final HighestScore hScore = new HighestScore();
			 hScore.setPlayerName(playerName);
			 hScore.setScore(score);
			 hScore.save(app, new SaveListener() {
				@Override
				public void onFailure(int code, String msg) {
					log("save fail " + msg);
				}
				@Override
				public void onSuccess() {
					log("save succeed, object id: " + hScore.getObjectId());
					sdata.putString(HIGHEST_SCORE_OBJECT_ID, hScore.getObjectId());
				}
				 
			 });
		 } else {
			 HighestScore hScore = new HighestScore();
			 hScore.setScore(score);
			 hScore.update(app, objectId, new UpdateListener() {
				@Override
				public void onFailure(int code, String msg) {
					log("update fail: " + msg);
				}
				@Override
				public void onSuccess() {
					log("update succeed");
				}
			 });
		 }
	 }
	 public static void showLeaderboard() {
		 app.startActivity(new Intent(app, LeaderBoardActivity.class));
	 }
	 
	 public static void makeToast(final String x) {
		 log("make toast: " + x);
		 app.runOnUiThread(new Runnable() {
			@Override
			public void run() {
				log("in run");
				Toast.makeText(xApplication.getContxt(), x, Toast.LENGTH_SHORT).show();
			}
		 });
	 }
	 
	@Override
	protected void onDestroy() {
		ShareSDKUtils.stopSDK();
		super.onDestroy();
		// 调用 onActivityDestroy
		//MzGameCenterPlatform.logout(this);
		//mzGameBarPlatform.onActivityDestroy();
	}
	@Override
	protected void onResume() {
		super.onResume();
		// 调用 onActivityResume
		//mzGameBarPlatform.onActivityResume();
	}
	@Override
	protected void onPause() {
		super.onPause();
	    // 调用 onActivityPause
	    //mzGameBarPlatform.onActivityPause();
	}
}
