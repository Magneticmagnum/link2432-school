package com.joe;

import android.util.FloatMath;

public class Vector3 {
	public float x;
	public float y;
	public float z;
	public static final Vector3 ZERO = new Vector3(0, 0, 0);

	public Vector3(float a, float b, float c) {
		this.x = a;
		this.y = b;
		this.z = c;
	}

	public Vector3(Vector3 a) {
		cp(this, a);
	}

	public Vector3() {
		this(0, 0, 0);
	}

	public void reinit(float a, float b, float c) {
		reinit(this, a, b, c);
	}

	public static void reinit(Vector3 v, float a, float b, float c) {
		v.x = a;
		v.y = b;
		v.z = c;
	}

	public void scale(float b) {
		scale(this, b);
	}

	public static void scale(Vector3 a, float b) {
		a.x *= b;
		a.y *= b;
		a.z *= b;
	}

	public void add(Vector3 o) {
		add(this, o);
	}

	public static void add(Vector3 a, Vector3 b) {
		a.x += b.x;
		a.y += b.y;
		a.z += b.z;
	}

	public void normalize() {
		normalize(this);
	}

	public float magnitude() {
		return (float) FloatMath.sqrt(this.x * this.x + this.y * this.y
				+ this.z * this.z);
	}

	public static void normalize(Vector3 a) {
		float mag = a.magnitude();
		if (mag != 0) {
			mag = 1.0f / mag;
			a.x *= mag;
			a.y *= mag;
			a.z *= mag;
		}
	}

	public static void cp(Vector3 v1, Vector3 v2) {
		v1.x = v2.x;
		v1.y = v2.y;
		v1.z = v2.z;
	}

	public void rotate(Quaternion q) {
		Vector3.rotate(this, q);
	}

	public static void rotate(Vector3 a, Quaternion q) {
		float mag = a.magnitude();
		a.normalize();
		Quaternion vecQuat = new Quaternion(0f, a.x, a.y, a.z);
		Quaternion q1 = new Quaternion(q);
		q.conjugate();
		Quaternion resQuat = Quaternion.mult(vecQuat, q);
		resQuat = Quaternion.mult(q1, resQuat);
		a.x = resQuat.x;
		a.y = resQuat.y;
		a.z = resQuat.z;
		a.scale(mag);
	}

	@Override
	public String toString() {
		// TODO Auto-generated method stub
		return "<" + x + "," + y + "," + z + ">";
	}
}
