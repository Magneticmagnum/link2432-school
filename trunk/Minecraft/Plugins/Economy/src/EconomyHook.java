public class EconomyHook extends PluginListener {

	private static int ATM_ID = 54;

	@Override
	public boolean onBlockCreate(Player player, Block blockPlaced,
			Block blockClicked, int itemInHand) {
		HitBlox cast = new HitBlox(player, 2, .25);
		Block block = cast.getTargetBlock();
		if (block != null) {
			int id = block.getType();
			if (id == ATM_ID) {
				player.sendMessage("You clicked on an atm block");
				return true;
			}
		}
		return false;
	}

	@Override
	public boolean onComplexBlockChange(Player player, ComplexBlock block) {
		// TODO Auto-generated method stub
		player.sendMessage("Complex block changed");
		if (block instanceof Chest) {
			Chest chest = (Chest) block;
		}
		return true;
	}

	@Override
	public boolean onSendComplexBlock(Player player, ComplexBlock block) {
		// TODO Auto-generated method stub
		Block simple = etc.getServer().getBlockAt(block.getX(), block.getY(),
				block.getZ());
		if (simple.getType() == 54) {
			player.sendMessage("ATM!");
		}
		return true;
	}

	@Override
	public void onLogin(Player player) {
		// TODO Auto-generated method stub
		EconomyPerson person = EconomyData.playerLoggedIn(player);
		if (person != null) {
			Economy.log.info("Loaded economy profile for " + player.getName());
			player.sendMessage("Welcome, " + player.getName()
					+ "!  Your wallet contains $" + person.getMoney());
		}
	}

	@Override
	public void onDisconnect(Player player) {
		// TODO Auto-generated method stub
		EconomyPerson person = EconomyData.getEconomyProfile(player);
		EconomyData.saveProfile(person);
	}
}
