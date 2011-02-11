package com.joe.rangesim.sensor;

import android.hardware.Sensor;
import android.hardware.SensorEvent;
import android.hardware.SensorEventListener;
import android.hardware.SensorManager;

public class OrientationManager {
	private SensorManager mSensorManager;
	private Sensor mAccelerometer;
	private Sensor mMagnet;
	private final SensorEventListener mAccelListener = new SensorEventListener() {

		@Override
		public void onSensorChanged(SensorEvent event) {
			// alpha is calculated as t / (t + dT)
			// with t, the low-pass filter's time-constant
			// and dT, the event delivery rate

			final float alpha = (float) 0.8;

			mGrav[0] = alpha * mGrav[0] + (1 - alpha) * event.values[0];
			mGrav[1] = alpha * mGrav[1] + (1 - alpha) * event.values[1];
			mGrav[2] = alpha * mGrav[2] + (1 - alpha) * event.values[2];

			mVel[0] = event.values[0] - mGrav[0];
			mVel[1] = event.values[1] - mGrav[1];
			mVel[2] = event.values[2] - mGrav[2];

			mPos[0] += event.values[0] - mGrav[0];
			mPos[1] += event.values[1] - mGrav[1];
			mPos[2] += event.values[2] - mGrav[2];

		}

		@Override
		public void onAccuracyChanged(Sensor sensor, int accuracy) {
			// TODO Auto-generated method stub

		}
	};
	private final SensorEventListener mMagnetListener = new SensorEventListener() {

		@Override
		public void onAccuracyChanged(Sensor sensor, int accuracy) {
			// TODO Auto-generated method stub

		}

		@Override
		public void onSensorChanged(SensorEvent event) {
			final float alpha = 0.6f;
			mMag[0] = mMag[0] * alpha + (1 - alpha) * event.values[0];
			mMag[1] = mMag[1] * alpha + (1 - alpha) * event.values[1];
			mMag[2] = mMag[2] * alpha + (1 - alpha) * event.values[2];
			// calculate orientation
			final float rot[] = new float[16];
			SensorManager.getRotationMatrix(rot, mInclination, mGrav, mMag);
			// hi pass filter it
			for (int i = 0; i < 16; i++)
				mRotation[i] = mRotation[i] * alpha + (1 - i) * rot[i];
			final float angs[] = new float[3];
			float[] outR = new float[16];
			SensorManager.remapCoordinateSystem(rot, SensorManager.AXIS_X,
					SensorManager.AXIS_Z, outR);
			SensorManager.getOrientation(outR, angs);
			mOmega[0] = angs[0] - mAngle[0];
			mOmega[1] = angs[1] - mAngle[1];
			mOmega[2] = angs[2] - mAngle[2];
			mAngle[0] = angs[0];
			mAngle[1] = angs[1];
			mAngle[2] = angs[2];

		}

	};
	private float mAngle[];
	private float mOmega[];
	private float mPos[];
	private float mGrav[];
	private float mMag[];
	private float mVel[];

	private float mRotation[];
	private float mInclination[];

	public OrientationManager(SensorManager sm, int sampleSize) {
		// TODO Auto-generated constructor stub
		mAngle = new float[3];
		mOmega = new float[3];
		mPos = new float[3];
		mGrav = new float[3];
		mMag = new float[3];
		mVel = new float[3];
		mRotation = new float[16];
		mInclination = new float[9];
		mSensorManager = sm;
		mAccelerometer = sm.getDefaultSensor(Sensor.TYPE_ACCELEROMETER);
		mMagnet = sm.getDefaultSensor(Sensor.TYPE_MAGNETIC_FIELD);

	}

	public void init() {
		mSensorManager.registerListener(mAccelListener, mAccelerometer,
				SensorManager.SENSOR_DELAY_GAME);
		mSensorManager.registerListener(mMagnetListener, mMagnet,
				SensorManager.SENSOR_DELAY_GAME);
	}

	public void onResume() {
		init();
	}

	public void onPause() {
		mSensorManager.unregisterListener(mAccelListener);
		mSensorManager.unregisterListener(mMagnetListener);
	}

	public float[] getAngles() {
		return mAngle;
	}

	public float[] getOmega() {
		return mOmega;
	}

	public float[] getRotationMatrix() {
		return mRotation;
	}

	public float[] getPosition() {
		// TODO Auto-generated method stub
		return mPos;
	}

	public float[] getVelocity() {
		// TODO Auto-generated method stub
		return mVel;
	}
}
