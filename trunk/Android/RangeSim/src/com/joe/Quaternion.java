package com.joe;

import android.util.FloatMath;

public class Quaternion {
	private static final float PIOVER180 = (float) (Math.PI / 180.0);
	public float w;
	public float x;
	public float y;
	public float z;

	public Quaternion() {
		this(0, 0, 0, 0);
	}

	// from euler angles
	public Quaternion(float ex, float ey, float ez) {
		// TODO Auto-generated constructor stub
		reinit(this, ex, ey, ez);
	}

	public void reinit(float ex, float ey, float ez) {
		reinit(this, ex, ey, ez);
	}

	// Convert from Euler Angles
	public static Quaternion euler2Rot(float pitch, float yaw, float roll) {
		Quaternion q = new Quaternion();
		float p = pitch * PIOVER180 / 2.0f;
		float y = yaw * PIOVER180 / 2.0f;
		float r = roll * PIOVER180 / 2.0f;

		float sinp = (float) FloatMath.sin(p);
		float siny = (float) FloatMath.sin(y);
		float sinr = (float) FloatMath.sin(r);
		float cosp = (float) FloatMath.cos(p);
		float cosy = (float) FloatMath.cos(y);
		float cosr = (float) FloatMath.cos(r);

		q.x = sinr * cosp * cosy - cosr * sinp * siny;
		q.y = cosr * sinp * cosy + sinr * cosp * siny;
		q.z = cosr * cosp * siny - sinr * sinp * cosy;
		q.w = cosr * cosp * cosy + sinr * sinp * siny;

		q.normalize();
		return q;
	}

	// Convert from Axis Angle
	public static Quaternion fromAxis(Vector3 v, float angle) {
		Quaternion q = new Quaternion();
		float sinAngle;
		angle *= 0.5f;
		Vector3 vn = new Vector3(v);
		vn.normalize();

		sinAngle = FloatMath.sin(angle);

		q.x = (vn.x * sinAngle);
		q.y = (vn.y * sinAngle);
		q.z = (vn.z * sinAngle);
		q.w = FloatMath.cos(angle);
		return q;
	}

	public static void reinit(Quaternion q, float ex, float ey, float ez) {
		float cz = (float) FloatMath.cos(.5f * ez);
		float cy = (float) FloatMath.cos(.5f * ey);
		float cx = (float) FloatMath.cos(.5f * ex);
		float sz = (float) FloatMath.sin(.5f * ez);
		float sy = (float) FloatMath.sin(.5f * ey);
		float sx = (float) FloatMath.sin(.5f * ex);
		q.w = cz * cy * cx + sz * sy * sx;
		q.x = cz * cy * sx - sz * sy * cx;
		q.y = cz * sy * cx + sz * cy * sx;
		q.z = sz * cy * cx - cz * sy * sx;
	}

	public Quaternion(Quaternion a) {
		this(a.w, a.x, a.y, a.z);
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
		invert(q);
		return mult(a, q);
	}

	public static void scale(Quaternion a, float b) {
		a.w *= b;
		a.x *= b;
		a.y *= b;
		a.z *= b;
	}

	public static void invert(Quaternion a) {
		// TODO Auto-generated method stub
		conjugate(a);
		scale(a, 1.0f / a.length_squared(a));
	}

	public void conjugate() {
		conjugate(this);
	}

	public Quaternion normalize() {
		scale(this, 1 / (float) FloatMath.sqrt(length_squared(this)));
		return this;
	}

	public static void conjugate(Quaternion a) {
		a.x = -a.x;
		a.y = -a.y;
		a.z = -a.z;
	}

	public static float length_squared(Quaternion a) {
		return a.w * a.w + (a.x * a.x) + (a.y * a.y) + (a.z * a.z);
	}

	@Override
	public String toString() {
		// TODO Auto-generated method stub
		return "<" + w + "," + x + "," + y + "," + z + ">";
	}
}
