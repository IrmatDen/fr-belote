/*
** Lua binding: LuaInterface
** Generated automatically by tolua++-1.0.92 on 03/04/11 07:09:31.
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

/* function to register type */
static void tolua_reg_types (lua_State* tolua_S)
{
 tolua_usertype(tolua_S,"CEGUI::String");
 tolua_usertype(tolua_S,"SoundManager");
 tolua_usertype(tolua_S,"Game");
 tolua_usertype(tolua_S,"GUIManager");
 tolua_usertype(tolua_S,"Game::GameVars");
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

/* get function: m_HostIP of class  GameVars */
#ifndef TOLUA_DISABLE_tolua_get_Game__GameVars_m_HostIP
static int tolua_get_Game__GameVars_m_HostIP(lua_State* tolua_S)
{
  Game::GameVars* self = (Game::GameVars*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'm_HostIP'",NULL);
#endif
 tolua_pushcppstring(tolua_S,(const char*)self->m_HostIP);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: m_HostIP of class  GameVars */
#ifndef TOLUA_DISABLE_tolua_set_Game__GameVars_m_HostIP
static int tolua_set_Game__GameVars_m_HostIP(lua_State* tolua_S)
{
  Game::GameVars* self = (Game::GameVars*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'm_HostIP'",NULL);
 if (!tolua_iscppstring(tolua_S,2,0,&tolua_err))
 tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->m_HostIP = ((std::string)  tolua_tocppstring(tolua_S,2,0))
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

/* method: JoinGame of class  Game */
#ifndef TOLUA_DISABLE_tolua_LuaInterface_Game_JoinGame00
static int tolua_LuaInterface_Game_JoinGame00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"Game",0,&tolua_err) ||
 !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  Game* self = (Game*)  tolua_tousertype(tolua_S,1,0);
  const std::string hostIP = ((const std::string)  tolua_tocppstring(tolua_S,2,0));
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in function 'JoinGame'",NULL);
#endif
 {
  self->JoinGame(hostIP);
 tolua_pushcppstring(tolua_S,(const char*)hostIP);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'JoinGame'.",&tolua_err);
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

/* method: LuaStringToCEGUIString of class  GUIManager */
#ifndef TOLUA_DISABLE_tolua_LuaInterface_GUIManager_LuaStringToCEGUIString00
static int tolua_LuaInterface_GUIManager_LuaStringToCEGUIString00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertable(tolua_S,1,"GUIManager",0,&tolua_err) ||
 !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
 !tolua_isusertype(tolua_S,3,"CEGUI::String",0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  const std::string uft8EncodedStr = ((const std::string)  tolua_tocppstring(tolua_S,2,0));
  CEGUI::String* destStr = ((CEGUI::String*)  tolua_tousertype(tolua_S,3,0));
 {
  GUIManager::LuaStringToCEGUIString(uft8EncodedStr,*destStr);
 tolua_pushcppstring(tolua_S,(const char*)uft8EncodedStr);
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'LuaStringToCEGUIString'.",&tolua_err);
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
   tolua_variable(tolua_S,"m_HostIP",tolua_get_Game__GameVars_m_HostIP,tolua_set_Game__GameVars_m_HostIP);
  tolua_endmodule(tolua_S);
  tolua_variable(tolua_S,"m_GameVars",tolua_get_Game_m_GameVars,tolua_set_Game_m_GameVars);
  tolua_function(tolua_S,"getSingleton",tolua_LuaInterface_Game_getSingleton00);
  tolua_function(tolua_S,"Quit",tolua_LuaInterface_Game_Quit00);
  tolua_function(tolua_S,"StartServer",tolua_LuaInterface_Game_StartServer00);
  tolua_function(tolua_S,"JoinGame",tolua_LuaInterface_Game_JoinGame00);
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
  tolua_function(tolua_S,"LuaStringToCEGUIString",tolua_LuaInterface_GUIManager_LuaStringToCEGUIString00);
 tolua_endmodule(tolua_S);
 tolua_endmodule(tolua_S);
 return 1;
}


#if defined(LUA_VERSION_NUM) && LUA_VERSION_NUM >= 501
 int luaopen_LuaInterface (lua_State* tolua_S) {
 return tolua_LuaInterface_open(tolua_S);
};
#endif

