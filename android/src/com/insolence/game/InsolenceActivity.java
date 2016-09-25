package com.insolence.game;

import org.libsdl.app.SDLActivity;
import android.app.Activity;
import android.os.Bundle;
import android.content.Context;
import android.content.res.AssetManager;

public class InsolenceActivity extends SDLActivity
{
	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		nativeSetAssetManager(this.getAssets());
	}

	private static native void nativeSetAssetManager(AssetManager assetManager);
}
