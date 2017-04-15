#include <string>
#include <sstream>
#include <regex>

namespace NodeDOM {

	using namespace std;

	class TagBuildr {
		string tag, attrs, classes = "", id = "", attrStr = "";
		string selfClosingTags[3] = {
			"img", "br", "hr"
		};
		stringstream stream;

		void parse(string);
		void buildAttrs();
		string trim(string);
		bool isSelfClosing(string);
	public:
		string render(string, string);
	};

	void TagBuildr::parse(string tagString) {
		regex rgx("(.[^\\||\\.|#]*)");

		int attrPos = tagString.find("|");

		if (attrPos != -1) {
			attrStr = tagString.substr(attrPos);
			buildAttrs();
			tagString = tagString.substr(0, attrPos);
		}

		for (std::sregex_iterator i = std::sregex_iterator(tagString.begin(), tagString.end(), rgx);
			i != std::sregex_iterator();
			++i) {
			smatch m = *i;
			string str = m.str();

			if (str.find(".") != -1) {
				classes += str.substr(1) + " ";
			}
			else if (str.find("#") != -1) {
				id = str.substr(1);
			}
			else {
				tag = str;
			}
		}

		if (!classes.empty()) {
			classes = " class=\"" + trim(classes) + "\"";
		}

		if (!id.empty()) {
			id = " id=\"" + id + "\"";
		}

	}

	void TagBuildr::buildAttrs() {
		regex rgx("\\|(.*?)=(.[^\\|]*)");
		for (std::sregex_iterator i = std::sregex_iterator(attrStr.begin(), attrStr.end(), rgx);
			i != std::sregex_iterator();
			++i) {
			smatch m = *i;
			attrs += m[1].str() + "=\"" + m[2].str() + "\" ";
		}

		attrs.erase(attrs.length() - 1, 1);
	}

	string TagBuildr::render(string tagString, string children = "") {
		parse(tagString);

		stream << "<" << tag << id << classes;

		if (!attrStr.empty()) {
			stream << " " << attrs;
		}
		
		if (!isSelfClosing(tag)) {
			stream << ">";
			if (!children.empty()) {
				stream << children;
			}

			stream << "</" << tag << ">";
		}
		else {
			stream << " />";
		}

		return stream.str();

	}

	
	string TagBuildr::trim(string str) {
		// right trim
		while (str.length() > 0 && (str[str.length() - 1] == ' ' || str[str.length() - 1] == '\t'))
			str.erase(str.length() - 1, 1);

		// left trim
		while (str.length() > 0 && (str[0] == ' ' || str[0] == '\t'))
			str.erase(0, 1);

		return str;
	}

	bool TagBuildr::isSelfClosing(string tag) {
		return selfClosingTags->find(tag) != -1;
	}

	

	
}