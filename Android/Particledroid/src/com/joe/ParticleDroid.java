package com.joe;

import java.nio.ByteBuffer;
import java.nio.ByteOrder;
import java.nio.FloatBuffer;
import java.nio.IntBuffer;
import java.util.Random;

import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;

import android.app.Activity;
import android.content.Context;
import android.opengl.GLSurfaceView;
import android.opengl.GLU;
import android.os.Bundle;
import android.util.Log;

public class ParticleDroid extends Activity {
	private GLView view;

	/** Called when the activity is first created. */
	@Override
	public void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		view = new GLView(this);
		setContentView(view);
	}

	@Override
	protected void onPause() {
		// TODO Auto-generated method stub
		super.onPause();
		view.onPause();
	}
}

class GLView extends GLSurfaceView {
	private final GLRenderer renderer;

	public GLView(Context context) {
		// TODO Auto-generated constructor stub
		super(context);
		renderer = new GLRenderer(context);
		setRenderer(renderer);
	}
}

class GLRenderer implements GLSurfaceView.Renderer {
	private static final String TAG = "GLRenderer";
	private final Context context;
	private final Emitter emitter = new Emitter(Vector3.ZERO, 75);
	// private final GLCube cube = new GLCube();

	private long startTime;
	private long fpsStartTime;
	private long numFrames;

	public GLRenderer(Context context) {
		// TODO Auto-generated constructor stub
		this.context = context;
	}

	@Override
	public void onSurfaceChanged(GL10 gl, int width, int height) {
		// TODO Auto-generated method stub
		gl.glViewport(0, 0, width, height);
		gl.glMatrixMode(GL10.GL_PROJECTION);
		gl.glLoadIdentity();
		float ratio = (float) width / height;
		GLU.gluPerspective(gl, 45.0f, ratio, 1, 100f);

	}

	@Override
	public void onSurfaceCreated(GL10 gl, EGLConfig config) {
		// TODO Auto-generated method stub
		gl.glEnable(GL10.GL_DEPTH_TEST);
		gl.glDepthFunc(GL10.GL_LEQUAL);
		gl.glEnableClientState(GL10.GL_VERTEX_ARRAY);
		float lightambient[] = new float[] { .2f, .2f, .2f, 1 };
		float lightdiffuse[] = new float[] { 1, 1, 1, 1 };
		float lightpos[] = new float[] { 1, 1, 1, 1 };
		float matAmbient[] = new float[] { 1, 1, 1, 1 };
		float matDiffuse[] = new float[] { 1, 1, 1, 1 };

		FloatBuffer fblAmb = FloatBuffer.wrap(lightambient);
		FloatBuffer fblDif = FloatBuffer.wrap(lightdiffuse);
		FloatBuffer fblPos = FloatBuffer.wrap(lightpos);
		FloatBuffer fbmAmb = FloatBuffer.wrap(matAmbient);
		FloatBuffer fbmDif = FloatBuffer.wrap(matDiffuse);

		gl.glEnable(GL10.GL_LIGHTING);
		gl.glEnable(GL10.GL_LIGHT0);
		gl.glLightfv(GL10.GL_LIGHT0, GL10.GL_AMBIENT, fblAmb);
		gl.glLightfv(GL10.GL_LIGHT0, GL10.GL_DIFFUSE, fblDif);
		gl.glLightfv(GL10.GL_LIGHT0, GL10.GL_POSITION, fblPos);
		gl.glMaterialfv(GL10.GL_FRONT_AND_BACK, GL10.GL_DIFFUSE, fbmDif);
		gl.glMaterialfv(GL10.GL_FRONT_AND_BACK, GL10.GL_AMBIENT, fbmAmb);

		startTime = System.currentTimeMillis();
		fpsStartTime = startTime;
		numFrames = 0;

	}

	@Override
	public void onDrawFrame(GL10 gl) {
		// TODO Auto-generated method stub
		gl.glClear(GL10.GL_COLOR_BUFFER_BIT | GL10.GL_DEPTH_BUFFER_BIT);
		gl.glMatrixMode(GL10.GL_MODELVIEW);
		gl.glLoadIdentity();

		// rot and angle
		float dt = 1f / 30f;
		float omg = (System.currentTimeMillis() - startTime) / 1000.0f;
		gl.glRotatef(omg * 5, 0, 1, 0);
		gl.glTranslatef(0, 0, 0f);
		emitter.tick(dt);
		emitter.draw(gl);

	}
}

class Vector3 {
	public float x;
	public float y;
	public float z;
	public static final Vector3 ZERO = new Vector3(0, 0, 0);

	public Vector3(float a, float b, float c) {
		this.x = a;
		this.y = b;
		this.z = c;
	}

	public static void cp(Vector3 v1, Vector3 v2) {
		v1.x = v2.x;
		v1.y = v2.y;
		v1.z = v2.z;
	}
}

class Particle {
	// pos
	public Vector3 pos;
	public Vector3 vel;
	public Vector3 ang;
	public Vector3 omega;
	public static GLCube cube = new GLCube();

	public Particle(Vector3 pos, Vector3 vel, Vector3 ang, Vector3 omega) {
		this.pos = new Vector3(0, 0, 0);
		this.vel = new Vector3(0, 0, 0);
		this.ang = new Vector3(0, 0, 0);
		this.omega = new Vector3(0, 0, 0);
		Vector3.cp(this.pos, pos);
		Vector3.cp(this.vel, vel);
		Vector3.cp(this.ang, ang);
		Vector3.cp(this.omega, omega);

	}

