#include "VitaGUI.hpp"
#include "log.hpp"
#include <istream>
#include <sstream>
#include <iterator>
#include <psp2/io/dirent.h>
#include <psp2/power.h>
#include <psp2/rtc.h>
#include <debugnet.h>


#define min(a, b) (((a) < (b)) ? (a) : (b))
#define max(a, b) (((a) > (b)) ? (a) : (b))



VitaGUI::VitaGUI(){
	vita2d_init();
	vita2d_set_clear_color(RGBA8(0x40, 0x40, 0x40, 0xFF));
	//pgf = vita2d_load_default_pgf();
	
	//pgf = vita2d_load_custom_pgf("app0:assets/font/seguiemj.pgf");
	//vita2dFontSymbola = vita2d_load_font_file("app0:assets/font/symbola.ttf");
	//vita2dFontSymbolaHint = vita2d_load_font_file("app0:assets/font/symbolahint.ttf");
	//vita2dFontSeguiemEmoji = vita2d_load_font_file("app0:assets/font/seguiemj.ttf");
	//vita2dFontLastResort = vita2d_load_font_file("app0:assets/font/lastresort.ttf");
	//vita2dFontOpenSansEmoji = vita2d_load_font_file("app0:assets/font/opensansemoji.ttf");
	
	for(int f = 0; f < MAX_FONT_SIZE ; f++){
		vita2dFont[f] = vita2d_load_font_file("app0:assets/font/whitney-book.ttf"); //droidsans.ttf
	}
	
	vita2dFontSmall = vita2d_load_font_file("app0:assets/font/whitney-book.ttf");
	vita2dFontNormal = vita2d_load_font_file("app0:assets/font/whitney-book.ttf");
	vita2dFontBig = vita2d_load_font_file("app0:assets/font/whitney-book.ttf");
	
	texA = vita2d_load_PNG_file("app0:assets/1f64b.png");
	texB = vita2d_load_PNG_file("app0:assets/1f64c.png");
	texC = vita2d_load_PNG_file("app0:assets/1f64d.png");
	
	
	std::string bgPath = "app0:assets/images/Vitacord-Background-8BIT.png";
	std::string bgPathDay = "app0:assets/images/Vitacord-Background-8BIT-day.png";
	backgroundImage = vita2d_load_PNG_file(bgPath.c_str());
	backgroundImageDay = vita2d_load_PNG_file(bgPathDay.c_str());

	//std::string avatarPath = "https://discordapp.com/avatars/" + discordPtr->client.id + "/" + discordPtr->client.avatar + ".png";
	//sideAvatar = vita2d_load_PNG_file(avatarPath.c_str());

	loginFormImage = vita2d_load_PNG_file("app0:assets/images/Vitacord-LoginForm-8BIT.png");
	loadingImage = vita2d_load_PNG_file("app0:assets/images/Vitacord-Loading-8BIT.png");
	guildsBGImage = vita2d_load_PNG_file("app0:assets/images/Vitacord-GuildsBG-8BIT.png");
	dmIconImage = vita2d_load_PNG_file("app0:assets/images/Vitacord-DMIcon-8BIT.png");
	userIconDefaultImage = vita2d_load_PNG_file("app0:assets/images/Vitacord-USERIcondefault-8BIT.png");
	guildIconDefaultImage = vita2d_load_PNG_file("app0:assets/images/Vitacord-GUILDIcondefault-8BIT.png");
	channelIconDefaultImage = vita2d_load_PNG_file("app0:assets/images/Vitacord-CHANNELIcondefault-8BIT.png");
	statbarIconImage = vita2d_load_PNG_file("app0:assets/images/Vitacord-statbar-icon.png");
	statbarBatteryImage = vita2d_load_PNG_file("app0:assets/images/Vitacord-statbar-battery.png");
	statbarBatteryChargeImage = vita2d_load_PNG_file("app0:assets/images/Vitacord-statbar-battery-charge.png");
	sidepanelStateIconImage = vita2d_load_PNG_file("app0:assets/images/Vitacord-sidebar-default-usericon.png");
	messageInputImage = vita2d_load_PNG_file("app0:assets/images/Vitacord-messager-input.png");
	defaultBinaryThumbnail = vita2d_load_PNG_file("app0:assets/images/BinaryFile.png");
	
	loginInputs.clear();
	
	inputbox emailI;
	emailI.x = 431;
	emailI.y = 139;
	emailI.w = 375;
	emailI.h = 50;
	loginInputs.push_back(emailI);
	
	inputbox passwordI;
	passwordI.x = 431;
	passwordI.y = 219;
	passwordI.w = 375;
	passwordI.h = 50;
	loginInputs.push_back(passwordI);
	
	inputbox loginI;
	loginI.x = 449;
	loginI.y = 335;
	loginI.w = 349;
	loginI.h = 58;
	loginInputs.push_back(loginI);
	
	loginTexts.clear();
	loginTexts.push_back(" ");
	loginTexts.push_back(" ");
	loginTexts.push_back(" ");
	
	
	inputboxMessageInput.x = 230;
	inputboxMessageInput.y = 473;
	inputboxMessageInput.w = 730;
	inputboxMessageInput.h = 71;
	
	// L8R
	
	std::string filename = "app0:assets/images/loading/thumb0";
	std::string completeName = "";
	for(int fraL = 0 ; fraL < FRAMES_LOADING_IMAGE ; fraL++){
		if(fraL < 9){
			completeName = filename + "0"+ std::to_string(fraL+1) + ".png";
			loadingAnim[fraL] = vita2d_load_PNG_file(completeName.c_str());
		}else{
			completeName = filename + std::to_string(fraL+1) + ".png";
			loadingAnim[fraL] = vita2d_load_PNG_file(completeName.c_str());
		}

	}
	
	
	
	loadEmojiFiles();
	
	
	
}
void VitaGUI::loadEmojiFiles(){
	// EMOJI LOADER: .. OR NOT ^^ 
	//emojis.clear();
	//emojis.push_back(emoji_icon());
}

VitaGUI::~VitaGUI(){
	vita2d_fini();
	vita2d_free_texture(backgroundImage);
	vita2d_free_texture(loginFormImage);
	vita2d_free_texture(loadingImage);
	vita2d_free_font(vita2dFontSmall);
	vita2d_free_font(vita2dFontNormal);
	vita2d_free_font(vita2dFontBig);
	//vita2d_free_pgf(pgf);
}
void VitaGUI::updateBoxes(){
	
	
}


void VitaGUI::DrawStatusBar() {
	
	vita2d_draw_rectangle(0, 0, 960, 30, RGBA8(30, 33, 36, 255));
	vita2d_draw_texture(statbarIconImage, 10, 7);
	
	// Battery
	float batteryX = (949 - vita2d_texture_get_width(statbarBatteryImage));
	float percent = scePowerGetBatteryLifePercent();
	float width = ((29.0f * percent) / 100.0f);
	
	if (scePowerIsBatteryCharging()) {
		batteryChargeCycle += 0.1;
		if (batteryChargeCycle > width)
			batteryChargeCycle = 0;
		float min = min(width, batteryChargeCycle);
		vita2d_draw_rectangle((947.0f - min), 6, min, 16, RGBA8(91, 223, 38, 255));
		vita2d_draw_texture(statbarBatteryChargeImage, batteryX, 4);
		}
	else {
		if (scePowerIsLowBattery())
			vita2d_draw_rectangle((947.0f - width), 6, width, 16, RGBA8(255, 48, 48, 255));
		else
			vita2d_draw_rectangle((947.0f - width), 6, width, 16, RGBA8(91, 223, 38, 255));
		vita2d_draw_texture(statbarBatteryImage, batteryX, 4);
		}
	
	// Date & time
	SceDateTime time;
	sceRtcGetCurrentClockLocalTime(&time);
	char dateString[16];
	char timeString[24];
	char string[64];
	sprintf(dateString, "%02d/%02d/%04d", time.day, time.month, time.year);
	sprintf(timeString, "%02d:%02d %s", (time.hour > 12) ? (time.hour - 12) : ((time.hour == 0) ? 12 : time.hour), time.minute, time.hour >= 12 ? "PM" : "AM");
	sprintf(string, "%s  %s", dateString, timeString);
	vita2d_font_draw_text(vita2dFontSmall, 875 - vita2d_font_text_width(vita2dFontSmall, 20, string), 22, RGBA8(255, 255, 255, 255), 20, string);
}

