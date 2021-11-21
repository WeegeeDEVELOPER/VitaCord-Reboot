#include "Discord.hpp"
#include "log.hpp"

#include <bitset>
#include <pthread.h>
#include <locale>
#include <codecvt>
#include <string>
#include <iostream>
#include <iconv.h>
#include <wchar.h>
#include <stdlib.h>
#include <algorithm>   // for reverse
#include <debugnet.h>
#include <psp2/io/fcntl.h>

#include <cctype>
#include "json.hpp"
#include "easyencryptor.hpp"
#include "utf8.h"

#include <psp2/kernel/processmgr.h>

uint64_t Discord::osGetTimeMS(){
	return (sceKernelGetProcessTimeWide() / 1000);
}

Discord::Discord(){

	int fh = sceIoOpen("app0:assets/emoji/emojispritesheettable.json" , SCE_O_RDONLY , 0777);
	if(fh < 0){
		debugNetPrintf(DEBUG , "ERROR : Could not load emojispritesheettable.json !\n" );
	}else {
		debugNetPrintf(DEBUG , "Loaded emojispritesheettable.json !\n" );

		int fileSize = sceIoLseek ( fh, 0, SCE_SEEK_END );
		sceIoLseek ( fh, 0, SCE_SEEK_SET ); // reset 'cursor' in file


		std::string jsonString(fileSize , '\0');
		sceIoRead(fh , &jsonString[0] , fileSize );


		emojiJsonData = nlohmann::json::parse(jsonString);

		if( !emojiJsonData.is_null() ){

			if( !emojiJsonData["emoji"].is_null() ){

				int emojiCount = emojiJsonData["emoji"].size(); // amount of emojis in json

				if( !emojiJsonData["amount"].is_null() ){
					int amount2 = emojiJsonData["amount"].get<int>();
					if(amount2 == emojiCount){
						debugNetPrintf(DEBUG, " Got right amount of emojis in JSON!\n");
						std::string out = "Counted emojis : " + std::to_string( emojiCount) + "  , json amount var : " + std::to_string(amount2) + "\n";
						debugNetPrintf(DEBUG, out.c_str());
					}else{
						debugNetPrintf(DEBUG, " amount of emojis is different in json!\n");
						std::string out = "Counted emojis : " + std::to_string( emojiCount) + "  , json amount var : " + std::to_string(amount2) + "\n";
						debugNetPrintf(DEBUG, out.c_str());
					}

				}

				emojiTestArray.clear();
				int loadedIconsChecked = 0;

				debugNetPrintf(DEBUG, "> Loading big spritesheet !\n");
				spritesheetEmoji = vita2d_load_PNG_file("app0:assets/emoji/emojispritesheet.png");
				debugNetPrintf(DEBUG, ">>> Loaded big spritesheet !!!\n");


				debugNetPrintf(DEBUG, " Get dimensions of emoji!\n");
				if( !emojiJsonData["spritewidth"].is_null() ){
					emojiWidth = emojiJsonData["spritewidth"].get<int>();
				}
				if( !emojiJsonData["spriteheight"].is_null() ){
					emojiHeight = emojiJsonData["spriteheight"].get<int>();
				}
				
				int code = 0;

				debugNetPrintf(DEBUG, " Loop to add emoji coords to map!\n");
				for(int e = 0; e < emojiCount ; e++){
					if( !emojiJsonData["emoji"][e].is_null() ){
						if( !emojiJsonData["emoji"][e]["utf32code"].is_null() ){
							if( !emojiJsonData["emoji"][e]["utf32code"][0].is_null() ){
								if( !emojiJsonData["emoji"][e]["x"].is_null() ){
									if( !emojiJsonData["emoji"][e]["y"].is_null() ){
										debugNetPrintf(DEBUG, " Declare new emoji!\n");
										emoji addEmoji;
										debugNetPrintf(DEBUG, " assign emoji x\n");
										addEmoji.x = emojiJsonData["emoji"][e]["x"].get<int>();
										debugNetPrintf(DEBUG, " assign emoji y\n");
										addEmoji.y = emojiJsonData["emoji"][e]["y"].get<int>();
										debugNetPrintf(DEBUG, " assign code \n");
										code = emojiJsonData["emoji"][e]["utf32code"][0].get<int>();
										debugNetPrintf(DEBUG, " assign map key code's value to emoji\n");
										emojiMap[code] = addEmoji;
										debugNetPrintf(DEBUG, " push back code in testarray\n");
										emojiTestArray.push_back(code);
										debugNetPrintf(DEBUG, " inc loadedIcons\n");
										loadedIconsChecked++;
									}
								}
							}
						}
					}
				}

				debugNetPrintf(DEBUG , "Total loaded icons : %d \n\n " , loadedIconsChecked);

			}


		}


	}
	emojiCount = emojiMap.size();
	std::string mapSizeStr = "unordered_map of emoji contains : " + std::to_string(emojiCount) + " elements !\n";
	debugNetPrintf(DEBUG, mapSizeStr.c_str() );


	loadedGuilds = false;
	loadingData = false;
}

Discord::~Discord(){
		//?
}
bool Discord::sendDirectMessage(std::string msg){
	debugNetPrintf(DEBUG , "Sending DM\n" );
	std::string postData = "{ \"content\":\"" + msg + "\" }";
	std::string sendDMMessageUrl = "https://discordapp.com/api/v6/channels/"
							+ directMessages[currentDirectMessage].id + "/messages" ;
	VitaNet::http_response senddmmessageresponse = vitaNet.curlDiscordPost(sendDMMessageUrl , postData , token);
	if(senddmmessageresponse.httpcode == 200){
		debugNetPrintf(DEBUG , "DM SENT!\n" );
		return true;
	}

	return false;
}

bool Discord::sendMessage(std::string msg){
	debugNetPrintf(DEBUG , "Sending message\n" );
	std::string postData = "{ \"content\":\"" + msg + "\" }";
	std::string sendMessageUrl = "https://discordapp.com/api/channels/" + guilds[currentGuild].channels[currentChannel].id + "/messages" ;
	VitaNet::http_response sendmessageresponse = vitaNet.curlDiscordPost(sendMessageUrl , postData , token);
	if(sendmessageresponse.httpcode == 200){
		debugNetPrintf(DEBUG , "Message SENT!\n" );
		return true;
	}
	return false;
}


bool Discord::editMessage(std::string channelID , std::string messageID , std::string newContent){
	std::string editMessageUrl = "https://discordapp.com/api/channels/" + channelID + "/messages/" + messageID;
	std::string patchData = "{ \"content\":\"" + newContent + "\" }";
	VitaNet::http_response editmessageresponse = vitaNet.curlDiscordPatch(editMessageUrl , patchData , token);
	if(editmessageresponse.httpcode == 200){


		for(unsigned int i = 0 ; i < guilds[currentGuild].channels[currentChannel].messages.size();i++){
			if(guilds[currentGuild].channels[currentChannel].messages[i].id == messageID){

				guilds[currentGuild].channels[currentChannel].messages[i].content = newContent;
				refreshedMessages = true;// MAYBE MAKE ANOTHER VARIABLE
				i= 99999;
				break;
			}

		}




		return true;
	}
	return false;
}

