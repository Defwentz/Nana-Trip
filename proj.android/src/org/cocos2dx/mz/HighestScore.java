package org.cocos2dx.mz;

import cn.bmob.v3.BmobObject;

public class HighestScore extends BmobObject {
	private String playerName;
	private Integer score;
	public String getPlayerName() {
        return playerName;
    }

    public void setPlayerName(String playerName) {
        this.playerName = playerName;
    }

    public Integer getScore() {
        return score;
    }

    public void setScore(Integer score) {
        this.score = score;
    }
}
