import java.io.ObjectInputStream;

public class EconomyPerson {
	private String name;
	private int money;

	public EconomyPerson(String name, int money) {
		this.name = name;
		this.money = money;
	}

	public void setMoney(int money) {
		this.money = money;
	}

	public int getMoney() {
		return money;
	}

	public String getName() {
		return name;
	}

	public String toString() {
		return "Player " + name + " - Money $" + money;
	}
}