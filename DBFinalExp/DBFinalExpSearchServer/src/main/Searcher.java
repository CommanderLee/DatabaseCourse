package main;

import java.io.BufferedReader;
import java.io.FileNotFoundException;
import java.io.FileReader;
import java.io.IOException;
import java.util.Comparator;
import java.util.HashMap;
import java.util.HashSet;
import java.util.PriorityQueue;
import java.util.Vector;

import org.json.JSONArray;
import org.json.JSONObject;

/**
 * Main Searcher of the project.
 * @author lizhen
 *
 */
public class Searcher {

	/** Max number of entries to be returned */
	private static final int						MAX_K = 10;
	
	private static boolean 							isLoad;
	private static Vector<Entry> 					entryVector = new Vector<Entry>();
	private static HashMap<String, Vector<Integer>>	invertMap = new HashMap<String, Vector<Integer>>();
	private static TrieTree							tree = new TrieTree();
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
				String address = currJSON.getString("addr").replaceAll("\\n", ",");
				//System.out.println(keyString);
				//new java.util.Scanner(System.in).nextLine();
				String [] keyStringArray = address.replaceAll(",", " ")
						.toLowerCase().split(" +");
				
				/** Configure the Entry */
				entry.setLat(latlng.getDouble(0));
				entry.setLng(latlng.getDouble(1));
				entry.setKeySet(keyStringArray);
				//entry.setTree(keyStringArray);
				entry.setName(currJSON.getString("name"));
				entry.setPcode(currJSON.getInt("pcode"));
				entry.setAddress(address);
				entry.setUrl(currJSON.getString("url"));
				entryVector.add(entry);
				
				int strLen;
				String str, prefix;
				for (int i = 0; i < keyStringArray.length; ++i)
				{
					str = keyStringArray[i];
					strLen = str.length();
					for (int j = 1; j <= strLen; ++j)
					{
						prefix = str.substring(0, j);
						if (invertMap.containsKey(prefix))
						{
							invertMap.get(prefix).add(currID);
						}
						else
						{
							Vector<Integer> vec = new Vector<Integer>();
							vec.add(currID);
							invertMap.put(prefix, vec);
						}
					}
					tree.addString(str, currID);
				}
				++currID;
				
