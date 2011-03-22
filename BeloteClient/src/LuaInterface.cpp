/*
** Lua binding: LuaInterface
** Generated automatically by tolua++-1.0.92 on 03/22/11 20:36:14.
*/

#ifndef __cplusplus
#include "stdlib.h"
#endif
#include "string.h"

#include "tolua++.h"

/* Exported function */
int tolua_LuaInterface_open (lua_State* tolua_S);

#include "Game.h"
#include "../../include/BeloteContextTypes.h"
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
 tolua_usertype(tolua_S,"PlayedCardArgs");
 tolua_usertype(tolua_S,"PlayerConnectedEventArgs");
 tolua_usertype(tolua_S,"CurrentCardsInHandArgs");
 tolua_usertype(tolua_S,"ConnectionStatusEventArgs");
 tolua_usertype(tolua_S,"MatchWonArgs");
 tolua_usertype(tolua_S,"ClientSocket");
 tolua_usertype(tolua_S,"SoundManager");
 tolua_usertype(tolua_S,"ContractingTeamResultArgs");
 tolua_usertype(tolua_S,"BeloteAnnouncedArgs");
 tolua_usertype(tolua_S,"CEGUI::EventArgs");
 tolua_usertype(tolua_S,"ScoresArgs");
 tolua_usertype(tolua_S,"ErrorRaisedArgs");
 tolua_usertype(tolua_S,"CEGUI::String");
 tolua_usertype(tolua_S,"CEGUI::EventSet");
 tolua_usertype(tolua_S,"LitigeArgs");
 tolua_usertype(tolua_S,"PlayerAcceptedAssetArgs");
 tolua_usertype(tolua_S,"PlayerRefusedAssetArgs");
 tolua_usertype(tolua_S,"PotentialAssetArgs");
 tolua_usertype(tolua_S,"WaitingPlayArgs");
 tolua_usertype(tolua_S,"CEGUI::ListboxTextItem");
 tolua_usertype(tolua_S,"GUIManager");
 tolua_usertype(tolua_S,"LeftWrappedListItem");
 tolua_usertype(tolua_S,"CurrentPositioningArgs");
 tolua_usertype(tolua_S,"SystemMessageBroadcastedEventArgs");
 tolua_usertype(tolua_S,"BeloteContextRuleSet");
 tolua_usertype(tolua_S,"Game");
 tolua_usertype(tolua_S,"TextBroadcastedEventArgs");
 tolua_usertype(tolua_S,"PlayerDealingArgs");
}

