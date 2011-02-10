package com.joe;

import android.util.Log;
import min3d.vos.Number3d;

public class LinearMover {

	private Number3d start;
	private Number3d end;
	private int frames;
	private int maxFrames;
	private boolean done;
	private IMoveable target;

	public LinearMover(IMoveable target) {
		this.target = target;
		frames = 0;
		done = true;
	}

	public IMoveable getMover() {
		return target;
	}

	public void update() {
		if (!done) {
			frames++;
			float i = (float) frames / maxFrames;
			float x = interpolate(start.x, end.x, i);
			float y = interpolate(start.y, end.y, i);
			float z = interpolate(start.z, end.z, i);
			target.getNumber().setAll(x, y, z);
			if (frames > maxFrames) {
				done = true;
				target.getNumber().setAllFrom(end);
			}
		}
	}

	public void move(Number3d s, Number3d e) {
		move(s, e, 0);
	}

	public void move(Number3d s, Number3d e, int frames) {
		start = s;
		end = e;
		this.frames = 0;
		maxFrames = frames;
		done = false;
	}

	public void moveTo(Number3d e, int frames) {
		// don't know first position, use current
		move(target.getNumber(), e, frames);
	}

	public float interpolate(float p0, float p1, float i) {
		return p0 * (1 - i) + i * p1;

	}

	public boolean isDone() {
		// TODO Auto-generated method stub
		return done;
	}

	public void set(Number3d n) {
		// TODO Auto-generated method stub
		target.getNumber().setAllFrom(n);
		done = true;
	}

	/**
	 * Instantly add to target number, doing no interpolation
	 * 
	 * @param n
	 */
	public void add(Number3d n) {
		Number3d t = target.getNumber();
		t.x += n.x;
		t.y += n.y;
		t.z += n.z;
		done = true;
	}

	/**
	 * instantly add to target number, doing no interpolation
	 * 
	 * @param x
	 * @param y
	 * @param z
	 */
	public void add(float x, float y, float z) {
		Number3d t = target.getNumber();
		t.x += x;
		t.y += y;
		t.z += z;
		done = true;
	}

	@Override
	public String toString() {
		// TODO Auto-generated method stub
		return "Mover " + getMover().getNumber().toString();
	}

}
