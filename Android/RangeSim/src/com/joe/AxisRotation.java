package com.joe;

public class AxisRotation {

	public float x;
	public float y;
	public float z;
	public float a;

	public AxisRotation(float x, float y, float z, float angle) {
		// TODO Auto-generated constructor stub
		this.x = x;
		this.y = y;
		this.z = z;
		this.a = angle;
	}

	/*
	 * normalize in case of quaternion
	 */
	public AxisRotation norm() {
		float m = (float) Math.sqrt(this.x * this.x + this.y * this.y + this.z
				* this.z + this.a * this.a);
		x /= m;
		y /= m;
		z /= m;
		a /= m;
		return this;
	}
}
