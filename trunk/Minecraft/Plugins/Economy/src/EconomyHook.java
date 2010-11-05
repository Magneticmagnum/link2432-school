public class EconomyHook extends PluginListener {

	private static int ATM_ID = 54;

	@Override
	public boolean onBlockCreate(Player player, Block blockPlaced,
			Block blockClicked, int itemInHand) {
		HitBlox cast = new HitBlox(player, 2, .25);
		Block block = cast.getTargetBlock();
		if (block != null) {
			int id = block.getType();
			player.sendMessage("ATM! " + id);
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
		return true;
	}

	@Override
	public boolean onSendComplexBlock(Player player, ComplexBlock block) {
		// TODO Auto-generated method stub
		player.sendMessage("Send complex block!");
		Block simple = etc.getServer().getBlockAt(block.getX(), block.getY(),
				block.getZ());
		if (simple.getType() == 54) {
			player.sendMessage("ATM!");
		}
		return true;
	}
}
