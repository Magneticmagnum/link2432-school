public class EconomyHook extends PluginListener {

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
		Economy.log.info("Saved profile for player " + player.getName()
				+ ".  Goodbye!");
	}
}
