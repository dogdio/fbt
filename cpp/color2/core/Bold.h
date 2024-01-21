namespace Color {

class Bold : public DecorateItem {
public:
	Bold(std::shared_ptr<ItemIF> item) : DecorateItem(item)
	{
	}

	void Prepare(std::string before) override
	{
		Item->Prepare(before);
		Before = before;
		After ="\e[1m" + Matched + "\e[22m";
	}

	void Filter(std::string &line) override
	{
		Item->Filter(line);
		String::Replace(line, Before.c_str(), After.c_str());
	}
};

}
