package com.joe;

import min3d.core.Object3dContainer;
import min3d.vos.Number3d;

public class FirearmFactory {

	public static Firearm makePistol(Object3dContainer mdl) {
		Firearm f = new Firearm();
		f.hip = Firearm.HIP_HANDGUN;
		f.irons = Firearm.IRONS_HANDGUN;
		f.lookAt = new Number3d(0, 0, 0);
		f.object = mdl;
		f.rotatedY = true;
		f.scale((float) 0.05);
		return f;
	}
}
