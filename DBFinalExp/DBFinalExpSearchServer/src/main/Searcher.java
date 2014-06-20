package main;

import java.io.BufferedReader;
import java.io.FileNotFoundException;
import java.io.FileReader;
import java.io.IOException;
import java.util.HashMap;
import java.util.HashSet;
import java.util.Vector;

import org.json.JSONArray;
import org.json.JSONObject;

/**
 * Main Searcher of the project.
 * @author lizhen
 *
 */
public class Searcher {

	private static boolean 							isLoad;
	private static Vector<Entry> 					entryVector = new Vector<Entry>();
	private static HashMap<String, Vector<Integer>>	invertMap = new HashMap<String, Vector<Integer>>();
	
	/**
	 * Load the data from the disk, and do pre-process.
	 * @param fileName
	 * @return
	 */
	public static boolean loadData(String fileName) {
		isLoad = false;
		try {
			System.out.println("fileName:" + fileName);
			BufferedReader fReader = new BufferedReader(new FileReader(fileName));
			String currLine;
			JSONObject currJSON;
			int currID = 0;
			while (true) 
			{
				currLine = fReader.readLine();
				//System.out.println(currLine);
				if (currLine == null) 
				{
					break;
				}
				
				/** Decode the JSON String */
				currJSON = new JSONObject(currLine);
				Entry entry = new Entry();
				JSONArray latlng = currJSON.getJSONArray("latlng");
				String keyString = currJSON.getString("addr") + " " + currJSON.getString("url");
				String [] keyStringArray = keyString.split(" ");
				
				/** Configure the Entry */
				entry.setLat(latlng.getDouble(0));
				entry.setLng(latlng.getDouble(1));
				entry.setKeys(keyStringArray);
				entryVector.add(entry);
				
				for (int i = 0; i < keyStringArray.length; ++i)
				{
					if (invertMap.containsKey(keyStringArray[i]))
					{
						invertMap.get(keyStringArray[i]).add(currID);
					}
					else
					{
						Vector<Integer> vec = new Vector<Integer>();
						vec.add(currID);
						invertMap.put(keyStringArray[i], vec);
					}
				}
				
				++currID;
			}
			isLoad = true;
		} catch (FileNotFoundException e) {
			e.printStackTrace();
		} catch (IOException e) {
			e.printStackTrace();
		}

		return isLoad;
	}
	
	/**
	 * A simple testing function.
	 * @param st
	 * @param ed
	 */
	public static void printData(int st, int ed) {
		st = Math.max(0, st);
		ed = Math.min(entryVector.size(), ed);
		if (st < ed) 
		{
			System.out.println("Print from " + st + " to " + ed);
			for (int i = st; i < ed; ++i) 
			{
				System.out.println(entryVector.get(i).getLat() + " , " + entryVector.get(i).getLng());
				System.out.println(entryVector.get(i).getKeys().toString());
			}
		}
		
		System.out.println(invertMap.get("Blangah").toString());
	}
}

class Entry {
	public double			lat, lng;
	public HashSet<String>	keys;

	public Entry() {
		keys = new HashSet<String>();
	}

	public double getLat() {
		return lat;
	}

	public void setLat(double lat) {
		this.lat = lat;
	}

	public double getLng() {
		return lng;
	}

	public void setLng(double lng) {
		this.lng = lng;
	}

	public HashSet<String> getKeys() {
		return keys;
	}

	public void setKeys(String [] keyStringArray) {
		
		for (int i = 0; i < keyStringArray.length; ++i) {
			this.keys.add(keyStringArray[i]);
		}
	}

}