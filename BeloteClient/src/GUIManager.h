#ifndef GUIManager_H
#define GUIManager_H

#include <CEGUI.h>
#include <SFML/Graphics/RenderWindow.hpp>

class ErrorRaisedArgs : public CEGUI::EventArgs
{
public:
	std::string	m_ErrorTxt;
};

// Based on SFML/CEGUI bridge available here:
// http://www.sfml-dev.org/wiki/fr/tutoriels/utilisercegui
class GUIManager : public CEGUI::EventSet
{
public:
	static const CEGUI::String	EventNamespace;
	static const CEGUI::String	EventErrorRaised;

	static const std::string	ErrorUnknown;
	static const std::string	ErrorLostConnection;

public:
	GUIManager();
	~GUIManager();

	bool	Initialize(sf::RenderWindow* win);
	bool	HandleEvent(sf::Event& event);
	void	UpdateAndDraw(float elapsedSeconds);

	void	SetRootWindow(CEGUI::Window* win)	{ CEGUI::System::getSingleton().setGUISheet(win); }

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
