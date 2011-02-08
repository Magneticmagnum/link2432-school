package com.joe;

import min3d.vos.Number3d;

public class LookTarget implements IMoveable {

	private Number3d raw;

	public LookTarget(Number3d raw) {
		this.raw = raw;
	}

	@Override
	public void move(float x, float y, float z) {
		// TODO Auto-generated method stub
		raw.setAll(x, y, z);
	}

	@Override
	public Number3d getNumber() {
		// TODO Auto-generated method stub
		return raw;
	}

}
