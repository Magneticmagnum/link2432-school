import java.util.logging.Logger;

public class Economy extends Plugin {
	static final EconomyHook listener = new EconomyHook();
	private Logger log;
	private String name = "Economy";
	private String version = "1.0";

	@Override
	public void initialize() {
		log = Logger.getLogger("Minecraft");
		log.info(name + " " + version + " initialized");
		etc.getLoader().addListener(PluginLoader.Hook.BLOCK_CREATED, listener,
				this, PluginListener.Priority.MEDIUM);
		etc.getLoader().addListener(PluginLoader.Hook.ARM_SWING, listener,
				this, PluginListener.Priority.MEDIUM);
		etc.getLoader().addListener(PluginLoader.Hook.COMPLEX_BLOCK_CHANGE,
				listener, this, PluginListener.Priority.MEDIUM);
		etc.getLoader().addListener(PluginLoader.Hook.COMPLEX_BLOCK_SEND,
				listener, this, PluginListener.Priority.MEDIUM);
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
