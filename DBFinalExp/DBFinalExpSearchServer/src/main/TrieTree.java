package main;

import java.util.HashMap;
import java.util.Iterator;

/**
 * A simple trie-tree used to find exist prefix.
 * @author lizhen
 *
 */
public class TrieTree {
	TrieNode	node;
	
	public TrieTree()
	{
		node = new TrieNode();
		//children = new ArrayList<TrieTree>();
		//children = new HashMap<String, TrieTree>();
	}
	
	public void addString(String str)
	{
		int strLen = str.length();
		TrieNode tmp = node;
		for (int st = 0; st < strLen; ++st)
		{
			String first = str.substring(st, st+1);
			if (!tmp.children.containsKey(first))
			{
				TrieNode child = new TrieNode();
				tmp.children.put(first, child);
			}
			tmp = tmp.children.get(first);
		}
/*		if (str.length() > 0)
		{
			String first = str.substring(0, 1);
			if (children.containsKey(first))
			{
				if (str.length() > 1)
				{
					children.get(first).addString(str.substring(1));
				}
			}
			else
			{
				TrieTree subTree = new TrieTree();
				subTree.node = first;
				subTree.addString(str.substring(1));
				children.put(first, subTree);
			}*/
			//int num = children.size();
/*			boolean jud = false;
			//System.out.println("node:" + node);
			Iterator<TrieTree> it = children.iterator();
			while(it.hasNext())
			//for (int i = 0; i < num; ++i)
			{
				TrieTree child = it.next();
				if (child.node.compareTo(str.substring(0, 1)) == 0)
				//if (children.get(i).node.compareTo(str.substring(0, 1)) == 0)
				{
					//System.out.println("add:" + str.substring(0, 1));
					//children.get(i).addString(str.substring(1));
					if (str.length() > 1)
					{
						child.addString(str.substring(1));
					}
					jud = true;
					break;
				}
			}
			if (!jud)
			{
				//System.out.println("new:" + str.substring(0, 1));
				TrieTree subTree = new TrieTree();
				subTree.node = str.substring(0, 1);
				subTree.addString(str.substring(1));
				children.add(subTree);
			}*/
		//}
	}
	
	public boolean containString(String str)
	{
		boolean ret = true;
		int strLen = str.length();
		TrieNode tmp = node;
		for (int st = 0; st < strLen; ++st)
		{
			String first = str.substring(st, st+1);
			if (!tmp.children.containsKey(first))
			{
				ret = false;
				break;
			}
			tmp = tmp.children.get(first);
		}
		//if (str.length() > 0)
		//{
			//int num = children.size();
			//TODO:
/*			boolean jud = false;
			Iterator<TrieTree> it = children.iterator();
			while(it.hasNext())
			//for (int i = 0; i < num; ++i)
			{
				TrieTree child = it.next();
				if (child.node.compareTo(str.substring(0, 1)) == 0)
				//if (children.get(i).node.compareTo(str.substring(0, 1)) == 0)
				{
					//ret = children.get(i).containString(str.substring(1));
					ret = child.containString(str.substring(1));
					jud = true;
				}
			}
			if (!jud)
			{
				ret = false;
			}*/
		//}
		return ret;
	}
}

class TrieNode
{
	//public String 				nodeStr;
	HashMap<String, TrieNode>	children;
	
	public TrieNode()
	{
		//nodeStr = "";
		children = new HashMap<String, TrieNode>();	
	}
}