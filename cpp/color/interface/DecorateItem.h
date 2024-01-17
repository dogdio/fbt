#ifndef DECORATE_ITEM_H
#define DECORATE_ITEM_H

#include <map>

namespace Color {

	class DecorateItem : public ItemIF {
	public:
		DecorateItem(ItemIF *item) { Item = item; }
		virtual ~DecorateItem() {}

	protected:
		ItemIF *Item;

		std::map<std::string, std::string> ColorList = {
			{"black", "0"},
			{"red", "1"},
			{"green", "2"},
			{"yellow", "3"},
			{"blue", "4"},
			{"pink", "5"},
			{"cyan", "6"},
			{"white", "7"},
		};
	};
}

#endif