bool Discord::deleteMessage(std::string channelID , std::string messageID){
	std::string deleteMessageUrl = "https://discordapp.com/api/channels/" + channelID + "/messages/" + messageID;
	VitaNet::http_response deletemessageresponse = vitaNet.curlDiscordDelete(deleteMessageUrl , token);
	if(deletemessageresponse.httpcode == 204){

		/* this code probably is cause of gpu crash , because vitagui tries to read the last message somewhere or so which does not exist .. maybe.
		// delete from deque

		for(unsigned int i = 0 ; i < guilds[currentGuild].channels[currentChannel].messages.size();i++){
			if(guilds[currentGuild].channels[currentChannel].messages[i].id == messageID){

				if(guilds[currentGuild].channels[currentChannel].messages[i].attachment.loadedThumbImage && guilds[currentGuild].channels[currentChannel].messages[i].attachment.thumbnail != NULL){
					vita2d_free_texture(guilds[currentGuild].channels[currentChannel].messages[i].attachment.thumbnail);
				}
				guilds[currentGuild].channels[currentChannel].messages.erase(guilds[currentGuild].channels[currentChannel].messages.begin() + i);
				refreshedMessages = true;// MAYBE MAKE ANOTHER VARIABLE
				i = 99999;
				break;
			}

		}
		refreshedMessages = true;
		*/
		//guilds[currentGuild].channels[currentChannel].messages.clear();

		return true;


	}
	return false;
}


bool Discord::refreshMessages(){

	//debugNetPrintf(DEBUG , "checking time to refresh messages\n" );
	currentTimeMS = osGetTimeMS();
	if(( currentTimeMS - lastFetchTimeMS > fetchTimeMS || forceRefreshMessages ) && !currentlyRefreshingMessages){
		//debugNetPrintf(DEBUG , "get new messages\n" );

		refreshingMessages = true;
		currentlyRefreshingMessages = true;
		getChannelMessages(currentChannel);
		currentlyRefreshingMessages = false;
		lastFetchTimeMS = osGetTimeMS();
		refreshedMessages = true;
		refreshingMessages = false;
		forceRefreshMessages = false;

	}
	return true;

}

void Discord::utf16_to_utf8(uint16_t *src, uint8_t *dst) {
	int i;
	for (i = 0; src[i]; i++) {
		if ((src[i] & 0xFF80) == 0) {
			*(dst++) = src[i] & 0xFF;
		} else if((src[i] & 0xF800) == 0) {
			*(dst++) = ((src[i] >> 6) & 0xFF) | 0xC0;
			*(dst++) = (src[i] & 0x3F) | 0x80;
		} else if((src[i] & 0xFC00) == 0xD800 && (src[i + 1] & 0xFC00) == 0xDC00) {
			*(dst++) = (((src[i] + 64) >> 8) & 0x3) | 0xF0;
			*(dst++) = (((src[i] >> 2) + 16) & 0x3F) | 0x80;
			*(dst++) = ((src[i] >> 4) & 0x30) | 0x80 | ((src[i + 1] << 2) & 0xF);
			*(dst++) = (src[i + 1] & 0x3F) | 0x80;
			i += 1;
		} else {
			*(dst++) = ((src[i] >> 12) & 0xF) | 0xE0;
			*(dst++) = ((src[i] >> 6) & 0x3F) | 0x80;
			*(dst++) = (src[i] & 0x3F) | 0x80;
		}
	}

	*dst = '\0';
}

/*static int strConv(const string &src, wstring &dst)
{
    iconv_t cd = iconv_open("UCS-4-INTERNAL", "UTF-8");
    if (cd == (iconv_t)-1)
        return -1;

    size_t src_length = strlen(src.c_str());
    int wlen = (int)src_length/3;
    size_t buf_length = src_length + wlen;

    char src_buf[src_length];
    strcpy(src_buf, src.c_str());
    char *buf = new char [buf_length];
    char *inptr = src_buf;
    char *outptr = buf;
    if (iconv(cd, &inptr, &src_length, &outptr, &buf_length) == -1)
    {
        if (buf!=NULL)
            delete [] buf;
        return -1;
    }
    iconv_close(cd);

    dst = wstring(reinterpret_cast<wchar_t*>(buf));
    dst = dst.substr(0, wlen);

    if (buf!=NULL)
        delete [] buf;

    return wlen;
}*/

void Discord::parseMessageContentEmoji(message *m , std::string str){


	int currentLineY = 0;
	m->emojis.clear();
	std::vector<unsigned char> utf8result;
	std::vector<unsigned int> utf32str;
	utf8::utf8to32(str.begin(), str.end(), back_inserter(utf32str));
	for(unsigned int x = 0 ; x < utf32str.size() ; x++){
		emojiMapIterator = emojiMap.find( static_cast<int>(utf32str[x]) );
		if(utf32str[x] > 0xFF){
			// ignoring all <0xFF (ascii) for now until supporting emojis consisting of two emoji ( modifier )
			
			if(emojiMapIterator != emojiMap.end()){
				
				message_emoji mEmoji;
				
				mEmoji.codepoint = static_cast<int>(utf32str[x]);
				mEmoji.spriteSheetX = emojiMapIterator->second.x;
				mEmoji.spriteSheetY = emojiMapIterator->second.y;
				mEmoji.posX = x % 40;  // HARDCODED max char per line = 30 :>
				mEmoji.posY = currentLineY;
				
				m->emojis.push_back(mEmoji);
				
				
				
				utf32str[x] = 0x20;
				
			}else if(utf32str[x] >= 0x2139 && utf32str[x] <= 0x3299){
				utf32str[x] = 0x20;
			}else if(utf32str[x] >= 0x1F004 && utf32str[x] <= 0x1F9E6){
				utf32str[x] = 0x20;
				
			}
		}
		
		
		
		if(x % 39 == 0 && x != 0){
			//utf32str.insert(utf32str.begin() + x , 0xA);
			utf32str.insert( utf32str.begin() + x , 0xA);
			currentLineY++;
		}
	}

	utf8::utf32to8(utf32str.begin(), utf32str.end(), back_inserter(utf8result));
	
	m->content = std::string( utf8result.begin() , utf8result.end() );
}