void VitaGUI::DrawUserBar(){
	//vita2d_draw_rectangle(785, 0, 175, 544, RGBA8(46, 49, 54, 255));
}

void VitaGUI::DrawServerStats(){

}

unsigned int currentEmojiIndexTest = 0;
unsigned int testFrames = 0;

void VitaGUI::Draw(){
	
	//COMMENT debugNetPrintf(DEBUG, "Draw()\n");
	
	if(state == 2){
		
		//COMMENT debugNetPrintf(DEBUG, "Call SetGuildBoxes()\n");
		setGuildBoxes();
	} else if(state == 3){
		//COMMENT debugNetPrintf(DEBUG, "Call SetChannelBoxes()\n");
		setChannelBoxes();
	}else if(state == 4){
		//COMMENT debugNetPrintf(DEBUG, "Call SetChannel+MessageBoxes()\n");
		setChannelBoxes();
		setMessageBoxes();
	}else if(state == 6){
		
		//COMMENT debugNetPrintf(DEBUG, "Call SetDMBoxes()\n");
		setDirectMessageBoxes();
	}else if(state == 7){
		
		//COMMENT debugNetPrintf(DEBUG, "Call SetDM+MsgBoxes()\n");
		setDirectMessageBoxes();
		setDirectMessageMessagesBoxes();
	}
		
		//COMMENT debugNetPrintf(DEBUG, "All boxes set!\n");
	
	
	vita2d_start_drawing();
	vita2d_clear_screen();
	
	
	if(state == 0){
		
		DrawLoginScreen();
		
		/*
		for(unsigned int drawEmojiTest = 0; drawEmojiTest <  discordPtr->emojiTestArray.size() ; drawEmojiTest++){
			
			vita2d_draw_texture_part(discordPtr->spritesheetEmoji 
				, emojiTestScrollX + discordPtr->emojiMap[discordPtr->emojiTestArray[drawEmojiTest]].x * discordPtr->emojiWidth
				, emojiTestScrollY + discordPtr->emojiMap[discordPtr->emojiTestArray[drawEmojiTest]].y * discordPtr->emojiHeight
				, discordPtr->emojiMap[discordPtr->emojiTestArray[drawEmojiTest]].x * discordPtr->emojiWidth
				, discordPtr->emojiMap[discordPtr->emojiTestArray[drawEmojiTest]].y * discordPtr->emojiHeight
				, discordPtr->emojiWidth
				, discordPtr->emojiHeight );
		}
		*/
		
		/*if(texA != NULL){
			vita2d_draw_texture( texA , 0 , 72);
		}
		if(texB != NULL){
			vita2d_draw_texture( texB , 72 , 72);
		}
		if(texC != NULL){
			vita2d_draw_texture( texC , 144 , 72);
		}*/
		
		
	}else if(state == 1){
		// BG
		
		
		

		
		framePassed++;
		if(framePassed >= 2){
			
			loadingImageFrame++;
			framePassed = 0;
		}
		
		if(loadingImageFrame >= FRAMES_LOADING_IMAGE ){
			loadingImageFrame = 0;
		}else if(loadingImageFrame < 0){
			loadingImageFrame = 0;
		}
		if(loadingAnim[loadingImageFrame] != NULL){
			vita2d_draw_rectangle(0, 0, 960, 544, RGBA8(39, 43, 47, 255));
			// frame of anim
			vita2d_draw_texture(loadingAnim[loadingImageFrame], loadingAnimX , loadingAnimY);
		
		}else{
			vita2d_draw_rectangle(0, 0, 960, 544, RGBA8(95, 118, 198, 255));
			vita2d_draw_texture_rotate(loadingImage, 480 , 247, loadingImageAngle);
			loadingImageAngle += 0.25f;
		}
		// text
		//vita2d_font_draw_text(vita2dFont[25], 300, 355, RGBA8(255,255,255,255), 25, loadingString.c_str());
		vita2d_font_draw_text(vita2dFont[25], 480 - loadingStringLength * 6, 355, RGBA8(255,255,255,255), 25, loadingString.c_str());
		vita2d_font_draw_text(vita2dFont[25], 480 - userStringLength * 15, 385, RGBA8(114, 137, 218 ,255), 25, userString.c_str());

		
	}else if(state == 2){
		
		vita2d_draw_rectangle(0, 0, 960, 544, RGBA8(54, 57, 62, 255)); // Background

		
		
		
		/// SIDEPANEL
		// BG
		vita2d_draw_rectangle(0, 30, 230, 449, RGBA8(46, 49, 54, 255));
		
		// GUILDS
		DrawGuildsOnSidebar();
		
		// TOP sidepanel to hide guilds underneath
		vita2d_draw_rectangle(0, 0, 230, 100, RGBA8(46, 49, 54, 255));
		vita2d_draw_rectangle(0, 99, 230, 1, RGBA8(5, 5, 6, 255));
		
		// BOTTOM SIDEPANEL
		vita2d_draw_rectangle(0, 479, 230, 1, RGBA8(5, 5, 6, 255));
		vita2d_draw_rectangle(0, 480, 230, 64, RGBA8(40, 43, 48, 255));

		

		vita2d_draw_texture(sidepanelStateIconImage, 14, 493); // sidepanelStateIconImage = user icon or Vitacord-default-usericon.png
		vita2d_font_draw_text(vita2dFont[24], 70, 514, RGBA8(255, 255, 255, 255), 24, panelUsername.c_str());
		vita2d_font_draw_text(vita2dFont[14], 70, 530, RGBA8(255, 255, 255, 255), 14, panelUserDiscriminator.c_str()); // create a vita2dfont for each font-size or your font will get messed up.
		
		vita2d_draw_texture(dmIconImage, 166, 41); // DM ICON 
		
		
		// maybe add something on the big right 
		
		/// STATBAR
		DrawUserBar();
		DrawStatusBar();
		
	}else if(state == 3){
		
		vita2d_draw_rectangle(0, 0, 960, 544, RGBA8(54, 57, 62, 255)); // Background
		
		
		
		/// SIDEPANEL
		// BG
		vita2d_draw_rectangle(0, 30, 230, 449, RGBA8(46, 49, 54, 255));
		
		// CHANNELS
		DrawChannelsOnSidebar();
		
		// TOP sidepanel to hide guilds underneath
		vita2d_draw_rectangle(0, 0, 230, 100, RGBA8(46, 49, 54, 255));
		vita2d_draw_rectangle(0, 99, 230, 1, RGBA8(5, 5, 6, 255));
		
		// BOTTOM SIDEPANEL
		vita2d_draw_rectangle(0, 479, 230, 1, RGBA8(5, 5, 6, 255));
		vita2d_draw_rectangle(0, 480, 230, 64, RGBA8(40, 43, 48, 255));
		vita2d_draw_texture(sidepanelStateIconImage, 14, 493); // sidepanelStateIconImage = user icon or Vitacord-default-usericon.png
		vita2d_font_draw_text(vita2dFont[24], 70, 514, RGBA8(255, 255, 255, 255), 24, panelUsername.c_str());
		vita2d_font_draw_text(vita2dFont[14], 70, 530, RGBA8(255, 255, 255, 255), 14, panelUserDiscriminator.c_str()); // create a vita2dfont for each font-size or your font will get messed up.
		
		vita2d_draw_texture(dmIconImage, 166, 41); // DM ICON  
		
		
		// maybe add something on the big right 
		
		/// STATBAR
		DrawUserBar();
		DrawStatusBar();
		
	}else if(state == 4){
		vita2d_draw_rectangle(0, 0, 960, 544, RGBA8(54, 57, 62, 255)); // Background
		
		
		/// SIDEPANEL
		// BG
		vita2d_draw_rectangle(0, 30, 230, 449, RGBA8(46, 49, 54, 255));
		
		
		
		//CHANNELS AND AFTER THAT MESSAGES
		
		DrawChannelsOnSidebar();
		
		// MESSAGES
		DrawMessages();
		
		
		// DELETEMSG
		if(clickedMessage){
			vita2d_draw_rectangle(editMessageBox.x , messageScrollY + editMessageBox.y , editMessageBox.w , editMessageBox.h , RGBA8(0 , 0 , 0 , 255));
			vita2d_font_draw_text(vita2dFont[25] , editMessageBox.x + 64 , messageScrollY + editMessageBox.y + editMessageBox.h - 8 , RGBA8(0,255,0,255), 25 , "Edit"); 
			vita2d_draw_rectangle(deleteMessageBox.x , messageScrollY + deleteMessageBox.y , deleteMessageBox.w , deleteMessageBox.h , RGBA8(0 , 0 , 0 , 255));
			vita2d_font_draw_text(vita2dFont[25] , deleteMessageBox.x + 64 , messageScrollY + deleteMessageBox.y + deleteMessageBox.h - 8, RGBA8(255,0,0,255), 25 , "Delete"); 
		}
		
		
		// TOP sidepanel to hide guilds underneath
		vita2d_draw_rectangle(0, 0, 230, 100, RGBA8(46, 49, 54, 255));
		vita2d_draw_rectangle(0, 99, 230, 1, RGBA8(5, 5, 6, 255));
		
		
		
		
		// BOTTOM SIDEPANEL
		vita2d_draw_rectangle(0, 479, 230, 1, RGBA8(5, 5, 6, 255));
		vita2d_draw_rectangle(0, 480, 230, 64, RGBA8(40, 43, 48, 255));
		vita2d_draw_texture(sidepanelStateIconImage, 14, 493); // sidepanelStateIconImage = user icon or Vitacord-default-usericon.png
		vita2d_font_draw_text(vita2dFont[24], 70, 514, RGBA8(255, 255, 255, 255), 24, panelUsername.c_str());
		vita2d_font_draw_text(vita2dFont[14], 70, 530, RGBA8(255, 255, 255, 255), 14, panelUserDiscriminator.c_str()); // create a vita2dfont for each font-size or your font will get messed up.
		
		vita2d_draw_texture(dmIconImage, 166, 41); // DM ICON 
		
		
		// maybe add something on the big right 
		
		
		
		/// STATBAR
		DrawUserBar();
		DrawStatusBar();
		
		// MESSAGEINPUT
		vita2d_draw_texture(messageInputImage, 230, 473);
		

		
	}else if(state == 6){

		vita2d_draw_rectangle(0, 0, 960, 544, RGBA8(54, 57, 62, 255)); // Background
		
		
		/// SIDEPANEL
		// BG
		vita2d_draw_rectangle(0, 30, 230, 449, RGBA8(46, 49, 54, 255));
		
		DrawDirectMessageUsersOnSidebar();
		
		
		// TOP sidepanel to hide guilds underneath
		vita2d_draw_rectangle(0, 0, 230, 100, RGBA8(46, 49, 54, 255));
		vita2d_draw_rectangle(0, 99, 230, 1, RGBA8(5, 5, 6, 255));
		
		// BOTTOM SIDEPANEL
		vita2d_draw_rectangle(0, 479, 230, 1, RGBA8(5, 5, 6, 255));
		vita2d_draw_rectangle(0, 480, 230, 64, RGBA8(40, 43, 48, 255));
		vita2d_draw_texture(sidepanelStateIconImage, 14, 493); // sidepanelStateIconImage = user icon or Vitacord-default-usericon.png
		vita2d_font_draw_text(vita2dFont[24], 70, 514, RGBA8(255, 255, 255, 255), 24, panelUsername.c_str());
		vita2d_font_draw_text(vita2dFont[14], 70, 530, RGBA8(255, 255, 255, 255), 14, panelUserDiscriminator.c_str()); // create a vita2dfont for each font-size or your font will get messed up.
		
		vita2d_draw_rectangle(146, 30, 84, 69, RGBA8(66, 70, 77, 225));
		vita2d_draw_texture(dmIconImage, 166, 41); // DM ICON 
		
		
		// maybe add something on the big right 
		
		/// STATBAR
		DrawUserBar();
		DrawStatusBar();
		
	}else if(state == 7){
		vita2d_draw_rectangle(0, 0, 960, 544, RGBA8(54, 57, 62, 255)); // Background
			
			
		/// SIDEPANEL
		// BG
		vita2d_draw_rectangle(0, 30, 230, 449, RGBA8(46, 49, 54, 255));
		
		DrawDirectMessageUsersOnSidebar();
		
		DrawDirectMessageMessages();
		
		// TOP sidepanel to hide guilds underneath
		vita2d_draw_rectangle(0, 0, 230, 100, RGBA8(46, 49, 54, 255));
		vita2d_draw_rectangle(0, 99, 230, 1, RGBA8(5, 5, 6, 255));
		
		// BOTTOM SIDEPANEL
		vita2d_draw_rectangle(0, 479, 230, 1, RGBA8(5, 5, 6, 255));
		vita2d_draw_rectangle(0, 480, 230, 64, RGBA8(40, 43, 48, 255));
		vita2d_draw_texture(sidepanelStateIconImage, 14, 493); // sidepanelStateIconImage = user icon or Vitacord-default-usericon.png
		vita2d_font_draw_text(vita2dFont[24], 70, 514, RGBA8(255, 255, 255, 255), 24, panelUsername.c_str());
		vita2d_font_draw_text(vita2dFont[14], 70, 530, RGBA8(255, 255, 255, 255), 14, panelUserDiscriminator.c_str()); // create a vita2dfont for each font-size or your font will get messed up.
		
		vita2d_draw_rectangle(146, 30, 84, 69, RGBA8(66, 70, 77, 225));
		vita2d_draw_texture(dmIconImage, 166, 41); // DM ICON 
		
		
		// maybe add something on the big right 
		
		/// STATBAR
		DrawUserBar();
		DrawStatusBar();
		
		
		// MESSAGEINPUT
		vita2d_draw_texture(messageInputImage, 230, 473);
		
	}else if(state == 9){
		vita2d_draw_rectangle(0, 0, 960, 544, RGBA8(114, 137, 217, 255));
		unsigned int i = 0;
		for( i = 0; i < rectangles.size() ; i++){
			vita2d_draw_rectangle(rectangles[i].x, rectangles[i].y, rectangles[i].w, rectangles[i].h, rectangles[i].color);
		}
	}

	
	vita2d_end_drawing();
	vita2d_swap_buffers();
	vita2d_wait_rendering_done();
	
}

