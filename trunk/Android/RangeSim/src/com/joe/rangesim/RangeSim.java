package com.joe.rangesim;

import java.util.Hashtable;
import java.util.Map;
import java.util.Random;

import min3d.core.Object3dContainer;
import min3d.core.RendererActivity;
import min3d.objectPrimitives.Box;
import min3d.objectPrimitives.Rectangle;
import min3d.parser.IParser;
import min3d.parser.Parser;
import min3d.vos.Color4;
import min3d.vos.Light;
import min3d.vos.LightType;
import min3d.vos.Number3d;
import android.graphics.Bitmap;
import android.hardware.SensorManager;
import android.os.Bundle;
import android.view.MotionEvent;

import com.joe.R;
import com.joe.rangesim.sensor.OrientationManager;
import com.joe.rangesim.tools.LinearMover;
import com.joe.rangesim.tools.LookTarget;
import com.joe.rangesim.tools.Vector3;

public class RangeSim extends RendererActivity {
	private Map<String, Object3dContainer> models;
	private Map<String, Firearm3d> guns;
	private Map<String, LinearMover> moveSolvers;
	private String currentGun = "Glock 19";
	private LinearMover cameraMover;
	private LinearMover cameraTarget;
	private final Number3d CAMERA_TARGET = new Number3d(0, 0, -10);
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

	public void singleTouch(float x, float y, int action) {
		Firearm3d g = getGun();
		if (action == MotionEvent.ACTION_DOWN
				|| action == MotionEvent.ACTION_MOVE) {
			if (g.getLookMode() == Firearm3d.LOOK_FREE
					|| g.getLookMode() == Firearm3d.LOOK_AIM_TO_FREE) {
				g.getCrossHairs().moveTo(cameraTarget.getMover().getNumber(),
						Firearm3d.AIM_FRAME);
				g.setIrons(true);
			}

		} else {
			if (g.getLookMode() == Firearm3d.LOOK_AIM
					|| g.getLookMode() == Firearm3d.LOOK_FREE_TO_AIM) {
				g.setIrons(false);
				cameraTarget.moveTo(CAMERA_TARGET, Firearm3d.AIM_FRAME);
			}
		}
	}

	@Override
	public boolean onTouchEvent(MotionEvent event) {
		// TODO Auto-generated method stub
		int action = event.getAction();
		if (event.getPointerCount() > 1) {
			// multitouch
		} else {
			singleTouch(event.getX(), event.getY(), action);
		}
		return super.onTouchEvent(event);
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
		IParser parser = Parser.createParser(Parser.Type.OBJ, getResources(),
				"com.joe:raw/range_obj", true);
		parser.parse();
		Object3dContainer range = parser.getParsedObject();
		Rectangle target = new Rectangle(1, 1, 4, 4);
		target.normalsEnabled(true);

		Bitmap targTex = min3d.Utils.makeBitmapFromResourceId(this,
				R.raw.target);
		min3d.Shared.textureManager().addTextureId(targTex, "target");
		target.textures().addById("target");
		targTex.recycle();

		range.scale().x = range.scale().y = range.scale().z = 1f;
		range.position().setAll(0, 0, -30);
		range.rotation().y = 180;
		target.position().setAll(0, 0, -10);
		target.rotation().y = 180;

		scene.camera().target.setAllFrom(CAMERA_TARGET);
		cameraTarget = new LinearMover(new LookTarget(scene.camera().target));
		cameraMover = new LinearMover(new LookTarget(scene.camera().position));

		models.put("range", range);

		FirearmFactory fMaker = new FirearmFactory(this);
		Firearm3d gun = fMaker.makeGlock();

		scene.addChild(gun.getObject());
		scene.addChild(range);
		scene.addChild(target);

		moveSolvers.put("crosshair", gun.getCrossHairs());
		moveSolvers.put("gun", gun.getGunPos());
		moveSolvers.put("cameraTarget", cameraTarget);
		moveSolvers.put("cameraPos", cameraMover);

		cameraMover.moveTo(new Number3d(0, 0, 2), 12);

		guns.put(gun.getFirearmActor().getFirearmModel().getName(), gun);

		gun.setIrons(false);
	}

	@Override
	public void updateScene() {
		final float aimscale = 0.025f;
		final float freescale = 0.08f;

		// update linear movers
		for (LinearMover m : moveSolvers.values())
			m.update();
		Firearm3d gun = getGun();
		gun.update();

		if (gun.getLookMode() == Firearm3d.LOOK_AIM) {
			// aim logic here
			float x = (float) (getVelocity()[0] * aimscale);
			float y = (float) (getVelocity()[1] * aimscale);
			cameraTarget.add(x, y, 0);
			gun.getCrossHairs().set(cameraTarget.getMover().getNumber());
			Vector3 irons = new Vector3(gun.getIronOffset().x,
					gun.getIronOffset().y, gun.getIronOffset().z);
			gun.getGunPos().getMover().getNumber()
					.setAllFrom(cameraMover.getMover().getNumber());
			gun.pointOffset(irons);

		} else if (gun.getLookMode() == Firearm3d.LOOK_FREE) {
			// free look logic here
			float x = getVelocity()[0] * freescale;
			float y = getVelocity()[1] * freescale;

			gun.getCrossHairs().add(x * 2, y * 2, 0);
			cameraTarget.add(x, y, 0);

			gun.getGunPos().set(cameraMover.getMover().getNumber());
			Vector3 free = new Vector3(gun.getFreeOffset().x,
					gun.getFreeOffset().y, gun.getFreeOffset().z);
			gun.pointOffset(free);
		}
	}

	public void viewKick() {
		Random r = new Random();
		Number3d old = cameraTarget.getMover().getNumber();
		old.x += r.nextFloat() - 0.5;
		old.y += r.nextFloat() - 0.5;
		cameraTarget.getMover().getNumber()
				.setAll(r.nextFloat() * 2, r.nextFloat() * 2 + 1, 0);
		cameraTarget.moveTo(old, 12);
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