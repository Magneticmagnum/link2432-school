package com.joe;

import java.util.Hashtable;
import java.util.Map;

import javax.microedition.khronos.opengles.GL10;

import min3d.Shared;
import min3d.Utils;
import min3d.core.Object3dContainer;
import min3d.core.RendererActivity;
import min3d.core.Scene;
import min3d.objectPrimitives.Box;
import min3d.objectPrimitives.Rectangle;
import min3d.objectPrimitives.SkyBox;
import min3d.objectPrimitives.SkyBox.Face;
import min3d.parser.IParser;
import min3d.parser.Parser;
import min3d.vos.Color4;
import min3d.vos.Light;
import min3d.vos.LightType;
import min3d.vos.Number3d;
import min3d.vos.TextureVo;
import android.content.res.Resources;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.graphics.Camera;
import android.hardware.SensorManager;
import android.os.Bundle;
import android.util.Log;
import android.view.MotionEvent;

public class RangeSim extends RendererActivity {
	private static RangeSim rsim;
	private Map<String, Object3dContainer> models;
	private Map<String, Firearm> guns;
	private final static Number3d headPos = new Number3d(0f, 0f, 0f);
	private final static int LOOK_FREE = 1;
	private final static int LOOK_AIM = 2;
	private final static int LOOK_AIM_TO_FREE = 3;
	private final static int LOOK_FREE_TO_AIM = 4;
	private final static int INTERP_FRAMES = 25;
	private int frameCount;
	private final static float LOOK_TARGET_RANGE = -20f;
	private int lookMode;
	private static OrientationManager orientationManager;

	@Override
	protected void onCreate(Bundle savedInstanceState) {
		// TODO Auto-generated method stub
		super.onCreate(savedInstanceState);
		models = new Hashtable<String, Object3dContainer>();
		guns = new Hashtable<String, Firearm>();
		rsim = this;
		orientationManager = new OrientationManager(
				(SensorManager) getSystemService(SENSOR_SERVICE), 1);
		orientationManager.init();

	}

	private boolean ironUp, hipUp;

	@Override
	public boolean onTouchEvent(MotionEvent event) {
		// TODO Auto-generated method stub
		if (event.getAction() == MotionEvent.ACTION_DOWN
				|| event.getAction() == MotionEvent.ACTION_MOVE) {
			if (!ironUp) {
				RangeSim.engageIrons();
				ironUp = true;
				hipUp = false;
			}
		} else {
			if (!hipUp) {
				RangeSim.disEngageIrons();
				hipUp = true;
				ironUp = false;
			}
		}
		return super.onTouchEvent(event);
	}

	protected static void engageIrons() {
		// TODO Auto-generated method stub
		// get angle between camera target and into the screen
		Firearm gun = rsim.guns.get("glock19");
		rsim.lookMode = RangeSim.LOOK_FREE_TO_AIM;
		rsim.frameCount = 0;
		Number3d cameraTarg = rsim.scene.camera().target;
		gun.lookAt.setAllFrom(cameraTarg);
	}

	protected static void disEngageIrons() {
		// TODO Auto-generated method stub
		Firearm gun = rsim.guns.get("glock19");
		rsim.scene.camera().position.setAllFrom(headPos);
		rsim.scene.camera().target.setAll(0, 0, LOOK_TARGET_RANGE);
		gun.lookAt.setAll(0, 0, LOOK_TARGET_RANGE);
		rsim.lookMode = RangeSim.LOOK_AIM_TO_FREE;
		rsim.frameCount = 0;

	}

	public static Object3dContainer loadModel(Resources r, String p) {
		IParser parser = Parser.createParser(Parser.Type.OBJ, r, p, true);
		parser.parse();
		return parser.getParsedObject();
	}

