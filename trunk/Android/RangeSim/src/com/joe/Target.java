package com.joe;

import min3d.core.Object3d;

public class Target {

	public static float HARDNESS_STEEL = 800;
	public static float HARDNESS_PAPER = 50;
	public float hardness;
	public String texture;
	public Object3d targetMesh;

	public Target(Object3d mesh) {
		targetMesh = mesh;
	}

	public boolean doesPenetrate(float energy) {
		return energy > hardness;
	}
}
