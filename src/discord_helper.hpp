#ifndef DISCORD_HELPER_HPP
#define DISCORD_HELPER_HPP

#include <vector>
#include <string>

struct messageauthor{
	std::string username;
	std::string discriminator;
	std::string id;
	std::string avatar;
	
};

struct message{
	std::string timestamp;
	std::string id;
	//std::string avatar;
	struct messageauthor author;
	std::string content;
};

struct channel{
	std::string name;
	
	std::string last_pin_timestamp;
	std::string topic;
	std::string last_message_id;
	int type;
	std::string id;
	bool is_private;
	
	std::vector<message> messages;
};


struct guild{
	bool owner;
	long permissions;
	std::string icon;
	std::string id;
	std::string name;
	std::vector<channel> channels;
};

struct user {
	std::string id;
	std::string name;
	std::string icon;
};
#endif