int VitaGUI::scroll(int x , int y , int posx , int posy){
	if(state == 2){
		if(posx < 230 && posx > 0 && posy < 522 && posy > 22){
			guildScrollX = 0;
			guildScrollY += y;
			if(guildScrollY < guildScrollYMin)
				guildScrollY = guildScrollYMin;
			else if(guildScrollY > guildScrollYMax )
				guildScrollY = guildScrollYMax;
			
		}
		

		
		return 0;
	}else if(state == 3){
		if(posx < 230 && posx > 0 && posy < 522 && posy > 22){
			channelScrollX = 0;
			channelScrollY += y;

			
			if(channelScrollY < channelScrollYMin)
				channelScrollY = channelScrollYMin;
			else if(channelScrollY > channelScrollYMax )
				channelScrollY = channelScrollYMax;
		}
		

		
		return 0;
	}else if(state==4){
		if(posx < 230 && posx > 0 && posy < 522 && posy > 22){
			channelScrollX = 0;
			channelScrollY += y;
			
			
			if(channelScrollY < channelScrollYMin)
				channelScrollY = channelScrollYMin;
			else if(channelScrollY > channelScrollYMax )
				channelScrollY = channelScrollYMax;
			


		}else if (posx > 230){
			messageScrollX = 0;
			messageScrollY += y;

			
			if(messageScrollY < messageScrollYMin)
				messageScrollY = messageScrollYMin;
			else if(messageScrollY > messageScrollYMax )
				messageScrollY = messageScrollYMax;
		}
		

		return 0;
	}else if(state==6){
		if(posx < 230 && posx > 0 && posy < 522 && posy > 22){
			directMessageScrollX = 0;
			directMessageScrollY += y;
			
			if(directMessageScrollY < directMessageScrollYMin)
				directMessageScrollY = directMessageScrollYMin;
			else if(directMessageScrollY > directMessageScrollYMax )
				directMessageScrollY = directMessageScrollYMax;
		}

		return 0;
	}else if(state==7){
		
		if(posx < 230 && posx > 0 && posy < 522 && posy > 22){
			directMessageScrollX = 0;
			directMessageScrollY += y;
			
			if(directMessageScrollY < directMessageScrollYMin)
				directMessageScrollY = directMessageScrollYMin;
			else if(directMessageScrollY > directMessageScrollYMax )
				directMessageScrollY = directMessageScrollYMax;
		}
		if(posx> 230){
			directMessageMessagesScrollX = 0;
			directMessageMessagesScrollY += y;
			
			
			if(directMessageMessagesScrollY < directMessageMessagesScrollYMin)
				directMessageMessagesScrollY = directMessageMessagesScrollYMin;
			else if(directMessageMessagesScrollY > directMessageMessagesScrollYMax )
				directMessageMessagesScrollY = directMessageMessagesScrollYMax;
		}
		
		return 0;
	}
	return -1;
}