void Discord::getChannelMessages(int channelIndex){
	currentChannel = channelIndex;
	std::string channelMessagesUrl = "https://discordapp.com/api/channels/" + guilds[currentGuild].channels[currentChannel].id + "/messages?limit=100";

	if(guilds[currentGuild].channels[currentChannel].gotMessagesOnce ){
		channelMessagesUrl += "&after=" + guilds[currentGuild].channels[currentChannel].last_message_id;
	}

	VitaNet::http_response channelmessagesresponse = vitaNet.curlDiscordGet(channelMessagesUrl , token);
	logSD(channelmessagesresponse.body);
	if(channelmessagesresponse.httpcode == 200){
		nlohmann::json j_complete = nlohmann::json::parse(channelmessagesresponse.body);
		int messagesAmount = j_complete.size();


		if(!j_complete.is_null()){

			//guilds[currentGuild].channels[currentChannel].messages.clear();

			for( int i = 0 ; i < messagesAmount ; i++){

				int iR = messagesAmount - i - 1;

				message newMessage;

				if(!j_complete[iR].is_null()){

					if(!j_complete[iR]["timestamp"].is_null()){
						newMessage.timestamp = j_complete[iR]["timestamp"].get<std::string>();
					}else{
						newMessage.timestamp = "0";
					}

					if(!j_complete[iR]["id"].is_null()){
						newMessage.id = j_complete[iR]["id"].get<std::string>();
						guilds[currentGuild].channels[currentChannel].last_message_id = newMessage.id;
					}else{
						newMessage.id = "0";
					}

					if(!j_complete[iR]["content"].is_null()){

						//std::string str =
						//char * content = new char [str.length()+1];
						//std::strcpy (content, str.c_str());
						//char * contentUtf8 = new char [str.length()+1];
						//utf16_to_utf8((uint16_t *)content , (uint8_t *) contentUtf8);
						parseMessageContentEmoji(&newMessage , j_complete[iR]["content"].get<std::string>() );
						//newMessage.content = j_complete[iR]["content"].get<std::string>();
					}else{
						newMessage.content = "";
					}
					// author :
					if(!j_complete[iR]["author"]["username"].is_null()){
						newMessage.author.username = j_complete[iR]["author"]["username"].get<std::string>();
					}else{
						newMessage.author.username = "N/A";
					}

					if(!j_complete[iR]["author"]["discriminator"].is_null()){
						newMessage.author.discriminator = j_complete[iR]["author"]["discriminator"].get<std::string>();
					}else{
						newMessage.author.discriminator = "N/A";
					}

					if(!j_complete[iR]["author"]["id"].is_null()){
						newMessage.author.id = j_complete[iR]["author"]["id"].get<std::string>();
					}else{
						newMessage.author.id = "0";
					}

					if(!j_complete[iR]["author"]["avatar"].is_null()){
						newMessage.author.avatar = j_complete[iR]["author"]["avatar"].get<std::string>();
					}else{
						newMessage.author.avatar = "0";
					}

					newMessage.attachment.isEmpty = true;
					
					bool attachmentDownloadEnabled = false;
					
					if(!j_complete[iR]["attachments"].is_null() && attachmentDownloadEnabled){
						if(!j_complete[iR]["attachments"][0].is_null()){

							newMessage.attachment.isEmpty = false;
							newMessage.attachment.isImage = false ;
							newMessage.attachment.isData = false ;
							newMessage.attachment.loadedThumbImage = false;

							bool proxyAvailable = false;
							bool filenameAvailable = false;
							bool imageDimensionAvailable = false;
							bool sizeAvailable = false;
							bool urlAvailable = false;

							if(!j_complete[iR]["attachments"][0]["url"].is_null()){
								newMessage.attachment.url = j_complete[iR]["attachments"][0]["url"].get<std::string>();
								urlAvailable=true;
							}else{
								newMessage.attachment.url = "";
							}

							if(!j_complete[iR]["attachments"][0]["proxy_url"].is_null()){
								newMessage.attachment.proxy_url = j_complete[iR]["attachments"][0]["proxy_url"].get<std::string>();
								proxyAvailable = true;
							}else{
								newMessage.attachment.proxy_url = "";
							}

							if(!j_complete[iR]["attachments"][0]["filename"].is_null()){
								newMessage.attachment.filename = j_complete[iR]["attachments"][0]["filename"].get<std::string>();
								filenameAvailable = true;
							}else{
								newMessage.attachment.filename = "noname.png";
							}

							if(!j_complete[iR]["attachments"][0]["id"].is_null()){
								newMessage.attachment.id = j_complete[iR]["attachments"][0]["id"].get<std::string>();
							}else{
								newMessage.attachment.id = "";
							}


							if ( !j_complete[iR]["attachments"][0]["width"].is_null() ){
								newMessage.attachment.width = j_complete[iR]["attachments"][0]["width"].get<int>();
								imageDimensionAvailable = true;
							} else {
								newMessage.attachment.width = -1;
							}

							if ( !j_complete[iR]["attachments"][0]["height"].is_null() ){
								newMessage.attachment.height = j_complete[iR]["attachments"][0]["height"].get<int>();
							} else {
								newMessage.attachment.height = -1;
							}

							if ( !j_complete[iR]["attachments"][0]["size"].is_null() ){
								newMessage.attachment.size = j_complete[iR]["attachments"][0]["size"].get<int>();
								sizeAvailable = true;

								if(newMessage.attachment.size > 1024*1024){
									newMessage.attachment.readableSize = static_cast<int> (  newMessage.attachment.size / ( 1024 * 1024 ) );
									newMessage.attachment.readableSizeUnit = "MiB";
								}else if(newMessage.attachment.size > 1024){
									newMessage.attachment.readableSize =  static_cast<int> (  newMessage.attachment.size / ( 1024 ) );
									newMessage.attachment.readableSizeUnit = "KiB";
								}else{
									newMessage.attachment.readableSize =  static_cast<int> (  newMessage.attachment.size );
									newMessage.attachment.readableSizeUnit = "Byte";
								}


							} else {
								newMessage.attachment.size = -1;
							}

							if ( proxyAvailable && filenameAvailable && imageDimensionAvailable ){
								newMessage.attachment.isImage = true ;
								newMessage.attachment.isData = false ;
								int thumbW = 64;//static_cast<int>( 64 * ( newMessage.attachment.width / newMessage.attachment.height ) );
								int thumbH = 64;
								std::string thumbUrl = newMessage.attachment.proxy_url + "?width=" + std::to_string(thumbW) + "&height=" +std::to_string( thumbH );
								debugNetPrintf( DEBUG , "Loading thumbnail : " );
								debugNetPrintf( DEBUG , thumbUrl.c_str() );
								std::string imageThumbFileName = "ux0:data/vitacord/attachments/thumbnails/" + newMessage.attachment.filename;
								VitaNet::http_response thumbnailResponse = vitaNet.curlDiscordDownloadImage( thumbUrl , token , imageThumbFileName );
								if( thumbnailResponse.httpcode == 200){


									// check which format ! ( magic numbers check ) a little big , should make a function for it # TODO

									int imageThumbFD = sceIoOpen( imageThumbFileName.c_str() , SCE_O_RDONLY , 0777 );
									int MAGIC_BUFFER_SIZE = 50;
									char magicNumberBuffer [MAGIC_BUFFER_SIZE];
									sceIoRead(imageThumbFD , magicNumberBuffer , MAGIC_BUFFER_SIZE);
									sceIoClose(imageThumbFD);

									// START OF MAGIC NUMBER CHECKER !
									// BMP first :)
									if( magicNumberBuffer[0] == (char)0x42 ){
										if( magicNumberBuffer[1] == 0x4D ){
											newMessage.attachment.thumbnail = vita2d_load_BMP_file( imageThumbFileName.c_str() );
											if( newMessage.attachment.thumbnail != NULL){
												debugNetPrintf( DEBUG , "Could load bmp!");
												newMessage.attachment.loadedThumbImage = true;
											}
										}
									} // now PNG :
									else if ( magicNumberBuffer[0] == (char)0x89 ){
										if ( magicNumberBuffer[1] == 0x50 ){
											if ( magicNumberBuffer[2] == 0x4E ){
												if ( magicNumberBuffer[3] == 0x47 ){
													if ( magicNumberBuffer[4] == 0x0D ){
														if ( magicNumberBuffer[5] == 0x0A ){
															if ( magicNumberBuffer[6] == 0x1A ){
																if ( magicNumberBuffer[7] == 0x0A ){
																	newMessage.attachment.thumbnail = vita2d_load_PNG_file( imageThumbFileName.c_str() );
																	if( newMessage.attachment.thumbnail != NULL){
																		debugNetPrintf( DEBUG , "Could load png!");
																		newMessage.attachment.loadedThumbImage = true;
																	}
																}
															}
														}
													}
												}
											}
										}
									} // now 3 different JPG magic numbers  [ raw | Exif | JFIF ]
									else if( magicNumberBuffer[0] == (char)0xFF ){
										if( magicNumberBuffer[1] == 0xD8 ){
											if( magicNumberBuffer[2] == 0xFF ){
												if( magicNumberBuffer[3] == 0xD8 ){
													// ÿØÿÛ
													newMessage.attachment.thumbnail = vita2d_load_JPEG_file( imageThumbFileName.c_str() );
													if( newMessage.attachment.thumbnail != NULL){
														debugNetPrintf( DEBUG , "Could load jpg [ raw ] !");
														newMessage.attachment.loadedThumbImage = true;
													}

												}
												else if( magicNumberBuffer[3] == (char)0xE0 ){
													if( magicNumberBuffer[6] == 0x4A ){
														if( magicNumberBuffer[7] == 0x46 ){
															if( magicNumberBuffer[8] == 0x49 ){
																if( magicNumberBuffer[9] == 0x46 ){
																	if( magicNumberBuffer[10] == 0x00 ){
																		if( magicNumberBuffer[11] == 0x01 ){
																			//ÿØÿà ..JFIF..
																			newMessage.attachment.thumbnail = vita2d_load_JPEG_file( imageThumbFileName.c_str() );
																			if( newMessage.attachment.thumbnail != NULL){
																				debugNetPrintf( DEBUG , "Could load jpg [ JFIF ] !");
																				newMessage.attachment.loadedThumbImage = true;
																			}
																		}
																	}
																}
															}
														}
													}


												}else if( magicNumberBuffer[3] == (char)0xE1 ){
													if( magicNumberBuffer[6] == 0x45 ){
														if( magicNumberBuffer[7] == 0x78 ){
															if( magicNumberBuffer[8] == 0x69 ){
																if( magicNumberBuffer[9] == 0x66 ){
																	if( magicNumberBuffer[10] == 0x00 ){
																		if( magicNumberBuffer[11] == 0x00 ){
																			//ÿØÿá ..Exif..
																			newMessage.attachment.thumbnail = vita2d_load_JPEG_file( imageThumbFileName.c_str() );
																			if( newMessage.attachment.thumbnail != NULL){
																				debugNetPrintf( DEBUG , "Could load jpg [ Exif ] !");
																				newMessage.attachment.loadedThumbImage = true;
																			}
																		}
																	}
																}
															}
														}
													}


												}

											}

										}


									}else{
										newMessage.attachment.loadedThumbImage = false;
										newMessage.attachment.isImage = false ;
										newMessage.attachment.isData = false ;
										if ( urlAvailable && filenameAvailable && sizeAvailable ){
											goto loaddata; //
										}
										debugNetPrintf(DEBUG , "Loading thumbnail error : No matching magic numbers ! Tested :[bmp | png | jpg] !");
									}
									// END OF MAGIC NUMBER CHECKER !


									debugNetPrintf(DEBUG , "LOADED THUMBNAIL!");





								}else{
									newMessage.attachment.isEmpty = true ;
									newMessage.attachment.isImage = false ;
									newMessage.attachment.isData = false ;
									debugNetPrintf(DEBUG , "FAiled loading THUMBNAIL!");
								}
							}else if ( urlAvailable && filenameAvailable && sizeAvailable ){

								loaddata:		// Label for goto
								if ( newMessage.attachment.size < 1024*1024 ){
									newMessage.attachment.isImage = false ;
									newMessage.attachment.isData = true ;
									// if less than 1 mb download :
									std::string fileUrl = newMessage.attachment.url;
									debugNetPrintf( DEBUG , "Loading attachment : " );
									debugNetPrintf( DEBUG , fileUrl.c_str() );
									std::string fileName = "ux0:data/vitacord/attachments/other/" + newMessage.attachment.filename;
									VitaNet::http_response fileGetResponse = vitaNet.curlDiscordDownloadImage( fileUrl , token , fileName);
									if( fileGetResponse.httpcode == 200){


										debugNetPrintf(DEBUG , "LOADED ATTACHED FILE!");



									}else{
										newMessage.attachment.isEmpty = true ;
										newMessage.attachment.isImage = false ;
										newMessage.attachment.isData = false ;
										debugNetPrintf(DEBUG , "FAiled loading ATTACHED FILE!");
									}



								}
							}else{
								newMessage.attachment.isEmpty = true ;
								newMessage.attachment.isImage = false ;
								newMessage.attachment.isData = false ;

							}


						}
					}



				}


				guilds[currentGuild].channels[currentChannel].messages.push_back(newMessage);



			}


			//std::reverse(guilds[currentGuild].channels[currentChannel].messages.begin() , guilds[currentGuild].channels[currentChannel].messages.end());

			guilds[currentGuild].channels[currentChannel].gotMessagesOnce = true;

		}
		debugNetPrintf(DEBUG , "End of getchannelmessages!!");
		lastFetchTimeMS = osGetTimeMS();

	}

}
void Discord::JoinGuild(int gIndex){
	currentGuild = gIndex;
}
void Discord::JoinChannel(int cIndex){
	inChannel = true;
	currentChannel = cIndex;
	forceRefreshMessages = true;
	refreshMessages();
	forceRefreshMessages = false;

	if(!pthreadStarted){
		debugNetPrintf(DEBUG , "Startint pthread refresh Messages\n");

		pthreadStarted = true;
		logSD("pthread_create( loadDataThread , NULL , wrapper , 0)");
		debugNetPrintf(DEBUG , "pthread_create coming\n");
		if( int errP = pthread_create(&loadMessagesThread, NULL, &Discord::refreshMessages_wrapper, this) != 0){
			debugNetPrintf(DEBUG , "PTHREAD_CREATE ERROR : %d\n" , errP);
			debugNetPrintf(DEBUG , "PTHREAD_CREATE ERROR : %d\n" , errP);
			debugNetPrintf(DEBUG , "PTHREAD_CREATE ERROR : %d\n" , errP);
			pthreadStarted = false;

		}else{

			debugNetPrintf(DEBUG , "successfully started pthread\n");

		}
	}
	//getChannelMessages(currentChannel);
}
void Discord::LeaveChannel(){
	inChannel = false;
	currentChannel = 0;
}
void Discord::setToken(std::string tok){
	token = tok;
}


