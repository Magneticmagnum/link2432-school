package com.joe;

import min3d.core.Object3dContainer;
import min3d.vos.Number3d;

public class Firearm {
	public final static Number3d IRONS_HANDGUN = new Number3d(0f, -0.3f, -1.75f);
	public final static Number3d HIP_HANDGUN = new Number3d(0.75f, -0.75f,
			-2.5f);
	private static final float RAD_TO_DEG = (float) (180 / Math.PI);
	// offset of iron sight
	public Number3d irons;
	// offset of hip
	public Number3d hip;
	public float scale;
	public Number3d lookAt;
	public Object3dContainer object;
	public boolean rotatedY;

	public Firearm() {
	}

	public Number3d getLookAt() {
		return lookAt;
	}

	public void scale(float f) {
		scale = f;
		object.scale().x = object.scale().y = object.scale().z = f;
	}

	public void pointAt(boolean isIron) {
		if (isIron)
			pointOffset(new Vector3(irons.x, irons.y, irons.z));
		else
			pointOffset(new Vector3(hip.x, hip.y, hip.z));
	}

	public void pointOffset(Vector3 offset) {
		Number3d toBack = object.position().clone();
		toBack.z = -1;
		Number3d dir = lookAt.clone();
		dir.subtract(object.position());
		Quaternion bet = com.joe.Utils.quaternionBetween(dir, toBack);
		Number3d euls = com.joe.Utils.rot2Euler(bet);
		euls.multiply(RAD_TO_DEG);
		offset.rotate(bet);

		object.rotation().setAllFrom(euls);
		if (rotatedY)
			object.rotation().y += 180;
		object.position().add(offset);
	}

	// smooth cubic interpolation
	// 0 is hip, 1 is irons
	public void pointTransition(float i) {
		float x = Utils.splineTransition(hip.x, 0, irons.x, 0, i);
		float y = Utils.splineTransition(hip.y, 0, irons.y, 0, i);
		float z = Utils.splineTransition(hip.z, 0, irons.z, 0, i);

		Vector3 trans = new Vector3(x, y, z);
		pointOffset(trans);
	}

	public void setPosition(Number3d n) {
		object.position().setAllFrom(n);
	}

}
