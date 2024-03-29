import java.io.File;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.ObjectInputStream;
import java.io.ObjectOutputStream;
import java.util.HashMap;
import java.util.Map;

public class EconomyData {

	public static final int STARTING_FUNDS = 25;

	public static Map<Player, EconomyPerson> economyProfiles;
	static {
		economyProfiles = new HashMap<Player, EconomyPerson>();
	}

	public static File getAccountPath(String name) {
		return new File("plugins/accounts/" + name + ".act");
	}

	public static void saveProfile(EconomyPerson person) {
		File file = getAccountPath(person.getName());
		FileOutputStream fos;
		try {
			new File("plugins/accounts/").mkdirs();
			file.createNewFile();
			fos = new FileOutputStream(file);
			ObjectOutputStream objects = new ObjectOutputStream(fos);
			objects.writeObject(person);
			objects.close();
		} catch (Exception e) {
			Economy.log.info("Unable to open file for writing "
					+ file.getAbsolutePath());
			e.printStackTrace();
		}

	}

	public static EconomyPerson loadProfile(Player player) {
		File personFile = new File("./accounts/" + player.getName() + ".act");
		EconomyPerson person = null;
		if (personFile.exists()) {
			try {
				FileInputStream fis;
				fis = new FileInputStream(personFile);
				ObjectInputStream objects = new ObjectInputStream(fis);
				person = (EconomyPerson) objects.readObject();
			} catch (Exception e) {
				Economy.log.info("Unable to open file for reading "
						+ personFile.getAbsolutePath());
			}

		} else {
			person = new EconomyPerson(player.getName(), STARTING_FUNDS);
		}
		return person;

	}

	public static EconomyPerson getEconomyProfile(Player player) {
		return economyProfiles.get(player);
	}

	public static EconomyPerson playerLoggedIn(Player player) {
		EconomyPerson person = loadProfile(player);
		economyProfiles.put(player, person);
		return person;
	}
}