void * Discord::thread_loadData(void *arg){

	Discord *discordPtr = reinterpret_cast<Discord *>(arg);
	logSD("start of thread_loadData");
	discordPtr->loadingData = true;
	while(discordPtr->loadingData){
		if(!discordPtr->loadedGuilds){
			std::string guildsUrl = "https://discordapp.com/api/users/@me/guilds";
			VitaNet::http_response guildsresponse = discordPtr->vitaNet.curlDiscordGet(guildsUrl , token);
			logSD(guildsresponse.body);
			if(guildsresponse.httpcode == 200){
				try{
					nlohmann::json j_complete = nlohmann::json::parse(guildsresponse.body);
					if(!j_complete.is_null()){
						discordPtr->guilds.clear();
						discordPtr->guildsAmount = j_complete.size();
						for(int i = 0; i < guildsAmount; i++){

							discordPtr->guilds.push_back(guild());

							if(!j_complete[i].is_null()){


								if(!j_complete[i]["owner"].is_null()){
									discordPtr->guilds[i].owner = j_complete[i]["owner"].get<bool>();
								}else{
									discordPtr->guilds[i].owner = false;
								}

								if(!j_complete[i]["permissions"].is_null()){
									discordPtr->guilds[i].permissions = j_complete[i]["permissions"].get<long>();
								}else{
									discordPtr->guilds[i].permissions = 0;
								}

								if(!j_complete[i]["icon"].is_null()){
									discordPtr->guilds[i].icon = j_complete[i]["icon"].get<std::string>();
								}else{
									discordPtr->guilds[i].icon = "";
								}

								if(!j_complete[i]["id"].is_null()){
									discordPtr->guilds[i].id = j_complete[i]["id"].get<std::string>();
									logSD(discordPtr->guilds[i].id);
								}else{
									discordPtr->guilds[i].id = "";
									logSD(discordPtr->guilds[i].id);
								}

								if(!j_complete[i]["name"].is_null()){
									discordPtr->guilds[i].name = j_complete[i]["name"].get<std::string>();
									logSD(discordPtr->guilds[i].name);

								}else{
									discordPtr->guilds[i].name = "";
									logSD(discordPtr->guilds[i].name);
								}



							}


						}
						discordPtr->loadedGuilds = true;
					}
				}catch(const std::exception& e){
					discordPtr->loadedGuilds = true;
				}

			}else{
				discordPtr->loadedGuilds = true;
			}
		}else if(discordPtr->loadedGuilds && !discordPtr->loadedChannels){





			for(int i = 0; i < discordPtr->guildsAmount ; i++){



				std::string myRolesUrl ="https://discordapp.com/api/guilds/" + discordPtr->guilds[i].id + "/members/" + discordPtr->id;
				VitaNet::http_response myRolesResponse = discordPtr->vitaNet.curlDiscordGet(myRolesUrl , token);
				if(myRolesResponse.httpcode == 200){
					try{
						nlohmann::json j_complete = nlohmann::json::parse(myRolesResponse.body);
						if(!j_complete.is_null()){

							if(!j_complete["roles"].is_null()){

								discordPtr->guilds[i].myroles.clear();
								int rolesAmount = j_complete["roles"].size();
								for(int rol = 0; rol < rolesAmount ; rol++){
									if(!j_complete["roles"][rol].is_null()){
										std::string role = j_complete["roles"][rol].get<std::string>();
										discordPtr->guilds[i].myroles.push_back(role);

									}

								}

							}

						}
					}catch(const std::exception& e){
						// nothing
					}

				}






				std::string channelUrl = "https://discordapp.com/api/guilds/" + discordPtr->guilds[i].id + "/channels";
				VitaNet::http_response channelresponse = discordPtr->vitaNet.curlDiscordGet(channelUrl , token);
				logSD(channelresponse.body);
				if(channelresponse.httpcode == 200){
					try{
						logSD("Create nlohmann json object by parsing response");
						nlohmann::json j_complete = nlohmann::json::parse(channelresponse.body);
						if(!j_complete.is_null()){
							discordPtr->guilds[i].channels.clear();
							int channelsAmount = j_complete.size();
							
							logSD("Channel amount " + std::to_string(channelsAmount));
							
							for(int c = 0; c < channelsAmount; c++){

								logSD("Adding to be filled out channel to channel vector");
								discordPtr->guilds[i].channels.push_back(channel());
								logSD("Added to be filled out channel to channel vector");

								logSD("Check current json object is null");
								if(!j_complete[c].is_null()){

									logSD(" current json object was not null.");
									if(!j_complete[c]["type"].is_null()){
										logSD("parsing type, which was not null");
										discordPtr->guilds[i].channels[c].type = j_complete[c]["type"].get<int>();
										logSD(std::to_string(discordPtr->guilds[i].channels[c].type));
									}else{
										logSD("setting type = 0 because json.type was null");
										discordPtr->guilds[i].channels[c].type = 0;
										logSD(std::to_string(discordPtr->guilds[i].channels[c].type));
									}

									if(!j_complete[c]["id"].is_null()){
										logSD("parsing id, which was not null");
										discordPtr->guilds[i].channels[c].id = j_complete[c]["id"].get<std::string>();
										logSD(discordPtr->guilds[i].channels[c].id);
									}else{
										logSD("setting id = 00000000 because json.id was null");
										discordPtr->guilds[i].channels[c].id = "00000000";
										logSD(discordPtr->guilds[i].channels[c].id);
									}

									if(!j_complete[c]["name"].is_null()){
										logSD("parsing name, which was not null");
										discordPtr->guilds[i].channels[c].name = j_complete[c]["name"].get<std::string>();
										logSD(discordPtr->guilds[i].channels[c].name);
									}else{
										logSD("setting name unavailable because json.name was null");
										discordPtr->guilds[i].channels[c].name = "name unavailable";
										logSD(discordPtr->guilds[i].channels[c].name);
									}

									if(!j_complete[c]["topic"].is_null()){
										logSD("parsing topic, which was not null");
										discordPtr->guilds[i].channels[c].topic = j_complete[c]["topic"].get<std::string>();
										logSD(discordPtr->guilds[i].channels[c].topic);
									}else{
										logSD("setting topic empty because json.topic was null");
										discordPtr->guilds[i].channels[c].topic = " ";
										logSD(discordPtr->guilds[i].channels[c].topic);
									}

									if(!j_complete[c]["is_private"].is_null()){
										logSD("parsing is_private, which was not null");
										discordPtr->guilds[i].channels[c].is_private = j_complete[c]["is_private"].get<bool>();
										logSD(std::to_string(discordPtr->guilds[i].channels[c].is_private));
									}else{
										logSD("setting is_private false because json.is_private was null");
										discordPtr->guilds[i].channels[c].is_private = false;
										logSD(std::to_string(discordPtr->guilds[i].channels[c].is_private));
									}

									if(!j_complete[c]["last_message_id"].is_null()){
										logSD("parsing last_message_id, which was not null");
										discordPtr->guilds[i].channels[c].last_message_id = j_complete[c]["last_message_id"].get<std::string>();
										logSD(discordPtr->guilds[i].channels[c].last_message_id);
									}else{
										logSD("setting last_message_id = 00000000 because json.last_message_id was null");
										discordPtr->guilds[i].channels[c].last_message_id = "00000000";
										logSD(discordPtr->guilds[i].channels[c].last_message_id);
									}

									if(!j_complete[c]["permission_overwrites"].is_null()){

										logSD("parsing permission_overwrites , which was not null");
										int p = j_complete[c]["permission_overwrites"].size();
										discordPtr->guilds[i].channels[c].permission_overwrites.clear();
										for(int per = 0; per < p; per++){
											discordPtr->guilds[i].channels[c].permission_overwrites.push_back(permission_overwrite());
											if(!j_complete[c]["permission_overwrites"][per]["allow"].is_null()){
												discordPtr->guilds[i].channels[c].permission_overwrites[per].allow = j_complete[c]["permission_overwrites"][per]["allow"].get<int>();

											}else{
												discordPtr->guilds[i].channels[c].permission_overwrites[per].allow = 0;
											}

											if(!j_complete[c]["permission_overwrites"][per]["type"].is_null()){
												discordPtr->guilds[i].channels[c].permission_overwrites[per].type = j_complete[c]["permission_overwrites"][per]["type"].get<std::string>();
											}else{
												discordPtr->guilds[i].channels[c].permission_overwrites[per].type = "role";
											}

											if(!j_complete[c]["permission_overwrites"][per]["id"].is_null()){
												discordPtr->guilds[i].channels[c].permission_overwrites[per].id = j_complete[c]["permission_overwrites"][per]["id"].get<std::string>();
											}else{
												discordPtr->guilds[i].channels[c].permission_overwrites[per].id = "0";
											}

											if(!j_complete[c]["permission_overwrites"][per]["deny"].is_null()){
												discordPtr->guilds[i].channels[c].permission_overwrites[per].deny = j_complete[c]["permission_overwrites"][per]["deny"].get<int>();
											}else{
												discordPtr->guilds[i].channels[c].permission_overwrites[per].deny = 0;
											}

										}
										logSD("end of parsing permission_overwrites.");


										// TODO : LEARN HOW TO REALLY CHECK PERMISSION !!!
										//bool readAllowedForMeOnce = false; // if one role has read rest doesnt matter
										//bool readDeniedForMeOnce = false;
										//
										//discordPtr->guilds[i].channels[c].readallowed = false;
										//
										//for(int permC = 0; permC < discordPtr->guilds[i].channels[c].permission_overwrites.size() ; permC++){
										//
										//	// check role "@everyone" ( = guildid)
										//	if(discordPtr->guilds[i].channels[c].permission_overwrites[permC].id == discordPtr->guilds[i].id){
										//		if(!(discordPtr->guilds[i].channels[c].permission_overwrites[permC].deny & PERMISSION_READ_MESSAGES)){
										//			readDeniedForMeOnce = true;
										//		}else{
										//			readAllowedForMeOnce = true;
										//		}
										//	}
										//
										//	// check all roles i have
										//	for(int myR = 0; myR < discordPtr->guilds[i].myroles.size() ; myR++){
										//		if(discordPtr->guilds[i].channels[c].permission_overwrites[permC].id == discordPtr->guilds[i].myroles[myR]){
										//
										//			if(!(discordPtr->guilds[i].channels[c].permission_overwrites[permC].deny & PERMISSION_READ_MESSAGES)){
										//				readDeniedForMeOnce = true;
										//			}else{
										//				readAllowedForMeOnce = true;
										//			}
										//		}
										//	}
										//
										//
										//
										//
										//
										//}
										//
										//if(readAllowedForMeOnce){
										//	discordPtr->guilds[i].channels[c].readallowed = true;
										//}else if(readDeniedForMeOnce){
										//	discordPtr->guilds[i].channels[c].readallowed = false;
										//}else{
										//	discordPtr->guilds[i].channels[c].readallowed = true;
										//}


									}else{
										// no permission_overwrites
									}



								}else{
									logSD(" channel json is null");
								}

							}

						}else{
							
							logSD(" channels! json is null");
						}
					}catch(const std::exception& e){

						logSD(" exception while loading channels");
						discordPtr->loadedChannels = true;
					}
				}

			}
			discordPtr->loadedChannels = true;
		}else if(discordPtr->loadedGuilds && discordPtr->loadedChannels && !discordPtr->loadedDMs){

			std::string directMessagesChannelsUrl = "https://discordapp.com/api/v6/users/@me/channels";
			VitaNet::http_response dmChannelsResponse = discordPtr->vitaNet.curlDiscordGet(directMessagesChannelsUrl , token);
			logSD(dmChannelsResponse.body);
			if(dmChannelsResponse.httpcode == 200){
				try{
					nlohmann::json j_complete = nlohmann::json::parse(dmChannelsResponse.body);
					if(!j_complete.is_null()){
						discordPtr->directMessages.clear();
						int dmChannels = j_complete.size();
						logSD("Amount of DM channels : " + std::to_string(dmChannels));
						for(int i = 0; i < dmChannels; i++){
							discordPtr->directMessages.push_back(directMessage());
							logSD("dm channel added.");

							if(!j_complete[i]["last_message_id"].is_null()){
								discordPtr->directMessages[i].last_message_id = j_complete[i]["last_message_id"].get<std::string>();
								logSD("last message id : ." + discordPtr->directMessages[i].last_message_id);
							}else{
								discordPtr->directMessages[i].last_message_id = "0000000000000000";
								logSD("last message id : ." + discordPtr->directMessages[i].last_message_id);
							}
							if(!j_complete[i]["type"].is_null()){
								discordPtr->directMessages[i].type = j_complete[i]["type"].get<long>();
								logSD("type : ." + std::to_string(discordPtr->directMessages[i].type));
							}else{
								discordPtr->directMessages[i].type = 1;
								logSD("type : ." + std::to_string(discordPtr->directMessages[i].type));
							}
							if(!j_complete[i]["id"].is_null()){
								discordPtr->directMessages[i].id = j_complete[i]["id"].get<std::string>();
								logSD("last id : ." + discordPtr->directMessages[i].id);
							}else{
								discordPtr->directMessages[i].id = "0000000000000000";
								logSD("last id : ." + discordPtr->directMessages[i].id);
							}
							logSD("checking for recipients");
							if(!j_complete[i]["recipients"].is_null()){
								discordPtr->directMessages[i].recipients.clear();
								int recAmount = j_complete[i]["recipients"].size();
								logSD("Amount of recipients : " + std::to_string(recAmount));
								for(int r = 0; r < recAmount  ; r++){
									// author :
									logSD("Adding recipient ");
									discordPtr->directMessages[i].recipients.push_back(user());
									if(!j_complete[i]["recipients"][r]["username"].is_null()){
										discordPtr->directMessages[i].recipients[r].username = j_complete[i]["recipients"][r]["username"].get<std::string>();
										logSD("username : " + discordPtr->directMessages[i].recipients[r].username);
									}else{
										discordPtr->directMessages[i].recipients[r].username = "";
										logSD("username : " + discordPtr->directMessages[i].recipients[r].username);
									}

									if(!j_complete[i]["recipients"][r]["discriminator"].is_null()){
										discordPtr->directMessages[i].recipients[r].discriminator = j_complete[i]["recipients"][r]["discriminator"].get<std::string>();
										logSD("discriminator : " + discordPtr->directMessages[i].recipients[r].discriminator);
									}else{
										discordPtr->directMessages[i].recipients[r].discriminator = "";
										logSD("discriminator : " + discordPtr->directMessages[i].recipients[r].discriminator);
									}

									if(!j_complete[i]["recipients"][r]["id"].is_null()){
										discordPtr->directMessages[i].recipients[r].id = j_complete[i]["recipients"][r]["id"].get<std::string>();
										logSD("id : " + discordPtr->directMessages[i].recipients[r].id);
									}else{
										discordPtr->directMessages[i].recipients[r].id = "";
										logSD("id : " + discordPtr->directMessages[i].recipients[r].id);
									}

									if(!j_complete[i]["recipients"][r]["avatar"].is_null()){
										discordPtr->directMessages[i].recipients[r].avatar = j_complete[i]["recipients"][r]["avatar"].get<std::string>();
										logSD("avatar : " + discordPtr->directMessages[i].recipients[r].avatar);
									}else{
										discordPtr->directMessages[i].recipients[r].avatar = "";
										logSD("avatar : " + discordPtr->directMessages[i].recipients[r].avatar);
									}


									logSD("end of adding recipient.");
								}
							}

							logSD("end of this DM channel.");



						}

					}

				}catch(const std::exception& e){
					logSD("/EXCEPTION THROWN!!!");
					logSD(":EXCEPTION THROWN!!!");
					logSD(":EXCEPTION THROWN!!!");
					logSD(":EXCEPTION THROWN!!!");
					logSD("\\EXCEPTION THROWN!!!");
				}

			}
			discordPtr->loadedDMs = true;
			discordPtr->loadingData = false;
		}
	}
	logSD("end of thread_loadData()");
	pthread_exit(NULL);
	return NULL;
}