				/*String str;
				for (int i = 0; i < keyStringArray.length; ++i)
				{
					str = keyStringArray[i];

					if (invertMap.containsKey(str))
					{
						invertMap.get(str).add(currID);
					}
					else
					{
						Vector<Integer> vec = new Vector<Integer>();
						vec.add(currID);
						invertMap.put(str, vec);
					}
				}*/
			}
			//System.out.println(invertMap.get("sing").size());
			isLoad = true;
		} catch (FileNotFoundException e) {
			e.printStackTrace();
		} catch (IOException e) {
			e.printStackTrace();
		}

		return isLoad;
	}
	
	/**
	 * Main function: get search results in JSON format.
	 * @param lat
	 * @param lng
	 * @param keyWords
	 * @return
	 */
	public static String doSearch(double lat, double lng, String keyWords)
	{
		/** Filter long blanks */
		String [] userKeyArray = keyWords.replaceAll(",", " ").toLowerCase().split(" +");
		//System.out.println("First word: " + userKeyArray[0]);
		
		/** Find the shortest invert list */
		int arrayLen = userKeyArray.length;
		int id  = -1, minLen = -1, tmpLen;
		for (int i = 0; i < arrayLen; ++i)
		{
			if (invertMap.containsKey(userKeyArray[i]))
			{
				tmpLen = invertMap.get(userKeyArray[i]).size();
				if (id == -1)
				{
					id = i;
					minLen = tmpLen;
				}
				else if (tmpLen < minLen)
				{
					id = i;
					minLen = tmpLen;
				}
			}
			else
			{
				id = -1;
			}
		}
		
		String resultJSON = "";
		if (id >= 0)
		{
			/** Get candidate */
			/* Compare: big-heap to get min-k-result */
			Comparator<Result> cmp = new Comparator<Result>() {
				@Override
				public int compare(Result arg0, Result arg1) {
					if (arg0.getDistance() < arg1.getDistance())
					{
						return 1;
					}
					else if(arg0.getDistance() > arg1.getDistance())
					{
						return -1;
					}
					return 0;
				}
			};
			PriorityQueue<Result> resultQueue = new PriorityQueue<Result>(MAX_K, cmp);
			//PriorityQueue<Result> resultQueue = new PriorityQueue<Result>(MAX_K);
			
			String str = userKeyArray[id];
			Vector<Integer> vec = invertMap.get(str);
			//System.out.println("str: " + str + " id: " + id + " invertMap: " + vec.toString());
			int vecLen = vec.size();
			for (int i = 0; i < vecLen; ++i)
			{
				boolean jud = true;
				int curr = vec.get(i);
				/*for (int j = 0; j < arrayLen; ++j)
				{
					if (!invertMap.get(userKeyArray[j]).contains(vec.get(i)))
					{
						jud = false;
						break;
					}
				}*/
				HashSet<String> strSet = entryVector.get(curr).getKeySet();
				//TrieTree tree = entryVector.get(vec.get(i)).getTree();
				//System.out.println("i:" + vec.get(i));
				for (int j = 0; j < arrayLen - 1; ++j)
				{
					if (!strSet.contains(userKeyArray[j]))
					//if (!tree.containString(userKeyArray[j]))
					{
						jud = false;
						break;
					}
					//System.out.println("Contains:" + userKeyArray[j]);
				}
				HashSet<Integer> idSet = tree.findString(userKeyArray[arrayLen-1]);
				if (idSet != null && !idSet.contains(curr))
				{
					jud = false;
				}
				
				/** Add to candidate; maintain a k-size-heap */
				if (jud)
				{
					Entry entry = entryVector.get(vec.get(i));
					double dist = calcDistance(lat, lng, 
							entry.getLat(), entry.getLng());
					
					if (resultQueue.size() >= MAX_K && dist < resultQueue.peek().getDistance())
					{
						resultQueue.poll();
					}
					
					if (resultQueue.size() < MAX_K)
					{
						Result result = new Result();
						result.setLat(entry.getLat());
						result.setLng(entry.getLng());
						result.setDistance(calcDistance(lat, lng, 
								entry.getLat(), entry.getLng()));
						result.setName(entry.getName());
						result.setPcode(entry.getPcode());
						result.setAddress(entry.getAddress());
						result.setUrl(entry.getUrl());
						resultQueue.add(result);
						//System.out.println("Lat:" + result.getLat() + " Lng:" + result.getLng());
						//System.out.println("Add: dist:" + result.getDistance() + 
						//		"peek:" + resultQueue.peek().getDistance());
					}
				}
			}
			
			//System.out.println("Print queue: ");
			JSONObject TotalObj = new JSONObject();
			JSONArray array = new JSONArray();
			//HashMap<String, String> map = new HashMap<String, String>();
			//while (!resultQueue.isEmpty()) 
			while (resultQueue.size() > 1)
			{
				Result result = resultQueue.poll();
				JSONObject map = new JSONObject();
				//System.out.println("Size: " + resultQueue.size());
				//map.clear();
				map.put("Lat", result.getLat());
				map.put("Lng", result.getLng());
				map.put("Name", result.getName());
				map.put("Pcode", result.getPcode());
				map.put("Addr", result.getAddress());
				array.put(map);
				//System.out.println(map.get("Lat") + ", " + map.get("Lng"));
				//System.out.println(result.getLat() + ", " + result.getLng() + ", " + result.getDistance());
			}
			if (resultQueue.size() == 1)
			{
				Result result = resultQueue.poll();
				JSONObject map = new JSONObject();
				map.put("Lat", result.getLat());
				map.put("Lng", result.getLng());
				map.put("Name", result.getName());
				map.put("Pcode", result.getPcode());
				map.put("Addr", result.getAddress());
				array.put(map);
				
				TotalObj.put("BestMap", result.getUrl());
			}
			
			//JSONArray array = new JSONArray(resultQueue);
			
			TotalObj.put("Pos", array);
			resultJSON = TotalObj.toString();
			//System.out.println(resultJSON);
		}
		//System.out.println(userKeyArray.length);
		return resultJSON;
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
				//System.out.println(entryVector.get(i).getKeys().toString());
			}
		}
		
		System.out.println(invertMap.get("Blangah").toString());
	}
	
	public static double calcDistance(double lat1, double lng1, double lat2, double lng2)
	{
		/*double EARTH_R = 6378.137, C, D;
		
		lat1 = (90 - lat1) * Math.PI / 180.0;
		lat2 = (90 - lat2) * Math.PI / 180.0;
		lng1 = lng1 * Math.PI / 180.0;
		lng2 = lng2 * Math.PI / 180.0;
		C = Math.sin(lat1) * Math.sin(lat2) * Math.cos(lng1 - lng2) 
				+ Math.cos(lat1) * Math.cos(lat2);
		D = EARTH_R * Math.acos(C);// * Math.PI / 180.0;
		return D;*/
		return Math.abs(lat1 - lat2) + Math.abs(lng1 - lng2);
	}
}

