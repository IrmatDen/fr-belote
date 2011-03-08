/*
** Lua binding: LuaInterface
** Generated automatically by tolua++-1.0.92 on 03/08/11 18:32:10.
*/

#ifndef __cplusplus
#include "stdlib.h"
#endif
#include "string.h"

#include "tolua++.h"

/* Exported function */
int tolua_LuaInterface_open (lua_State* tolua_S);

#include "Game.h"
#include "SoundManager.h"
#include "GUIManager.h"
#include "LeftWrappedListItem.h"
#include "ClientSocket.h"

/* function to release collected object via destructor */
#ifdef __cplusplus

static int tolua_collect_LeftWrappedListItem (lua_State* tolua_S)
{
 LeftWrappedListItem* self = (LeftWrappedListItem*) tolua_tousertype(tolua_S,1,0);
 delete self;
 return 0;
}
#endif


/* function to register type */
static void tolua_reg_types (lua_State* tolua_S)
{
 tolua_usertype(tolua_S,"Game::GameVars");
 tolua_usertype(tolua_S,"GUIManager");
 tolua_usertype(tolua_S,"CEGUI::String");
 tolua_usertype(tolua_S,"ClientSocket");
 tolua_usertype(tolua_S,"LeftWrappedListItem");
 tolua_usertype(tolua_S,"SoundManager");
 tolua_usertype(tolua_S,"CEGUI::EventArgs");
 tolua_usertype(tolua_S,"CEGUI::ListboxTextItem");
 tolua_usertype(tolua_S,"Game");
 tolua_usertype(tolua_S,"TextBroadcastedEventArgs");
 tolua_usertype(tolua_S,"CEGUI::EventSet");
}