void * Discord::thread_refreshMessages(void *arg){
	Discord *discordPtr = reinterpret_cast<Discord *>(arg);

	//discordPtr->getChannelMessages(discordPtr->currentChannel);
	while(discordPtr->inChannel){
		discordPtr->refreshMessages();
		sceKernelDelayThread(1000000);

	}
	pthreadStarted = false;
	pthread_exit(NULL);
	return NULL;
}

void Discord::LeaveDirectMessageChannel(){
	currentDirectMessage = 0;
	inDirectMessageChannel = false;


}
void Discord::JoinDirectMessageChannel(int dIndex){
	currentDirectMessage = dIndex;
	inDirectMessageChannel = true;
	loadingDirectMessages = true;
	getCurrentDirectMessages();
	loadingDirectMessages = false;


}

void Discord::getDirectMessageChannels(){
	std::string directMessagesChannelsUrl = "https://discordapp.com/api/v6/users/@me/channels";
	VitaNet::http_response dmChannelsResponse = vitaNet.curlDiscordGet(directMessagesChannelsUrl , token);

	if(dmChannelsResponse.httpcode == 200){
		try{
			nlohmann::json j_complete = nlohmann::json::parse(dmChannelsResponse.body);
			if(!j_complete.is_null()){
				directMessages.clear();
				int dmChannels = j_complete.size();
				for(int i = 0; i < dmChannels; i++){
					directMessages.push_back(directMessage());

					if(!j_complete[i]["last_message_id"].is_null()){
						directMessages[i].last_message_id = j_complete[i]["last_message_id"].get<std::string>();
					}else{
						directMessages[i].last_message_id = "0000000000000000";
					}
					if(!j_complete[i]["type"].is_null()){
						directMessages[i].type = j_complete[i]["type"].get<long>();
					}else{
						directMessages[i].type = 1;
					}
					if(!j_complete[i]["id"].is_null()){
						directMessages[i].id = j_complete[i]["id"].get<std::string>();
					}else{
						directMessages[i].id = "0000000000000000";
					}
					if(!j_complete[i]["recipients"].is_null()){
						directMessages[i].recipients.clear();
						int recAmount = j_complete[i]["recipients"].size();
						for(int r = 0; r < recAmount  ; r++){
							// author :
							directMessages[i].recipients.push_back(user());
							if(!j_complete[i]["recipients"][r]["username"].is_null()){
								directMessages[i].recipients[r].username = j_complete[i]["recipients"][r]["username"].get<std::string>();
							}else{
								directMessages[i].recipients[r].username = "";
							}

							if(!j_complete[i]["recipients"][r]["discriminator"].is_null()){
								directMessages[i].recipients[r].discriminator = j_complete[i]["recipients"][r]["discriminator"].get<std::string>();
							}else{
								directMessages[i].recipients[r].discriminator = "";
							}

							if(!j_complete[i]["recipients"][r]["id"].is_null()){
								directMessages[i].recipients[r].id = j_complete[i]["recipients"][r]["id"].get<std::string>();
							}else{
								directMessages[i].recipients[r].id = "";
							}

							if(!j_complete[i]["recipients"][r]["avatar"].is_null()){
								directMessages[i].recipients[r].avatar = j_complete[i]["recipients"][r]["avatar"].get<std::string>();
							}else{
								directMessages[i].recipients[r].avatar = "";
							}


						}
					}




				}

			}

		}catch(const std::exception& e){
			logSD("/EXCEPTION THROWN!!!");
			logSD(":EXCEPTION THROWN!!!");
			logSD(":EXCEPTION THROWN!!!");
			logSD(":EXCEPTION THROWN!!!");
			logSD("\\EXCEPTION THROWN!!!");
		}

	}
	lastFetchTimeMS = osGetTimeMS();

}