int VitaGUI::analogScrollRight(int x , int y){
	
	
	if(state == 4){
		messageScrollX = 0;
		messageScrollY += y;

		
		if(messageScrollY < messageScrollYMin)
			messageScrollY = messageScrollYMin;
		else if(messageScrollY > messageScrollYMax )
			messageScrollY = messageScrollYMax;
		
	}else if(state == 7){
		directMessageMessagesScrollX = 0;
		directMessageMessagesScrollY += y;
		
		
		if(directMessageMessagesScrollY < directMessageMessagesScrollYMin)
			directMessageMessagesScrollY = directMessageMessagesScrollYMin;
		else if(directMessageMessagesScrollY > directMessageMessagesScrollYMax )
			directMessageMessagesScrollY = directMessageMessagesScrollYMax;
	
	}
	return 1;
	
}

int VitaGUI::analogScrollLeft(int x , int y){
	
	
	if(state==0){
		emojiTestScrollX += x;
		emojiTestScrollY -= y;
	}else if(state == 1){
		loadingAnimX += x;
		loadingAnimY -= y;
	}
	else if (state==2){
		guildScrollX = 0;
		guildScrollY += y;
		if(guildScrollY < guildScrollYMin)
			guildScrollY = guildScrollYMin;
		else if(guildScrollY > guildScrollYMax )
			guildScrollY = guildScrollYMax;
		
	}else if(state == 3){
		
		channelScrollX = 0;
		channelScrollY += y;

		
		if(channelScrollY < channelScrollYMin)
			channelScrollY = channelScrollYMin;
		else if(channelScrollY > channelScrollYMax )
			channelScrollY = channelScrollYMax;
	}else if(state == 4){
		channelScrollX = 0;
		channelScrollY += y;
		
		
		if(channelScrollY < channelScrollYMin)
			channelScrollY = channelScrollYMin;
		else if(channelScrollY > channelScrollYMax )
			channelScrollY = channelScrollYMax;
	}else if(state== 6){
		
		directMessageScrollX = 0;
		directMessageScrollY += y;
		
		if(directMessageScrollY < directMessageScrollYMin)
			directMessageScrollY = directMessageScrollYMin;
		else if(directMessageScrollY > directMessageScrollYMax )
			directMessageScrollY = directMessageScrollYMax;
		
	}else if(state == 7){
		directMessageScrollX = 0;
		directMessageScrollY += y;
		
		if(directMessageScrollY < directMessageScrollYMin)
			directMessageScrollY = directMessageScrollYMin;
		else if(directMessageScrollY > directMessageScrollYMax )
			directMessageScrollY = directMessageScrollYMax;
	}
	return 1;
	
}


