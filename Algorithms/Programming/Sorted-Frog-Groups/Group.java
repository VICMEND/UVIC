import java.util.ArrayList;

public class Group {

	ArrayList<Frog> frogs = new ArrayList<>();
	int size = 0;
	
  public void addFrog(Frog f) {
	if (size == 0) {
		frogs.add(f);
		size++;
	} else {
		addFrogRec(0,size-1,f);
		size++;
	  }
	  
  }
  
  public void addFrogRec(int low, int high, Frog f){
	if (low > high){
		frogs.add(low,f);
	} else {
		int mid = (low+high)/2;
		if ( f.compareTo(frogs.get(mid)) <= 0){
			addFrogRec(low,mid-1,f);
		} else if (f.compareTo(frogs.get(mid)) > 0) {
			addFrogRec(mid+1,high,f);
		} 
	}
 }

  public int size() {
	  return frogs.size();
  }


  public Frog get(int i) { 
	return frogs.get(i);
  }

  public Group[] halfGroups() {
		
		Group g1 = new Group();
		Group g2 = new Group();
		
	 for (int i=0; i < size; i++){
		 if (i <= (size/2) -1){
			 g1.addFrog(frogs.get(i));
		 } else {
			 g2.addFrog(frogs.get(i));
		}
	 }
	 		Group[] fr = {g1,g2};
			return fr;
 }

  @Override
  public String toString() {
	return frogs.toString();
  }

	public static boolean FrogEquals(Group g1, Group g2) {
	 	if (g1.toString().equals(g2.toString())){
			return true;
		} else if (g1.size() % 2 != 0){
			return false;
		} else if ( g1.size() != g2.size()){
			return false;
		} else {
			Group[] group1 = g1.halfGroups();
			Group[] group2 = g2.halfGroups();
			return FrogEquals(group1[0],group2[1]) || FrogEquals(group1[1],group2[0]);
		}
	} 
}
	