bool Discord::refreshDirectMessages(){

	currentTimeMS = osGetTimeMS();
	if(currentTimeMS - lastFetchTimeMS > fetchTimeMS){
		lastFetchTimeMS = osGetTimeMS();
		getDirectMessageChannels();
		return true;
	}
	return false;
}
bool Discord::refreshCurrentDirectMessages(){

	currentTimeMS = osGetTimeMS();
	if(currentTimeMS - lastFetchTimeMS > fetchTimeMS){
		lastFetchTimeMS = osGetTimeMS();
		getCurrentDirectMessages();
		return true;
	}
	return false;
}

void Discord::getCurrentDirectMessages(){
	std::string dmChannelUrl = "https://discordapp.com/api/v6/channels/" + directMessages[currentDirectMessage].id + "/messages";
	VitaNet::http_response dmChannelResponse = vitaNet.curlDiscordGet(dmChannelUrl , token);



	if(dmChannelResponse.httpcode == 200){
		try{
			nlohmann::json j_complete = nlohmann::json::parse(dmChannelResponse.body);
			if(!j_complete.is_null()){
				directMessages[currentDirectMessage].messages.clear();
				int msgAmount = j_complete.size();
				for(int i = 0; i < msgAmount; i++){

					directMessages[currentDirectMessage].messages.push_back(message());

					if(!j_complete[i]["timestamp"].is_null()){
						directMessages[currentDirectMessage].messages[i].timestamp = j_complete[i]["timestamp"].get<std::string>();
					}else{
						directMessages[currentDirectMessage].messages[i].timestamp = "";
					}

					if(!j_complete[i]["id"].is_null()){
						directMessages[currentDirectMessage].messages[i].id = j_complete[i]["id"].get<std::string>();
					}else{
						directMessages[currentDirectMessage].messages[i].id = "";
					}

					if(!j_complete[i]["content"].is_null()){
						directMessages[currentDirectMessage].messages[i].content = j_complete[i]["content"].get<std::string>();
					}else{
						directMessages[currentDirectMessage].messages[i].content = "";
					}
					// author :
					if(!j_complete[i]["author"]["username"].is_null()){
						directMessages[currentDirectMessage].messages[i].author.username = j_complete[i]["author"]["username"].get<std::string>();
					}else{
						directMessages[currentDirectMessage].messages[i].author.username = "";
					}

					if(!j_complete[i]["author"]["discriminator"].is_null()){
						directMessages[currentDirectMessage].messages[i].author.discriminator = j_complete[i]["author"]["discriminator"].get<std::string>();
					}else{
						directMessages[currentDirectMessage].messages[i].author.discriminator = "";
					}

					if(!j_complete[i]["author"]["id"].is_null()){
						directMessages[currentDirectMessage].messages[i].author.id = j_complete[i]["author"]["id"].get<std::string>();
					}else{
						directMessages[currentDirectMessage].messages[i].author.id = "";
					}

					if(!j_complete[i]["author"]["avatar"].is_null()){
						directMessages[currentDirectMessage].messages[i].author.avatar = j_complete[i]["author"]["avatar"].get<std::string>();
					}else{
						directMessages[currentDirectMessage].messages[i].author.avatar = "";
					}
				}
			}
		}catch(const std::exception& e){

		}


		std::reverse( directMessages[currentDirectMessage].messages.begin() , directMessages[currentDirectMessage].messages.end() );

	}
	lastFetchTimeMS = osGetTimeMS();
}

