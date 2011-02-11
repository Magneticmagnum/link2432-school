package com.joe.rangesim.models;

public class TargetModel {

	public static float HARDNESS_STEEL = 800;
	public static float HARDNESS_PAPER = 50;
	public final String name;
	public final String description;
	public final float hardness;

	public TargetModel(String name, String desc, float hardness) {
		this.name = name;
		this.description = desc;
		this.hardness = hardness;
	}

	public boolean doesPenetrate(float energy) {
		return energy > hardness;
	}
}
