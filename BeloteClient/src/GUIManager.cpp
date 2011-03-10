#include <iostream>
#include <string>

#include <RendererModules/OpenGL/CEGUIOpenGLRenderer.h>
#include <ScriptingModules/LuaScriptModule/CEGUILua.h>

#include <lua.h>

#include "GUIManager.h"
#include "LuaInterface.h"

GUIManager::GUIManager()
: m_Window(0), m_Input(0)
{
}

GUIManager::~GUIManager()
{
}

bool GUIManager::Initialize(sf::RenderWindow* win)
{
	m_Window	= win;
	m_Input		= &win->GetInput();

	InitializeMaps();

	try
	{
		using namespace CEGUI;

		OpenGLRenderer::bootstrapSystem();

		// initialise the required dirs for the DefaultResourceProvider
		DefaultResourceProvider* rp = static_cast<DefaultResourceProvider*>
												(System::getSingleton().getResourceProvider());
		
		rp->setResourceGroupDirectory("animations",		"Gui/animations/");
		rp->setResourceGroupDirectory("schemes",		"Gui/schemes/");
		rp->setResourceGroupDirectory("imagesets",		"Gui/imagesets/");
		rp->setResourceGroupDirectory("fonts",			"Gui/fonts/");
		rp->setResourceGroupDirectory("layouts",		"Gui/layouts/");
		rp->setResourceGroupDirectory("looknfeels",		"Gui/looknfeel/");
		rp->setResourceGroupDirectory("lua_scripts",	"Gui/lua_scripts/");

		// This is only really needed if you are using Xerces and need to
		// specify the schemas location
		rp->setResourceGroupDirectory("schemas", "Gui/xml_schemas/");

		// set the default resource groups to be used
		AnimationManager::setDefaultResourceGroup		("animations");
		Scheme::setDefaultResourceGroup					("schemes");
		ImageManager::setImagesetDefaultResourceGroup	("imagesets");
		Font::setDefaultResourceGroup					("fonts");
		WindowManager::setDefaultResourceGroup			("layouts");
		WidgetLookManager::setDefaultResourceGroup		("looknfeels");
		ScriptModule::setDefaultResourceGroup			("lua_scripts");

		// setup default group for validation schemas
		XMLParser* parser = System::getSingleton().getXMLParser();
		if (parser->isPropertyPresent("SchemaDefaultResourceGroup"))
		{
			parser->setProperty("SchemaDefaultResourceGroup", "schemas");
		}

		LuaScriptModule &scriptModule(LuaScriptModule::create());
		System::getSingleton().setScriptingModule(&scriptModule);

		lua_State *luaState = scriptModule.getLuaState();
		tolua_LuaInterface_open(luaState);
	}
	catch (CEGUI::Exception& e)
	{
		std::string msg(e.getMessage().c_str());
		std::cerr << "CEGUI Error: " << msg << std::endl;
		return false;
	}
	return true;
}

bool GUIManager::HandleEvent(sf::Event& event)
{
	using namespace CEGUI;

	System &sys = System::getSingleton();

	switch (event.Type)
	{
	case sf::Event::TextEntered:
		return sys.injectChar(event.Text.Unicode);

	case sf::Event::KeyPressed:
		return sys.injectKeyDown(getMapping<CEGUI::Key::Scan, (CEGUI::Key::Scan)0>
										(event.Key.Code, keyboardMap));

	case sf::Event::KeyReleased:
		return sys.injectKeyUp(getMapping<CEGUI::Key::Scan, (CEGUI::Key::Scan)0>
										(event.Key.Code, keyboardMap));

	case sf::Event::MouseMoved:
		return sys.injectMousePosition(static_cast<float>(m_Input->GetMouseX()),
									   static_cast<float>(m_Input->GetMouseY()));
	
	case sf::Event::MouseButtonPressed:
		return sys.injectMouseButtonDown(getMapping<CEGUI::MouseButton, CEGUI::NoButton>
											(event.MouseButton.Button, mouseMap));

	case sf::Event::MouseButtonReleased:
		return sys.injectMouseButtonUp(getMapping<CEGUI::MouseButton, CEGUI::NoButton>
											(event.MouseButton.Button, mouseMap));

	case sf::Event::MouseWheelMoved:
		return sys.injectMouseWheelChange(static_cast<float>(event.MouseWheel.Delta));

	case sf::Event::Resized:
		sys.notifyDisplaySizeChanged(Size<float>((float)event.Size.Width, (float)event.Size.Height));
		break;
	}
	return false;
}