int VitaGUI::click(int x , int y){
	if(state == 0){
		currentEmojiIndexTest+=13;
		for(unsigned int i = 0 ; i < loginInputs.size() ; i++){
			if( x > loginInputs[i].x && x < loginInputs[i].x + loginInputs[i].w){
				if( y > loginInputs[i].y && y < loginInputs[i].y + loginInputs[i].h){
					return i;
				}
			}
		}
	}else if(state == 2){
		
		if(x > DMICONX && x < DMICONX2 && y > DMICONY && y < DMICONY2){
			return CLICKED_DM_ICON;
		}
		if( y < 515  &&  y > 99){
			for(unsigned int i = 0 ; i < guildBoxes.size() ; i++){
				if( x  > guildBoxes[i].x && x  < guildBoxes[i].x + guildBoxes[i].w){
					if( y > guildBoxes[i].y && y  < guildBoxes[i].y + guildBoxes[i].h){
						return guildBoxes[i].guildIndex;
					}
				}
			}
		}
		
	}else if(state == 3){
		
		if(x > DMICONX && x < DMICONX2 && y > DMICONY && y < DMICONY2){
			return CLICKED_DM_ICON;
		}
		
		if( y < 515  &&  y > 99){
			for(unsigned int i = 0 ; i < channelBoxes.size() ; i++){
				if( x  > channelBoxes[i].x && x  < channelBoxes[i].x + channelBoxes[i].w){
					if( y  > channelBoxes[i].y && y  < channelBoxes[i].y + channelBoxes[i].h){
						return channelBoxes[i].channelIndex;
					}
				}
			}
		}
	}else if(state == 4){
		
		
		if(x > DMICONX && x < DMICONX2 && y > DMICONY && y < DMICONY2){
			return CLICKED_DM_ICON;
		}
		
		if( x > inputboxMessageInput.x && y < inputboxMessageInput.x + inputboxMessageInput.w){
			if( y > inputboxMessageInput.y && y < inputboxMessageInput.y + inputboxMessageInput.h){
				return CLICKED_MESSAGE_INPUT;
			}
		}
		
		if( y < 515  &&  y > 99 && x < 230){
			for(unsigned int i = 0 ; i < channelBoxes.size() ; i++){
				if( x  > channelBoxes[i].x && x  < channelBoxes[i].x + channelBoxes[i].w){
					if( y  > channelBoxes[i].y && y  < channelBoxes[i].y + channelBoxes[i].h){
						return channelBoxes[i].channelIndex;
					}
				}
			}
		}
		
		if(clickedMessage){
			if( x > deleteMessageBox.x && x < deleteMessageBox.x + deleteMessageBox.w){
				if( y > messageScrollY + deleteMessageBox.y && y < messageScrollY + deleteMessageBox.y + deleteMessageBox.h){
					debugNetPrintf(DEBUG,"DELETING MESSAGE!\n");
					discordPtr->deleteMessage(deleteMessageBox.channelID , deleteMessageBox.messageID);
					debugNetPrintf(DEBUG,"DELETED MESSAGE!!!\n");
					clickedMessage = false;
					
					return -1;
				}
				
			}
			if( x > editMessageBox.x && x < editMessageBox.x + editMessageBox.w){
				if( y > messageScrollY + editMessageBox.y && y < messageScrollY + editMessageBox.y + editMessageBox.h){
					debugNetPrintf(DEBUG,"DELETING MESSAGE!\n");
					std::string newContent = vitaIMEPtr->getUserText((char*)"Edit message" , (char*) editMessageBox.content.c_str());
					discordPtr->editMessage(editMessageBox.channelID , editMessageBox.messageID , newContent);
					debugNetPrintf(DEBUG,"DELETED MESSAGE!!!\n");
					clickedMessage = false;
					
					return -1;
				}
				
			}
			
		}

		
		if( y < 515  &&  y > 30 && x > 230){
			for(unsigned int i = 0 ; i < messageBoxes.size() ; i++){
				if( messageScrollX + x  > messageBoxes[i].x && messageScrollX + x  < messageBoxes[i].x + messageBoxes[i].w){
					if( (-messageScrollY) + y  > messageBoxes[i].y && (-messageScrollY) + y  < messageBoxes[i].y + messageBoxes[i].h){
						
						if( clickedMessage ){
							debugNetPrintf(DEBUG , "un-clicked message\n");
							clickedMessage = false;
						}else{
							debugNetPrintf(DEBUG , "clicked message : \n");
							debugNetPrintf(DEBUG , messageBoxes[i].content.c_str());
							debugNetPrintf(DEBUG , " \n");
							clickedMessage = true;
							editMessageBox.w = 200;
							editMessageBox.h = 64;
							deleteMessageBox.w = 200;
							deleteMessageBox.h = 64;
							
							editMessageBox.y = messageBoxes[i].y + - 64;
							
							deleteMessageBox.y = messageBoxes[i].y + 8;//static_cast<int>( messageBoxes[i].y + (messageBoxes[i].h / 2 ) ); // "updating" height in draw call so => - messageScrollY
							if(x > 704){
								editMessageBox.x = 704;
								deleteMessageBox.x = 704;
							}else{
								editMessageBox.x = x;
								deleteMessageBox.x = x;
							}
							editMessageBox.messageID = messageBoxes[i].messageID;
							editMessageBox.channelID = messageBoxes[i].channelID;
							editMessageBox.content = messageBoxes[i].content;
							deleteMessageBox.messageID = messageBoxes[i].messageID;
							deleteMessageBox.channelID = messageBoxes[i].channelID;
						}
						
						return -1;
					}
				}
			}
		}
		
	}else if(state == 6){
		
		if(x > DMICONX && x < DMICONX2 && y > DMICONY && y < DMICONY2){
			return CLICKED_DM_ICON;
		}
		
		if( y < 515  &&  y > 99){
			for(unsigned int i = 0 ; i < directMessageBoxes.size() ; i++){
				if( x  > directMessageBoxes[i].x && x  < directMessageBoxes[i].x + directMessageBoxes[i].w){
					if( y  > directMessageBoxes[i].y && y  < directMessageBoxes[i].y + directMessageBoxes[i].h){
						return directMessageBoxes[i].dmIndex;
					}
				}
			}
		}
	}else if(state == 7){
		
		if(x > DMICONX && x < DMICONX2 && y > DMICONY && y < DMICONY2){
			return CLICKED_DM_ICON;
		}
		
		
		if( x > inputboxMessageInput.x && y < inputboxMessageInput.x + inputboxMessageInput.w){
			if( y > inputboxMessageInput.y && y < inputboxMessageInput.y + inputboxMessageInput.h){
				return CLICKED_MESSAGE_INPUT;
			}
		}
		
		if( y < 515  &&  y > 99){
			for(unsigned int i = 0 ; i < directMessageBoxes.size() ; i++){
				if( x  > directMessageBoxes[i].x && x  < directMessageBoxes[i].x + directMessageBoxes[i].w){
					if( y  > directMessageBoxes[i].y && y  < directMessageBoxes[i].y + directMessageBoxes[i].h){
						return directMessageBoxes[i].dmIndex;
					}
				}
			}
		}
		

		
		// ? messages
	}
	return -1;
}

void VitaGUI::AddRectangle(float nx , float ny , float nw , float nh , unsigned int ncolor){
	rectangle r;
	r.x = nx;
	r.y = ny;
	r.w = nw;
	r.h = nh;
	r.color = ncolor;
	rectangles.push_back(r);
}
void VitaGUI::RemoveRectangle(unsigned int index){
	
	if(index < rectangles.size()){
		rectangles.erase(rectangles.begin()+index);
	}
	
}

int VitaGUI::GetState(){
	return state;
}
void VitaGUI::SetState(int s){
	lastState = state;
	state = s;
	
	if(state == 4){
		messageScrollY = 0;
		directMessageMessagesScrollY = 0;
		directMessageScrollY = 0;
		messageScrollSet = false;
		setMessageBoxes();
	}else if(state == 7 && lastState == 6){
		messageScrollY = 0;
		directMessageMessagesScrollY = 0;
		setDirectMessageMessagesBoxes();
		directMessageMessagesScrollSet = false;
		
	}else if(lastState == 7 && state == 7){
		
		directMessageMessagesScrollY = 0;
		setDirectMessageMessagesBoxes();
		directMessageMessagesScrollSet = false;
		
		
	}else{
		messageScrollY = 0;
		channelScrollY = 0;
		directMessageMessagesScrollY = 0;
		directMessageScrollY = 0;
	}
	

	
}
void VitaGUI::SetStateToLastState(){
	state = lastState;
}

void VitaGUI::passVITAIMEPointer( VitaIME * vmeptr ){
	vitaIMEPtr = vmeptr;
}

