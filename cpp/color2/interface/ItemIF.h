#ifndef ITEM_IF_H
#define ITEM_IF_H

namespace Color {

	class ItemIF {
	public:
		ItemIF() {}
		virtual ~ItemIF() {}

		virtual void Prepare(std::string before) {}
		virtual void Filter(std::string &line) {}

	protected:
		const std::string Matched = "$&";
		std::string Before;
		std::string After;
	};
}

#endif


