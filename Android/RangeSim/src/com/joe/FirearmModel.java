package com.joe;

/*
 * Model class containing all firearm properties
 * Nothing to do with .obj model
 */
public class FirearmModel {

	public final static int MODE_SEMI = 1;
	public final static int MODE_AUTO = 2;
	public final static int MODE_BOLT = 3;
	public final static int STYLE_HANDGUN = 1;
	public final static int STYLE_RIFLE = 2;
	private final int firearmStyle;
	private final int rateOfFire;
	private final Cartridge chamber;
	private final String name;
	private boolean selectiveFire;
	private int fireMode;

	public FirearmModel(String name, Cartridge c, int rof, int style) {
		// TODO Auto-generated constructor stub
		this.name = name;
		this.chamber = c;
		this.rateOfFire = rof;
		this.firearmStyle = style;
		this.fireMode = MODE_SEMI;
	}

	public Cartridge getChamber() {
		return chamber;
	}

	public int getFirearmStyle() {
		return firearmStyle;
	}

	public int getFireMode() {
		return fireMode;
	}

	public String getName() {
		return name;
	}

	public int getRateOfFire() {
		return rateOfFire;
	}

	public boolean isSelectiveFire() {
		return selectiveFire;
	}

}