void GUIManager::UpdateAndDraw(float elapsedSeconds)
{
	CEGUI::System::getSingleton().injectTimePulse(elapsedSeconds);
	CEGUI::System::getSingleton().renderGUI();
}

void GUIManager::InitializeMaps()
{
	keyboardMap.push_back(KeyMapping(sf::Key::Escape	,	CEGUI::Key::Escape		));
	keyboardMap.push_back(KeyMapping(sf::Key::Num1		,	CEGUI::Key::One			));
	keyboardMap.push_back(KeyMapping(sf::Key::Num2		,	CEGUI::Key::Two			));
	keyboardMap.push_back(KeyMapping(sf::Key::Num3		,	CEGUI::Key::Three		));
	keyboardMap.push_back(KeyMapping(sf::Key::Num4		,	CEGUI::Key::Four		));
	keyboardMap.push_back(KeyMapping(sf::Key::Num5		,	CEGUI::Key::Five		));
	keyboardMap.push_back(KeyMapping(sf::Key::Num6		,	CEGUI::Key::Six			));
	keyboardMap.push_back(KeyMapping(sf::Key::Num7		,	CEGUI::Key::Seven		));
	keyboardMap.push_back(KeyMapping(sf::Key::Num8		,	CEGUI::Key::Eight		));
	keyboardMap.push_back(KeyMapping(sf::Key::Num9		,	CEGUI::Key::Nine		));
	keyboardMap.push_back(KeyMapping(sf::Key::Num0		,	CEGUI::Key::Zero		));
	keyboardMap.push_back(KeyMapping(sf::Key::Dash		,	CEGUI::Key::Minus		));
	keyboardMap.push_back(KeyMapping(sf::Key::Equal		,	CEGUI::Key::Equals		));
	keyboardMap.push_back(KeyMapping(sf::Key::Back		,	CEGUI::Key::Backspace	));
	keyboardMap.push_back(KeyMapping(sf::Key::Tab		,	CEGUI::Key::Tab			));
	keyboardMap.push_back(KeyMapping(sf::Key::Q			,	CEGUI::Key::Q			));
	keyboardMap.push_back(KeyMapping(sf::Key::W			,	CEGUI::Key::W			));
	keyboardMap.push_back(KeyMapping(sf::Key::E			,	CEGUI::Key::E			));
	keyboardMap.push_back(KeyMapping(sf::Key::R			,	CEGUI::Key::R			));
	keyboardMap.push_back(KeyMapping(sf::Key::T			,	CEGUI::Key::T			));
	keyboardMap.push_back(KeyMapping(sf::Key::Y			,	CEGUI::Key::Y			));
	keyboardMap.push_back(KeyMapping(sf::Key::U			,	CEGUI::Key::U			));
	keyboardMap.push_back(KeyMapping(sf::Key::I			,	CEGUI::Key::I			));
	keyboardMap.push_back(KeyMapping(sf::Key::O			,	CEGUI::Key::O			));
	keyboardMap.push_back(KeyMapping(sf::Key::P			,	CEGUI::Key::P			));
	keyboardMap.push_back(KeyMapping(sf::Key::LBracket	,	CEGUI::Key::LeftBracket	));
	keyboardMap.push_back(KeyMapping(sf::Key::RBracket	,	CEGUI::Key::RightBracket));
	keyboardMap.push_back(KeyMapping(sf::Key::Return	,	CEGUI::Key::Return		));
	keyboardMap.push_back(KeyMapping(sf::Key::LControl	,	CEGUI::Key::LeftControl	));
	keyboardMap.push_back(KeyMapping(sf::Key::A			,	CEGUI::Key::A			));
	keyboardMap.push_back(KeyMapping(sf::Key::S			,	CEGUI::Key::S			));
	keyboardMap.push_back(KeyMapping(sf::Key::D			,	CEGUI::Key::D			));
	keyboardMap.push_back(KeyMapping(sf::Key::F			,	CEGUI::Key::F			));
	keyboardMap.push_back(KeyMapping(sf::Key::G			,	CEGUI::Key::G			));
	keyboardMap.push_back(KeyMapping(sf::Key::H			,	CEGUI::Key::H			));
	keyboardMap.push_back(KeyMapping(sf::Key::J			,	CEGUI::Key::J			));
	keyboardMap.push_back(KeyMapping(sf::Key::K			,	CEGUI::Key::K			));
	keyboardMap.push_back(KeyMapping(sf::Key::L			,	CEGUI::Key::L			));
	keyboardMap.push_back(KeyMapping(sf::Key::SemiColon	,	CEGUI::Key::Semicolon	));
	keyboardMap.push_back(KeyMapping(sf::Key::LShift	,	CEGUI::Key::LeftShift	));
	keyboardMap.push_back(KeyMapping(sf::Key::BackSlash	,	CEGUI::Key::Backslash	));
	keyboardMap.push_back(KeyMapping(sf::Key::Z			,	CEGUI::Key::Z			));
	keyboardMap.push_back(KeyMapping(sf::Key::X			,	CEGUI::Key::X			));
	keyboardMap.push_back(KeyMapping(sf::Key::C			,	CEGUI::Key::C			));
	keyboardMap.push_back(KeyMapping(sf::Key::V			,	CEGUI::Key::V			));
	keyboardMap.push_back(KeyMapping(sf::Key::B			,	CEGUI::Key::B			));
	keyboardMap.push_back(KeyMapping(sf::Key::N			,	CEGUI::Key::N			));
	keyboardMap.push_back(KeyMapping(sf::Key::M			,	CEGUI::Key::M			));
	keyboardMap.push_back(KeyMapping(sf::Key::Comma		,	CEGUI::Key::Comma		));
	keyboardMap.push_back(KeyMapping(sf::Key::Period	,	CEGUI::Key::Period		));
	keyboardMap.push_back(KeyMapping(sf::Key::Slash		,	CEGUI::Key::Slash		));
	keyboardMap.push_back(KeyMapping(sf::Key::RShift	,	CEGUI::Key::RightShift	));
	keyboardMap.push_back(KeyMapping(sf::Key::Multiply	,	CEGUI::Key::Multiply	));
	keyboardMap.push_back(KeyMapping(sf::Key::LAlt		,	CEGUI::Key::LeftAlt		));
	keyboardMap.push_back(KeyMapping(sf::Key::Space		,	CEGUI::Key::Space		));
	keyboardMap.push_back(KeyMapping(sf::Key::F1		,	CEGUI::Key::F1			));
	keyboardMap.push_back(KeyMapping(sf::Key::F2		,	CEGUI::Key::F2			));
	keyboardMap.push_back(KeyMapping(sf::Key::F3		,	CEGUI::Key::F3			));
	keyboardMap.push_back(KeyMapping(sf::Key::F4		,	CEGUI::Key::F4			));
	keyboardMap.push_back(KeyMapping(sf::Key::F5		,	CEGUI::Key::F5			));
	keyboardMap.push_back(KeyMapping(sf::Key::F6		,	CEGUI::Key::F6			));
	keyboardMap.push_back(KeyMapping(sf::Key::F7		,	CEGUI::Key::F7			));
	keyboardMap.push_back(KeyMapping(sf::Key::F8		,	CEGUI::Key::F8			));
	keyboardMap.push_back(KeyMapping(sf::Key::F9		,	CEGUI::Key::F9			));
	keyboardMap.push_back(KeyMapping(sf::Key::F10		,	CEGUI::Key::F10			));
	keyboardMap.push_back(KeyMapping(sf::Key::Numpad7	,	CEGUI::Key::Numpad7		));
	keyboardMap.push_back(KeyMapping(sf::Key::Numpad8	,	CEGUI::Key::Numpad8		));
	keyboardMap.push_back(KeyMapping(sf::Key::Numpad9	,	CEGUI::Key::Numpad9		));
	keyboardMap.push_back(KeyMapping(sf::Key::Subtract	,	CEGUI::Key::Subtract	));
	keyboardMap.push_back(KeyMapping(sf::Key::Numpad4	,	CEGUI::Key::Numpad4		));
	keyboardMap.push_back(KeyMapping(sf::Key::Numpad5	,	CEGUI::Key::Numpad5		));
	keyboardMap.push_back(KeyMapping(sf::Key::Numpad6	,	CEGUI::Key::Numpad6		));
	keyboardMap.push_back(KeyMapping(sf::Key::Add		,	CEGUI::Key::Add			));
	keyboardMap.push_back(KeyMapping(sf::Key::Numpad1	,	CEGUI::Key::Numpad1		));
	keyboardMap.push_back(KeyMapping(sf::Key::Numpad2	,	CEGUI::Key::Numpad2		));
	keyboardMap.push_back(KeyMapping(sf::Key::Numpad3	,	CEGUI::Key::Numpad3		));
	keyboardMap.push_back(KeyMapping(sf::Key::Numpad0	,	CEGUI::Key::Numpad0		));
	keyboardMap.push_back(KeyMapping(sf::Key::F11		,	CEGUI::Key::F11			));
	keyboardMap.push_back(KeyMapping(sf::Key::F12		,	CEGUI::Key::F12			));
	keyboardMap.push_back(KeyMapping(sf::Key::F13		,	CEGUI::Key::F13			));
	keyboardMap.push_back(KeyMapping(sf::Key::F14		,	CEGUI::Key::F14			));
	keyboardMap.push_back(KeyMapping(sf::Key::F15		,	CEGUI::Key::F15			));
	keyboardMap.push_back(KeyMapping(sf::Key::RControl	,	CEGUI::Key::RightControl));
	keyboardMap.push_back(KeyMapping(sf::Key::Divide	,	CEGUI::Key::Divide		));
	keyboardMap.push_back(KeyMapping(sf::Key::RAlt		,	CEGUI::Key::RightAlt	));
	keyboardMap.push_back(KeyMapping(sf::Key::Pause		,	CEGUI::Key::Pause		));
	keyboardMap.push_back(KeyMapping(sf::Key::Home		,	CEGUI::Key::Home		));
	keyboardMap.push_back(KeyMapping(sf::Key::Up		,	CEGUI::Key::ArrowUp		));
	keyboardMap.push_back(KeyMapping(sf::Key::PageUp	,	CEGUI::Key::PageUp		));
	keyboardMap.push_back(KeyMapping(sf::Key::Left		,	CEGUI::Key::ArrowLeft	));
	keyboardMap.push_back(KeyMapping(sf::Key::Right		,	CEGUI::Key::ArrowRight	));
	keyboardMap.push_back(KeyMapping(sf::Key::End		,	CEGUI::Key::End			));
	keyboardMap.push_back(KeyMapping(sf::Key::Down		,	CEGUI::Key::ArrowDown	));
	keyboardMap.push_back(KeyMapping(sf::Key::PageDown	,	CEGUI::Key::PageDown	));
	keyboardMap.push_back(KeyMapping(sf::Key::Insert	,	CEGUI::Key::Insert		));
	keyboardMap.push_back(KeyMapping(sf::Key::Delete	,	CEGUI::Key::Delete		));

	std::sort(keyboardMap.begin(), keyboardMap.end(), MappingCompare());

	mouseMap.push_back(MouseMapping(sf::Mouse::Left,		CEGUI::LeftButton));
	mouseMap.push_back(MouseMapping(sf::Mouse::Middle,		CEGUI::MiddleButton));
	mouseMap.push_back(MouseMapping(sf::Mouse::Right,		CEGUI::RightButton));
	mouseMap.push_back(MouseMapping(sf::Mouse::XButton1,	CEGUI::X1Button));
	mouseMap.push_back(MouseMapping(sf::Mouse::XButton2,	CEGUI::X2Button));

	std::sort(mouseMap.begin(), mouseMap.end(), MappingCompare());
}
