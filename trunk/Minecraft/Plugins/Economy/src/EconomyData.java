import java.sql.Connection;
import java.sql.DriverManager;
import java.sql.ResultSet;
import java.sql.SQLException;
import java.sql.Statement;
import java.util.Map;

public class EconomyData {

	public static Map<Block, ItemVendor> vendors;
	public static Connection sqlConnect;
	public static int PORT = 3306;
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

	public class EconomyPerson {
		Player player;
		int money;
	}

	public static void savePerson(EconomyPerson person) {
	}

	public static void openDb() {
		sqlConnect = null;
		try {
			sqlConnect = DriverManager.getConnection(
					"jdbc:mysql://localhost:3306/economy", "root", "root");
			Statement statement = sqlConnect.createStatement();
			ResultSet set = statement.executeQuery("SHOW TABLES");
			if (!set.next()) {
				// create the table
				Statement create = sqlConnect.createStatement();
				create.executeUpdate("create table economy.ACCOUNTS "
						+ "(ACCOUNT varchar(32) NOT NULL, "
						+ "FUNDS integer NOT NULL, " + "PRIMARY KEY (ACCOUNT))");
				// table created created
				Economy.log.info("Created table for accounts");
			}
		} catch (SQLException e) {
			// TODO Auto-generated catch block
			Economy.log.info("Database error");
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
}
