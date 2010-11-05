import java.sql.Connection;
import java.sql.DriverManager;
import java.sql.ResultSet;
import java.sql.SQLException;
import java.sql.Statement;
import java.util.Map;

public class EconomyData {

	private static final int STARTING_FUNDS = 25;
	public static Map<Block, ItemVendor> vendors;
	public static Map<Player, EconomyPerson> economyProfiles;
	public static Connection sqlConnect;
	public static String database = "jdbc:mysql://localhost:3306/economy";
	public static String user = "root";
	public static String password = "root";
	public static int CLOSED = 1;
	public static int OPEN = 2;

	// This represents a single item vendor
	// It has a location, an owner, and an inventory
	public class ItemVendor {
		Block location;
		Player owner;
		int state = CLOSED;
		ItemListing[] inventory;
	}

	public class ItemListing {
		int item;
		int price;
	}

	public static void saveProfile(EconomyPerson person) {
		Statement statement;
		try {
			statement = sqlConnect
					.createStatement(ResultSet.TYPE_SCROLL_SENSITIVE,
							ResultSet.CONCUR_UPDATABLE);
			ResultSet rs = statement
					.executeQuery("select * from accounts where account = '"
							+ person.player.getName() + "'");
			if (!rs.next()) {
				statement.executeUpdate("insert into accounts values('"
						+ person.player.getName() + "', " + person.money + ")");
				Economy.log.info("Added " + person.player.getName()
						+ " to accounts");
			} else {
				rs.updateInt("funds", person.money);
				rs.updateRow();
			}
		} catch (SQLException e) {
			// TODO Auto-generated catch block
			Economy.log.info("Database exception, unable to save person");
		}
	}

	public static EconomyPerson loadProfile(Player player) {
		try {
			Statement statement;
			statement = sqlConnect.createStatement();
			ResultSet rs = statement
					.executeQuery("select * from accounts where name = "
							+ player.getName());
			EconomyPerson person;
			if (!rs.next()) {
				// create it
				person = new EconomyPerson();
				person.money = STARTING_FUNDS;
				person.player = player;
			} else {
				person = new EconomyPerson();
				person.money = rs.getInt("funds");
				person.player = player;
			}
			return person;
		} catch (SQLException e) {
			// TODO Auto-generated catch block
			Economy.log.info("Database exception, unable to load person");
			return null;
		}

	}

	public static void openDb() {
		sqlConnect = null;
		try {
			sqlConnect = DriverManager.getConnection(database
					+ "?autoReconnect=true&user=" + user + "&password="
					+ password);
			Statement statement = sqlConnect.createStatement();
			ResultSet set = statement.executeQuery("SHOW TABLES");
			if (!set.next()) {
				// create the table
				Statement create = sqlConnect.createStatement();
				create.executeUpdate("create table if not exists economy.ACCOUNTS "
						+ "(ACCOUNT varchar(32) NOT NULL, "
						+ "FUNDS integer NOT NULL, " + "PRIMARY KEY (ACCOUNT))");
				// table created created
				Economy.log.info("Created table for accounts");
			}
		} catch (Exception e) {
			// TODO Auto-generated catch block
			Economy.log.info("Database exception");
		}
	}

	public static void closeDb() {
		try {
			sqlConnect.close();
		} catch (SQLException e) {
			// TODO Auto-generated catch block
			Economy.log.info("Unable to close database");
		}
	}

	public static EconomyPerson getEconomyProfile(Player player) {
		// TODO Auto-generated method stub
		return economyProfiles.get(player);
	}

	public static void addLoggedInProfile(EconomyPerson profile) {
		economyProfiles.put(profile.player, profile);
	}
}