	@Override
	public void initScene() {
		scene.camera().frustum.zNear(1);
		Light light = new Light();
		scene.lights().add(light);
		light.position.setAll(3f, 5f, -1f);
		light.type(LightType.POSITIONAL);

		scene.lightingEnabled(true);
		light.isVisible(true);

		Object3dContainer g = loadModel(getResources(),
				"com.joe:raw/glock19_obj");
		Object3dContainer range = loadModel(getResources(),
				"com.joe:raw/range_obj");
		Firearm gun = FirearmFactory.makePistol(g);
		gun.object.texturesEnabled(true);
		Rectangle target = new Rectangle(1, 1, 4, 4);
		Box laser = new Box(0.2f, 0.2f, 400);
		laser.defaultColor(new Color4(255, 0, 0, 0));
		laser.normalsEnabled(true);
		laser.colorMaterialEnabled(true);
		target.normalsEnabled(true);
		Bitmap targTex = Utils.makeBitmapFromResourceId(this, R.raw.target);
		Shared.textureManager().addTextureId(targTex, "target");
		target.textures().addById("target");
		targTex.recycle();

		scene.addChild(gun.object);
		scene.addChild(range);
		scene.addChild(target);
		gun.object.addChild(laser);
		laser.position().setAllFrom(gun.object.position());
		laser.position().z += 200;
		laser.position().y -= 10;

		guns.put("glock19", gun);
		models.put("range", range);

		range.scale().x = range.scale().y = range.scale().z = 1f;
		range.position().setAll(0, 0, -30);
		range.rotation().y = 180;

		target.position().setAll(0, 0, -10);
		target.rotation().y = 180;
		scene.camera().target.z = LOOK_TARGET_RANGE;
		disEngageIrons();
	}

	@Override
	public void updateScene() {
		final float aimscale = 0.0075f;
		final float rotscale = 0.03f;
		final float freescale = 0.04f;

		final float RAD_TO_DEG = 1 / Utils.DEG;
		Firearm gun = guns.get("glock19");
		if (lookMode == LOOK_AIM) {
			scene.camera().position.x -= getVelocity()[0] * aimscale * 0.5;
			scene.camera().position.y -= getVelocity()[1] * aimscale * 0.5;
			scene.camera().target.x += getVelocity()[0] * aimscale * 0.25;
			scene.camera().target.y += getVelocity()[1] * aimscale * 0.25;
			gun.setPosition(scene.camera().position);
			gun.pointAt(true);

		}

		else if (lookMode == LOOK_FREE) {
			scene.camera().target.x += getVelocity()[0] * freescale;
			scene.camera().target.y += getVelocity()[1] * freescale;
			gun.lookAt.x += getVelocity()[0] * freescale * 2;
			gun.lookAt.y += getVelocity()[1] * freescale * 2;

			gun.setPosition(scene.camera().position);
			gun.pointAt(false);
		} else if (lookMode == LOOK_AIM_TO_FREE) {
			float i = (float) frameCount / INTERP_FRAMES;
			gun.setPosition(scene.camera().position);
			gun.pointTransition(1 - i);
			scene.camera().target.x += getVelocity()[0] * freescale;
			scene.camera().target.y += getVelocity()[1] * freescale;
			gun.lookAt.x += getVelocity()[0] * freescale * 2;
			gun.lookAt.y += getVelocity()[1] * freescale * 2;
			frameCount++;
			if (frameCount > INTERP_FRAMES) {
				lookMode = LOOK_FREE;
			}
		} else if (lookMode == LOOK_FREE_TO_AIM) {
			float i = (float) frameCount / INTERP_FRAMES;
			gun.setPosition(scene.camera().position);
			gun.pointTransition(i);
			scene.camera().position.x -= getVelocity()[0] * aimscale * 0.5;
			scene.camera().position.y -= getVelocity()[1] * aimscale * 0.5;
			scene.camera().target.x += getVelocity()[0] * aimscale * 0.25;
			scene.camera().target.y += getVelocity()[1] * aimscale * 0.25;
			frameCount++;
			if (frameCount > INTERP_FRAMES) {
				lookMode = LOOK_AIM;
			}
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
}