/* get function: m_PlayDir of class  BeloteContextRuleSet */
#ifndef TOLUA_DISABLE_tolua_get_BeloteContextRuleSet_m_PlayDir
static int tolua_get_BeloteContextRuleSet_m_PlayDir(lua_State* tolua_S)
{
  BeloteContextRuleSet* self = (BeloteContextRuleSet*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'm_PlayDir'",NULL);
#endif
 tolua_pushnumber(tolua_S,(lua_Number)self->m_PlayDir);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: m_PlayDir of class  BeloteContextRuleSet */
#ifndef TOLUA_DISABLE_tolua_set_BeloteContextRuleSet_m_PlayDir
static int tolua_set_BeloteContextRuleSet_m_PlayDir(lua_State* tolua_S)
{
  BeloteContextRuleSet* self = (BeloteContextRuleSet*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'm_PlayDir'",NULL);
 if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
 tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->m_PlayDir = ((PlayDirection) (int)  tolua_tonumber(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: m_WinningScore of class  BeloteContextRuleSet */
#ifndef TOLUA_DISABLE_tolua_get_BeloteContextRuleSet_unsigned_m_WinningScore
static int tolua_get_BeloteContextRuleSet_unsigned_m_WinningScore(lua_State* tolua_S)
{
  BeloteContextRuleSet* self = (BeloteContextRuleSet*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'm_WinningScore'",NULL);
#endif
 tolua_pushnumber(tolua_S,(lua_Number)self->m_WinningScore);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: m_WinningScore of class  BeloteContextRuleSet */
#ifndef TOLUA_DISABLE_tolua_set_BeloteContextRuleSet_unsigned_m_WinningScore
static int tolua_set_BeloteContextRuleSet_unsigned_m_WinningScore(lua_State* tolua_S)
{
  BeloteContextRuleSet* self = (BeloteContextRuleSet*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'm_WinningScore'",NULL);
 if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
 tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->m_WinningScore = ((unsigned int)  tolua_tonumber(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

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

/* get function: m_RuleSet of class  GameVars */
#ifndef TOLUA_DISABLE_tolua_get_Game__GameVars_m_RuleSet
static int tolua_get_Game__GameVars_m_RuleSet(lua_State* tolua_S)
{
  Game::GameVars* self = (Game::GameVars*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'm_RuleSet'",NULL);
#endif
 tolua_pushusertype(tolua_S,(void*)&self->m_RuleSet,"BeloteContextRuleSet");
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: m_RuleSet of class  GameVars */
#ifndef TOLUA_DISABLE_tolua_set_Game__GameVars_m_RuleSet
static int tolua_set_Game__GameVars_m_RuleSet(lua_State* tolua_S)
{
  Game::GameVars* self = (Game::GameVars*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'm_RuleSet'",NULL);
 if (!tolua_isusertype(tolua_S,2,"BeloteContextRuleSet",0,&tolua_err))
 tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->m_RuleSet = *((BeloteContextRuleSet*)  tolua_tousertype(tolua_S,2,0))
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

/* method: GetGUIManager of class  Game */
#ifndef TOLUA_DISABLE_tolua_LuaInterface_Game_GetGUIManager00
static int tolua_LuaInterface_Game_GetGUIManager00(lua_State* tolua_S)
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
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GetGUIManager'",NULL);
#endif
 {
  GUIManager& tolua_ret = (GUIManager&)  self->GetGUIManager();
 tolua_pushusertype(tolua_S,(void*)&tolua_ret,"GUIManager");
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetGUIManager'.",&tolua_err);
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
 !tolua_isnumber(tolua_S,2,1,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  Game* self = (Game*)  tolua_tousertype(tolua_S,1,0);
  Game::MenuReason reason = ((Game::MenuReason) (int)  tolua_tonumber(tolua_S,2,Game::MR_Normal));
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'LoadMenu'",NULL);
#endif
 {
  self->LoadMenu(reason);
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

/* get function: m_ErrorTxt of class  ErrorRaisedArgs */
#ifndef TOLUA_DISABLE_tolua_get_ErrorRaisedArgs_m_ErrorTxt
static int tolua_get_ErrorRaisedArgs_m_ErrorTxt(lua_State* tolua_S)
{
  ErrorRaisedArgs* self = (ErrorRaisedArgs*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'm_ErrorTxt'",NULL);
#endif
 tolua_pushcppstring(tolua_S,(const char*)self->m_ErrorTxt);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: m_ErrorTxt of class  ErrorRaisedArgs */
#ifndef TOLUA_DISABLE_tolua_set_ErrorRaisedArgs_m_ErrorTxt
static int tolua_set_ErrorRaisedArgs_m_ErrorTxt(lua_State* tolua_S)
{
  ErrorRaisedArgs* self = (ErrorRaisedArgs*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'm_ErrorTxt'",NULL);
 if (!tolua_iscppstring(tolua_S,2,0,&tolua_err))
 tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->m_ErrorTxt = ((std::string)  tolua_tocppstring(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: ErrorUnknown of class  GUIManager */
#ifndef TOLUA_DISABLE_tolua_get_GUIManager_ErrorUnknown
static int tolua_get_GUIManager_ErrorUnknown(lua_State* tolua_S)
{
 tolua_pushcppstring(tolua_S,(const char*)GUIManager::ErrorUnknown);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: ErrorLostConnection of class  GUIManager */
#ifndef TOLUA_DISABLE_tolua_get_GUIManager_ErrorLostConnection
static int tolua_get_GUIManager_ErrorLostConnection(lua_State* tolua_S)
{
 tolua_pushcppstring(tolua_S,(const char*)GUIManager::ErrorLostConnection);
 return 1;
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

/* get function: m_ConnectionStatus of class  ConnectionStatusEventArgs */
#ifndef TOLUA_DISABLE_tolua_get_ConnectionStatusEventArgs_m_ConnectionStatus
static int tolua_get_ConnectionStatusEventArgs_m_ConnectionStatus(lua_State* tolua_S)
{
  ConnectionStatusEventArgs* self = (ConnectionStatusEventArgs*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'm_ConnectionStatus'",NULL);
#endif
 tolua_pushnumber(tolua_S,(lua_Number)self->m_ConnectionStatus);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: m_ConnectionStatus of class  ConnectionStatusEventArgs */
#ifndef TOLUA_DISABLE_tolua_set_ConnectionStatusEventArgs_m_ConnectionStatus
static int tolua_set_ConnectionStatusEventArgs_m_ConnectionStatus(lua_State* tolua_S)
{
  ConnectionStatusEventArgs* self = (ConnectionStatusEventArgs*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'm_ConnectionStatus'",NULL);
 if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
 tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->m_ConnectionStatus = ((ConnectionStatusEventArgs::ConnectionStatus) (int)  tolua_tonumber(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: m_Connected of class  PlayerConnectedEventArgs */
#ifndef TOLUA_DISABLE_tolua_get_PlayerConnectedEventArgs_m_Connected
static int tolua_get_PlayerConnectedEventArgs_m_Connected(lua_State* tolua_S)
{
  PlayerConnectedEventArgs* self = (PlayerConnectedEventArgs*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'm_Connected'",NULL);
#endif
 tolua_pushboolean(tolua_S,(bool)self->m_Connected);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: m_Connected of class  PlayerConnectedEventArgs */
#ifndef TOLUA_DISABLE_tolua_set_PlayerConnectedEventArgs_m_Connected
static int tolua_set_PlayerConnectedEventArgs_m_Connected(lua_State* tolua_S)
{
  PlayerConnectedEventArgs* self = (PlayerConnectedEventArgs*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'm_Connected'",NULL);
 if (!tolua_isboolean(tolua_S,2,0,&tolua_err))
 tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->m_Connected = ((bool)  tolua_toboolean(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: m_PlayerName of class  PlayerConnectedEventArgs */
#ifndef TOLUA_DISABLE_tolua_get_PlayerConnectedEventArgs_m_PlayerName
static int tolua_get_PlayerConnectedEventArgs_m_PlayerName(lua_State* tolua_S)
{
  PlayerConnectedEventArgs* self = (PlayerConnectedEventArgs*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'm_PlayerName'",NULL);
#endif
 tolua_pushcppstring(tolua_S,(const char*)self->m_PlayerName);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: m_PlayerName of class  PlayerConnectedEventArgs */
#ifndef TOLUA_DISABLE_tolua_set_PlayerConnectedEventArgs_m_PlayerName
static int tolua_set_PlayerConnectedEventArgs_m_PlayerName(lua_State* tolua_S)
{
  PlayerConnectedEventArgs* self = (PlayerConnectedEventArgs*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'm_PlayerName'",NULL);
 if (!tolua_iscppstring(tolua_S,2,0,&tolua_err))
 tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->m_PlayerName = ((std::string)  tolua_tocppstring(tolua_S,2,0))
;
 return 0;
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

/* get function: m_Message of class  SystemMessageBroadcastedEventArgs */
#ifndef TOLUA_DISABLE_tolua_get_SystemMessageBroadcastedEventArgs_m_Message
static int tolua_get_SystemMessageBroadcastedEventArgs_m_Message(lua_State* tolua_S)
{
  SystemMessageBroadcastedEventArgs* self = (SystemMessageBroadcastedEventArgs*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'm_Message'",NULL);
#endif
 tolua_pushcppstring(tolua_S,(const char*)self->m_Message);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: m_Message of class  SystemMessageBroadcastedEventArgs */
#ifndef TOLUA_DISABLE_tolua_set_SystemMessageBroadcastedEventArgs_m_Message
static int tolua_set_SystemMessageBroadcastedEventArgs_m_Message(lua_State* tolua_S)
{
  SystemMessageBroadcastedEventArgs* self = (SystemMessageBroadcastedEventArgs*)  tolua_tousertype(tolua_S,1,0);
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

/* get function: m_Pos of class  CurrentPositioningArgs */
#ifndef TOLUA_DISABLE_tolua_get_LuaInterface_CurrentPositioningArgs_m_Pos
static int tolua_get_LuaInterface_CurrentPositioningArgs_m_Pos(lua_State* tolua_S)
{
 int tolua_index;
  CurrentPositioningArgs* self;
 lua_pushstring(tolua_S,".self");
 lua_rawget(tolua_S,1);
 self = (CurrentPositioningArgs*)  lua_touserdata(tolua_S,-1);
#ifndef TOLUA_RELEASE
 {
 tolua_Error tolua_err;
 if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
 tolua_error(tolua_S,"#vinvalid type in array indexing.",&tolua_err);
 }
#endif
 tolua_index = (int)tolua_tonumber(tolua_S,2,0);
#ifndef TOLUA_RELEASE
 if (tolua_index<0 || tolua_index>=4)
 tolua_error(tolua_S,"array indexing out of range.",NULL);
#endif
 tolua_pushcppstring(tolua_S,(const char*)self->m_Pos[tolua_index]);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: m_Pos of class  CurrentPositioningArgs */
#ifndef TOLUA_DISABLE_tolua_set_LuaInterface_CurrentPositioningArgs_m_Pos
static int tolua_set_LuaInterface_CurrentPositioningArgs_m_Pos(lua_State* tolua_S)
{
 int tolua_index;
  CurrentPositioningArgs* self;
 lua_pushstring(tolua_S,".self");
 lua_rawget(tolua_S,1);
 self = (CurrentPositioningArgs*)  lua_touserdata(tolua_S,-1);
#ifndef TOLUA_RELEASE
 {
 tolua_Error tolua_err;
 if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
 tolua_error(tolua_S,"#vinvalid type in array indexing.",&tolua_err);
 }
#endif
 tolua_index = (int)tolua_tonumber(tolua_S,2,0);
#ifndef TOLUA_RELEASE
 if (tolua_index<0 || tolua_index>=4)
 tolua_error(tolua_S,"array indexing out of range.",NULL);
#endif
  self->m_Pos[tolua_index] = ((std::string)  tolua_tocppstring(tolua_S,3,0));
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: m_Who of class  PlayerDealingArgs */
#ifndef TOLUA_DISABLE_tolua_get_PlayerDealingArgs_m_Who
static int tolua_get_PlayerDealingArgs_m_Who(lua_State* tolua_S)
{
  PlayerDealingArgs* self = (PlayerDealingArgs*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'm_Who'",NULL);
#endif
 tolua_pushcppstring(tolua_S,(const char*)self->m_Who);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: m_Who of class  PlayerDealingArgs */
#ifndef TOLUA_DISABLE_tolua_set_PlayerDealingArgs_m_Who
static int tolua_set_PlayerDealingArgs_m_Who(lua_State* tolua_S)
{
  PlayerDealingArgs* self = (PlayerDealingArgs*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'm_Who'",NULL);
 if (!tolua_iscppstring(tolua_S,2,0,&tolua_err))
 tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->m_Who = ((std::string)  tolua_tocppstring(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: m_PlayerPos of class  PlayerRefusedAssetArgs */
#ifndef TOLUA_DISABLE_tolua_get_PlayerRefusedAssetArgs_m_PlayerPos
static int tolua_get_PlayerRefusedAssetArgs_m_PlayerPos(lua_State* tolua_S)
{
  PlayerRefusedAssetArgs* self = (PlayerRefusedAssetArgs*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'm_PlayerPos'",NULL);
#endif
 tolua_pushnumber(tolua_S,(lua_Number)self->m_PlayerPos);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: m_PlayerPos of class  PlayerRefusedAssetArgs */
#ifndef TOLUA_DISABLE_tolua_set_PlayerRefusedAssetArgs_m_PlayerPos
static int tolua_set_PlayerRefusedAssetArgs_m_PlayerPos(lua_State* tolua_S)
{
  PlayerRefusedAssetArgs* self = (PlayerRefusedAssetArgs*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'm_PlayerPos'",NULL);
 if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
 tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->m_PlayerPos = ((int)  tolua_tonumber(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: m_PlayerPos of class  PlayerAcceptedAssetArgs */
#ifndef TOLUA_DISABLE_tolua_get_PlayerAcceptedAssetArgs_m_PlayerPos
static int tolua_get_PlayerAcceptedAssetArgs_m_PlayerPos(lua_State* tolua_S)
{
  PlayerAcceptedAssetArgs* self = (PlayerAcceptedAssetArgs*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'm_PlayerPos'",NULL);
#endif
 tolua_pushnumber(tolua_S,(lua_Number)self->m_PlayerPos);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: m_PlayerPos of class  PlayerAcceptedAssetArgs */
#ifndef TOLUA_DISABLE_tolua_set_PlayerAcceptedAssetArgs_m_PlayerPos
static int tolua_set_PlayerAcceptedAssetArgs_m_PlayerPos(lua_State* tolua_S)
{
  PlayerAcceptedAssetArgs* self = (PlayerAcceptedAssetArgs*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'm_PlayerPos'",NULL);
 if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
 tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->m_PlayerPos = ((int)  tolua_tonumber(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: m_Asset of class  PlayerAcceptedAssetArgs */
#ifndef TOLUA_DISABLE_tolua_get_PlayerAcceptedAssetArgs_m_Asset
static int tolua_get_PlayerAcceptedAssetArgs_m_Asset(lua_State* tolua_S)
{
  PlayerAcceptedAssetArgs* self = (PlayerAcceptedAssetArgs*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'm_Asset'",NULL);
#endif
 tolua_pushcppstring(tolua_S,(const char*)self->m_Asset);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: m_Asset of class  PlayerAcceptedAssetArgs */
#ifndef TOLUA_DISABLE_tolua_set_PlayerAcceptedAssetArgs_m_Asset
static int tolua_set_PlayerAcceptedAssetArgs_m_Asset(lua_State* tolua_S)
{
  PlayerAcceptedAssetArgs* self = (PlayerAcceptedAssetArgs*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'm_Asset'",NULL);
 if (!tolua_iscppstring(tolua_S,2,0,&tolua_err))
 tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->m_Asset = ((std::string)  tolua_tocppstring(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: m_AcceptedByNSTeam of class  PlayerAcceptedAssetArgs */
#ifndef TOLUA_DISABLE_tolua_get_PlayerAcceptedAssetArgs_m_AcceptedByNSTeam
static int tolua_get_PlayerAcceptedAssetArgs_m_AcceptedByNSTeam(lua_State* tolua_S)
{
  PlayerAcceptedAssetArgs* self = (PlayerAcceptedAssetArgs*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'm_AcceptedByNSTeam'",NULL);
#endif
 tolua_pushboolean(tolua_S,(bool)self->m_AcceptedByNSTeam);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: m_AcceptedByNSTeam of class  PlayerAcceptedAssetArgs */
#ifndef TOLUA_DISABLE_tolua_set_PlayerAcceptedAssetArgs_m_AcceptedByNSTeam
static int tolua_set_PlayerAcceptedAssetArgs_m_AcceptedByNSTeam(lua_State* tolua_S)
{
  PlayerAcceptedAssetArgs* self = (PlayerAcceptedAssetArgs*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'm_AcceptedByNSTeam'",NULL);
 if (!tolua_isboolean(tolua_S,2,0,&tolua_err))
 tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->m_AcceptedByNSTeam = ((bool)  tolua_toboolean(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: m_Cards of class  CurrentCardsInHandArgs */
#ifndef TOLUA_DISABLE_tolua_get_LuaInterface_CurrentCardsInHandArgs_m_Cards
static int tolua_get_LuaInterface_CurrentCardsInHandArgs_m_Cards(lua_State* tolua_S)
{
 int tolua_index;
  CurrentCardsInHandArgs* self;
 lua_pushstring(tolua_S,".self");
 lua_rawget(tolua_S,1);
 self = (CurrentCardsInHandArgs*)  lua_touserdata(tolua_S,-1);
#ifndef TOLUA_RELEASE
 {
 tolua_Error tolua_err;
 if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
 tolua_error(tolua_S,"#vinvalid type in array indexing.",&tolua_err);
 }
#endif
 tolua_index = (int)tolua_tonumber(tolua_S,2,0);
#ifndef TOLUA_RELEASE
 if (tolua_index<0 || tolua_index>=8)
 tolua_error(tolua_S,"array indexing out of range.",NULL);
#endif
 tolua_pushcppstring(tolua_S,(const char*)self->m_Cards[tolua_index]);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: m_Cards of class  CurrentCardsInHandArgs */
#ifndef TOLUA_DISABLE_tolua_set_LuaInterface_CurrentCardsInHandArgs_m_Cards
static int tolua_set_LuaInterface_CurrentCardsInHandArgs_m_Cards(lua_State* tolua_S)
{
 int tolua_index;
  CurrentCardsInHandArgs* self;
 lua_pushstring(tolua_S,".self");
 lua_rawget(tolua_S,1);
 self = (CurrentCardsInHandArgs*)  lua_touserdata(tolua_S,-1);
#ifndef TOLUA_RELEASE
 {
 tolua_Error tolua_err;
 if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
 tolua_error(tolua_S,"#vinvalid type in array indexing.",&tolua_err);
 }
#endif
 tolua_index = (int)tolua_tonumber(tolua_S,2,0);
#ifndef TOLUA_RELEASE
 if (tolua_index<0 || tolua_index>=8)
 tolua_error(tolua_S,"array indexing out of range.",NULL);
#endif
  self->m_Cards[tolua_index] = ((std::string)  tolua_tocppstring(tolua_S,3,0));
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: m_Card of class  PotentialAssetArgs */
#ifndef TOLUA_DISABLE_tolua_get_PotentialAssetArgs_m_Card
static int tolua_get_PotentialAssetArgs_m_Card(lua_State* tolua_S)
{
  PotentialAssetArgs* self = (PotentialAssetArgs*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'm_Card'",NULL);
#endif
 tolua_pushcppstring(tolua_S,(const char*)self->m_Card);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: m_Card of class  PotentialAssetArgs */
#ifndef TOLUA_DISABLE_tolua_set_PotentialAssetArgs_m_Card
static int tolua_set_PotentialAssetArgs_m_Card(lua_State* tolua_S)
{
  PotentialAssetArgs* self = (PotentialAssetArgs*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'm_Card'",NULL);
 if (!tolua_iscppstring(tolua_S,2,0,&tolua_err))
 tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->m_Card = ((std::string)  tolua_tocppstring(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: m_PossibleCards of class  WaitingPlayArgs */
#ifndef TOLUA_DISABLE_tolua_get_LuaInterface_WaitingPlayArgs_m_PossibleCards
static int tolua_get_LuaInterface_WaitingPlayArgs_m_PossibleCards(lua_State* tolua_S)
{
 int tolua_index;
  WaitingPlayArgs* self;
 lua_pushstring(tolua_S,".self");
 lua_rawget(tolua_S,1);
 self = (WaitingPlayArgs*)  lua_touserdata(tolua_S,-1);
#ifndef TOLUA_RELEASE
 {
 tolua_Error tolua_err;
 if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
 tolua_error(tolua_S,"#vinvalid type in array indexing.",&tolua_err);
 }
#endif
 tolua_index = (int)tolua_tonumber(tolua_S,2,0);
#ifndef TOLUA_RELEASE
 if (tolua_index<0 || tolua_index>=8)
 tolua_error(tolua_S,"array indexing out of range.",NULL);
#endif
 tolua_pushcppstring(tolua_S,(const char*)self->m_PossibleCards[tolua_index]);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: m_PossibleCards of class  WaitingPlayArgs */
#ifndef TOLUA_DISABLE_tolua_set_LuaInterface_WaitingPlayArgs_m_PossibleCards
static int tolua_set_LuaInterface_WaitingPlayArgs_m_PossibleCards(lua_State* tolua_S)
{
 int tolua_index;
  WaitingPlayArgs* self;
 lua_pushstring(tolua_S,".self");
 lua_rawget(tolua_S,1);
 self = (WaitingPlayArgs*)  lua_touserdata(tolua_S,-1);
#ifndef TOLUA_RELEASE
 {
 tolua_Error tolua_err;
 if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
 tolua_error(tolua_S,"#vinvalid type in array indexing.",&tolua_err);
 }
#endif
 tolua_index = (int)tolua_tonumber(tolua_S,2,0);
#ifndef TOLUA_RELEASE
 if (tolua_index<0 || tolua_index>=8)
 tolua_error(tolua_S,"array indexing out of range.",NULL);
#endif
  self->m_PossibleCards[tolua_index] = ((std::string)  tolua_tocppstring(tolua_S,3,0));
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: m_Card of class  PlayedCardArgs */
#ifndef TOLUA_DISABLE_tolua_get_PlayedCardArgs_m_Card
static int tolua_get_PlayedCardArgs_m_Card(lua_State* tolua_S)
{
  PlayedCardArgs* self = (PlayedCardArgs*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'm_Card'",NULL);
#endif
 tolua_pushcppstring(tolua_S,(const char*)self->m_Card);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: m_Card of class  PlayedCardArgs */
#ifndef TOLUA_DISABLE_tolua_set_PlayedCardArgs_m_Card
static int tolua_set_PlayedCardArgs_m_Card(lua_State* tolua_S)
{
  PlayedCardArgs* self = (PlayedCardArgs*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'm_Card'",NULL);
 if (!tolua_iscppstring(tolua_S,2,0,&tolua_err))
 tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->m_Card = ((std::string)  tolua_tocppstring(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: m_Player of class  PlayedCardArgs */
#ifndef TOLUA_DISABLE_tolua_get_PlayedCardArgs_m_Player
static int tolua_get_PlayedCardArgs_m_Player(lua_State* tolua_S)
{
  PlayedCardArgs* self = (PlayedCardArgs*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'm_Player'",NULL);
#endif
 tolua_pushnumber(tolua_S,(lua_Number)self->m_Player);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: m_Player of class  PlayedCardArgs */
#ifndef TOLUA_DISABLE_tolua_set_PlayedCardArgs_m_Player
static int tolua_set_PlayedCardArgs_m_Player(lua_State* tolua_S)
{
  PlayedCardArgs* self = (PlayedCardArgs*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'm_Player'",NULL);
 if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
 tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->m_Player = ((int)  tolua_tonumber(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: m_NorthSouthScore of class  ScoresArgs */
#ifndef TOLUA_DISABLE_tolua_get_ScoresArgs_m_NorthSouthScore
static int tolua_get_ScoresArgs_m_NorthSouthScore(lua_State* tolua_S)
{
  ScoresArgs* self = (ScoresArgs*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'm_NorthSouthScore'",NULL);
#endif
 tolua_pushnumber(tolua_S,(lua_Number)self->m_NorthSouthScore);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: m_NorthSouthScore of class  ScoresArgs */
#ifndef TOLUA_DISABLE_tolua_set_ScoresArgs_m_NorthSouthScore
static int tolua_set_ScoresArgs_m_NorthSouthScore(lua_State* tolua_S)
{
  ScoresArgs* self = (ScoresArgs*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'm_NorthSouthScore'",NULL);
 if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
 tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->m_NorthSouthScore = ((int)  tolua_tonumber(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: m_WestEastScore of class  ScoresArgs */
#ifndef TOLUA_DISABLE_tolua_get_ScoresArgs_m_WestEastScore
static int tolua_get_ScoresArgs_m_WestEastScore(lua_State* tolua_S)
{
  ScoresArgs* self = (ScoresArgs*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'm_WestEastScore'",NULL);
#endif
 tolua_pushnumber(tolua_S,(lua_Number)self->m_WestEastScore);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: m_WestEastScore of class  ScoresArgs */
#ifndef TOLUA_DISABLE_tolua_set_ScoresArgs_m_WestEastScore
static int tolua_set_ScoresArgs_m_WestEastScore(lua_State* tolua_S)
{
  ScoresArgs* self = (ScoresArgs*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'm_WestEastScore'",NULL);
 if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
 tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->m_WestEastScore = ((int)  tolua_tonumber(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: m_ByPlayerPos of class  BeloteAnnouncedArgs */
#ifndef TOLUA_DISABLE_tolua_get_BeloteAnnouncedArgs_m_ByPlayerPos
static int tolua_get_BeloteAnnouncedArgs_m_ByPlayerPos(lua_State* tolua_S)
{
  BeloteAnnouncedArgs* self = (BeloteAnnouncedArgs*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'm_ByPlayerPos'",NULL);
#endif
 tolua_pushnumber(tolua_S,(lua_Number)self->m_ByPlayerPos);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: m_ByPlayerPos of class  BeloteAnnouncedArgs */
#ifndef TOLUA_DISABLE_tolua_set_BeloteAnnouncedArgs_m_ByPlayerPos
static int tolua_set_BeloteAnnouncedArgs_m_ByPlayerPos(lua_State* tolua_S)
{
  BeloteAnnouncedArgs* self = (BeloteAnnouncedArgs*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'm_ByPlayerPos'",NULL);
 if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
 tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->m_ByPlayerPos = ((int)  tolua_tonumber(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: m_IsNorthSouthTeam of class  ContractingTeamResultArgs */
#ifndef TOLUA_DISABLE_tolua_get_ContractingTeamResultArgs_m_IsNorthSouthTeam
static int tolua_get_ContractingTeamResultArgs_m_IsNorthSouthTeam(lua_State* tolua_S)
{
  ContractingTeamResultArgs* self = (ContractingTeamResultArgs*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'm_IsNorthSouthTeam'",NULL);
#endif
 tolua_pushboolean(tolua_S,(bool)self->m_IsNorthSouthTeam);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: m_IsNorthSouthTeam of class  ContractingTeamResultArgs */
#ifndef TOLUA_DISABLE_tolua_set_ContractingTeamResultArgs_m_IsNorthSouthTeam
static int tolua_set_ContractingTeamResultArgs_m_IsNorthSouthTeam(lua_State* tolua_S)
{
  ContractingTeamResultArgs* self = (ContractingTeamResultArgs*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'm_IsNorthSouthTeam'",NULL);
 if (!tolua_isboolean(tolua_S,2,0,&tolua_err))
 tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->m_IsNorthSouthTeam = ((bool)  tolua_toboolean(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: m_HasWon of class  ContractingTeamResultArgs */
#ifndef TOLUA_DISABLE_tolua_get_ContractingTeamResultArgs_m_HasWon
static int tolua_get_ContractingTeamResultArgs_m_HasWon(lua_State* tolua_S)
{
  ContractingTeamResultArgs* self = (ContractingTeamResultArgs*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'm_HasWon'",NULL);
#endif
 tolua_pushboolean(tolua_S,(bool)self->m_HasWon);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: m_HasWon of class  ContractingTeamResultArgs */
#ifndef TOLUA_DISABLE_tolua_set_ContractingTeamResultArgs_m_HasWon
static int tolua_set_ContractingTeamResultArgs_m_HasWon(lua_State* tolua_S)
{
  ContractingTeamResultArgs* self = (ContractingTeamResultArgs*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'm_HasWon'",NULL);
 if (!tolua_isboolean(tolua_S,2,0,&tolua_err))
 tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->m_HasWon = ((bool)  tolua_toboolean(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: m_LitigeValue of class  LitigeArgs */
#ifndef TOLUA_DISABLE_tolua_get_LitigeArgs_m_LitigeValue
static int tolua_get_LitigeArgs_m_LitigeValue(lua_State* tolua_S)
{
  LitigeArgs* self = (LitigeArgs*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'm_LitigeValue'",NULL);
#endif
 tolua_pushnumber(tolua_S,(lua_Number)self->m_LitigeValue);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: m_LitigeValue of class  LitigeArgs */
#ifndef TOLUA_DISABLE_tolua_set_LitigeArgs_m_LitigeValue
static int tolua_set_LitigeArgs_m_LitigeValue(lua_State* tolua_S)
{
  LitigeArgs* self = (LitigeArgs*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'm_LitigeValue'",NULL);
 if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
 tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->m_LitigeValue = ((int)  tolua_tonumber(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: m_MatchWonByNSTeam of class  MatchWonArgs */
#ifndef TOLUA_DISABLE_tolua_get_MatchWonArgs_m_MatchWonByNSTeam
static int tolua_get_MatchWonArgs_m_MatchWonByNSTeam(lua_State* tolua_S)
{
  MatchWonArgs* self = (MatchWonArgs*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'm_MatchWonByNSTeam'",NULL);
#endif
 tolua_pushboolean(tolua_S,(bool)self->m_MatchWonByNSTeam);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: m_MatchWonByNSTeam of class  MatchWonArgs */
#ifndef TOLUA_DISABLE_tolua_set_MatchWonArgs_m_MatchWonByNSTeam
static int tolua_set_MatchWonArgs_m_MatchWonByNSTeam(lua_State* tolua_S)
{
  MatchWonArgs* self = (MatchWonArgs*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'm_MatchWonByNSTeam'",NULL);
 if (!tolua_isboolean(tolua_S,2,0,&tolua_err))
 tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->m_MatchWonByNSTeam = ((bool)  tolua_toboolean(tolua_S,2,0))
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

/* method: ChoosePosition of class  ClientSocket */
#ifndef TOLUA_DISABLE_tolua_LuaInterface_ClientSocket_ChoosePosition00
static int tolua_LuaInterface_ClientSocket_ChoosePosition00(lua_State* tolua_S)
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
  const std::string posName = ((const std::string)  tolua_tocppstring(tolua_S,2,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'ChoosePosition'",NULL);
#endif
 {
  self->ChoosePosition(posName);
 tolua_pushcppstring(tolua_S,(const char*)posName);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'ChoosePosition'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: UnseatMe of class  ClientSocket */
#ifndef TOLUA_DISABLE_tolua_LuaInterface_ClientSocket_UnseatMe00
static int tolua_LuaInterface_ClientSocket_UnseatMe00(lua_State* tolua_S)
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
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'UnseatMe'",NULL);
#endif
 {
  self->UnseatMe();
 }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'UnseatMe'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: StartGame of class  ClientSocket */
#ifndef TOLUA_DISABLE_tolua_LuaInterface_ClientSocket_StartGame00
static int tolua_LuaInterface_ClientSocket_StartGame00(lua_State* tolua_S)
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
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'StartGame'",NULL);
#endif
 {
  self->StartGame();
 }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'StartGame'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: AcceptAsset of class  ClientSocket */
#ifndef TOLUA_DISABLE_tolua_LuaInterface_ClientSocket_AcceptAsset00
static int tolua_LuaInterface_ClientSocket_AcceptAsset00(lua_State* tolua_S)
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
  const std::string assetColour = ((const std::string)  tolua_tocppstring(tolua_S,2,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'AcceptAsset'",NULL);
#endif
 {
  self->AcceptAsset(assetColour);
 tolua_pushcppstring(tolua_S,(const char*)assetColour);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'AcceptAsset'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: RefuseAsset of class  ClientSocket */
#ifndef TOLUA_DISABLE_tolua_LuaInterface_ClientSocket_RefuseAsset00
static int tolua_LuaInterface_ClientSocket_RefuseAsset00(lua_State* tolua_S)
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
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'RefuseAsset'",NULL);
#endif
 {
  self->RefuseAsset();
 }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'RefuseAsset'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: PlayCard of class  ClientSocket */
#ifndef TOLUA_DISABLE_tolua_LuaInterface_ClientSocket_PlayCard00
static int tolua_LuaInterface_ClientSocket_PlayCard00(lua_State* tolua_S)
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
  const std::string cardName = ((const std::string)  tolua_tocppstring(tolua_S,2,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'PlayCard'",NULL);
#endif
 {
  self->PlayCard(cardName);
 tolua_pushcppstring(tolua_S,(const char*)cardName);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'PlayCard'.",&tolua_err);
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
 tolua_constant(tolua_S,"PD_CW",PD_CW);
 tolua_constant(tolua_S,"PD_CCW",PD_CCW);
 tolua_cclass(tolua_S,"BeloteContextRuleSet","BeloteContextRuleSet","",NULL);
 tolua_beginmodule(tolua_S,"BeloteContextRuleSet");
  tolua_variable(tolua_S,"m_PlayDir",tolua_get_BeloteContextRuleSet_m_PlayDir,tolua_set_BeloteContextRuleSet_m_PlayDir);
  tolua_variable(tolua_S,"m_WinningScore",tolua_get_BeloteContextRuleSet_unsigned_m_WinningScore,tolua_set_BeloteContextRuleSet_unsigned_m_WinningScore);
 tolua_endmodule(tolua_S);
 tolua_cclass(tolua_S,"Game","Game","",NULL);
 tolua_beginmodule(tolua_S,"Game");
  tolua_constant(tolua_S,"GM_HOST",Game::GM_HOST);
  tolua_constant(tolua_S,"GM_CLIENT",Game::GM_CLIENT);
  tolua_constant(tolua_S,"MR_Normal",Game::MR_Normal);
  tolua_constant(tolua_S,"MR_ConnectionLost",Game::MR_ConnectionLost);
  tolua_cclass(tolua_S,"GameVars","Game::GameVars","",NULL);
  tolua_beginmodule(tolua_S,"GameVars");
   tolua_variable(tolua_S,"m_GameMode",tolua_get_Game__GameVars_m_GameMode,tolua_set_Game__GameVars_m_GameMode);
   tolua_variable(tolua_S,"m_PlayerName",tolua_get_Game__GameVars_m_PlayerName,tolua_set_Game__GameVars_m_PlayerName);
   tolua_variable(tolua_S,"m_RuleSet",tolua_get_Game__GameVars_m_RuleSet,tolua_set_Game__GameVars_m_RuleSet);
  tolua_endmodule(tolua_S);
  tolua_variable(tolua_S,"m_GameVars",tolua_get_Game_m_GameVars,tolua_set_Game_m_GameVars);
  tolua_function(tolua_S,"getSingleton",tolua_LuaInterface_Game_getSingleton00);
  tolua_function(tolua_S,"Quit",tolua_LuaInterface_Game_Quit00);
  tolua_function(tolua_S,"GetGUIManager",tolua_LuaInterface_Game_GetGUIManager00);
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
 tolua_cclass(tolua_S,"ErrorRaisedArgs","ErrorRaisedArgs","CEGUI::EventArgs",NULL);
 tolua_beginmodule(tolua_S,"ErrorRaisedArgs");
  tolua_variable(tolua_S,"m_ErrorTxt",tolua_get_ErrorRaisedArgs_m_ErrorTxt,tolua_set_ErrorRaisedArgs_m_ErrorTxt);
 tolua_endmodule(tolua_S);
 tolua_cclass(tolua_S,"GUIManager","GUIManager","CEGUI::EventSet",NULL);
 tolua_beginmodule(tolua_S,"GUIManager");
  tolua_variable(tolua_S,"ErrorUnknown",tolua_get_GUIManager_ErrorUnknown,NULL);
  tolua_variable(tolua_S,"ErrorLostConnection",tolua_get_GUIManager_ErrorLostConnection,NULL);
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
 tolua_cclass(tolua_S,"ConnectionStatusEventArgs","ConnectionStatusEventArgs","CEGUI::EventArgs",NULL);
 tolua_beginmodule(tolua_S,"ConnectionStatusEventArgs");
  tolua_constant(tolua_S,"CS_Connected",ConnectionStatusEventArgs::CS_Connected);
  tolua_constant(tolua_S,"CS_Disconnected",ConnectionStatusEventArgs::CS_Disconnected);
  tolua_constant(tolua_S,"CS_LobbyFull",ConnectionStatusEventArgs::CS_LobbyFull);
  tolua_variable(tolua_S,"m_ConnectionStatus",tolua_get_ConnectionStatusEventArgs_m_ConnectionStatus,tolua_set_ConnectionStatusEventArgs_m_ConnectionStatus);
 tolua_endmodule(tolua_S);
 tolua_cclass(tolua_S,"PlayerConnectedEventArgs","PlayerConnectedEventArgs","CEGUI::EventArgs",NULL);
 tolua_beginmodule(tolua_S,"PlayerConnectedEventArgs");
  tolua_variable(tolua_S,"m_Connected",tolua_get_PlayerConnectedEventArgs_m_Connected,tolua_set_PlayerConnectedEventArgs_m_Connected);
  tolua_variable(tolua_S,"m_PlayerName",tolua_get_PlayerConnectedEventArgs_m_PlayerName,tolua_set_PlayerConnectedEventArgs_m_PlayerName);
 tolua_endmodule(tolua_S);
 tolua_cclass(tolua_S,"TextBroadcastedEventArgs","TextBroadcastedEventArgs","CEGUI::EventArgs",NULL);
 tolua_beginmodule(tolua_S,"TextBroadcastedEventArgs");
  tolua_variable(tolua_S,"m_Teller",tolua_get_TextBroadcastedEventArgs_m_Teller,tolua_set_TextBroadcastedEventArgs_m_Teller);
  tolua_variable(tolua_S,"m_Message",tolua_get_TextBroadcastedEventArgs_m_Message,tolua_set_TextBroadcastedEventArgs_m_Message);
 tolua_endmodule(tolua_S);
 tolua_cclass(tolua_S,"SystemMessageBroadcastedEventArgs","SystemMessageBroadcastedEventArgs","CEGUI::EventArgs",NULL);
 tolua_beginmodule(tolua_S,"SystemMessageBroadcastedEventArgs");
  tolua_variable(tolua_S,"m_Message",tolua_get_SystemMessageBroadcastedEventArgs_m_Message,tolua_set_SystemMessageBroadcastedEventArgs_m_Message);
 tolua_endmodule(tolua_S);
 tolua_cclass(tolua_S,"CurrentPositioningArgs","CurrentPositioningArgs","CEGUI::EventArgs",NULL);
 tolua_beginmodule(tolua_S,"CurrentPositioningArgs");
  tolua_array(tolua_S,"m_Pos",tolua_get_LuaInterface_CurrentPositioningArgs_m_Pos,tolua_set_LuaInterface_CurrentPositioningArgs_m_Pos);
 tolua_endmodule(tolua_S);
 tolua_cclass(tolua_S,"PlayerDealingArgs","PlayerDealingArgs","CEGUI::EventArgs",NULL);
 tolua_beginmodule(tolua_S,"PlayerDealingArgs");
  tolua_variable(tolua_S,"m_Who",tolua_get_PlayerDealingArgs_m_Who,tolua_set_PlayerDealingArgs_m_Who);
 tolua_endmodule(tolua_S);
 tolua_cclass(tolua_S,"PlayerRefusedAssetArgs","PlayerRefusedAssetArgs","CEGUI::EventArgs",NULL);
 tolua_beginmodule(tolua_S,"PlayerRefusedAssetArgs");
  tolua_variable(tolua_S,"m_PlayerPos",tolua_get_PlayerRefusedAssetArgs_m_PlayerPos,tolua_set_PlayerRefusedAssetArgs_m_PlayerPos);
 tolua_endmodule(tolua_S);
 tolua_cclass(tolua_S,"PlayerAcceptedAssetArgs","PlayerAcceptedAssetArgs","CEGUI::EventArgs",NULL);
 tolua_beginmodule(tolua_S,"PlayerAcceptedAssetArgs");
  tolua_variable(tolua_S,"m_PlayerPos",tolua_get_PlayerAcceptedAssetArgs_m_PlayerPos,tolua_set_PlayerAcceptedAssetArgs_m_PlayerPos);
  tolua_variable(tolua_S,"m_Asset",tolua_get_PlayerAcceptedAssetArgs_m_Asset,tolua_set_PlayerAcceptedAssetArgs_m_Asset);
  tolua_variable(tolua_S,"m_AcceptedByNSTeam",tolua_get_PlayerAcceptedAssetArgs_m_AcceptedByNSTeam,tolua_set_PlayerAcceptedAssetArgs_m_AcceptedByNSTeam);
 tolua_endmodule(tolua_S);
 tolua_cclass(tolua_S,"CurrentCardsInHandArgs","CurrentCardsInHandArgs","CEGUI::EventArgs",NULL);
 tolua_beginmodule(tolua_S,"CurrentCardsInHandArgs");
  tolua_array(tolua_S,"m_Cards",tolua_get_LuaInterface_CurrentCardsInHandArgs_m_Cards,tolua_set_LuaInterface_CurrentCardsInHandArgs_m_Cards);
 tolua_endmodule(tolua_S);
 tolua_cclass(tolua_S,"PotentialAssetArgs","PotentialAssetArgs","CEGUI::EventArgs",NULL);
 tolua_beginmodule(tolua_S,"PotentialAssetArgs");
  tolua_variable(tolua_S,"m_Card",tolua_get_PotentialAssetArgs_m_Card,tolua_set_PotentialAssetArgs_m_Card);
 tolua_endmodule(tolua_S);
 tolua_cclass(tolua_S,"WaitingPlayArgs","WaitingPlayArgs","CEGUI::EventArgs",NULL);
 tolua_beginmodule(tolua_S,"WaitingPlayArgs");
  tolua_array(tolua_S,"m_PossibleCards",tolua_get_LuaInterface_WaitingPlayArgs_m_PossibleCards,tolua_set_LuaInterface_WaitingPlayArgs_m_PossibleCards);
 tolua_endmodule(tolua_S);
 tolua_cclass(tolua_S,"PlayedCardArgs","PlayedCardArgs","CEGUI::EventArgs",NULL);
 tolua_beginmodule(tolua_S,"PlayedCardArgs");
  tolua_variable(tolua_S,"m_Card",tolua_get_PlayedCardArgs_m_Card,tolua_set_PlayedCardArgs_m_Card);
  tolua_variable(tolua_S,"m_Player",tolua_get_PlayedCardArgs_m_Player,tolua_set_PlayedCardArgs_m_Player);
 tolua_endmodule(tolua_S);
 tolua_cclass(tolua_S,"ScoresArgs","ScoresArgs","CEGUI::EventArgs",NULL);
 tolua_beginmodule(tolua_S,"ScoresArgs");
  tolua_variable(tolua_S,"m_NorthSouthScore",tolua_get_ScoresArgs_m_NorthSouthScore,tolua_set_ScoresArgs_m_NorthSouthScore);
  tolua_variable(tolua_S,"m_WestEastScore",tolua_get_ScoresArgs_m_WestEastScore,tolua_set_ScoresArgs_m_WestEastScore);
 tolua_endmodule(tolua_S);
 tolua_cclass(tolua_S,"BeloteAnnouncedArgs","BeloteAnnouncedArgs","CEGUI::EventArgs",NULL);
 tolua_beginmodule(tolua_S,"BeloteAnnouncedArgs");
  tolua_variable(tolua_S,"m_ByPlayerPos",tolua_get_BeloteAnnouncedArgs_m_ByPlayerPos,tolua_set_BeloteAnnouncedArgs_m_ByPlayerPos);
 tolua_endmodule(tolua_S);
 tolua_cclass(tolua_S,"ContractingTeamResultArgs","ContractingTeamResultArgs","CEGUI::EventArgs",NULL);
 tolua_beginmodule(tolua_S,"ContractingTeamResultArgs");
  tolua_variable(tolua_S,"m_IsNorthSouthTeam",tolua_get_ContractingTeamResultArgs_m_IsNorthSouthTeam,tolua_set_ContractingTeamResultArgs_m_IsNorthSouthTeam);
  tolua_variable(tolua_S,"m_HasWon",tolua_get_ContractingTeamResultArgs_m_HasWon,tolua_set_ContractingTeamResultArgs_m_HasWon);
 tolua_endmodule(tolua_S);
 tolua_cclass(tolua_S,"LitigeArgs","LitigeArgs","CEGUI::EventArgs",NULL);
 tolua_beginmodule(tolua_S,"LitigeArgs");
  tolua_variable(tolua_S,"m_LitigeValue",tolua_get_LitigeArgs_m_LitigeValue,tolua_set_LitigeArgs_m_LitigeValue);
 tolua_endmodule(tolua_S);
 tolua_cclass(tolua_S,"MatchWonArgs","MatchWonArgs","CEGUI::EventArgs",NULL);
 tolua_beginmodule(tolua_S,"MatchWonArgs");
  tolua_variable(tolua_S,"m_MatchWonByNSTeam",tolua_get_MatchWonArgs_m_MatchWonByNSTeam,tolua_set_MatchWonArgs_m_MatchWonByNSTeam);
 tolua_endmodule(tolua_S);
 tolua_cclass(tolua_S,"ClientSocket","ClientSocket","CEGUI::EventSet",NULL);
 tolua_beginmodule(tolua_S,"ClientSocket");
  tolua_function(tolua_S,"Connect",tolua_LuaInterface_ClientSocket_Connect00);
  tolua_function(tolua_S,"Disconnect",tolua_LuaInterface_ClientSocket_Disconnect00);
  tolua_function(tolua_S,"SendChatMessage",tolua_LuaInterface_ClientSocket_SendChatMessage00);
  tolua_function(tolua_S,"ChoosePosition",tolua_LuaInterface_ClientSocket_ChoosePosition00);
  tolua_function(tolua_S,"UnseatMe",tolua_LuaInterface_ClientSocket_UnseatMe00);
  tolua_function(tolua_S,"StartGame",tolua_LuaInterface_ClientSocket_StartGame00);
  tolua_function(tolua_S,"AcceptAsset",tolua_LuaInterface_ClientSocket_AcceptAsset00);
  tolua_function(tolua_S,"RefuseAsset",tolua_LuaInterface_ClientSocket_RefuseAsset00);
  tolua_function(tolua_S,"PlayCard",tolua_LuaInterface_ClientSocket_PlayCard00);
 tolua_endmodule(tolua_S);
 tolua_endmodule(tolua_S);
 return 1;
}


#if defined(LUA_VERSION_NUM) && LUA_VERSION_NUM >= 501
 int luaopen_LuaInterface (lua_State* tolua_S) {
 return tolua_LuaInterface_open(tolua_S);
};
#endif

