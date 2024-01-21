namespace Color {

class StrikeThrough : public ItemIF {
public:
	void Prepare(std::string before) override
	{
		Before = before;
		After ="\e[9m" + Matched + "\e[29m";
	}

	void Filter(std::string &line) override
	{
		String::Replace(line, Before.c_str(), After.c_str());
	}
};

}