/* get function: m_GameMode of class  GameVars */
#ifndef TOLUA_DISABLE_tolua_get_Game__GameVars_m_GameMode
static int tolua_get_Game__GameVars_m_GameMode(lua_State* tolua_S)
{
  Game::GameVars* self = (Game::GameVars*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'm_GameMode'",NULL);
#endif
 tolua_pushnumber(tolua_S,(lua_Number)self->m_GameMode);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: m_GameMode of class  GameVars */
#ifndef TOLUA_DISABLE_tolua_set_Game__GameVars_m_GameMode
static int tolua_set_Game__GameVars_m_GameMode(lua_State* tolua_S)
{
  Game::GameVars* self = (Game::GameVars*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'm_GameMode'",NULL);
 if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
 tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->m_GameMode = ((Game::GameMode) (int)  tolua_tonumber(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: m_PlayerName of class  GameVars */
#ifndef TOLUA_DISABLE_tolua_get_Game__GameVars_m_PlayerName
static int tolua_get_Game__GameVars_m_PlayerName(lua_State* tolua_S)
{
  Game::GameVars* self = (Game::GameVars*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'm_PlayerName'",NULL);
#endif
 tolua_pushusertype(tolua_S,(void*)&self->m_PlayerName,"CEGUI::String");
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: m_PlayerName of class  GameVars */
#ifndef TOLUA_DISABLE_tolua_set_Game__GameVars_m_PlayerName
static int tolua_set_Game__GameVars_m_PlayerName(lua_State* tolua_S)
{
  Game::GameVars* self = (Game::GameVars*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'm_PlayerName'",NULL);
 if (!tolua_isusertype(tolua_S,2,"CEGUI::String",0,&tolua_err))
 tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->m_PlayerName = *((CEGUI::String*)  tolua_tousertype(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: m_GameVars of class  Game */
#ifndef TOLUA_DISABLE_tolua_get_Game_m_GameVars
static int tolua_get_Game_m_GameVars(lua_State* tolua_S)
{
  Game* self = (Game*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'm_GameVars'",NULL);
#endif
 tolua_pushusertype(tolua_S,(void*)&self->m_GameVars,"Game::GameVars");
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: m_GameVars of class  Game */
#ifndef TOLUA_DISABLE_tolua_set_Game_m_GameVars
static int tolua_set_Game_m_GameVars(lua_State* tolua_S)
{
  Game* self = (Game*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'm_GameVars'",NULL);
 if (!tolua_isusertype(tolua_S,2,"Game::GameVars",0,&tolua_err))
 tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->m_GameVars = *((Game::GameVars*)  tolua_tousertype(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* method: getSingleton of class  Game */
#ifndef TOLUA_DISABLE_tolua_LuaInterface_Game_getSingleton00
static int tolua_LuaInterface_Game_getSingleton00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertable(tolua_S,1,"Game",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
 {
  Game& tolua_ret = (Game&)  Game::getSingleton();
 tolua_pushusertype(tolua_S,(void*)&tolua_ret,"Game");
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getSingleton'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: Quit of class  Game */
#ifndef TOLUA_DISABLE_tolua_LuaInterface_Game_Quit00
static int tolua_LuaInterface_Game_Quit00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"Game",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  Game* self = (Game*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'Quit'",NULL);
#endif
 {
  self->Quit();
 }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'Quit'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: LoadMenu of class  Game */
#ifndef TOLUA_DISABLE_tolua_LuaInterface_Game_LoadMenu00
static int tolua_LuaInterface_Game_LoadMenu00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"Game",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  Game* self = (Game*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'LoadMenu'",NULL);
#endif
 {
  self->LoadMenu();
 }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'LoadMenu'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: LoadGame of class  Game */
#ifndef TOLUA_DISABLE_tolua_LuaInterface_Game_LoadGame00
static int tolua_LuaInterface_Game_LoadGame00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"Game",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  Game* self = (Game*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'LoadGame'",NULL);
#endif
 {
  self->LoadGame();
 }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'LoadGame'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: GetClientSocket of class  Game */
#ifndef TOLUA_DISABLE_tolua_LuaInterface_Game_GetClientSocket00
static int tolua_LuaInterface_Game_GetClientSocket00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"Game",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  Game* self = (Game*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GetClientSocket'",NULL);
#endif
 {
  ClientSocket& tolua_ret = (ClientSocket&)  self->GetClientSocket();
 tolua_pushusertype(tolua_S,(void*)&tolua_ret,"ClientSocket");
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetClientSocket'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: StartServer of class  Game */
#ifndef TOLUA_DISABLE_tolua_LuaInterface_Game_StartServer00
static int tolua_LuaInterface_Game_StartServer00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"Game",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  Game* self = (Game*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'StartServer'",NULL);
#endif
 {
  self->StartServer();
 }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'StartServer'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: StopServer of class  Game */
#ifndef TOLUA_DISABLE_tolua_LuaInterface_Game_StopServer00
static int tolua_LuaInterface_Game_StopServer00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"Game",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  Game* self = (Game*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'StopServer'",NULL);
#endif
 {
  self->StopServer();
 }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'StopServer'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getSingleton of class  SoundManager */
#ifndef TOLUA_DISABLE_tolua_LuaInterface_SoundManager_getSingleton00
static int tolua_LuaInterface_SoundManager_getSingleton00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertable(tolua_S,1,"SoundManager",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
 {
  SoundManager& tolua_ret = (SoundManager&)  SoundManager::getSingleton();
 tolua_pushusertype(tolua_S,(void*)&tolua_ret,"SoundManager");
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getSingleton'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: PlayFX of class  SoundManager */
#ifndef TOLUA_DISABLE_tolua_LuaInterface_SoundManager_PlayFX00
static int tolua_LuaInterface_SoundManager_PlayFX00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"SoundManager",0,&tolua_err) ||
 !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  SoundManager* self = (SoundManager*)  tolua_tousertype(tolua_S,1,0);
  SoundManager::SoundFX fx = ((SoundManager::SoundFX) (int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'PlayFX'",NULL);
#endif
 {
  self->PlayFX(fx);
 }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'PlayFX'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: new of class  LeftWrappedListItem */
#ifndef TOLUA_DISABLE_tolua_LuaInterface_LeftWrappedListItem_new00
static int tolua_LuaInterface_LeftWrappedListItem_new00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertable(tolua_S,1,"LeftWrappedListItem",0,&tolua_err) ||
 !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  const std::string text = ((const std::string)  tolua_tocppstring(tolua_S,2,0));
 {
  LeftWrappedListItem* tolua_ret = (LeftWrappedListItem*)  new LeftWrappedListItem(text);
 tolua_pushusertype(tolua_S,(void*)tolua_ret,"LeftWrappedListItem");
 tolua_pushcppstring(tolua_S,(const char*)text);
 }
 }
 return 2;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: new_local of class  LeftWrappedListItem */
#ifndef TOLUA_DISABLE_tolua_LuaInterface_LeftWrappedListItem_new00_local
static int tolua_LuaInterface_LeftWrappedListItem_new00_local(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertable(tolua_S,1,"LeftWrappedListItem",0,&tolua_err) ||
 !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  const std::string text = ((const std::string)  tolua_tocppstring(tolua_S,2,0));
 {
  LeftWrappedListItem* tolua_ret = (LeftWrappedListItem*)  new LeftWrappedListItem(text);
 tolua_pushusertype_and_takeownership(tolua_S,(void *)tolua_ret,"LeftWrappedListItem");
 tolua_pushcppstring(tolua_S,(const char*)text);
 }
 }
 return 2;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* get function: m_Teller of class  TextBroadcastedEventArgs */
#ifndef TOLUA_DISABLE_tolua_get_TextBroadcastedEventArgs_m_Teller
static int tolua_get_TextBroadcastedEventArgs_m_Teller(lua_State* tolua_S)
{
  TextBroadcastedEventArgs* self = (TextBroadcastedEventArgs*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'm_Teller'",NULL);
#endif
 tolua_pushcppstring(tolua_S,(const char*)self->m_Teller);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: m_Teller of class  TextBroadcastedEventArgs */
#ifndef TOLUA_DISABLE_tolua_set_TextBroadcastedEventArgs_m_Teller
static int tolua_set_TextBroadcastedEventArgs_m_Teller(lua_State* tolua_S)
{
  TextBroadcastedEventArgs* self = (TextBroadcastedEventArgs*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'm_Teller'",NULL);
 if (!tolua_iscppstring(tolua_S,2,0,&tolua_err))
 tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->m_Teller = ((std::string)  tolua_tocppstring(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: m_Message of class  TextBroadcastedEventArgs */
#ifndef TOLUA_DISABLE_tolua_get_TextBroadcastedEventArgs_m_Message
static int tolua_get_TextBroadcastedEventArgs_m_Message(lua_State* tolua_S)
{
  TextBroadcastedEventArgs* self = (TextBroadcastedEventArgs*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'm_Message'",NULL);
#endif
 tolua_pushcppstring(tolua_S,(const char*)self->m_Message);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: m_Message of class  TextBroadcastedEventArgs */
#ifndef TOLUA_DISABLE_tolua_set_TextBroadcastedEventArgs_m_Message
static int tolua_set_TextBroadcastedEventArgs_m_Message(lua_State* tolua_S)
{
  TextBroadcastedEventArgs* self = (TextBroadcastedEventArgs*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'm_Message'",NULL);
 if (!tolua_iscppstring(tolua_S,2,0,&tolua_err))
 tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->m_Message = ((std::string)  tolua_tocppstring(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* method: Connect of class  ClientSocket */
#ifndef TOLUA_DISABLE_tolua_LuaInterface_ClientSocket_Connect00
static int tolua_LuaInterface_ClientSocket_Connect00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"ClientSocket",0,&tolua_err) ||
 !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
 !tolua_iscppstring(tolua_S,3,0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  ClientSocket* self = (ClientSocket*)  tolua_tousertype(tolua_S,1,0);
  const std::string hostIP = ((const std::string)  tolua_tocppstring(tolua_S,2,0));
  const std::string utf8EncodedName = ((const std::string)  tolua_tocppstring(tolua_S,3,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'Connect'",NULL);
#endif
 {
  self->Connect(hostIP,utf8EncodedName);
 tolua_pushcppstring(tolua_S,(const char*)hostIP);
 tolua_pushcppstring(tolua_S,(const char*)utf8EncodedName);
 }
 }
 return 2;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'Connect'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: SendChatMessage of class  ClientSocket */
#ifndef TOLUA_DISABLE_tolua_LuaInterface_ClientSocket_SendChatMessage00
static int tolua_LuaInterface_ClientSocket_SendChatMessage00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"ClientSocket",0,&tolua_err) ||
 !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  ClientSocket* self = (ClientSocket*)  tolua_tousertype(tolua_S,1,0);
  const std::string utf8EncodedMessage = ((const std::string)  tolua_tocppstring(tolua_S,2,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'SendChatMessage'",NULL);
#endif
 {
  self->SendChatMessage(utf8EncodedMessage);
 tolua_pushcppstring(tolua_S,(const char*)utf8EncodedMessage);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'SendChatMessage'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: Disconnect of class  ClientSocket */
#ifndef TOLUA_DISABLE_tolua_LuaInterface_ClientSocket_Disconnect00
static int tolua_LuaInterface_ClientSocket_Disconnect00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"ClientSocket",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  ClientSocket* self = (ClientSocket*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'Disconnect'",NULL);
#endif
 {
  self->Disconnect();
 }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'Disconnect'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* Open function */
int tolua_LuaInterface_open (lua_State* tolua_S)
{
 tolua_open(tolua_S);
 tolua_reg_types(tolua_S);
 tolua_module(tolua_S,NULL,0);
 tolua_beginmodule(tolua_S,NULL);
 tolua_cclass(tolua_S,"Game","Game","",NULL);
 tolua_beginmodule(tolua_S,"Game");
  tolua_constant(tolua_S,"GM_HOST",Game::GM_HOST);
  tolua_constant(tolua_S,"GM_CLIENT",Game::GM_CLIENT);
  tolua_cclass(tolua_S,"GameVars","Game::GameVars","",NULL);
  tolua_beginmodule(tolua_S,"GameVars");
   tolua_variable(tolua_S,"m_GameMode",tolua_get_Game__GameVars_m_GameMode,tolua_set_Game__GameVars_m_GameMode);
   tolua_variable(tolua_S,"m_PlayerName",tolua_get_Game__GameVars_m_PlayerName,tolua_set_Game__GameVars_m_PlayerName);
  tolua_endmodule(tolua_S);
  tolua_variable(tolua_S,"m_GameVars",tolua_get_Game_m_GameVars,tolua_set_Game_m_GameVars);
  tolua_function(tolua_S,"getSingleton",tolua_LuaInterface_Game_getSingleton00);
  tolua_function(tolua_S,"Quit",tolua_LuaInterface_Game_Quit00);
  tolua_function(tolua_S,"LoadMenu",tolua_LuaInterface_Game_LoadMenu00);
  tolua_function(tolua_S,"LoadGame",tolua_LuaInterface_Game_LoadGame00);
  tolua_function(tolua_S,"GetClientSocket",tolua_LuaInterface_Game_GetClientSocket00);
  tolua_function(tolua_S,"StartServer",tolua_LuaInterface_Game_StartServer00);
  tolua_function(tolua_S,"StopServer",tolua_LuaInterface_Game_StopServer00);
 tolua_endmodule(tolua_S);
 tolua_cclass(tolua_S,"SoundManager","SoundManager","",NULL);
 tolua_beginmodule(tolua_S,"SoundManager");
  tolua_constant(tolua_S,"FX_CLICK",SoundManager::FX_CLICK);
  tolua_constant(tolua_S,"_SoundFXCount",SoundManager::_SoundFXCount);
  tolua_function(tolua_S,"getSingleton",tolua_LuaInterface_SoundManager_getSingleton00);
  tolua_function(tolua_S,"PlayFX",tolua_LuaInterface_SoundManager_PlayFX00);
 tolua_endmodule(tolua_S);
 tolua_cclass(tolua_S,"GUIManager","GUIManager","",NULL);
 tolua_beginmodule(tolua_S,"GUIManager");
 tolua_endmodule(tolua_S);
 #ifdef __cplusplus
 tolua_cclass(tolua_S,"LeftWrappedListItem","LeftWrappedListItem","CEGUI::ListboxTextItem",tolua_collect_LeftWrappedListItem);
 #else
 tolua_cclass(tolua_S,"LeftWrappedListItem","LeftWrappedListItem","CEGUI::ListboxTextItem",NULL);
 #endif
 tolua_beginmodule(tolua_S,"LeftWrappedListItem");
  tolua_function(tolua_S,"new",tolua_LuaInterface_LeftWrappedListItem_new00);
  tolua_function(tolua_S,"new_local",tolua_LuaInterface_LeftWrappedListItem_new00_local);
  tolua_function(tolua_S,".call",tolua_LuaInterface_LeftWrappedListItem_new00_local);
 tolua_endmodule(tolua_S);
 tolua_cclass(tolua_S,"TextBroadcastedEventArgs","TextBroadcastedEventArgs","CEGUI::EventArgs",NULL);
 tolua_beginmodule(tolua_S,"TextBroadcastedEventArgs");
  tolua_variable(tolua_S,"m_Teller",tolua_get_TextBroadcastedEventArgs_m_Teller,tolua_set_TextBroadcastedEventArgs_m_Teller);
  tolua_variable(tolua_S,"m_Message",tolua_get_TextBroadcastedEventArgs_m_Message,tolua_set_TextBroadcastedEventArgs_m_Message);
 tolua_endmodule(tolua_S);
 tolua_cclass(tolua_S,"ClientSocket","ClientSocket","CEGUI::EventSet",NULL);
 tolua_beginmodule(tolua_S,"ClientSocket");
  tolua_function(tolua_S,"Connect",tolua_LuaInterface_ClientSocket_Connect00);
  tolua_function(tolua_S,"SendChatMessage",tolua_LuaInterface_ClientSocket_SendChatMessage00);
  tolua_function(tolua_S,"Disconnect",tolua_LuaInterface_ClientSocket_Disconnect00);
 tolua_endmodule(tolua_S);
 tolua_endmodule(tolua_S);
 return 1;
}


#if defined(LUA_VERSION_NUM) && LUA_VERSION_NUM >= 501
 int luaopen_LuaInterface (lua_State* tolua_S) {
 return tolua_LuaInterface_open(tolua_S);
};
#endif

