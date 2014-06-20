package main;

import java.io.BufferedReader;
import java.io.InputStreamReader;
import java.io.PrintWriter;
import java.net.Socket;
import java.lang.String;

import org.json.JSONObject;

/**
 * Communication Thread of Search Server.
 * @author lizhen
 *
 */
public class ComThread implements Runnable {

	private Socket 			mSocket;
	private BufferedReader 	mReader;
	private PrintWriter 	mWriter;
	private int 			id = 0;

	public ComThread(Socket socket, int id) {
		this.mSocket = socket;
		this.id = id;
		System.out.println(id + " : start.");
	}

	@Override
	public void run() {
		try {
			/** Receive data from django-python */
			mReader = new BufferedReader(new InputStreamReader(mSocket.getInputStream()));      
			String recvData = mReader.readLine();
			System.out.println(id + " : " + recvData);
			
			/** Decode the JSON */
			JSONObject json = new JSONObject(recvData);
			double lat, lng;
			String keyWords;
			lat = json.getDouble("lat");
			lng = json.getDouble("lng");
			keyWords = json.getString("keyWords");
			System.out.println(id + " : " + lat + " ; " + lng + " ; " + keyWords);
			
			/** Get search result in JSON format */
			String sendData = Searcher.doSearch(lat, lng, keyWords);
			
			/** Send JSON result to the front client */
			mWriter = new PrintWriter(mSocket.getOutputStream());
			//String sendData = "{\"Pos\":[{\"Lat\":15,\"Lng\":100}]}";
			System.out.println(id + " : " + sendData);
			mWriter.write(sendData);
			mWriter.flush();
			mSocket.close();   
		} catch (Exception e) {
			e.printStackTrace();
		} 

	}
	
	public void setSocket(Socket socket) {
		this.mSocket = socket;
	}
	
	public Socket getSocket() {
		return mSocket;
	}
}
