package com.joe;

import android.util.FloatMath;
import min3d.vos.Number3d;

public class Utils {
	public static AxisRotation angleBetween(Number3d v1, Number3d v2) {
		float angle;
		// turn vectors into unit vectors
		v1.normalize();
		v2.normalize();
		angle = (float) Math.acos(Number3d.dot(v1, v2));
		// if no noticeable rotation is available return zero rotation
		// this way we avoid Cross product artifacts
		if (Math.abs(angle) < 0.0001)
			return new AxisRotation(0, 0, 1, 0);
		// in this case there are 2 lines on the same axis
		if (Math.abs(angle) - Math.PI < 0.001) {
			v1.rotateX(28.6478898f);
			// there are an infinite number of normals
			// in this case. Anyone of these normals will be
			// a valid rotation (180 degrees). so I rotate the curr axis by 0.5
			// radians this way we get one of these normals
		}
		Number3d axis = v1;
		axis = Number3d.cross(v1, v2);
		return new AxisRotation(axis.x, axis.y, axis.z, angle);
	}

	public static Number3d rot2Euler(Quaternion q1) {
		Number3d ang = new Number3d(0, 0, 0);
		double sqw = q1.w * q1.w;
		double sqx = q1.x * q1.x;
		double sqy = q1.y * q1.y;
		double sqz = q1.z * q1.z;
		double unit = sqx + sqy + sqz + sqw; // if normalised is one, otherwise
		// is correction factor
		double test = q1.x * q1.y + q1.z * q1.w;
		float heading;
		float attitude;
		float bank;
		if (test > 0.499 * unit) { // singularity at north pole
			heading = (float) (2 * Math.atan2(q1.x, q1.w));
			attitude = (float) (Math.PI / 2);
			bank = 0;
			ang.setAll(bank, heading, attitude);
			return ang;
		}
		if (test < -0.499 * unit) { // singularity at south pole
			heading = (float) (-2 * Math.atan2(q1.x, q1.w));
			attitude = (float) (-Math.PI / 2);
			bank = 0;
			ang.setAll(bank, heading, attitude);
			return ang;
		}
		heading = (float) Math.atan2(2 * q1.y * q1.w - 2 * q1.x * q1.z, sqx
				- sqy - sqz + sqw);
		attitude = (float) Math.asin(2 * test / unit);
		bank = (float) Math.atan2(2 * q1.x * q1.w - 2 * q1.y * q1.z, -sqx + sqy
				- sqz + sqw);
		ang.y = heading;
		ang.x = bank;
		ang.z = attitude;
		return ang;
	}

	/**
	 * note v1 and v2 dont have to be nomalised, thanks to minorlogic for
	 * telling me about this:
	 * http://www.euclideanspace.com/maths/algebra/vectors
	 * /angleBetween/minorlogic.htm
	 */
	public static Quaternion quaternionBetween(Number3d v1, Number3d v2) {
		float d = Number3d.dot(v1, v2);
		Number3d axis = Number3d.cross(v1, v2);
		float qw = (float) FloatMath.sqrt(v1.length_sq() * v2.length_sq()) + d;
		if (qw < 0.0001) { // vectors are 180 degrees apart
			return (new Quaternion(0, -v1.z, v1.y, v1.x)).normalize();
		}
		Quaternion q = new Quaternion(qw, axis.x, axis.y, axis.z);
		return q.normalize();
	}

	// assumes axis is already normalised
	public static Quaternion axis2Rot(Vector3 fwd, Vector3 left, Vector3 up) {
		float s;
		float tr = (float) (fwd.x + left.y + up.z + 1.0);
		if (tr >= 1.0) {
			s = (float) (0.5 / FloatMath.sqrt(tr));
			return new Quaternion((float) 0.25 / s, (left.z - up.y) * s,
					(up.x - fwd.z) * s, (fwd.y - left.x) * s);
		} else {
			float max = (left.y > up.z) ? left.y : up.z;
			if (max < fwd.x) {
				s = (float) (FloatMath
						.sqrt((float) (fwd.x - (left.y + up.z) + 1.0)));
				float x = (float) (s * 0.5);
				s = (float) (0.5 / s);
				return new Quaternion((left.z - up.y) * s, x, (fwd.y + left.x)
						* s, (up.x + fwd.z) * s);
			} else if (max == left.y) {
				s = (float) (FloatMath
						.sqrt((float) (left.y - (up.z + fwd.x) + 1.0)));
				float y = (float) (s * 0.5);
				s = (float) (0.5 / s);
				return new Quaternion((up.x - fwd.z) * s, (fwd.y + left.x) * s,
						y, (left.z + up.y) * s);
			} else {
				s = (float) (FloatMath
						.sqrt((float) (up.z - (fwd.x + left.y) + 1.0)));
				float z = (float) (s * 0.5);
				s = (float) (0.5 / s);
				return new Quaternion((fwd.y - left.x) * s, (up.x + fwd.z) * s,
						(left.z + up.y) * s, z);
			}
		}
	}

	public static Quaternion getConstrainedRot(Quaternion bet) {
		Vector3 back = new Vector3(0, 0, -1);
		back.rotate(bet);
		Vector3 right = new Vector3(-1, 0, 0);
		Vector3.cp(right, back);
		Vector3 up = new Vector3(back);
		Vector3.cp(up, right);
		Quaternion finalrot = com.joe.Utils.axis2Rot(back, right, up);
		return finalrot;
	}

	// p0 start
	// p1 end
	// m0 start tangent
	// m1 end tangent
	// i is your time function
	public static float splineTransition(float p0, float m0, float p1,
			float m1, float i) {
		// Cubic hermite spline
		float t2 = i * i;
		float t3 = t2 * i;
		float p = (2 * t3 - 3 * t2 + 1) * p0 + (t3 - 2 * t2 + i) * m0
				+ (-2 * t3 + 3 * t2) * p1 + (t3 - t2) * m1;
		return p;
	}
}