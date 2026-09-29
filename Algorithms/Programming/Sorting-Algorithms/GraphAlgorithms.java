import java.awt.Color;
import java.util.*;

public class GraphAlgorithms{

  /* 
   * To draw a list of integers int_list (of type List<Integer)
   * to the canvas, call drawSequence(int_list, writer).
   *
   * The index of each integer in the list will be
   * plotted along the x-axis; the integer value itself
   * is plotted on the y-axis.
   *                                                      */

  public static List<Integer> MergeSort(List<Integer> S, PixelWriter writer) {
    if (S.size() <2){
      return S;
    }
    
    List<Integer> S1 = new ArrayList<>();
    List<Integer> S2 = new ArrayList<>();
    
    divide(S1,S2,S);
    S1 = MergeSort(S1,writer);
    S2 = MergeSort(S2,writer);
    merge(S1,S2,S);

    drawSequence(S, writer);
    return S;
  }


private static void divide(List<Integer> S1, List<Integer> S2, List<Integer> S) {
    S1.addAll(S.subList(0, (S.size()/2)));
    S2.addAll(S.subList((S.size()/2), S.size()));
}

  private static void merge(List<Integer> S1, List<Integer> S2, List<Integer> S) {
   int i = 0;
   int j = 0;
    
    while (i < S1.size() && j < S2.size()){
      if (S1.get(i) <= S2.get(j)){
        S.set(i+j, S1.get(i));
        i++;
      } else {
        S.set(i+j, S2.get(j));
        j++;
      }
    }
      while (i < S1.size()){
        S.set(i+j, S1.get(i));
        i++;
      }
      while (j < S2.size()){
        S.set(i+j, S2.get(j));
        j++;
      }
    }
  

  public static List<Integer> QuickSort(List<Integer> S, PixelWriter writer) {
    if (S.size() <2){
      return S;
    }
    List<Integer> L = new ArrayList<>();
    List<Integer> E = new ArrayList<>();
    List<Integer> G = new ArrayList<>();
    int x = pickPivot(S);
    split(L, E, G, S, x);
    L = QuickSort(L,writer);
    G = QuickSort(G,writer);
    concatenate(L,E,G,S);

    drawSequence(S, writer);
    return S;
  }
  
  private static int pickPivot(List<Integer> S) {
  return S.size()/2;
  }

  private static void split(List<Integer> L, List<Integer> E, List<Integer> G, List<Integer> S, int x){
  
    for(int i= 0; i <S.size();i++){
      if (S.get(i) < S.get(x)){
        L.add(S.get(i));
     } else  if (S.get(i) > S.get(x)){
        G.add(S.get(i));
     } else if (S.get(i) == S.get(x)){
         E.add(S.get(i));
     }
    }
  }

  private static void concatenate(List<Integer> L, List<Integer> E, List<Integer> G, List<Integer> S){
    S.clear();
    S.addAll(L);
    S.addAll(E);
    S.addAll(G);
  }

  public static List<Integer> InsertionSort(List<Integer> S, PixelWriter writer) {
    for(int i=1; i < S.size(); i++){
      int val = S.get(i);
        int k = i-1;
        while ((k >= 0) && (S.get(k)>val)){
           S.set(k+1, S.get(k));
           k = k-1;
        }
           S.set(k+1, val);
		   drawSequence(S, writer);
      }
      return S;
  }

  public static List<Integer> RadixSort(List<Integer> S, PixelWriter writer) {
	int y = 0;
	int N = (int) Math.log10(Collections.max(S)) + 1;
	List<List<Integer>> bucket = new ArrayList<>();
    
	for (int i = 0; i < 10;i++){
		bucket.add(new ArrayList<Integer>());
    }
    for(int i=0; i < N;i++){
     for(int p=0; p < S.size();p++){
        int digit = (int) (S.get(p)/Math.pow(10,i) % 10);
        bucket.get(digit).add(S.get(p));
      }
		y=0;
		for(int k=0; k < 10;k++){
			while(!bucket.get(k).isEmpty()){
			S.set(y,bucket.get(k).remove(0));
			y++;
      }
    }
	    drawSequence(S, writer);
 }
    return S;
  }




  /* DO NOT CHANGE THIS METHOD */
  private static void drawSequence(List<Integer> sequence, PixelWriter writer) {
    for (Integer curr : sequence) {
      for (int j=0; j<sequence.size(); j++) {
        Color c = writer.getColor(j, curr);
        if (c.equals(Color.BLACK))
          writer.setPixel(j, curr, Color.WHITE);
      }
      int x = sequence.indexOf(curr);
      if (!writer.getColor(x, curr).equals(Color.BLACK)) {
        writer.setPixel(sequence.indexOf(curr), curr, Color.BLACK);
      }
    }
  } 


  /* THE FOLLOWING METHODS WILL NOT BE MARKED;
   * YOU MAY IMPLEMENT THEM OPTIONALLY
   */

	/* FloodFillDFS(v, writer, fillColour)
	   Traverse the component the vertex v using DFS and set the colour 
	   of the pixels corresponding to all vertices encountered during the 
	   traversal to fillColour.
	   
	   To change the colour of a pixel at position (x,y) in the image to a 
	   colour c, use
			writer.setPixel(x,y,c);
	*/
	public static void FloodFillDFS(PixelVertex v, PixelWriter writer, Color fillColour){
	}
	
	/* FloodFillBFS(v, writer, fillColour)
	   Traverse the component the vertex v using BFS and set the colour 
	   of the pixels corresponding to all vertices encountered during the 
	   traversal to fillColour.
	   
	   To change the colour of a pixel at position (x,y) in the image to a 
	   colour c, use
			writer.setPixel(x,y,c);
	*/
	public static void FloodFillBFS(PixelVertex v, PixelWriter writer, Color fillColour){
	}
	
}
