public class EconomyPerson {
	public Player player;
	public int money;

	public EconomyPerson() {
	}

	public String toString() {
		return "Player " + player.getName() + " - Money $" + money;
	}
}