	public void draw(GL10 gl) {
		gl.glPushMatrix();
		gl.glTranslatef(pos.x, pos.y, pos.z);
		gl.glRotatef(ang.x, 1, 0, 0);
		gl.glRotatef(ang.y, 0, 1, 0);
		gl.glRotatef(ang.z, 0, 0, 1);
		cube.draw(gl);
		gl.glPopMatrix();
	}

	public void tick(float dt) {
		pos.x += (vel.x * dt);
		pos.y += (vel.y * dt);
		pos.z += (vel.z * dt);

		ang.x += omega.x * dt;
		ang.y += omega.y * dt;
		ang.z += omega.z * dt;
	}

	public void initialize(Vector3 pos, Vector3 vel, Vector3 ang, Vector3 omega) {
		Vector3.cp(this.pos, pos);
		Vector3.cp(this.vel, vel);
		Vector3.cp(this.ang, ang);
		Vector3.cp(this.omega, omega);
	}

}

class Emitter {
	private static final int MAX_PARTICLES = 440;
	private static final Particle partArray[] = new Particle[MAX_PARTICLES];
	private int burstCount;
	private int counter;
	private Vector3 pos;
	private Random random;

	public Emitter(Vector3 pos, int burstCount) {
		random = new Random();
		this.counter = burstCount;
		this.pos = new Vector3(0, 0, 0);
		Vector3.cp(this.pos, pos);
		this.burstCount = burstCount;
		for (int i = 0; i < MAX_PARTICLES; i++) {
			partArray[i] = new Particle(pos, Vector3.ZERO, Vector3.ZERO,
					Vector3.ZERO);
		}
		burst();

	}

	public void burst() {
		float vel_amp = 45;
		float omega_amp = 25;
		for (int i = 0; i < MAX_PARTICLES; i++) {
			Particle p = partArray[i];
			float x = random.nextFloat() - 0.5f;
			float y = random.nextFloat() - 0.5f;
			float z = random.nextFloat() - 0.5f;
			float ax = random.nextFloat() - 0.5f;
			float ay = random.nextFloat() - 0.5f;
			float az = random.nextFloat() - 0.5f;
			ax *= omega_amp;
			ay *= omega_amp;
			az *= omega_amp;
			x *= vel_amp;
			y *= vel_amp;
			z *= vel_amp;

			p.initialize(pos, new Vector3(x, y, z), Vector3.ZERO, new Vector3(
					ax, ay, az));
		}
	}

	public void tick(float dt) {
		for (int i = 0; i < MAX_PARTICLES; i++) {
			Particle p = partArray[i];
			p.tick(dt);
			Vector3 pvel = p.vel;
			pvel.x *= 0.90;
			pvel.y *= 0.90;
			pvel.z *= 0.90;
		}
	}

	public void draw(GL10 gl) {
		for (int i = 0; i < MAX_PARTICLES; i++) {
			Particle p = partArray[i];
			p.draw(gl);
		}
	}

}

class GLCube {
	private final IntBuffer mVertexBuffer;

	public GLCube() {
		// TODO Auto-generated constructor stub
		int one = 65536;
		int half = one / 2;
		int vertices[] = {
				// FRONT
				-half, -half, half, half, -half, half, -half,
				half,
				half,
				half,
				half,
				half,
				// BACK
				-half, -half, -half, -half, half, -half, half, -half,
				-half,
				half,
				half,
				-half,
				// LEFT
				-half, -half, half, -half, half, half, -half, -half, -half,
				-half,
				half,
				-half,
				// RIGHT
				half, -half, -half, half, half, -half, half, -half, half, half,
				half,
				half,
				// TOP
				-half, half, half, half, half, half, -half, half, -half, half,
				half, -half,
				// BOTTOM
				-half, -half, half, -half, -half, -half, half, -half, half,
				half, -half, -half, };

		ByteBuffer vbb = ByteBuffer.allocateDirect(vertices.length * 4);
		vbb.order(ByteOrder.nativeOrder());
		mVertexBuffer = vbb.asIntBuffer();
		mVertexBuffer.put(vertices);
		mVertexBuffer.position(0);
	}

	public void draw(GL10 gl) {
		gl.glVertexPointer(3, GL10.GL_FIXED, 0, mVertexBuffer);
		gl.glColor4f(1f, 0f, 0f, 1f);
		
		gl.glNormal3f(0, 0, 1);
		gl.glDrawArrays(GL10.GL_TRIANGLE_STRIP, 0, 4);
		gl.glNormal3f(0, 0, -1);
		gl.glDrawArrays(GL10.GL_TRIANGLE_STRIP, 4, 4);

		gl.glNormal3f(-1, 0, 0);
		gl.glDrawArrays(GL10.GL_TRIANGLE_STRIP, 8, 4);
		gl.glNormal3f(1, 0, 0);
		gl.glDrawArrays(GL10.GL_TRIANGLE_STRIP, 12, 4);

		gl.glNormal3f(0, 1, 0);
		gl.glDrawArrays(GL10.GL_TRIANGLE_STRIP, 16, 4);
		gl.glNormal3f(0, -1, 0);
		gl.glDrawArrays(GL10.GL_TRIANGLE_STRIP, 20, 4);

	}
}