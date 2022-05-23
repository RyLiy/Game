#include "Message.h"
#include "MessageBus.h"
#include "MessageBusNode.h"
#include <vector>
#include <map>
#include <optional>
#include <iostream>
#include <algorithm>
using namespace std;
class MessageBus { 
	/* To-do:
	- Reduce redundency between nodes and nodeClass; as node contains all of the data that nodeClass already contains. Probably eliminate the nodes vector, and use the map as a singular data source.
	- Create the ability to classify nodes into more than one system space, without redundency.
	- Create a persistant map, without writing to an external file. I.e., "that can be considered at the same time a store and a cache, using a design where data is always modified and read from the main computer memory, but also stored on disk in a format that is unsuitable for random access of data, but only to reconstruct the data back in memory once the system restarts." Use redis for this.
	- Create the ability for a message to be directed towards more than 1 node, without having to be pushed to an entire system space. Probably make systemComponent in the Message class a vector.
	- Create a better way to modify the map. I.e., add or remove nodes on the bus.
	- Once all is done, probably refactor the map name (currently: nodeClass) into the name systemMap or engineMap. From thereafter, the map will play a central role for the message bus; every component (read: class or object) in the entire engine should be able to be referenced from it.*/
private:
	std::vector<MessageBusNode> nodes;
	std::vector<Message> messageQueue;

	//Creates a map structure to classify what system space a node belongs to. (E.g., does the node belong to the input system, render system, game-logic system, etc.) 
	map<std::string, std::vector<MessageBusNode>> nodeClass;

public:
	void addNode(MessageBusNode node) {
		nodes.push_back(node);
	}

	void sendMessage(Message msg) {
		messageQueue.push_back(msg);

	}

	void notify() {
		for (int i = 0; i > messageQueue.size(); i++) { //iterates through message queue
			auto msg = messageQueue.back(); 
			messageQueue.pop_back();
			
			if (std::find(nodes.begin(), nodes.end(), msg.systemComponent) != nodes.end()) { //if the message is to be delivered to a single node on the bus, instead of an entire system space.
				msg.systemComponent.value().receiveMessage(msg);
			}
			else {
				for (auto iterate = nodes.begin(); iterate != nodes.end(); iterate++) { //send the message to all nodes in the bus. I.e., all system spaces.
					(*iterate).receiveMessage(msg);
				}
			}

			for (auto iter = nodeClass.at(msg.systemSpace).begin(); iter != nodeClass.at(msg.systemSpace).end(); iter++) { //if message is directed towards a certain system space. If empty, the for loop will not execute.
				(*iter).receiveMessage(msg);
			}

		}
	}


	void addNode(MessageBusNode node, std::optional<std::string> nodeClass) {


	}
};