package com.joe;

import android.util.FloatMath;

public class Quaternion {
	public float w;
	public float x;
	public float y;
	public float z;

	// from euler angles
	public Quaternion(float ex, float ey, float ez) {
		// TODO Auto-generated constructor stub
		float cz = FloatMath.cos(.5f * ez);
		float cy = FloatMath.cos(.5f * ey);
		float cx = FloatMath.cos(.5f * ex);
		float sz = FloatMath.sin(.5f * ez);
		float sy = FloatMath.sin(.5f * ey);
		float sx = FloatMath.sin(.5f * ex);
		this.w = cz * cy * cx + sz * sy * sx;
		this.x = cz * cy * sx - sz * sy * cx;
		this.y = cz * sy * cx + sz * cy * sx;
		this.z = sz * cy * cx - cz * sy * sx;
	}

	public Quaternion(float w, float x, float y, float z) {
		this.w = w;
		this.x = x;
		this.y = y;
		this.z = z;
	}

	public Quaternion(Vector3 angs) {
		this(angs.x, angs.y, angs.z);

	}

	public static Quaternion add(Quaternion a, Quaternion b) {
		float w = a.w + b.w;
		float x = a.x + b.x;
		float y = a.y + b.y;
		float z = a.z + b.z;
		return new Quaternion(w, x, y, z);
	}

	public static Quaternion sub(Quaternion a, Quaternion b) {
		float w = a.w - b.w;
		float x = a.x - b.x;
		float y = a.y - b.y;
		float z = a.z - b.z;
		return new Quaternion(w, x, y, z);
	}

	public static Quaternion mult(Quaternion a, Quaternion b) {
		Quaternion q = new Quaternion(0, 0, 0, 0);
		float adb = a.x * b.x + a.y * b.y + a.z * b.z;
		q.w = a.w * b.w - adb;
		float x = a.x, y = a.y, z = a.z;
		q.x = y * b.z - z * b.y + a.w * b.x + x * b.w;
		q.y = z * b.x - x * b.z + a.w * b.y + y * b.w;
		q.z = x * b.y - y * b.x + a.w * b.z + z * b.w;
		return q;
	}

	public static Quaternion div(Quaternion a, Quaternion b) {
		Quaternion q = new Quaternion(b);
		invert(a);
		mult(a, q);
	}

	private static void invert(Quaternion a) {
		// TODO Auto-generated method stub
		conjugate(a);

	}
}
