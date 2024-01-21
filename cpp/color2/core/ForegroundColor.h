namespace Color {

class ForegroundColor : public DecorateItem {
public:
	ForegroundColor(std::string color, std::shared_ptr<ItemIF> item) :
		DecorateItem(item), Color(color)
	{
		try {
			ColorList.at(Color);
		}
		catch(std::exception &e) {
			throw(std::invalid_argument("not found"));
		}
	}

	void Prepare(std::string before) override
	{
		Item->Prepare(before);
		Before = before;
		After ="\e[3" + ColorList[Color] + "m" + Matched + "\e[39m";
	}

	void Filter(std::string &line) override
	{
		Item->Filter(line);
		String::Replace(line, Before.c_str(), After.c_str());
	}

private:
	std::string Color;
};

}