void VitaGUI::passDiscordPointer(Discord *ptr){
	discordPtr = ptr;
}
void VitaGUI::setGuildBoxes(){
	guildBoxes.clear();
	for(unsigned int i = 0; i < discordPtr->guilds.size() ; i++){
		guildBox boxG;
		boxG.x = guildScrollX ;
		boxG.y = 100 + guildScrollY + i * GUILD_HEIGHT;
		boxG.w = 130;
		boxG.h = GUILD_HEIGHT;
		boxG.guildIndex = i;
		boxG.name = discordPtr->guilds[i].name;
		guildBoxes.push_back(boxG);
	}
	guildScrollYMin = -((guildBoxes.size())*GUILD_HEIGHT - 100);
	guildScrollYMax = 0;
}
void VitaGUI::setChannelBoxes(){
	channelBoxes.clear();
	int amount = 0;
	for(unsigned int i = 0; i < discordPtr->guilds[discordPtr->currentGuild].channels.size() ; i++){
		if(discordPtr->guilds[discordPtr->currentGuild].channels[i].type == 0 && discordPtr->guilds[discordPtr->currentGuild].channels[i].readallowed){
			channelBox boxC;
			boxC.x = channelScrollX ;
			boxC.y = 100 + channelScrollY + amount * CHANNEL_HEIGHT;
			boxC.w = 130;
			boxC.h = CHANNEL_HEIGHT;
			boxC.channelIndex = i;
			boxC.name = discordPtr->guilds[discordPtr->currentGuild].channels[i].name;
			channelBoxes.push_back(boxC);
			amount++;
		}
	}

	//need to add emoji support to channel names

	channelScrollYMin = -((amount)*CHANNEL_HEIGHT - 100) ;
	channelScrollYMax = 0;
}
bool VitaGUI::setMessageBoxes(){
		
	int topMargin = 45;
	int bottomMargin = 18;
	int textHeight = 0;
	int allHeight = 0;
	if(!discordPtr->refreshingMessages && discordPtr->refreshedMessages){
		discordPtr->refreshedMessages = false;
		messageBoxes.clear();
		for(unsigned int i = 0; i < discordPtr->guilds[discordPtr->currentGuild].channels[discordPtr->currentChannel].messages.size() ; i++){
			
			messagebox boxC;
			
			boxC.messageID = discordPtr->guilds[discordPtr->currentGuild].channels[discordPtr->currentChannel].messages[i].id;
			boxC.x = messageScrollX + 230;
			boxC.y = 40  + allHeight ; // 40 = statusbar height
			boxC.username = discordPtr->guilds[discordPtr->currentGuild].channels[discordPtr->currentChannel].messages[i].author.username;
			boxC.content = discordPtr->guilds[discordPtr->currentGuild].channels[discordPtr->currentChannel].messages[i].content;
			//boxC.lineCount = wordWrap( discordPtr->guilds[discordPtr->currentGuild].channels[discordPtr->currentChannel].messages[i].content , 30 , boxC.content);
			// wrapping in discord.cpp bcz of emoji :
			// which is more expensive on the cpu ? searching the whole string for newlines when wordwrapping or text_Height() ?
			textHeight = vita2d_font_text_height(vita2dFont[32] , 32 , (char*)boxC.content.c_str() );
			// why not just use text_height() on the content?? :) 
			//textHeight = boxC.lineCount * vita2d_font_text_height(vita2dFont[15], 15, (char*)"H");
			boxC.messageHeight = max(64, textHeight + topMargin + bottomMargin);
			
			
			boxC.w = 730;
			boxC.h = boxC.messageHeight;
			
			boxC.channelID = discordPtr->guilds[discordPtr->currentGuild].channels[discordPtr->currentChannel].id;
			boxC.messageID = discordPtr->guilds[discordPtr->currentGuild].channels[discordPtr->currentChannel].messages[i].id;
			
			// ATTACHMENTS
			if( ! discordPtr->guilds[discordPtr->currentGuild].channels[discordPtr->currentChannel].messages[i].attachment.isEmpty ){
				if(discordPtr->guilds[discordPtr->currentGuild].channels[discordPtr->currentChannel].messages[i].attachment.isData ){
					boxC.showAttachmentAsBinary = true;
					boxC.showAttachmentAsImage = false;
					//boxC.attachmentThumbnail =  defaultBinaryThumbnail;// default thumbnail ( txt or binary indicator )  . ACTUALLY not gonna do this for now
					boxC.attachmentReadableSize = discordPtr->guilds[discordPtr->currentGuild].channels[discordPtr->currentChannel].messages[i].attachment.readableSize;
					boxC.attachmentReadableSizeUnit = discordPtr->guilds[discordPtr->currentGuild].channels[discordPtr->currentChannel].messages[i].attachment.readableSizeUnit;
					boxC.attachmentFilename = discordPtr->guilds[discordPtr->currentGuild].channels[discordPtr->currentChannel].messages[i].attachment.filename;
					boxC.attachmentFullText = std::to_string(  boxC.attachmentReadableSize ) + " " +  boxC.attachmentReadableSizeUnit + " " + boxC.attachmentFilename;
					
					// adjust box height !
					boxC.messageHeight += ATTACHMENT_HEIGHT + 16; // 16 = margin
				}else if ( discordPtr->guilds[discordPtr->currentGuild].channels[discordPtr->currentChannel].messages[i].attachment.isImage ){
					if( discordPtr->guilds[discordPtr->currentGuild].channels[discordPtr->currentChannel].messages[i].attachment.loadedThumbImage ){
						boxC.showAttachmentAsImage = true;
						boxC.showAttachmentAsBinary = false;
						//boxC.attachmentThumbnail = discordPtr->guilds[discordPtr->currentGuild].channels[discordPtr->currentChannel].messages[i].attachment.thumbnail;
						boxC.attachmentReadableSize = discordPtr->guilds[discordPtr->currentGuild].channels[discordPtr->currentChannel].messages[i].attachment.readableSize;
						boxC.attachmentReadableSizeUnit = discordPtr->guilds[discordPtr->currentGuild].channels[discordPtr->currentChannel].messages[i].attachment.readableSizeUnit;
						boxC.attachmentFilename = discordPtr->guilds[discordPtr->currentGuild].channels[discordPtr->currentChannel].messages[i].attachment.filename;
						boxC.attachmentFullText = std::to_string(  boxC.attachmentReadableSize ) + " " +  boxC.attachmentReadableSizeUnit + " " + boxC.attachmentFilename;
					
						// adjust box height !
						boxC.messageHeight += ATTACHMENT_HEIGHT + 16;
					}
				}else{
					boxC.showAttachmentAsImage = false;
					boxC.showAttachmentAsBinary = false;
				}
			}else{
				boxC.showAttachmentAsImage = false;
				boxC.showAttachmentAsBinary = false;
			}
			
			
			// EMOJIS:
			boxC.emojis.clear();
			for(unsigned int e = 0; e < discordPtr->guilds[discordPtr->currentGuild].channels[discordPtr->currentChannel].messages[i].emojis.size() ; e++){
				boxC.emojis.push_back(m_emoji());
				boxC.emojis[e].posX = discordPtr->guilds[discordPtr->currentGuild].channels[discordPtr->currentChannel].messages[i].emojis[e].posX;
				boxC.emojis[e].posY = discordPtr->guilds[discordPtr->currentGuild].channels[discordPtr->currentChannel].messages[i].emojis[e].posY;
				boxC.emojis[e].spriteSheetX = discordPtr->guilds[discordPtr->currentGuild].channels[discordPtr->currentChannel].messages[i].emojis[e].spriteSheetX;
				boxC.emojis[e].spriteSheetY = discordPtr->guilds[discordPtr->currentGuild].channels[discordPtr->currentChannel].messages[i].emojis[e].spriteSheetY;
			}
			
			
			allHeight += boxC.messageHeight;
			messageBoxes.push_back(boxC);
		}
		messageScrollYMin =  -( allHeight )  + 430; 
		messageScrollYMax = 0;
		
		if( !messageScrollSet ){
			messageScrollSet = true;
			messageScrollY = messageScrollYMin;
		}
		
		return true;
	}
	return false;
}


int VitaGUI::wordWrap(std::string str, unsigned int maxCharacters, std::string &out) {
	if(str.length() < maxCharacters ) {
		out = str;
		return 1; 
		
	}
	out = "";
	int breaks = str.length() / maxCharacters;
	for(int i = 0 ; i < breaks+1; i++){
		out += str.substr(i*maxCharacters, maxCharacters) + '\n';
		
	}
	
	return breaks;
}



void VitaGUI::setDirectMessageBoxes(){
	directMessageBoxes.clear();
	for(unsigned int i = 0; i < discordPtr->directMessages.size() ; i++){
		dmBox boxDM;
		boxDM.x = directMessageScrollX ;
		boxDM.y = 100 + directMessageScrollY + i * GUILD_HEIGHT;
		boxDM.w = 230;
		boxDM.h = GUILD_HEIGHT;
		boxDM.name = discordPtr->directMessages[i].recipients[0].username;
		boxDM.dmIndex = i;
		directMessageBoxes.push_back(boxDM);
	}
	directMessageMessagesScrollYMin = -((directMessageBoxes.size())*CHANNEL_HEIGHT - 100) ;
	directMessageMessagesScrollYMax = 0;
	//directMessageScrollYMin = -((discordPtr->directMessages.size()-1)*GUILD_HEIGHT -300) ;
	
}

