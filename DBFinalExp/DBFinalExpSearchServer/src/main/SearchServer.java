package main;

import java.io.IOException;
import java.net.ServerSocket;

/**
 * Search Server of DB Final Exp. Communicate with Django + Python.
 * @author lizhen
 * 
 */
public class SearchServer {

	private ServerSocket	mSocket;
	public final int		MY_PORT = 8080;

	public SearchServer() {
		int id = 0;
		try {
			mSocket = new ServerSocket(MY_PORT);
			System.out.println("Waiting...");
			while(true) 
			{
				++id;
				ComThread ct = new ComThread(mSocket.accept(), id);
				Thread t = new Thread(ct);
				t.start();
			}
		} catch (IOException e) {
			e.printStackTrace();
		} finally {
			if (mSocket != null)
			{
				try {
					mSocket.close();
				} catch (IOException e) {
					e.printStackTrace();
				}
			}
		}
	}
	/**
	 * @param args
	 */
	public static void main(String[] args) {
		String dataFileName = "G:\\Course\\Grade_Three_Spring\\Database\\DatabaseCourse\\DBFinalExp\\DBFinalExpSearchServer\\data\\zipcode-address.json";
		
		if (Searcher.loadData(dataFileName)) 
		{	
			System.out.println("Load succeeded.");
			Searcher.printData(8, 10);

			//SearchServer mServer = new SearchServer();
		}
		else 
		{
			System.out.println("Load failed.");
		}
		
	}

}
