#ifndef MIXER_HH
#define MIXER_HH

#include "Observer.hh"
#include "BooleanSetting.hh"
#include "EnumSetting.hh"
#include "IntegerSetting.hh"
#include "noncopyable.hh"
#include <vector>
#include <memory>

namespace openmsx {

class SoundDriver;
class Reactor;
class CommandController;
class MSXMixer;

class Mixer final : private Observer<Setting>, private noncopyable
{
public:
	enum SoundDriverType { SND_NULL, SND_SDL, SND_DIRECTX };

	Mixer(Reactor& reactor, CommandController& commandController);
	~Mixer();

	/** Register per-machine mixer
	 */
	void registerMixer(MSXMixer& mixer);

	/** Unregister per-machine mixer
	 */
	void unregisterMixer(MSXMixer& mixer);

	/**
	 * This methods (un)mute the sound.
	 * These methods may be called multiple times, as long as
	 * you never call unmute() more than mute()
	 */
	void mute();
	void unmute();

	// Called by MSXMixer

	/** Upload new sample data
	 */
	void uploadBuffer(MSXMixer& msxMixer, short* buffer, unsigned len);

	IntegerSetting& getMasterVolume() { return masterVolume; }

private:
	void reloadDriver();
	void muteHelper();

	// Observer<Setting>
	void update(const Setting& setting) override;

	std::vector<MSXMixer*> msxMixers;

	std::unique_ptr<SoundDriver> driver;
	Reactor& reactor;
	CommandController& commandController;

	EnumSetting<SoundDriverType> soundDriverSetting;
	BooleanSetting muteSetting;
	IntegerSetting masterVolume;
	IntegerSetting frequencySetting;
	IntegerSetting samplesSetting;

	int muteCount;
};

} // namespace openmsx

#endif
