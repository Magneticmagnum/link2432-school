package com.joe;

import min3d.core.Object3dContainer;
import min3d.vos.Number3d;

public class FirearmFactory {

	public final static Number3d IRONS_HANDGUN = new Number3d(0f, -0.3f, -1.75f);
	public final static Number3d FREELOOK_HANDGUN = new Number3d(0.75f, -0.75f,
			-2.5f);
	public final static Number3d POSITION_HANDGUN = new Number3d(0, 0, 0);
	public final static Number3d TARGET_HANDGUN = new Number3d(0, 0, -10);

	public static Firearm makePistol(Object3dContainer mdl) {
		Firearm f = new Firearm(POSITION_HANDGUN, TARGET_HANDGUN,
				IRONS_HANDGUN, FREELOOK_HANDGUN);
		f.setObject(mdl);
		f.setRotatedY(true);
		f.scale((float) 0.05);
		return f;
	}
}
