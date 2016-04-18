package org.cocos2dx.mz;

import java.util.ArrayList;
import java.util.List;

import org.cocos.nanatrip.R;

import android.content.Context;
import android.view.LayoutInflater;
import android.view.View;
import android.view.View.OnClickListener;
import android.view.ViewGroup;
import android.widget.BaseAdapter;
import android.widget.TextView;
import cn.bmob.v3.BmobQuery;
import cn.bmob.v3.listener.FindListener;

public class LeaderBoardAdapter extends BaseAdapter {

	private class ViewHolder {
        public TextView tv1, tv2, tv3;
    }
	
	LayoutInflater mInflater = null;
    Context activity;
    
    LeaderBoardAdapter(Context ctx) {
    	super();
    	data = new ArrayList<HighestScore>();
    	mInflater = (LayoutInflater) ctx.
                getSystemService(Context.LAYOUT_INFLATER_SERVICE);
    	activity = ctx;
    	requestUpdate();
    }
	
	List<HighestScore> data;
	boolean needMore = true;
	public void update(List<HighestScore> newData) {
		if(newData.size() < 10) {
			needMore = false;
		}
		data.addAll(newData);
		this.notifyDataSetChanged();
	}
	
	public void requestUpdate() {
		BmobQuery<HighestScore> query = new BmobQuery<HighestScore>();
		query.setLimit(10);
		query.setSkip(data.size());
		query.order("-score");
		query.findObjects(activity, new FindListener<HighestScore> () {
				@Override
				public void onError(int code, String msg) {
					AppActivity.makeToast("find error: " +code +" " + msg);
				}
				@Override
				public void onSuccess(List<HighestScore> objects) {
					AppActivity.makeToast("find succeed: "+objects.size()+"条数据。");
					update(objects);
				}
			 });
	}
	
	@Override
	public int getCount() {
		if(data.size() == 0) {
			return 0;
		} else if (needMore) {
			return data.size()+1;
		} else 
			return data.size();
	}

	@Override
	public Object getItem(int position) {
		return data.get(position);
	}

	@Override
	public long getItemId(int position) {
		return position;
	}

	@Override
	public View getView(int position, View convertView, ViewGroup parent) {
		final ViewHolder viewHolder;
		if (convertView == null) {
            convertView = mInflater.inflate(R.layout.list_leaderboard, null);

            viewHolder = new ViewHolder();
            viewHolder.tv1 = (TextView) convertView
                    .findViewById(R.id.tv1);
            viewHolder.tv2 = (TextView) convertView
                    .findViewById(R.id.tv2);
            viewHolder.tv3 = (TextView) convertView
                    .findViewById(R.id.tv3);

            convertView.setTag(viewHolder);
        } else {
            viewHolder = (ViewHolder) convertView.getTag();
        }
		if(position == data.size()) {
			viewHolder.tv1.setText("");
			viewHolder.tv2.setText("更多");
			viewHolder.tv3.setText("");
			convertView.setOnClickListener(new OnClickListener() {
				@Override
				public void onClick(View v) {
					requestUpdate();
					v.setClickable(false);
				}
				
			});
		} else {
			HighestScore rowData = data.get(position);
			
			viewHolder.tv1.setText((position+1)+"");
			viewHolder.tv2.setText(rowData.getPlayerName());
			viewHolder.tv3.setText(rowData.getScore()+"");
		}
		return convertView;
	}

}
