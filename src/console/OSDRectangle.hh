#ifndef OSDRECTANGLE_HH
#define OSDRECTANGLE_HH

#include "OSDImageBasedWidget.hh"
#include <memory>

namespace openmsx {

class BaseImage;

class OSDRectangle final : public OSDImageBasedWidget
{
public:
	OSDRectangle(Display& display, const TclObject& name);

	std::vector<string_ref> getProperties() const override;
	void setProperty(Interpreter& interp,
	                 string_ref name, const TclObject& value) override;
	void getProperty(string_ref name, TclObject& result) const override;
	string_ref getType() const override;

private:
	bool takeImageDimensions() const;

	gl::vec2 getSize(const OutputSurface& output) const override;
	uint8_t getFadedAlpha() const override;
	std::unique_ptr<BaseImage> createSDL(OutputSurface& output) override;
	std::unique_ptr<BaseImage> createGL (OutputSurface& output) override;
	template <typename IMAGE> std::unique_ptr<BaseImage> create(
		OutputSurface& output);

	std::string imageName;
	gl::vec2 size, relSize;
	float scale, borderSize, relBorderSize;
	unsigned borderRGBA;
};

} // namespace openmsx

#endif
