import java.util.Collections;
import java.util.Arrays;
import java.lang.StringBuilder;
import java.util.Random;

public class Tester {


private static String getRandomString() {
        final char[] CHARS = new char[] {'a','b','c','d','e','f','g','h','i','j','k','l','m','n','o','p','q','r','s','t','u','v','w','x','y','z'};
        StringBuilder sb = new StringBuilder();
        Random rnd = new Random();
        while (sb.length() < 3) { // length of the random string.
            int index = (rnd.nextInt(CHARS.length));
            sb.append(CHARS[index]);
        }
        String str = sb.toString();
        return str;
}



  public static void main(String[] args) {
    
    int testN = 1000;

    System.out.println("Initializing frogs...");
    Frog[] frogs = new Frog[testN];
    for (int i=1; i<=testN; i++) {
      frogs[i-1] = new Frog(getRandomString());
    }
    //System.out.println(Arrays.toString(frogs));

    Group g1 = new Group();
    for(int i=0; i<testN; i++) {
      g1.addFrog(frogs[i]);
    }
	

    Collections.sort(Arrays.asList(frogs));
    System.out.println("\nGroups test 1: "+(g1.toString().equals(Arrays.toString(frogs))));
    System.out.println("Groups test 2: " + (g1.size()==testN));
	

    Group collectionsTop = new Group();
    Group collectionsBot = new Group();
    for (int i=0; i<(testN/2); i++) {
      collectionsTop.addFrog(frogs[i]);
      collectionsBot.addFrog(frogs[i+(testN/2)]);
    }

    Group[] twoGroups = g1.halfGroups();
    Group top = twoGroups[0];
    Group bot = twoGroups[1];

    System.out.println("halfGroups test 1: " + (top.toString().equals(collectionsTop.toString())));
    System.out.println("halfGroups test 2: " + (bot.toString().equals(collectionsBot.toString())));
    System.out.println("Groups size test 1: " + (top.size()==(testN/2)));
    System.out.println("Groups size test 2: " + (top.size()==bot.size()));

    String[] f1 = new String[] {"A","B","C","D"};
    String[] f2 = new String[] {"B","A","C","D"};
    g1 = new Group();
    Group g2 = new Group();
   
    for(int i=0; i<f1.length; i++) {
      g1.addFrog(new Frog(f1[i]));
      g2.addFrog(new Frog(f2[i]));
    }

    System.out.println("FrogEquals basic test 1: " + Group.FrogEquals(g1, g2));

    f1 = new String[] {"A","B","C","D","E","F","H","G"};
    f2 = new String[] {"O","N","M","L","K","J","I","H"};
    g1 = new Group();
    g2 = new Group();
   
    for(int i=0; i<f1.length; i++) {
      g1.addFrog(new Frog(f1[i]));
      g2.addFrog(new Frog(f2[i]));
    }

    System.out.println("FrogEquals basic test 2: " + Group.FrogEquals(g1, g2));

    f1 = new String[] {"A","B","C","D","E","F"};
    f2 = new String[] {"F","G","H","I","J","K"};
    g1 = new Group();
    g2 = new Group();
   
    for(int i=0; i<f1.length; i++) {
      g1.addFrog(new Frog(f1[i]));
      g2.addFrog(new Frog(f2[i]));
    }

    System.out.println("FrogEquals basic test 3: " + (Group.FrogEquals(g1, g2)==false));

    f1 = new String[] {"H","I","J","K","L","M","N","O"};
    f2 = new String[] {"H","I","J","K","A","B","C","D"};
    g1 = new Group();
    g2 = new Group();
   
    for(int i=0; i<f1.length; i++) {
      g1.addFrog(new Frog(f1[i]));
      g2.addFrog(new Frog(f2[i]));
    }

    System.out.println("FrogEquals basic test 4: " + (Group.FrogEquals(g1, g2)));

    f1 = new String[] {"A","B"};
    f2 = new String[] {"A"};
    g1 = new Group();
    g2 = new Group();
   
    for(int i=0; i<f1.length; i++) {
      g1.addFrog(new Frog(f1[i]));
      if (i<f1.length-1) {
        g2.addFrog(new Frog(f2[i]));
      }
    }

    System.out.println("FrogEquals basic test 5: " + (Group.FrogEquals(g1, g2)==false));
  }
    


}

