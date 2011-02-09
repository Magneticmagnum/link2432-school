package com.joe;

import min3d.core.Object3dContainer;
import min3d.vos.Number3d;

public class Firearm3d {
	public final static int LOOK_FREE = 1;
	public final static int LOOK_AIM = 2;
	public final static int LOOK_AIM_TO_FREE = 3;
	public final static int LOOK_FREE_TO_AIM = 4;
	private static final float RAD_TO_DEG = (float) (180 / Math.PI);
	private static final int AIM_FRAME = 6;
	// offset of iron sight
	private final Number3d ironOffset;
	// offset of hip/freelook
	private final Number3d freeOffset;
	private float scale;
	private LinearMover gunPos;
	private LinearMover crossHairs;
	private final Number3d CROSSHAIR_POSITION;
	private Object3dContainer object;
	private boolean rotatedY;
	private int lookMode;
	private final FirearmActor firearmActor;

	public Firearm3d(FirearmActor firearmActor, Number3d pos, Number3d target,
			Number3d ironOffset, Number3d freeOffset) {
		// wrap the number3d class in a looktarget to be used by linearmover
		this.firearmActor = firearmActor;
		CROSSHAIR_POSITION = target.clone();
		this.ironOffset = ironOffset;
		this.freeOffset = freeOffset;
		crossHairs = new LinearMover(new LookTarget(target));
		gunPos = new LinearMover(new LookTarget(pos));
	}

	public void scale(float f) {
		scale = f;
		object.scale().x = object.scale().y = object.scale().z = f;
	}

	public void setIrons(boolean irons) {
		if (irons) {
			if (lookMode == LOOK_FREE || lookMode == LOOK_AIM_TO_FREE)
				lookMode = LOOK_FREE_TO_AIM;
			gunPos.moveTo(this.ironOffset, AIM_FRAME);
		} else {
			if (lookMode == LOOK_AIM || lookMode == LOOK_FREE_TO_AIM)
				lookMode = LOOK_AIM_TO_FREE;
			gunPos.moveTo(this.freeOffset, AIM_FRAME);
			// move crosshair back to start
			resetCrosshair();
		}
	}

	public void pointAt(boolean isIron) {
		if (isIron)
			pointOffset(new Vector3(ironOffset.x, ironOffset.y, ironOffset.z));
		else
			pointOffset(new Vector3(freeOffset.x, freeOffset.y, freeOffset.z));
	}

	public void pointOffset(Vector3 offset) {
		Number3d toBack = gunPos.getMover().getNumber().clone();
		toBack.z = -1;
		Number3d dir = crossHairs.getMover().getNumber().clone();
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

	public void setPosition(Number3d n) {
		gunPos.set(n);
	}

	// moves the crosshair to default position linearly
	public void resetCrosshair() {
		crossHairs.moveTo(CROSSHAIR_POSITION, AIM_FRAME);
	}

	// instantly sets crosshair (no movement)
	public void setCrosshair(Number3d n) {
		crossHairs.set(n);
	}

	public void setLookMode(int lookMode) {
		this.lookMode = lookMode;
	}

	public void setRotatedY(boolean rotatedY) {
		this.rotatedY = rotatedY;
	}

	public void setObject(Object3dContainer object) {
		this.object = object;
	}

	// update look mode
	// also update weapon to point at crosshair
	public void update() {
		if (lookMode == LOOK_FREE_TO_AIM) {
			if (gunPos.isDone())
				lookMode = LOOK_AIM;
		} else if (lookMode == LOOK_AIM_TO_FREE) {
			if (gunPos.isDone())
				lookMode = LOOK_FREE;
		}

	}

	public LinearMover getCrossHairs() {
		return crossHairs;
	}

	public FirearmActor getFirearmActor() {
		return firearmActor;
	}

	public Number3d getFreeOffset() {
		return freeOffset;
	}

	public LinearMover getGunPos() {
		return gunPos;
	}

	public Number3d getIronOffset() {
		return ironOffset;
	}

	public int getLookMode() {
		return lookMode;
	}

	public Object3dContainer getObject() {
		return object;
	}

	public float getScale() {
		return scale;
	}

}