class Entry {
	private double				lat, lng;
	private String				name = "";
	private int					pcode;
	private String				url = "";
	private String				address = "";
	//private TrieTree	tree;
	private HashSet<String>		keySet;

	public Entry() {
		keySet = new HashSet<String>();
		//tree = new TrieTree();
		//tree.node = "";
	}

/*	public TrieTree getTree() {
		return tree;
	}

	public void setTree(String [] keyStringArray) {
		String str;
		for (int i = 0; i < keyStringArray.length; ++i)
		{
			str = keyStringArray[i];
			this.tree.addString(str);
		}
	}*/

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

	public String getName() {
		return name;
	}

	public void setName(String name) {
		this.name = name;
	}

	public int getPcode() {
		return pcode;
	}

	public void setPcode(int pcode) {
		this.pcode = pcode;
	}

	public String getAddress() {
		return address;
	}

	public void setAddress(String address) {
		this.address = address;
	}

	public HashSet<String> getKeySet() {
		return keySet;
	}

	public void setKeySet(String [] keyStringArray) {
		String str;
		for (int i = 0; i < keyStringArray.length; ++i)
		{
			str = keyStringArray[i];
			this.keySet.add(str);
		}
	}

	public String getUrl() {
		return url;
	}

	public void setUrl(String url) {
		this.url = url;
	}

/*	public HashSet<String> getKeys() {
		return keys;
	}

	public void setKeys(String [] keyStringArray) {
		int strLen;
		String str, prefix;
		for (int i = 0; i < keyStringArray.length; ++i)
		{
			str = keyStringArray[i];
			this.keys.add(str);
			strLen = str.length();
			for (int j = 1; j <= strLen; ++j)
			{
				prefix = str.substring(0, j);
				this.keys.add(prefix);
			}
		}
		for (int i = 0; i < keyStringArray.length; ++i) {
			this.keys.add(keyStringArray[i]);
		}
	}*/
}

class Result// implements Comparable<Result>
{
	private double	lat, lng;
	private double	distance;
	private String	name = "";
	private int		pcode;
	private String	address;
	private String	url;
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
	public double getDistance() {
		return distance;
	}
	public void setDistance(double distance) {
		this.distance = distance;
	}
	public String getName() {
		return name;
	}
	public void setName(String name) {
		this.name = name;
	}
	public int getPcode() {
		return pcode;
	}
	public void setPcode(int pcode) {
		this.pcode = pcode;
	}
	/*
	@Override
	public int compareTo(Result res) {
		if (distance < res.getDistance())
		{
			return 1;
		}
		else if (distance > res.getDistance())
		{
			return -1;
		}
		return 0;
	}
	*/
	public String getAddress() {
		return address;
	}
	public void setAddress(String address) {
		this.address = address;
	}
	public String getUrl() {
		return url;
	}
	public void setUrl(String url) {
		this.url = url;
	}
}
