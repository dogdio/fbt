namespace Color {

class Blink : public DecorateItem {
public:
	Blink(ItemIF *item) : DecorateItem(item)
	{
	}

	void Prepare(std::string before) override
	{
		Item->Prepare(before);
		Before = before;
		After ="\e[5m" + Matched + "\e[25m";
	}

	void Filter(std::string &line) override
	{
		Item->Filter(line);
		String::Replace(line, Before.c_str(), After.c_str());
	}
};

}
