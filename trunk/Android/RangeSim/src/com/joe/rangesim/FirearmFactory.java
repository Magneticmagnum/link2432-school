package com.joe.rangesim;

import min3d.core.Object3dContainer;
import min3d.parser.IParser;
import min3d.parser.Parser;
import min3d.vos.Number3d;
import android.app.Activity;
import android.content.res.Resources;

import com.joe.rangesim.models.CartridgeModel;
import com.joe.rangesim.models.FirearmActor;
import com.joe.rangesim.models.FirearmModel;
import com.joe.rangesim.models.Magazine;

public class FirearmFactory {

	private Activity caller;

	private final static CartridgeModel nineMmJhp = new CartridgeModel(
			"9MM JHP", 368, 643);
	private final static CartridgeModel nineMmFmj = new CartridgeModel(
			"9MM FMJ", 360, 518);
	private final static CartridgeModel fortySw = new CartridgeModel(
			".40 S&W JHP", 360, 575);
	private final static CartridgeModel fortyFiveAcp = new CartridgeModel(
			".45 ACP JHP", 320, 559);
	private final static CartridgeModel fiftyAe = new CartridgeModel(".50 AE",
			430, 1918);

	private final static FirearmModel glock19 = new FirearmModel("Glock 19",
			nineMmJhp, 1, FirearmModel.STYLE_HANDGUN);

	public final static Number3d IRONS_HANDGUN = new Number3d(0f, -0.3f, -1.75f);
	public final static Number3d FREELOOK_HANDGUN = new Number3d(0.75f, -1.25f,
			-2.5f);
	public final static Number3d POSITION_HANDGUN = new Number3d(0, 0, 0);
	public final static Number3d TARGET_HANDGUN = new Number3d(0, 0, -10);

	public FirearmFactory(Activity caller) {
		this.caller = caller;
	}

	public static Object3dContainer loadModel(Resources r, String p) {
		IParser parser = Parser.createParser(Parser.Type.OBJ, r, p, true);
		parser.parse();
		return parser.getParsedObject();
	}

	public Firearm3d makeGlock() {
		Object3dContainer g = loadModel(caller.getResources(),
				"com.joe:raw/glock19_obj");
		g.texturesEnabled(true);
		g.normalsEnabled(true);
		Magazine magazine = new Magazine("10 rd", 10);
		FirearmActor a = new FirearmActor(glock19, magazine);
		Firearm3d f = new Firearm3d(g, a, POSITION_HANDGUN, TARGET_HANDGUN,
				IRONS_HANDGUN, FREELOOK_HANDGUN);
		f.setRotatedY(true);
		f.scale((float) 0.05);
		return f;
	}
}
