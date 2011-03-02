#ifndef GUIManager_H
#define GUIManager_H

#include <CEGUI.h>
#include <SFML/Graphics/RenderWindow.hpp>

// Based on SFML/CEGUI bridge available here:
// http://www.sfml-dev.org/wiki/fr/tutoriels/utilisercegui
class GUIManager
{
public:
	GUIManager();
	~GUIManager();

	bool	Initialize(sf::RenderWindow* win);
	bool	HandleEvent(sf::Event& event);
	void	Draw();

	void	SetRootWindow(CEGUI::Window* win)	{ CEGUI::System::getSingleton().setGUISheet(win); }

	// Helpers, mainly aimed at use from lua.
	static void		LuaStringToCEGUIString(const std::string &uft8EncodedStr, CEGUI::String &destStr)
	{
		destStr = CEGUI::String((CEGUI::utf8*)uft8EncodedStr.c_str());
	}

	/* Check if needed first, heh
	static void		SetWinTextUTF8(std::string winName, std::string winText)
	{
		CEGUI::WindowManager& winMgr = CEGUI::WindowManager::getSingleton();
		if (winMgr.isWindowPresent(winName))
		{
			CEGUI::Window* win = winMgr.getWindow(winName);
			win->setText(winText.c_str());
		}
	}*/

private:
	sf::RenderWindow*	m_Window;
	const sf::Input*	m_Input;

// SFML to CEGUI input mapping section
private:
	void	InitializeMaps();

	struct MappingCompare
	{
		// Sorting func
		template<typename T>
		bool operator()(const T &lhs, const T &rhs) const
		{ return keyLess<T>(lhs.first, rhs.first); }

		// Look-up #1
		template<typename T>
		bool operator()(const T &lhs, typename const T::first_type &k) const
		{ return keyLess<T>(lhs.first, k); }

		// Look-up #2
		template<typename T>
		bool operator()(typename const T::first_type &k, const T &rhs) const
		{ return keyLess<T>(k, rhs.first); }

	private:
		template<typename T>
		bool keyLess(typename const T::first_type &k1, typename const T::first_type &k2) const
		{ return k1 < k2; }
	};
	
	/** 
	 * Look for a value in a \b sorted container of \b std::pairs.
	 * Sample use:
	 * \code
	 * const CEGUI::MouseButton mapped = getMapping<CEGUI::MouseButton, CEGUI::NoButton>(button, mouseMap);
	 * \endcode
	 * where mouseMap is a std::vector< std::pair<MouseButtonInput, CEGUI::MouseButton> >
	 */
	template <typename ResType, ResType failValue, typename KeyType, typename Dict>
	inline ResType	getMapping(KeyType k, const Dict &mapping) const
	{
		typename Dict::const_iterator i = std::lower_bound(mapping.begin(), mapping.end(), k, MappingCompare());
		if (i != mapping.end() && !MappingCompare()(k, *i))
			return i->second;

		return failValue;
	}

	typedef std::pair<sf::Mouse::Button, CEGUI::MouseButton>	MouseMapping;
	typedef std::vector<MouseMapping>							MouseMappings;

	typedef std::pair<sf::Key::Code, CEGUI::Key::Scan>	KeyMapping;
	typedef std::vector<KeyMapping>						KeyboardMappings;

	MouseMappings		mouseMap;
	KeyboardMappings	keyboardMap;
};

#endif
