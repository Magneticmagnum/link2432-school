package com.joe;

import java.util.Hashtable;
import java.util.Map;

import min3d.core.Object3dContainer;
import min3d.core.RendererActivity;
import min3d.objectPrimitives.Box;
import min3d.objectPrimitives.Rectangle;
import min3d.parser.IParser;
import min3d.parser.Parser;
import min3d.vos.Color4;
import min3d.vos.Light;
import min3d.vos.LightType;
import android.content.res.Resources;
import android.graphics.Bitmap;
import android.hardware.SensorManager;
import android.os.Bundle;
import android.view.MotionEvent;

public class RangeSim extends RendererActivity {
	private Map<String, Object3dContainer> models;
	private Map<String, Firearm3d> guns;
	private String currentGun = "GLOCK19";
	private Map<String, LinearMover> moveSolvers;
	private LinearMover cameraMover;
	private LinearMover cameraTarget;
	private static OrientationManager orientationManager;

	@Override
	protected void onCreate(Bundle savedInstanceState) {
		// TODO Auto-generated method stub
		super.onCreate(savedInstanceState);
		models = new Hashtable<String, Object3dContainer>();
		guns = new Hashtable<String, Firearm3d>();
		moveSolvers = new Hashtable<String, LinearMover>();
		orientationManager = new OrientationManager(
				(SensorManager) getSystemService(SENSOR_SERVICE), 1);
		orientationManager.init();

	}

	@Override
	public boolean onTouchEvent(MotionEvent event) {
		// TODO Auto-generated method stub
		Firearm3d g = getGun();
		if (event.getAction() == MotionEvent.ACTION_DOWN
				|| event.getAction() == MotionEvent.ACTION_MOVE) {
			if (g.getLookMode() == Firearm3d.LOOK_FREE
					|| g.getLookMode() == Firearm3d.LOOK_AIM_TO_FREE)
				g.setIrons(true);
			else
				g.setIrons(false);
		}
		return super.onTouchEvent(event);
	}

	public static Object3dContainer loadModel(Resources r, String p) {
		IParser parser = Parser.createParser(Parser.Type.OBJ, r, p, true);
		parser.parse();
		return parser.getParsedObject();
	}

	@Override
	public void initScene() {
		Light light = new Light();
		scene.lights().add(light);
		light.position.setAll(3f, 5f, -1f);
		light.type(LightType.POSITIONAL);

		scene.lightingEnabled(true);
		light.isVisible(true);

		// model loading
		Object3dContainer g = loadModel(getResources(),
				"com.joe:raw/glock19_obj");
		Object3dContainer range = loadModel(getResources(),
				"com.joe:raw/range_obj");
		g.texturesEnabled(true);
		Rectangle target = new Rectangle(1, 1, 4, 4);
		Box laser = new Box(0.2f, 0.2f, 400);
		laser.defaultColor(new Color4(255, 0, 0, 0));
		target.normalsEnabled(true);
		laser.normalsEnabled(true);
		laser.colorMaterialEnabled(true);

		Bitmap targTex = min3d.Utils.makeBitmapFromResourceId(this,
				R.raw.target);
		min3d.Shared.textureManager().addTextureId(targTex, "target");
		target.textures().addById("target");
		targTex.recycle();

		laser.position().setAllFrom(g.position());
		laser.position().z += 200;
		laser.position().y -= 10;
		range.scale().x = range.scale().y = range.scale().z = 1f;
		range.position().setAll(0, 0, -30);
		range.rotation().y = 180;
		target.position().setAll(0, 0, -10);
		target.rotation().y = 180;

		g.addChild(laser);
		scene.addChild(g);
		scene.addChild(range);
		scene.addChild(target);

		scene.camera().target.z = -10;
		cameraTarget = new LinearMover(new LookTarget(scene.camera().target));
		cameraMover = new LinearMover(new LookTarget(scene.camera().position));

		models.put("range", range);

		Firearm3d gun = FirearmFactory.makePistol(g);
		moveSolvers.put("crosshair", gun.getCrossHairs());
		moveSolvers.put("gun", gun.getGunPos());
		moveSolvers.put("cameraTarget", cameraTarget);
		moveSolvers.put("cameraPos", cameraMover);

		guns.put("GLOCK19", gun);

		gun.setIrons(false);
	}

	@Override
	public void updateScene() {
		final float aimscale = 0.0075f;
		final float freescale = 0.04f;

		// update linear movers
		for (LinearMover e : moveSolvers.values())
			e.update();
		Firearm3d gun = getGun();
		gun.update();

		if (gun.getLookMode() == Firearm3d.LOOK_AIM) {
			// aim logic here
			float x = (float) (getVelocity()[0] * aimscale * 0.5);
			float y = (float) (getVelocity()[1] * aimscale * 0.5);
			cameraMover.add(-x, -y, 0);
			cameraTarget.add(x * .25f, y * .25f, 0);
			gun.getCrossHairs().set(cameraTarget.getMover().getNumber());

			gun.setPosition(cameraMover.getMover().getNumber());
			Vector3 irons = new Vector3(gun.getIronOffset().x,
					gun.getIronOffset().y, gun.getIronOffset().z);
			gun.pointOffset(irons);
		} else if (gun.getLookMode() == Firearm3d.LOOK_FREE) {
			// free look logic here
			float x = getVelocity()[0] * freescale;
			float y = getVelocity()[1] * freescale;

			gun.getCrossHairs().add(-x * 2, -y * 2, 0);

			gun.setPosition(cameraMover.getMover().getNumber());
			Vector3 irons = new Vector3(gun.getIronOffset().x,
					gun.getIronOffset().y, gun.getIronOffset().z);
			gun.pointOffset(irons);
		}
	}

	protected void onResume() {
		super.onResume();
		orientationManager.onResume();
	}

	protected void onPause() {
		super.onPause();
		orientationManager.onPause();
	}

	public static float[] getOrientationAngles() {
		return orientationManager.getAngles();
	}

	public static float[] getOrientationMatrix() {
		return orientationManager.getRotationMatrix();
	}

	public static float[] getOmega() {
		return orientationManager.getOmega();
	}

	public static float[] getVelocity() {
		return orientationManager.getVelocity();
	}

	public Firearm3d getGun() {
		return guns.get(currentGun);
	}
}