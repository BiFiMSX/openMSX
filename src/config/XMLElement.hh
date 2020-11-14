#ifndef XMLELEMENT_HH
#define XMLELEMENT_HH

#include "serialize_meta.hh"
#include <utility>
#include <string>
#include <string_view>
#include <vector>
#include <memory>

namespace openmsx {

class FileContext;

class XMLElement
{
public:
	//
	// Basic functions
	//

	// Construction.
	//  (copy, assign, move, destruct are default)
	XMLElement() = default;
	explicit XMLElement(std::string name_)
		: name(std::move(name_)) {}
	XMLElement(std::string name_, std::string data_)
		: name(std::move(name_)), data(std::move(data_)) {}

	// name
	[[nodiscard]] const std::string& getName() const { return name; }
	void setName(std::string name_) { name = std::move(name_); }
	void clearName() { name.clear(); }

	// data
	[[nodiscard]] const std::string& getData() const { return data; }
	void setData(std::string data_) {
		assert(children.empty()); // no mixed-content elements
		data = std::move(data_);
	}

	// attribute
	void addAttribute(std::string name, std::string value);
	void setAttribute(std::string_view name, std::string value);
	void removeAttribute(std::string_view name);
	[[nodiscard]] bool hasAttribute(std::string_view name) const;
	[[nodiscard]] const std::string& getAttribute(std::string_view attrName) const;
	[[nodiscard]] std::string_view getAttribute(std::string_view attrName,
	                                            std::string_view defaultValue) const;
	// Returns ptr to attribute value, or nullptr when not found.
	[[nodiscard]] const std::string* findAttribute(std::string_view attrName) const;

	// child
	using Children = std::vector<XMLElement>;
	//  note: returned XMLElement& is validated on the next addChild() call
	XMLElement& addChild(std::string name);
	XMLElement& addChild(std::string name, std::string data);
	void removeChild(const XMLElement& child);
	[[nodiscard]] const Children& getChildren() const { return children; }
	[[nodiscard]] bool hasChildren() const { return !children.empty(); }

	//
	// Convenience functions
	//

	// attribute
	[[nodiscard]] bool getAttributeAsBool(std::string_view attrName,
	                                      bool defaultValue = false) const;
	[[nodiscard]] int getAttributeAsInt(std::string_view attrName,
	                                    int defaultValue = 0) const;
	[[nodiscard]] bool findAttributeInt(std::string_view attrName,
	                                    unsigned& result) const;

	// child
	[[nodiscard]] const XMLElement* findChild(std::string_view childName) const;
	[[nodiscard]] XMLElement* findChild(std::string_view childName);
	[[nodiscard]] const XMLElement& getChild(std::string_view childName) const;
	[[nodiscard]] XMLElement& getChild(std::string_view childName);

	[[nodiscard]] const XMLElement* findChildWithAttribute(
		std::string_view childName, std::string_view attrName,
		std::string_view attValue) const;
	[[nodiscard]] XMLElement* findChildWithAttribute(
		std::string_view childName, std::string_view attrName,
		std::string_view attValue);
	[[nodiscard]] const XMLElement* findNextChild(std::string_view name,
	                                              size_t& fromIndex) const;

	[[nodiscard]] std::vector<const XMLElement*> getChildren(std::string_view childName) const;

	XMLElement& getCreateChild(std::string_view childName,
	                           std::string_view defaultValue = {});
	XMLElement& getCreateChildWithAttribute(
		std::string_view childName, std::string_view attrName,
		std::string_view attValue);

	[[nodiscard]] const std::string& getChildData(std::string_view childName) const;
	[[nodiscard]] std::string_view getChildData(std::string_view childName,
	                                            std::string_view defaultValue) const;
	[[nodiscard]] bool getChildDataAsBool(std::string_view childName,
	                                      bool defaultValue = false) const;
	[[nodiscard]] int getChildDataAsInt(std::string_view childName,
	                                    int defaultValue = 0) const;
	void setChildData(std::string_view childName, std::string value);

	void removeAllChildren();

	// various
	[[nodiscard]] std::string dump() const;
	[[nodiscard]] static std::string XMLEscape(std::string_view str);

	template<typename Archive>
	void serialize(Archive& ar, unsigned version);

	// For backwards compatibility with older savestates
	static std::unique_ptr<FileContext> getLastSerializedFileContext();

private:
	using Attribute = std::pair<std::string, std::string>;
	using Attributes = std::vector<Attribute>;
	[[nodiscard]] Attributes::iterator getAttributeIter(std::string_view attrName);
	[[nodiscard]] Attributes::const_iterator getAttributeIter(std::string_view attrName) const;
	void dump(std::string& result, unsigned indentNum) const;

	std::string name;
	std::string data;
	Children children;
	Attributes attributes;
};
SERIALIZE_CLASS_VERSION(XMLElement, 2);

} // namespace openmsx

#endif
