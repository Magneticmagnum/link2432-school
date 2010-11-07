/**
 * @author Joe Houlton
 * 
 */
public class Rayhook extends PluginListener {
	public static int RAY_TOOL_ID = 276;
	public Raytool tool;

	public Rayhook() {
		super();
		tool = new Raytool();

	}

	@Override
	public void onArmSwing(Player player) {
		// TODO Auto-generated method stub
		if (player.getItemInHand() == RAY_TOOL_ID) {
			// player has ray tool
			Raydigger.log.info(player + " swung ray tool");
			tool.use(player);

		}
	}

	@Override
	public boolean onCommand(Player player, String[] split) {
		// TODO Auto-generated method stub
		if (split[0].equalsIgnoreCase("/ray")) {
			int range = 0;
			int id = 0;
			if (split[1] != null) {
				Integer i = Integer.parseInt(split[1]);
				range = i.intValue();
			}
			if (split[2] != null) {
				Integer i = Integer.parseInt(split[2]);
				id = i.intValue();
			}
			tool.activate(player, range, id);
			return true;
		}
		return false;
	}
}