void Discord::loadData(){
	logSD("inside loadData()");
	loadingData = true;
	logSD("pthread_t loadDataThread");
	pthread_t loadDataThread;
	logSD("pthread_create( loadDataThread , NULL , wrapper , 0)");
	pthread_create(&loadDataThread, NULL, &Discord::loadData_wrapper, this);
	logSD("end of loadData()");

}

long Discord::fetchUserData(){

	logSD("Fetching userdata");
	std::string userDataUrl = "https://discordapp.com/api/users/@me";
	VitaNet::http_response userdataresponse = vitaNet.curlDiscordGet(userDataUrl , token);
	logSD("userdata response : " + userdataresponse.body);
	if(userdataresponse.httpcode == 200){
		// check if Two-Factor-Authentication is activated and needs further user action
		nlohmann::json j_complete = nlohmann::json::parse(userdataresponse.body);
		if(!j_complete.is_null()){
			if(!j_complete["username"].is_null()){
				username = j_complete["username"].get<std::string>();
			}
			if(!j_complete["verified"].is_null()){
				verified = j_complete["verified"].get<bool>();
			}
			if(!j_complete["mfa_enabled"].is_null()){
				mfa_enabled = j_complete["mfa_enabled"].get<bool>();
			}
			if(!j_complete["id"].is_null()){
				id = j_complete["id"].get<std::string>();
			}
			if(!j_complete["phone"].is_null()){
				phone = j_complete["phone"].get<std::string>();
			}
			if(!j_complete["avatar"].is_null()){
				avatar = j_complete["avatar"].get<std::string>();
			}
			if(!j_complete["discriminator"].is_null()){
				discriminator = j_complete["discriminator"].get<std::string>();
			}
		}


	}

	return userdataresponse.httpcode;

}

