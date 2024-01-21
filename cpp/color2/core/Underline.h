namespace Color {

class Underline : public ItemIF {
public:
	void Prepare(std::string before) override
	{
		Before = before;
		After ="\e[4m" + Matched + "\e[24m";
	}

	void Filter(std::string &line) override
	{
		String::Replace(line, Before.c_str(), After.c_str());
	}
};

}
