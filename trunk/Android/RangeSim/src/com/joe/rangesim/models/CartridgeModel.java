package com.joe.rangesim.models;

public class CartridgeModel {

	public final float muzzleVelocity;
	public final float muzzleEnergy;
	public final String caliber;

	public CartridgeModel(String caliber, float muzzleVelocity,
			float muzzleEnergy) {
		// TODO Auto-generated constructor stub
		this.caliber = caliber;
		this.muzzleVelocity = muzzleVelocity;
		this.muzzleEnergy = muzzleEnergy;
	}

}