void Discord::getGuilds(){
	std::string guildUrl = "https://discordapp.com/api/users/@me/guilds";
}
void Discord::getChannels(){

}
std::string Discord::getUsername(){
	return username;
}
std::string Discord::getEmail(){
	return email;
}
std::string Discord::getPassword(){
	return password;
}
void Discord::setEmail(std::string mail){
	email = mail;
}
void Discord::setPassword(std::string pass){
	password = pass;
}
long Discord::login(){
	return login(email , password);
}
long Discord::login(std::string mail , std::string pass){
	criticalLogSD("Login attempt.\n");
	email = mail;
	password = pass;

	/*if(token.length() > 20){
		if(fetchUserData() == 200){
			loggedin = true;
			return 200;
		}else{
			token = "";
		}

	}*/

	if(email.length() < 1){
		criticalLogSD("Email to short! \n");
		return -11;
	}else if(password.length() < 1){
		criticalLogSD("Password too short \n");
		return -12;
	}

	//std::string loginUrl = "http://jaynapps.com/psvita/httpdump.php";  // DBG
	std::string loginUrl = "https://discordapp.com/api/auth/login";
	std::string postData = "{ \"email\":\"" + email + "\" , \"password\":\"" + password + "\" }";

	criticalLogSD("Login request to : \n");
	criticalLogSD(loginUrl.c_str());
	criticalLogSD("with data : \n");
	criticalLogSD((xorEncrypt(postData)).c_str());

	VitaNet::http_response loginresponse = vitaNet.curlDiscordPost(loginUrl , postData , "");
	if(loginresponse.httpcode == 200){


		criticalLogSD("Login request success : \n");
		criticalLogSD((xorEncrypt(loginresponse.body)).c_str());
		criticalLogSD("\n");

		// check if Two-Factor-Authentication is activated and needs further user action
		nlohmann::json j_complete = nlohmann::json::parse(loginresponse.body);
		try{
			if(!j_complete.is_null()){
				if(!j_complete["mfa"].is_null()){
					if(!j_complete["ticket"].is_null()){
						// Two factor auth is enabled ! grab ticket
						criticalLogSD("Account uses 2FA. Need 2FA code to proceed login. \n");
						twoFactorAuthEnabled = true;
						ticket = j_complete["ticket"].get<std::string>();
						logSD("Need 2FA Code");
						loginresponse.httpcode = 200000; // NEED MFA
					}else{
						criticalLogSD("Login failed , mfa ticket was NULL! \n");
						return -129;
					}
				}else if(!j_complete["token"].is_null()){
					// Logged in !
					criticalLogSD("Logged in without 2FA! \n");
					token = j_complete["token"].get<std::string>();
					loggedin = true;
					fetchUserData();
				}else{
					criticalLogSD("Login failed, token and mfa were NULL !\n");
					return -127;
				}

			}else{
				criticalLogSD("Login failed, JSON was null! \n");
				return -125;
			}
		}catch(const std::exception& e){
			criticalLogSD("Login failed. Couldn't parse JSON ! \n");
			return -120;
		}


	}else{
		// login failed >_>
		criticalLogSD("Login request failed because : \n");
		criticalLogSD(loginresponse.body.c_str());
		criticalLogSD("\n");

	}
	return loginresponse.httpcode;

}
long Discord::submit2facode(std::string code){
	logSD("Submit 2FA Attempt");
	code2fa = code;

	if(code2fa.length() < 3){
		criticalLogSD("2FA code too short. aborting login. \n");
		return -15;
	}

	std::string postData = "{ \"code\":\"" + code2fa + "\" , \"ticket\":\"" + ticket + "\" }";
	VitaNet::http_response submit2facoderesponse = vitaNet.curlDiscordPost( "https://discordapp.com/api/v6/auth/mfa/totp" , postData , token);
	logSD("Submit 2FA Response:");
	if(submit2facoderesponse.httpcode == 200){
		criticalLogSD("2FA request success : \n");
		criticalLogSD((xorEncrypt(submit2facoderesponse.body)).c_str());
		criticalLogSD("\n");
		try{
			nlohmann::json j_complete2 = nlohmann::json::parse(submit2facoderesponse.body);
			if(!j_complete2.is_null()){
				if(!j_complete2["token"].is_null()){
					criticalLogSD("Logged in with 2FA. \n");
					token = j_complete2["token"].get<std::string>();
					loggedin = true;
					fetchUserData();
				}else{
					criticalLogSD("2FA failed, token was empty! \n");
					return -126;
				}
			}else{
				criticalLogSD("2FA failed, JSON NULL ! ! \n");
				return -125;
			}
		}catch(const std::exception& e){
			criticalLogSD("Login failed. Couldn't parse JSON ! \n");
			return -120;
		}

	}else{
		criticalLogSD("2FA request failed because : \n");
		criticalLogSD(submit2facoderesponse.body.c_str());
		criticalLogSD("\n");

	}
	return submit2facoderesponse.httpcode;

}
std::string Discord::getToken(){
	return token;
}
std::string Discord::getTicket(){
	return ticket;
}

