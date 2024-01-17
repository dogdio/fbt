namespace Color {

class SwapColor : public DecorateItem {
public:
	SwapColor(ItemIF *item) : DecorateItem(item)
	{
	}

	void Prepare(std::string before) override
	{
		Item->Prepare(before);
		Before = before;
		After ="\e[7m" + Matched + "\e[27m";
	}

	void Filter(std::string &line) override
	{
		Item->Filter(line);
		String::Replace(line, Before.c_str(), After.c_str());
	}

};

}
