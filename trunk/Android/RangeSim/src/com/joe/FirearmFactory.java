package com.joe;

import min3d.core.Object3dContainer;
import min3d.vos.Number3d;

public class FirearmFactory {

	private final static Cartridge nineMmJhp = new Cartridge("9MM JHP", 368,
			643);
	private final static Cartridge nineMmFmj = new Cartridge("9MM FMJ", 360,
			518);
	private final static Cartridge fortySw = new Cartridge(".40 S&W JHP", 360,
			575);
	private final static Cartridge fortyFiveAcp = new Cartridge(".45 ACP JHP",
			320, 559);
	private final static Cartridge fiftyAe = new Cartridge(".50 AE", 430, 1918);

	public final static Number3d IRONS_HANDGUN = new Number3d(0f, -0.3f, -1.75f);
	public final static Number3d FREELOOK_HANDGUN = new Number3d(0.75f, -0.75f,
			-2.5f);
	public final static Number3d POSITION_HANDGUN = new Number3d(0, 0, 0);
	public final static Number3d TARGET_HANDGUN = new Number3d(0, 0, -10);

	public static Firearm3d makePistol(Object3dContainer mdl) {
		Firearm3d f = new Firearm3d(POSITION_HANDGUN, TARGET_HANDGUN,
				IRONS_HANDGUN, FREELOOK_HANDGUN);
		f.setObject(mdl);
		f.setRotatedY(true);
		f.scale((float) 0.05);
		return f;
	}
}
