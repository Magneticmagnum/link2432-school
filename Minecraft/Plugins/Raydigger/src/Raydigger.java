import java.util.logging.Logger;

public class Raydigger extends Plugin {
	static final Rayhook listener = new Rayhook();
	private Logger log;
	private String name = "Ray Digger";
	private String version = "1.0";

	@Override
	public void initialize() {
		log = Logger.getLogger("Minecraft");
		log.info(name + " " + version + " initialized");
		etc.getLoader().addListener(PluginLoader.Hook.BLOCK_CREATED, listener,
				this, PluginListener.Priority.MEDIUM);
	}

	@Override
	public void enable() {
		// TODO Auto-generated method stub

	}

	@Override
	public void disable() {
		// TODO Auto-generated method stub

	}

}
