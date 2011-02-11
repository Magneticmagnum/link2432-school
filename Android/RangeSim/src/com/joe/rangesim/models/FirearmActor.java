package com.joe.rangesim.models;

/*
 * handles all logic of a firearm
 */
public class FirearmActor {

	private boolean hot;
	private boolean aiming;
	private final Magazine magazine;
	private final FirearmModel firearmModel;

	public FirearmActor(FirearmModel fModel, Magazine magazine) {
		// TODO Auto-generated constructor stub
		this.firearmModel = fModel;
		this.magazine = magazine;
	}

	public FirearmModel getFirearmModel() {
		return firearmModel;
	}

	public Magazine getMagazine() {
		return magazine;
	}

	public boolean isHot() {
		return hot;
	}

	public boolean isAiming() {
		return aiming;
	}

}
