/**
 * @author Joe Houlton
 * 
 */
public class Rayhook extends PluginListener {
	public static int RAY_TOOL_ID = 270;

	@Override
	public void onArmSwing(Player player) {
		// TODO Auto-generated method stub
		if (player.getItemInHand() == RAY_TOOL_ID) {
			// player has ray tool
			player.sendMessage("You are using the ray tool!");
		}
	}
}
