import java.util.HashMap;
import java.util.Map;

public class Raytool {

	public static Server server = etc.getServer();
	public static int ACTIVE = 1;
	public static int INACTIVE = 0;
	public Map<Player, RaytoolProperties> activatedTools;

	private class RaytoolProperties {
		public int range = 5;
		public int blockid = 0;
		public int status;
	}

	public Raytool() {
		// TODO Auto-generated constructor stub
		activatedTools = new HashMap<Player, RaytoolProperties>();
	}

	public void activate(Player player, int range, int id) {
		RaytoolProperties props = activatedTools.get(player);
		if (props != null) {
			if (range == 0) {
				props.status = INACTIVE;
			} else if (range > 0) {
				props.status = ACTIVE;
				props.range = range;
			}
			props.blockid = id;
		} else {
			props = new RaytoolProperties();
			if (range > 0) {
				props.range = range;
				props.status = ACTIVE;
			} else {
				props.range = 0;
				props.status = INACTIVE;
			}
			props.blockid = id;
			activatedTools.put(player, props);
		}
		notifyPlayer(player, props);
	}

	public void notifyPlayer(Player player, RaytoolProperties props) {
		String status = "Active";
		if (props.status == INACTIVE) {
			status = "Inactive";
		}
		player.sendMessage("Ray tool: " + status);
	}

	public void use(Player player) {
		RaytoolProperties props = activatedTools.get(player);
		if (props != null) {
			// Raydigger.log.info("Properties for " + player + " " +
			// props.status
			// + " " + props.range);
			if (props.status == ACTIVE) {
				if (props.range > 0) {
					Raydigger.log.info(player.getName()
							+ " used a ray with length " + props.range);
					HitBlox caster = new HitBlox(player);
					int i = 0;
					while (i < props.range) {
						caster.setCurBlock(props.blockid);
						caster.getNextBlock();
						i++;
					}
				}
			}
		}
	}
}