void VitaGUI::setDirectMessageMessagesBoxes(){
	
	int topMargin = 45;
	int bottomMargin = 18;
	int textHeight = 0;
	int allHeight = 0;
	// TODO CHECK FOR REFRESHED AND REFRESHING DMSGS + PTHREAD THE refreshDm() in Discord.cpp
	//if(!discordPtr->refreshingMessages && discordPtr->refreshedMessages){
		//discordPtr->refreshedMessages = false;
		directMessageMessagesBoxes.clear();
		for(unsigned int i = 0; i < discordPtr->directMessages[discordPtr->currentDirectMessage].messages.size() ; i++){
			messagebox boxC;
			boxC.x = directMessageMessagesScrollX + 230;
			boxC.y = directMessageMessagesScrollY + allHeight;
			boxC.username = discordPtr->directMessages[discordPtr->currentDirectMessage].messages[i].author.username;
			boxC.content = "";
			boxC.lineCount = wordWrap( discordPtr->directMessages[discordPtr->currentDirectMessage].messages[i].content , 30 , boxC.content);
			textHeight = boxC.lineCount * vita2d_font_text_height(vita2dFont[32], 32, (char*)"H");
			boxC.messageHeight = max(64, textHeight + topMargin + bottomMargin);
			allHeight += boxC.messageHeight;
			
			
			
			boxC.w = 730;
			boxC.h = boxC.messageHeight;
			
			
			directMessageMessagesBoxes.push_back(boxC);
		}
		directMessageMessagesScrollYMin =  -( allHeight - 100  ); //-( allHeight )
		directMessageMessagesScrollYMax =   0; 
		
		
		if(!directMessageMessagesScrollSet){
			directMessageMessagesScrollSet = true;
			directMessageMessagesScrollY = directMessageMessagesScrollYMin;
		}
		

	//}
	

	
}


void VitaGUI::setUserInfo(){
	
	panelUsername = discordPtr->username;
	panelUserDiscriminator = "#" + discordPtr->discriminator;
}


void VitaGUI::showLoginCue(){
	vita2d_start_drawing();
	vita2d_clear_screen();
	
	DrawLoginScreen();
	
	vita2d_draw_rectangle(455 , 335, 330 , 58 , RGBA8(0xFF , 0xFF , 0xFF , 0x24));
	
	vita2d_end_drawing();
	vita2d_swap_buffers();
	vita2d_wait_rendering_done();
}

void VitaGUI::unshowLoginCue(){
	
	vita2d_start_drawing();
	vita2d_clear_screen();
	
	DrawLoginScreen();
	
	vita2d_end_drawing();
	vita2d_swap_buffers();
	vita2d_wait_rendering_done();
}

void VitaGUI::DrawLoginScreen(){
	
	
		//vita2d_draw_rectangle(0, 0, 960, 544, RGBA8(114, 137, 217, 255));

		SceDateTime time;
		sceRtcGetCurrentClockLocalTime(&time);
		if (time.hour >= 18){
			vita2d_draw_texture( backgroundImage, 0, 0);
		}
		else{
			vita2d_draw_texture( backgroundImageDay, 0, 0);
		}
		vita2d_draw_texture( loginFormImage , 161, 53 );
		vita2d_font_draw_text(vita2dFont[22] , 438, 179, RGBA8(255,255,255,255), 22, loginTexts[0].c_str());
		vita2d_font_draw_text(vita2dFont[22] , 438, 259, RGBA8(255,255,255,255), 22, loginTexts[1].c_str());
		vita2d_font_draw_text(vita2dFont[22] , 750, 259, RGBA8(255,0,0,255), 22, loginTexts[2].c_str());
	
}

void VitaGUI::DrawGuildsOnSidebar(){
	int height = 0;
	for(unsigned int i = 0 ; i < guildBoxes.size() ; i++){
		height = guildScrollY + i * GUILD_HEIGHT;
		if(height < MAX_DRAW_HEIGHT && height  > MIN_DRAW_HEIGHT){
			vita2d_font_draw_text(vita2dFont[GUILD_TITLE_TEXT_SIZE_PIXEL] , guildScrollX + 58, 100 + guildScrollY + i * GUILD_HEIGHT + 40, RGBA8(255,255,255,255), GUILD_TITLE_TEXT_SIZE_PIXEL, guildBoxes[i].name.c_str());
		}
		if (guildIconDefaultImage != NULL){
			if (discordPtr->serverIcons[i] != NULL){
				vita2d_draw_texture(discordPtr->serverIcons[i], 6, 68 + guildScrollY + i * GUILD_HEIGHT + 40);
			}
			vita2d_draw_texture(guildIconDefaultImage, 6, 68 + guildScrollY + i * GUILD_HEIGHT + 40);
		}
	}
}

void VitaGUI::DrawChannelsOnSidebar(){
	int channelsAmount = static_cast<int>(channelBoxes.size());
	for(int i = 0 ; i < channelsAmount ; i++){
		if(channelScrollY + i * CHANNEL_HEIGHT < MAX_DRAW_HEIGHT && channelScrollY + i * CHANNEL_HEIGHT > MIN_DRAW_HEIGHT){
			if(discordPtr->currentChannel == channelBoxes[i].channelIndex && discordPtr->inChannel){
				vita2d_draw_rectangle(channelScrollX , 100 + channelScrollY + i * CHANNEL_HEIGHT, 215 , CHANNEL_HEIGHT, RGBA8(40, 43, 48, 255));
				vita2d_draw_rectangle(channelScrollX , 100 + channelScrollY + i * CHANNEL_HEIGHT, 4 , CHANNEL_HEIGHT, RGBA8(95, 118, 198, 255));
			}
			vita2d_font_draw_text(vita2dFont[CHANNEL_TITLE_TEXT_SIZE_PIXEL] , channelScrollX + 38, 100 + channelScrollY + i * CHANNEL_HEIGHT + 42  , RGBA8(255,255,255,255), CHANNEL_TITLE_TEXT_SIZE_PIXEL, channelBoxes[i].name.c_str());
		}
		if (channelIconDefaultImage != NULL){
			vita2d_draw_texture(channelIconDefaultImage, 6, 78 + channelScrollY + i * CHANNEL_HEIGHT + 42);
		}
	}
	
}

typedef struct RGB {
    double r;
    double g;
    double b;
} RGB1;

struct RGB colorConverter(int hexValue){
  struct RGB rgbColor;
  rgbColor.r = ((hexValue >> 16) & 0xFF) / 255.0;  // Extract the RR byte
  rgbColor.g = ((hexValue >> 8) & 0xFF) / 255.0;   // Extract the GG byte
  rgbColor.b = ((hexValue) & 0xFF) / 255.0;        // Extract the BB byte

  return rgbColor; 
}

