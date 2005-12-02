// $Id$

#include "Scaler.hh"
#include "SimpleScaler.hh"
#include "SaI2xScaler.hh"
#include "SaI3xScaler.hh"
#include "Scale2xScaler.hh"
#include "Scale3xScaler.hh"
#include "HQ2xScaler.hh"
#include "HQ3xScaler.hh"
#include "HQ2xLiteScaler.hh"
#include "HQ3xLiteScaler.hh"
#include "RGBTriplet3xScaler.hh"
#include "Simple3xScaler.hh"
#include "LowScaler.hh"
#include <cassert>

using std::auto_ptr;

namespace openmsx {

template <class Pixel>
auto_ptr<Scaler<Pixel> > Scaler<Pixel>::createScaler(
	ScalerID id, SDL_PixelFormat* format, RenderSettings& renderSettings)
{
	switch(id) {
	case SCALER_SIMPLE:
		return auto_ptr<Scaler<Pixel> >(
			new SimpleScaler<Pixel>(format, renderSettings));
	case SCALER_SAI2X:
		return auto_ptr<Scaler<Pixel> >(
			new SaI2xScaler<Pixel>(format));
	case SCALER_SAI3X:
		return auto_ptr<Scaler<Pixel> >(
			new SaI3xScaler<Pixel>(format));
	case SCALER_SCALE2X:
		return auto_ptr<Scaler<Pixel> >(
			new Scale2xScaler<Pixel>(format));
	case SCALER_SCALE3X:
		return auto_ptr<Scaler<Pixel> >(
			new Scale3xScaler<Pixel>(format));
	case SCALER_HQ2X:
		return auto_ptr<Scaler<Pixel> >(
			new HQ2xScaler<Pixel>(format));
	case SCALER_HQ3X:
		return auto_ptr<Scaler<Pixel> >(
			new HQ3xScaler<Pixel>(format));
	case SCALER_HQ2XLITE:
		return auto_ptr<Scaler<Pixel> >(
			new HQ2xLiteScaler<Pixel>(format));
	case SCALER_HQ3XLITE:
		return auto_ptr<Scaler<Pixel> >(
			new HQ3xLiteScaler<Pixel>(format));
	case SCALER_RGBTRIPLET3X:
		return auto_ptr<Scaler<Pixel> >(
			new RGBTriplet3xScaler<Pixel>(format, renderSettings));
	case SCALER_SIMPLE3X:
		return auto_ptr<Scaler<Pixel> >(
			new Simple3xScaler<Pixel>(format, renderSettings));
	case SCALER_LOW:
		return auto_ptr<Scaler<Pixel> >(
			new LowScaler<Pixel>(format));
	default:
		assert(false);
		return auto_ptr<Scaler<Pixel> >();
	}
}

// Force template instantiation.
template class Scaler<word>;
template class Scaler<unsigned>;

} // namespace openmsx

