import java.io.Serializable;

public class EconomyPerson implements Serializable {
	private static final long serialVersionUID = 1565470329567136861L;
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