void VitaGUI::DrawMessages(){
	
	int yPos = messageScrollY + 40 , height;
	unsigned int messageBoxesAmount = messageBoxes.size();
	
	for(unsigned int i =  0 ; i < messageBoxesAmount ; i++){
		
		height = messageBoxes[i].messageHeight;
		
		if(yPos < MAX_DRAW_HEIGHT && yPos > MIN_DRAW_HEIGHT){
			
			
			if(i == messageBoxesAmount-1){
				
				vita2d_draw_rectangle(242, yPos + height - 1, 706, 1, RGBA8(120, 115, 120, 255)); 
				vita2d_draw_rectangle(240, yPos + height , 710, 1, RGBA8(100, 100, 100, 255)); 
				vita2d_draw_rectangle(242, yPos + height + 1, 706, 1, RGBA8(120, 115, 120, 255)); 
			}else{
				vita2d_draw_rectangle(242, yPos + height - 1, 706, 1, RGBA8(62, 65, 70, 255));
				vita2d_draw_rectangle(240, yPos + height, 710, 1, RGBA8(51, 53, 55, 255)); 
				vita2d_draw_rectangle(242, yPos + height + 1, 706, 1, RGBA8(62, 65, 70, 255)); 
				
			}
				int rVal = 255;
				int gVal = 255;
				int bVal = 255;
				vita2d_font_draw_text(vita2dFont[22], 300, yPos + 26, RGBA8(rVal, gVal, bVal, 255), 22, messageBoxes[i].username.c_str());
				vita2d_font_draw_text(vita2dFont[22], 300, yPos + 55, RGBA8(153, 170, 181, 255), 22, messageBoxes[i].content.c_str());
				 
			if( messageBoxes[i].showAttachmentAsImage ){
				vita2d_draw_rectangle( 240 , yPos + height - ATTACHMENT_HEIGHT - 16 , ATTACHMENT_HEIGHT , ATTACHMENT_HEIGHT , RGBA8(0x9F , 0x9F , 0x9F , 0xFF) );
				if( i < discordPtr->guilds[discordPtr->currentGuild].channels[discordPtr->currentChannel].messages.size() ){
					if ( discordPtr->guilds[discordPtr->currentGuild].channels[discordPtr->currentChannel].messages[i].attachment.loadedThumbImage == true ){
						if ( discordPtr->guilds[discordPtr->currentGuild].channels[discordPtr->currentChannel].messages[i].attachment.isEmpty == false ){
							if ( discordPtr->guilds[discordPtr->currentGuild].channels[discordPtr->currentChannel].messages[i].attachment.thumbnail != NULL ){

								vita2d_draw_texture( discordPtr->guilds[discordPtr->currentGuild].channels[discordPtr->currentChannel].messages[i].attachment.thumbnail , 240 , yPos + height - ATTACHMENT_HEIGHT - 16);
							}
						}
					}
				}
				vita2d_font_draw_text(vita2dFont[24] ,  240 + ATTACHMENT_HEIGHT + 8 , yPos + height - 32 , RGBA8(255, 255, 255, 255), 24 , messageBoxes[i].attachmentFullText.c_str() );
			}else if( messageBoxes[i].showAttachmentAsBinary ){
				vita2d_draw_rectangle( 240 , yPos + height - ATTACHMENT_HEIGHT - 16 , ATTACHMENT_HEIGHT , ATTACHMENT_HEIGHT , RGBA8(0x9F , 0x9F , 0x9F , 0xFF) );
				
				if (  defaultBinaryThumbnail != NULL){
					vita2d_draw_texture( defaultBinaryThumbnail , 240 , yPos + height - ATTACHMENT_HEIGHT - 16);
				}
				vita2d_font_draw_text(vita2dFont[24] ,  240 + ATTACHMENT_HEIGHT + 8 , yPos + height - 32 , RGBA8(255, 255, 255, 255), 24 , messageBoxes[i].attachmentFullText.c_str() );
			}
			
			// DRAW EMOJIS:
			for(unsigned int em = 0; em < messageBoxes[i].emojis.size() ; em++){
				if(discordPtr->spritesheetEmoji  != NULL){
					vita2d_draw_texture_part(discordPtr->spritesheetEmoji 
					, 300 + ( messageBoxes[i].emojis[em].posX * discordPtr->emojiWidth )
					, yPos + 32 + messageBoxes[i].emojis[em].posY * discordPtr->emojiHeight
					, messageBoxes[i].emojis[em].spriteSheetX * discordPtr->emojiWidth
					, messageBoxes[i].emojis[em].spriteSheetY * discordPtr->emojiHeight
					, discordPtr->emojiWidth
					, discordPtr->emojiHeight);
				}
			}
				
			// draw default icon.
			// When user icons is implemented, add vita2d_texture pointer to user data.
			// Then apply either the default icon pointer or loaded user icon pionter to this vita2d_texture pointer.
			// For now we'll just draw the default icon for all users.

			/*
			std::string avatarPath = "https://discordapp.com/avatars/" + discordPtr->user.id + "/" + discordPtr->user.avatar + ".png";
			vita2d_texture *avatar = vita2d_load_PNG_file(avatarPath.c_str());

			if (avatar != NULL){
				vita2d_draw_texture(avatar, 243, yPos + 6);
			}
			else{
				vita2d_draw_texture(userIconDefaultImage, 243, yPos + 6);
			}
			*/

			/*
			VitaNet::curlDiscordGetAvatars(("https://discordapp.com/avatars/" + messageBoxes[i].id + "/" + messageBoxes[i].avatar + ".png"), (discordPtr->token), ("ux0:data/vitacord/cache/avatars/users/"));
			std::string avatarLocation = "ux0:data/vitacord/cache/avatars/users/" + messageBoxes[i].avatar + ".png";
			vita2d_texture *_newAvatar = vita2d_load_PNG_file(avatarLocation.c_str());

			if (_newAvatar != NULL){
				vita2d_draw_texture(_newAvatar, 243, yPos + 6);
			}
			else{
				vita2d_draw_texture(userIconDefaultImage, 243, yPos + 6);
			}
			*/

			vita2d_draw_texture(userIconDefaultImage, 243, yPos + 6);
			
		}

		
		yPos += height; // add message height to yPos
		
	}

		
}


void VitaGUI::DrawDirectMessageUsersOnSidebar(){
	
	
	int dmBoxes = static_cast<int>(directMessageBoxes.size());
	for(int i = 0 ; i < dmBoxes ; i++){
		if(directMessageScrollY + i * CHANNEL_HEIGHT < MAX_DRAW_HEIGHT && directMessageScrollY + i * CHANNEL_HEIGHT > MIN_DRAW_HEIGHT){
			
			if(discordPtr->currentDirectMessage == directMessageBoxes[i].dmIndex && discordPtr->inDirectMessageChannel){
				vita2d_draw_rectangle(directMessageScrollX , 100 + directMessageScrollY + i * CHANNEL_HEIGHT, 215 , CHANNEL_HEIGHT, RGBA8(40, 43, 48, 255));
				vita2d_draw_rectangle(directMessageScrollX , 100 + directMessageScrollY + i * CHANNEL_HEIGHT, 4 , CHANNEL_HEIGHT, RGBA8(95, 118, 198, 255));
			}

			vita2d_font_draw_text(vita2dFont[CHANNEL_TITLE_TEXT_SIZE_PIXEL] , directMessageScrollX + 8, 100 + directMessageScrollY + i * CHANNEL_HEIGHT + 42, RGBA8(255,255,255,255), CHANNEL_TITLE_TEXT_SIZE_PIXEL, directMessageBoxes[i].name.c_str());
			
		}
	}

	
}

void VitaGUI::DrawDirectMessageMessages(){

	
	int yPos = 0,height;
	unsigned int messageBoxesAmount = directMessageMessagesBoxes.size();

	yPos = directMessageMessagesScrollY + 40;
	for(unsigned int i =  0 ; i < messageBoxesAmount ; i++){
		
		height = directMessageMessagesBoxes[i].messageHeight;
		
		if(yPos < MAX_DRAW_HEIGHT && yPos > MIN_DRAW_HEIGHT){
			if(i == messageBoxesAmount-1){
				
				vita2d_draw_rectangle(242, yPos + height - 1, 706, 1, RGBA8(120, 115, 120, 255));
				vita2d_draw_rectangle(240, yPos + height , 710, 1, RGBA8(100, 100, 100, 255)); 
				vita2d_draw_rectangle(242, yPos + height + 1, 706, 1, RGBA8(120, 115, 120, 255)); 
			}else{
				
				vita2d_draw_rectangle(242, yPos + height - 1, 706, 1, RGBA8(62, 65, 70, 255)); 
				vita2d_draw_rectangle(240, yPos + height, 710, 1, RGBA8(51, 53, 55, 255));
				vita2d_draw_rectangle(242, yPos + height + 1, 706, 1, RGBA8(62, 65, 70, 255)); 
				
			}
			
				vita2d_font_draw_text(vita2dFont[22], 300, yPos + 26, RGBA8(255, 255, 255, 255), 22, directMessageMessagesBoxes[i].username.c_str());

				vita2d_font_draw_text(vita2dFont[22], 300, yPos + 55, RGBA8(255, 255, 255, 255), 22, directMessageMessagesBoxes[i].content.c_str());

			
			// draw default icon.
			// When user icons is implemented, add vita2d_texture pointer to user data.
			// Then apply either the default icon pointer or loaded user icon pionter to this vita2d_texture pointer.
			// For now we'll just draw the default icon for all users.
			vita2d_draw_texture(userIconDefaultImage, 243, yPos + 6);
		}

		
		yPos += height; // add message height to yPos
	}
	
}




