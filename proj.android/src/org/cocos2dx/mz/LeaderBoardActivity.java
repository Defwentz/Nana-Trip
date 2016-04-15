package org.cocos2dx.mz;

import java.util.List;

import org.cocos.nanatrip.R;
import org.json.JSONException;
import org.json.JSONObject;

import cn.bmob.v3.AsyncCustomEndpoints;
import cn.bmob.v3.BmobQuery;
import cn.bmob.v3.listener.CloudCodeListener;
import cn.bmob.v3.listener.FindListener;
import android.app.Activity;
import android.os.Bundle;
import android.widget.ListAdapter;
import android.widget.ListView;
import android.widget.TextView;

public class LeaderBoardActivity extends Activity {
	
	LeaderBoardAdapter lbAdapter;
	ListView lbView;
	
	TextView myInfoView;
	
	@Override
	protected void onCreate(Bundle savedInstanceState) {
	    super.onCreate(savedInstanceState);
	    this.setContentView(R.layout.activity_leaderboard);
	    myInfoView = (TextView) this.findViewById(R.id.myInfo);
	    lbView = (ListView) this.findViewById(R.id.lb);
	    lbView.setDividerHeight(0);
	    lbAdapter = new LeaderBoardAdapter(this);
	    lbView.setAdapter((ListAdapter) lbAdapter);
	    ask4MyRanking();
	}
	
	public void ask4MyRanking() {
		String objectId = AppActivity.sdata.getString(AppActivity.HIGHEST_SCORE_OBJECT_ID);
		if(objectId != "") {
			JSONObject cloudCodeParams = new JSONObject();
			try {
				cloudCodeParams.put("objectId", objectId);
				AsyncCustomEndpoints ace = new AsyncCustomEndpoints();
				//第一个参数是上下文对象，第二个参数是云端逻辑的方法名称，第三个参数是上传到云端逻辑的参数列表（JSONObject cloudCodeParams），第四个参数是回调类
				ace.callEndpoint(this, "getRanking", cloudCodeParams, new CloudCodeListener() {
				    @Override
				    public void onSuccess(Object object) {
				    	myInfoView.setText(object.toString());
				    	//AppActivity.makeToast("云端usertest方法返回:" + object.toString());
				    }
				    @Override
				    public void onFailure(int code, String msg) {
				    	AppActivity.makeToast("访问云端usertest方法失败:" + msg);
				    }
				});
			} catch (JSONException e) {
				e.printStackTrace();
			}
		}
	}
}
