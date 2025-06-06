#include <QSettings>
#include <QColor>
#include <QFile>

#ifdef OS_WINDOWS
#include <windows.h>
#else
#include "../winlnxdefs.h"
#endif
#include "../GBI.h"
#include "../Config.h"

#include "Settings.h"

static const char * strIniFileName = "GLideN64.ini";
static const char * strDefaultIniFileName = "GLideN64.default.ini";
static const char * strCustomSettingsFileName = "GLideN64.custom.ini";
static QString strUserProfile("User");

static
void _loadSettings(QSettings & settings)
{
	config.version = settings.value("version").toInt();

	settings.beginGroup("video");
	config.video.fullscreenWidth = settings.value("fullscreenWidth", config.video.fullscreenWidth).toInt();
	config.video.fullscreenHeight = settings.value("fullscreenHeight", config.video.fullscreenHeight).toInt();
	config.video.windowedWidth = settings.value("windowedWidth", config.video.windowedWidth).toInt();
	config.video.windowedHeight = settings.value("windowedHeight", config.video.windowedHeight).toInt();
	config.video.borderless = settings.value("borderless", config.video.borderless).toInt();
	config.video.fullscreenRefresh = settings.value("fullscreenRefresh", config.video.fullscreenRefresh).toInt();
	config.video.multisampling = settings.value("multisampling", config.video.multisampling).toInt();
	config.video.maxMultiSampling = settings.value("maxMultiSampling", config.video.maxMultiSampling).toInt();
	config.video.fxaa= settings.value("fxaa", config.video.fxaa).toInt();
	config.video.verticalSync = settings.value("verticalSync", config.video.verticalSync).toInt();
	config.video.threadedVideo = settings.value("threadedVideo", config.video.threadedVideo).toInt();
	QString deviceName = QString::fromWCharArray(config.video.deviceName);
	config.video.deviceName[settings.value("deviceName", deviceName).toString().toWCharArray(config.video.deviceName)] = L'\0';
	settings.endGroup();

	settings.beginGroup("texture");
	config.texture.anisotropy = settings.value("anisotropy", config.texture.anisotropy).toInt();
	config.texture.maxAnisotropy = settings.value("maxAnisotropy", config.texture.maxAnisotropy).toInt();
	config.texture.bilinearMode = settings.value("bilinearMode", config.texture.bilinearMode).toInt();
	config.texture.enableHalosRemoval = settings.value("enableHalosRemoval", config.texture.enableHalosRemoval).toInt();
	settings.endGroup();

	settings.beginGroup("generalEmulation");
	config.generalEmulation.enableDitheringPattern = settings.value("enableDitheringPattern", config.generalEmulation.enableDitheringPattern).toInt();
	config.generalEmulation.enableDitheringQuantization = settings.value("enableDitheringQuantization", config.generalEmulation.enableDitheringQuantization).toInt();
	config.generalEmulation.enableHiresNoiseDithering = settings.value("enableHiresNoiseDithering", config.generalEmulation.enableHiresNoiseDithering).toInt();
	config.generalEmulation.rdramImageDitheringMode = settings.value("rdramImageDitheringMode", config.generalEmulation.rdramImageDitheringMode).toInt();
	config.generalEmulation.enableLOD = settings.value("enableLOD", config.generalEmulation.enableLOD).toInt();
	config.generalEmulation.enableInaccurateTextureCoordinates = settings.value("enableInaccurateTextureCoordinates", config.generalEmulation.enableInaccurateTextureCoordinates).toInt();
	config.generalEmulation.enableHWLighting = settings.value("enableHWLighting", config.generalEmulation.enableHWLighting).toInt();
	config.generalEmulation.enableCoverage = settings.value("enableCoverage", config.generalEmulation.enableCoverage).toInt();
	config.generalEmulation.enableShadersStorage = settings.value("enableShadersStorage", config.generalEmulation.enableShadersStorage).toInt();
	config.generalEmulation.enableLegacyBlending = settings.value("enableLegacyBlending", config.generalEmulation.enableLegacyBlending).toInt();			 //ini only
	config.generalEmulation.enableHybridFilter = settings.value("enableHybridFilter", config.generalEmulation.enableHybridFilter).toInt();					 //ini only
	config.generalEmulation.enableFragmentDepthWrite = settings.value("enableFragmentDepthWrite", config.generalEmulation.enableFragmentDepthWrite).toInt(); //ini only
	config.generalEmulation.hacks |= settings.value("hacks", config.generalEmulation.hacks).toInt();														 //ini only
	config.generalEmulation.enableCustomSettings = settings.value("enableCustomSettings", config.generalEmulation.enableCustomSettings).toInt();
	settings.endGroup();

	settings.beginGroup("graphics2D");
	config.graphics2D.correctTexrectCoords = settings.value("correctTexrectCoords", config.graphics2D.correctTexrectCoords).toInt();
	config.graphics2D.enableNativeResTexrects = settings.value("enableNativeResTexrects", config.graphics2D.enableNativeResTexrects).toInt();
	config.graphics2D.bgMode = settings.value("bgMode", config.graphics2D.bgMode).toInt();
	config.graphics2D.enableTexCoordBounds = settings.value("enableTexCoordBounds", config.graphics2D.enableTexCoordBounds).toInt();
	settings.endGroup();

	settings.beginGroup("frameBufferEmulation");
	config.frameBufferEmulation.enable = settings.value("enable", config.frameBufferEmulation.enable).toInt();
	config.frameBufferEmulation.aspect = settings.value("aspect", config.frameBufferEmulation.aspect).toInt();
	config.frameBufferEmulation.nativeResFactor = settings.value("nativeResFactor", config.frameBufferEmulation.nativeResFactor).toInt();
	config.frameBufferEmulation.bufferSwapMode = settings.value("bufferSwapMode", config.frameBufferEmulation.bufferSwapMode).toInt();
	config.frameBufferEmulation.N64DepthCompare = settings.value("N64DepthCompare", config.frameBufferEmulation.N64DepthCompare).toInt();
	config.frameBufferEmulation.forceDepthBufferClear = settings.value("forceDepthBufferClear", config.frameBufferEmulation.forceDepthBufferClear).toInt();
	config.frameBufferEmulation.copyAuxToRDRAM = settings.value("copyAuxToRDRAM", config.frameBufferEmulation.copyAuxToRDRAM).toInt();
	config.frameBufferEmulation.copyToRDRAM = settings.value("copyToRDRAM", config.frameBufferEmulation.copyToRDRAM).toInt();
	config.frameBufferEmulation.copyDepthToRDRAM = settings.value("copyDepthToRDRAM", config.frameBufferEmulation.copyDepthToRDRAM).toInt();
	config.frameBufferEmulation.copyFromRDRAM = settings.value("copyFromRDRAM", config.frameBufferEmulation.copyFromRDRAM).toInt();
	config.frameBufferEmulation.fbInfoDisabled = settings.value("fbInfoDisabled", config.frameBufferEmulation.fbInfoDisabled).toInt();
	config.frameBufferEmulation.fbInfoReadColorChunk = settings.value("fbInfoReadColorChunk", config.frameBufferEmulation.fbInfoReadColorChunk).toInt();
	config.frameBufferEmulation.fbInfoReadDepthChunk = settings.value("fbInfoReadDepthChunk", config.frameBufferEmulation.fbInfoReadDepthChunk).toInt();
	config.frameBufferEmulation.copyDepthToMainDepthBuffer = settings.value("copyDepthToMainDepthBuffer", config.frameBufferEmulation.copyDepthToMainDepthBuffer).toInt();
	config.frameBufferEmulation.enableOverscan = settings.value("enableOverscan", config.frameBufferEmulation.enableOverscan).toInt();
	config.frameBufferEmulation.overscanPAL.left = settings.value("overscanPalLeft", config.frameBufferEmulation.overscanPAL.left).toInt();
	config.frameBufferEmulation.overscanPAL.right = settings.value("overscanPalRight", config.frameBufferEmulation.overscanPAL.right).toInt();
	config.frameBufferEmulation.overscanPAL.top = settings.value("overscanPalTop", config.frameBufferEmulation.overscanPAL.top).toInt();
	config.frameBufferEmulation.overscanPAL.bottom= settings.value("overscanPalBottom", config.frameBufferEmulation.overscanPAL.bottom).toInt();
	config.frameBufferEmulation.overscanNTSC.left = settings.value("overscanNtscLeft", config.frameBufferEmulation.overscanNTSC.left).toInt();
	config.frameBufferEmulation.overscanNTSC.right = settings.value("overscanNtscRight", config.frameBufferEmulation.overscanNTSC.right).toInt();
	config.frameBufferEmulation.overscanNTSC.top = settings.value("overscanNtscTop", config.frameBufferEmulation.overscanNTSC.top).toInt();
	config.frameBufferEmulation.overscanNTSC.bottom = settings.value("overscanNtscBottom", config.frameBufferEmulation.overscanNTSC.bottom).toInt();
	settings.endGroup();

	settings.beginGroup("textureFilter");
	config.textureFilter.txFilterMode = settings.value("txFilterMode", config.textureFilter.txFilterMode).toInt();
	config.textureFilter.txEnhancementMode = settings.value("txEnhancementMode", config.textureFilter.txEnhancementMode).toInt();
	config.textureFilter.txDeposterize = settings.value("txDeposterize", config.textureFilter.txDeposterize).toInt();
	config.textureFilter.txFilterIgnoreBG = settings.value("txFilterIgnoreBG", config.textureFilter.txFilterIgnoreBG).toInt();
	config.textureFilter.txCacheSize = settings.value("txCacheSize", config.textureFilter.txCacheSize).toInt();
	config.textureFilter.txHiresEnable = settings.value("txHiresEnable", config.textureFilter.txHiresEnable).toInt();
	config.textureFilter.txHiresFullAlphaChannel = settings.value("txHiresFullAlphaChannel", config.textureFilter.txHiresFullAlphaChannel).toInt();
	config.textureFilter.txHresAltCRC = settings.value("txHresAltCRC", config.textureFilter.txHresAltCRC).toInt();
	config.textureFilter.txStrongCRC = settings.value("txStrongCRC", config.textureFilter.txStrongCRC).toInt();
	config.textureFilter.txForce16bpp = settings.value("txForce16bpp", config.textureFilter.txForce16bpp).toInt();
	config.textureFilter.txCacheCompression = settings.value("txCacheCompression", config.textureFilter.txCacheCompression).toInt();
	config.textureFilter.txSaveCache = settings.value("txSaveCache", config.textureFilter.txSaveCache).toInt();
	config.textureFilter.txEnhancedTextureFileStorage = settings.value("txEnhancedTextureFileStorage", config.textureFilter.txEnhancedTextureFileStorage).toInt();
	config.textureFilter.txHiresTextureFileStorage = settings.value("txHiresTextureFileStorage", config.textureFilter.txHiresTextureFileStorage).toInt();
	config.textureFilter.txNoTextureFileStorage = settings.value("txNoTextureFileStorage", config.textureFilter.txNoTextureFileStorage).toInt();
	config.textureFilter.txHiresVramLimit = settings.value("txHiresVramLimit", config.textureFilter.txHiresVramLimit).toInt();
	QString txPath = QString::fromWCharArray(config.textureFilter.txPath);
	config.textureFilter.txPath[settings.value("txPath", txPath).toString().toWCharArray(config.textureFilter.txPath)] = L'\0';
	QString txCachePath = QString::fromWCharArray(config.textureFilter.txCachePath);
	config.textureFilter.txCachePath[settings.value("txCachePath", txCachePath).toString().toWCharArray(config.textureFilter.txCachePath)] = L'\0';
	QString txDumpPath = QString::fromWCharArray(config.textureFilter.txDumpPath);
	config.textureFilter.txDumpPath[settings.value("txDumpPath", txDumpPath).toString().toWCharArray(config.textureFilter.txDumpPath)] = L'\0';

	settings.endGroup();

	settings.beginGroup("font");
	config.font.name = settings.value("name", config.font.name.c_str()).toString().toLocal8Bit().constData();
	config.font.size = settings.value("size", config.font.size).toInt();
	QColor fontColor = settings.value("color", QColor(config.font.color[0], config.font.color[1], config.font.color[2])).value<QColor>();
	config.font.color[0] = fontColor.red();
	config.font.color[1] = fontColor.green();
	config.font.color[2] = fontColor.blue();
	config.font.color[3] = fontColor.alpha();
	config.font.colorf[0] = _FIXED2FLOAT(config.font.color[0], 8);
	config.font.colorf[1] = _FIXED2FLOAT(config.font.color[1], 8);
	config.font.colorf[2] = _FIXED2FLOAT(config.font.color[2], 8);
	config.font.colorf[3] = config.font.color[3] == 0 ? 1.0f : _FIXED2FLOAT(config.font.color[3], 8);
	settings.endGroup();

	settings.beginGroup("gammaCorrection");
	config.gammaCorrection.force = settings.value("force", config.gammaCorrection.force).toInt();
	config.gammaCorrection.level = settings.value("level", config.gammaCorrection.level).toFloat();
	settings.endGroup();

	settings.beginGroup("onScreenDisplay");
	config.onScreenDisplay.fps = settings.value("showFPS", config.onScreenDisplay.fps).toInt();
	config.onScreenDisplay.vis = settings.value("showVIS", config.onScreenDisplay.vis).toInt();
	config.onScreenDisplay.percent = settings.value("showPercent", config.onScreenDisplay.percent).toInt();
	config.onScreenDisplay.internalResolution = settings.value("showInternalResolution", config.onScreenDisplay.internalResolution).toInt();
	config.onScreenDisplay.renderingResolution = settings.value("showRenderingResolution", config.onScreenDisplay.renderingResolution).toInt();
	config.onScreenDisplay.statistics = settings.value("showStatistics", config.onScreenDisplay.statistics).toInt();
	config.onScreenDisplay.pos = settings.value("osdPos", config.onScreenDisplay.pos).toInt();
	settings.endGroup();

	settings.beginGroup("hotkeys");
	for (u32 idx = 0; idx < Config::HotKey::hkTotal; ++idx) {
		config.hotkeys.keys[idx] = settings.value(Config::hotkeyIniName(idx), config.hotkeys.keys[idx]).toInt();
		config.hotkeys.enabledKeys[idx] = settings.value(Config::enabledHotkeyIniName(idx), config.hotkeys.enabledKeys[idx]).toInt();
	}
	settings.endGroup();

	settings.beginGroup("debug");
	config.debug.dumpMode = settings.value("dumpMode", config.debug.dumpMode).toInt();
	settings.endGroup();
}

static
void _writeSettingsToFile(const QString & filename)
{
//	QSettings settings("Emulation", "GLideN64");
	QSettings settings(filename, QSettings::IniFormat);
	settings.setValue("version", config.version);
	settings.setValue("translation", config.translationFile.c_str());
	QString profile = settings.value("profile", strUserProfile).toString();

	settings.beginGroup(profile);
	settings.setValue("version", config.version);

	settings.beginGroup("video");
	settings.setValue("fullscreenWidth", config.video.fullscreenWidth);
	settings.setValue("fullscreenHeight", config.video.fullscreenHeight);
	settings.setValue("windowedWidth", config.video.windowedWidth);
	settings.setValue("windowedHeight", config.video.windowedHeight);
	settings.setValue("borderless", config.video.borderless);
	settings.setValue("fullscreenRefresh", config.video.fullscreenRefresh);
	settings.setValue("multisampling", config.video.multisampling);
	settings.setValue("maxMultiSampling", config.video.maxMultiSampling);
	settings.setValue("fxaa", config.video.fxaa);
	settings.setValue("verticalSync", config.video.verticalSync);
	settings.setValue("threadedVideo", config.video.threadedVideo);
	settings.setValue("deviceName", QString::fromWCharArray(config.video.deviceName));
	settings.endGroup();

	settings.beginGroup("texture");
	settings.setValue("anisotropy", config.texture.anisotropy);
	settings.setValue("maxAnisotropy", config.texture.maxAnisotropy);
	settings.setValue("bilinearMode", config.texture.bilinearMode);
	settings.setValue("enableHalosRemoval", config.texture.enableHalosRemoval);
	settings.endGroup();

	settings.beginGroup("generalEmulation");
	settings.setValue("enableDitheringPattern", config.generalEmulation.enableDitheringPattern);
	settings.setValue("enableDitheringQuantization", config.generalEmulation.enableDitheringQuantization);
	settings.setValue("enableHiresNoiseDithering", config.generalEmulation.enableHiresNoiseDithering);
	settings.setValue("rdramImageDitheringMode", config.generalEmulation.rdramImageDitheringMode);
	settings.setValue("enableLOD", config.generalEmulation.enableLOD);
	settings.setValue("enableInaccurateTextureCoordinates", config.generalEmulation.enableInaccurateTextureCoordinates);
	settings.setValue("enableHWLighting", config.generalEmulation.enableHWLighting);
	settings.setValue("enableCoverage", config.generalEmulation.enableCoverage);
	settings.setValue("enableShadersStorage", config.generalEmulation.enableShadersStorage);
	settings.setValue("enableLegacyBlending", config.generalEmulation.enableLegacyBlending);		 //ini only
	settings.setValue("enableHybridFilter", config.generalEmulation.enableHybridFilter);			 //ini only
	settings.setValue("enableFragmentDepthWrite", config.generalEmulation.enableFragmentDepthWrite); //ini only
	settings.setValue("enableCustomSettings", config.generalEmulation.enableCustomSettings);
	settings.endGroup();

	settings.beginGroup("graphics2D");
	settings.setValue("correctTexrectCoords", config.graphics2D.correctTexrectCoords);
	settings.setValue("enableNativeResTexrects", config.graphics2D.enableNativeResTexrects);
	settings.setValue("bgMode", config.graphics2D.bgMode);
	settings.setValue("enableTexCoordBounds", config.graphics2D.enableTexCoordBounds);
	settings.endGroup();

	settings.beginGroup("frameBufferEmulation");
	settings.setValue("enable", config.frameBufferEmulation.enable);
	settings.setValue("aspect", config.frameBufferEmulation.aspect);
	settings.setValue("nativeResFactor", config.frameBufferEmulation.nativeResFactor);
	settings.setValue("bufferSwapMode", config.frameBufferEmulation.bufferSwapMode);
	settings.setValue("N64DepthCompare", config.frameBufferEmulation.N64DepthCompare);
	settings.setValue("forceDepthBufferClear", config.frameBufferEmulation.forceDepthBufferClear);
	settings.setValue("copyAuxToRDRAM", config.frameBufferEmulation.copyAuxToRDRAM);
	settings.setValue("copyFromRDRAM", config.frameBufferEmulation.copyFromRDRAM);
	settings.setValue("copyToRDRAM", config.frameBufferEmulation.copyToRDRAM);
	settings.setValue("copyDepthToRDRAM", config.frameBufferEmulation.copyDepthToRDRAM);
	settings.setValue("fbInfoDisabled", config.frameBufferEmulation.fbInfoDisabled);
	settings.setValue("fbInfoReadColorChunk", config.frameBufferEmulation.fbInfoReadColorChunk);
	settings.setValue("fbInfoReadDepthChunk", config.frameBufferEmulation.fbInfoReadDepthChunk);
	settings.setValue("copyDepthToMainDepthBuffer", config.frameBufferEmulation.copyDepthToMainDepthBuffer);
	settings.setValue("enableOverscan", config.frameBufferEmulation.enableOverscan);
	settings.setValue("overscanPalLeft", config.frameBufferEmulation.overscanPAL.left);
	settings.setValue("overscanPalRight", config.frameBufferEmulation.overscanPAL.right);
	settings.setValue("overscanPalTop", config.frameBufferEmulation.overscanPAL.top);
	settings.setValue("overscanPalBottom", config.frameBufferEmulation.overscanPAL.bottom);
	settings.setValue("overscanNtscLeft", config.frameBufferEmulation.overscanNTSC.left);
	settings.setValue("overscanNtscRight", config.frameBufferEmulation.overscanNTSC.right);
	settings.setValue("overscanNtscTop", config.frameBufferEmulation.overscanNTSC.top);
	settings.setValue("overscanNtscBottom", config.frameBufferEmulation.overscanNTSC.bottom);
	settings.endGroup();

	settings.beginGroup("textureFilter");
	settings.setValue("txFilterMode", config.textureFilter.txFilterMode);
	settings.setValue("txEnhancementMode", config.textureFilter.txEnhancementMode);
	settings.setValue("txDeposterize", config.textureFilter.txDeposterize);
	settings.setValue("txFilterIgnoreBG", config.textureFilter.txFilterIgnoreBG);
	settings.setValue("txCacheSize", config.textureFilter.txCacheSize);
	settings.setValue("txHiresEnable", config.textureFilter.txHiresEnable);
	settings.setValue("txHiresFullAlphaChannel", config.textureFilter.txHiresFullAlphaChannel);
	settings.setValue("txHresAltCRC", config.textureFilter.txHresAltCRC);
	settings.setValue("txStrongCRC", config.textureFilter.txStrongCRC);
	settings.setValue("txForce16bpp", config.textureFilter.txForce16bpp);
	settings.setValue("txCacheCompression", config.textureFilter.txCacheCompression);
	settings.setValue("txSaveCache", config.textureFilter.txSaveCache);
	settings.setValue("txEnhancedTextureFileStorage", config.textureFilter.txEnhancedTextureFileStorage);
	settings.setValue("txHiresTextureFileStorage", config.textureFilter.txHiresTextureFileStorage);
	settings.setValue("txNoTextureFileStorage", config.textureFilter.txNoTextureFileStorage);
	settings.setValue("txHiresVramLimit", config.textureFilter.txHiresVramLimit);
	settings.setValue("txPath", QString::fromWCharArray(config.textureFilter.txPath));
	settings.setValue("txCachePath", QString::fromWCharArray(config.textureFilter.txCachePath));
	settings.setValue("txDumpPath", QString::fromWCharArray(config.textureFilter.txDumpPath));
	settings.endGroup();

	settings.beginGroup("font");
	settings.setValue("name", config.font.name.c_str());
	settings.setValue("size", config.font.size);
	settings.setValue("color", QColor(config.font.color[0], config.font.color[1], config.font.color[2], config.font.color[3]));
	settings.endGroup();

	settings.beginGroup("gammaCorrection");
	settings.setValue("force", config.gammaCorrection.force);
	settings.setValue("level", config.gammaCorrection.level);
	settings.endGroup();

	settings.beginGroup("onScreenDisplay");
	settings.setValue("showFPS", config.onScreenDisplay.fps);
	settings.setValue("showVIS", config.onScreenDisplay.vis);
	settings.setValue("showPercent", config.onScreenDisplay.percent);
	settings.setValue("showInternalResolution", config.onScreenDisplay.internalResolution);
	settings.setValue("showRenderingResolution", config.onScreenDisplay.renderingResolution);
	settings.setValue("showStatistics", config.onScreenDisplay.statistics);
	settings.setValue("osdPos", config.onScreenDisplay.pos);
	settings.endGroup();

	settings.beginGroup("hotkeys");
	for (u32 idx = 0; idx < Config::HotKey::hkTotal; ++idx) {
		settings.setValue(Config::hotkeyIniName(idx), config.hotkeys.keys[idx]);
		settings.setValue(Config::enabledHotkeyIniName(idx), config.hotkeys.enabledKeys[idx]);
	}
	settings.endGroup();

	settings.beginGroup("debug");
	settings.setValue("dumpMode", config.debug.dumpMode);
	settings.endGroup();

	settings.endGroup();
}

static
void _loadSettingsFromFile(const QString & filename)
{
	bool rewriteSettings = false;
	{
		const u32 hacks = config.generalEmulation.hacks;
		QSettings settings(filename, QSettings::IniFormat);
		const u32 configVersion = settings.value("version", 0).toInt();
		QString configTranslationFile = settings.value("translation", config.translationFile.c_str()).toString();
		config.resetToDefaults();
		config.generalEmulation.hacks = hacks;
		config.translationFile = configTranslationFile.toLocal8Bit().constData();
		if (configVersion < CONFIG_WITH_PROFILES) {
			_loadSettings(settings);
			config.version = CONFIG_VERSION_CURRENT;
			settings.clear();
			settings.setValue("version", CONFIG_VERSION_CURRENT);
			settings.setValue("profile", strUserProfile);
			settings.setValue("translation", config.translationFile.c_str());
			settings.beginGroup(strUserProfile);
			_writeSettingsToFile(filename);
			settings.endGroup();
		}
		QString profile = settings.value("profile", strUserProfile).toString();
		if (settings.childGroups().indexOf(profile) >= 0) {
			settings.beginGroup(profile);
			_loadSettings(settings);
			settings.endGroup();
		} else
			rewriteSettings = true;
		if (config.version != CONFIG_VERSION_CURRENT)
			rewriteSettings = true;
	}
	if (rewriteSettings) {
		// Keep settings up-to-date
		{
			QSettings settings(filename, QSettings::IniFormat);
			QString profile = settings.value("profile", strUserProfile).toString();
			settings.remove(profile);
		}
		config.version = CONFIG_VERSION_CURRENT;
		_writeSettingsToFile(filename);
	}
}

void loadSettings(const QString & _strIniFolder, const QString & _strSharedIniFolder)
{
	QString sharedSettingsFilename = _strSharedIniFolder + "/" + strIniFileName;
	QString settingsFilename = _strIniFolder + "/" + strIniFileName;
	QFile settingsFile(settingsFilename);
	QFile sharedSettingsFile(sharedSettingsFilename);

	// fallback to shared file if no config file exists
	// in the config directory yet
	if (sharedSettingsFile.exists() && !settingsFile.exists()) {
		_loadSettingsFromFile(sharedSettingsFilename);
	} else {
		_loadSettingsFromFile(settingsFilename);
	}
}

void writeSettings(const QString & _strIniFolder)
{
	_writeSettingsToFile(_strIniFolder + "/" + strIniFileName);
}

void resetSettings(const QString & _strIniFolder)
{
	QString defaultSettingsFilename = _strIniFolder + "/" + strDefaultIniFileName;
	QFile defaultFile(defaultSettingsFilename);
	if (defaultFile.exists()) {
		_loadSettingsFromFile(defaultSettingsFilename);
	} else {
		config.resetToDefaults();
	}
}

static
u32 Adler32(u32 crc, const void *buffer, u32 count)
{
	u32 s1 = crc & 0xFFFF;
	u32 s2 = (crc >> 16) & 0xFFFF;
	int k;
	const u8 *Buffer = (const u8*)buffer;

	if (Buffer == NULL)
		return 0;

	while (count > 0) {
		/* 5552 is the largest n such that 255n(n+1)/2 + (n+1)(BASE-1) <= 2^32-1 */
		k = (count < 5552 ? count : 5552);
		count -= k;
		while (k--) {
			s1 += *Buffer++;
			s2 += s1;
		}
		/* 65521 is the largest prime smaller than 65536 */
		s1 %= 65521;
		s2 %= 65521;
	}

	return (s2 << 16) | s1;
}

static
QString _getRomName(const char * _strRomName) {
	const QByteArray bytes(_strRomName);
	bool bASCII = true;
	for (int i = 0; i < bytes.length() && bASCII; ++i)
		bASCII = bytes.at(i) >= 0;

	return bASCII ?
		QString::fromLatin1(_strRomName).toUpper() :
		QString::number(Adler32(0xFFFFFFFF, bytes.data(), bytes.length()), 16).toUpper();
}

void loadCustomRomSettings(const QString & _strIniFolder, const QString & _strSharedIniFolder, const char * _strRomName)
{
	QSettings settings(_strIniFolder + "/" + strCustomSettingsFileName, QSettings::IniFormat);
	QSettings sharedSettings(_strSharedIniFolder + "/" + strCustomSettingsFileName, QSettings::IniFormat);

	const QString romName = _getRomName(_strRomName);
	if (settings.childGroups().indexOf(romName) < 0 &&
		sharedSettings.childGroups().indexOf(romName) < 0) {
		return;
	}

	if (settings.childGroups().indexOf(romName) >= 0) {
		// use user settings
		settings.beginGroup(romName);
		_loadSettings(settings);
		settings.endGroup();
	} else {
		// use shared settings
		sharedSettings.beginGroup(romName);
		_loadSettings(sharedSettings);
		sharedSettings.endGroup();
	}

	config.version = CONFIG_VERSION_CURRENT;
}

void saveCustomRomSettings(const QString & _strIniFolder, const QString & _strSharedIniFolder, const char * _strRomName)
{
	Config origConfig;
	origConfig.resetToDefaults();
	std::swap(config, origConfig);
	loadSettings(_strIniFolder, _strSharedIniFolder);
	std::swap(config, origConfig);

	QSettings settings(_strIniFolder + "/" + strCustomSettingsFileName, QSettings::IniFormat);
	const QString romName = _getRomName(_strRomName);

#define WriteCustomSetting(G, S) \
	if (origConfig.G.S  != config.G.S || \
		origConfig.G.S != settings.value(#S, config.G.S).toInt()) \
		settings.setValue(#S, config.G.S)
#define WriteCustomSetting2(G, N, S) \
	if (origConfig.G.S  != config.G.S || \
		origConfig.G.S != settings.value(#N, config.G.S).toInt()) \
		settings.setValue(#N, config.G.S)
#define WriteCustomSettingF(G, S) \
	if (origConfig.G.S  != config.G.S || \
		origConfig.G.S != settings.value(#S, config.G.S).toFloat()) \
		settings.setValue(#S, config.G.S)
#define WriteCustomSettingS(S) \
	const QString new##S = QString::fromWCharArray(config.textureFilter.S); \
	const QString orig##S = QString::fromWCharArray(origConfig.textureFilter.S); \
	if (orig##S  != new##S || \
		orig##S != settings.value(#S, new##S).toString()) \
		settings.setValue(#S, new##S)

	settings.beginGroup(romName);

	settings.beginGroup("video");
	WriteCustomSetting(video, fullscreenWidth);
	WriteCustomSetting(video, fullscreenHeight);
	WriteCustomSetting(video, windowedWidth);
	WriteCustomSetting(video, windowedHeight);
	WriteCustomSetting(video, borderless);
	WriteCustomSetting(video, fullscreenRefresh);
	WriteCustomSetting(video, multisampling);
	WriteCustomSetting(video, fxaa);
	WriteCustomSetting(video, verticalSync);
	settings.endGroup();

	settings.beginGroup("texture");
	WriteCustomSetting(texture, anisotropy);
	WriteCustomSetting(texture, maxAnisotropy);
	WriteCustomSetting(texture, bilinearMode);
	WriteCustomSetting(texture, enableHalosRemoval);
	settings.endGroup();

	settings.beginGroup("generalEmulation");
	WriteCustomSetting(generalEmulation, enableDitheringPattern);
	WriteCustomSetting(generalEmulation, enableDitheringQuantization);
	WriteCustomSetting(generalEmulation, enableHiresNoiseDithering);
	WriteCustomSetting(generalEmulation, rdramImageDitheringMode);
	WriteCustomSetting(generalEmulation, enableLOD);
	WriteCustomSetting(generalEmulation, enableInaccurateTextureCoordinates);
	WriteCustomSetting(generalEmulation, enableHWLighting);
	WriteCustomSetting(generalEmulation, enableCoverage);
	WriteCustomSetting(generalEmulation, enableShadersStorage);
	settings.endGroup();

	settings.beginGroup("graphics2D");
	WriteCustomSetting(graphics2D, correctTexrectCoords);
	WriteCustomSetting(graphics2D, enableNativeResTexrects);
	WriteCustomSetting(graphics2D, bgMode);
	WriteCustomSetting(graphics2D, enableTexCoordBounds);
	settings.endGroup();

	settings.beginGroup("frameBufferEmulation");
	WriteCustomSetting(frameBufferEmulation, enable);
	WriteCustomSetting(frameBufferEmulation, aspect);
	WriteCustomSetting(frameBufferEmulation, nativeResFactor);
	WriteCustomSetting(frameBufferEmulation, bufferSwapMode);
	WriteCustomSetting(frameBufferEmulation, N64DepthCompare);
	WriteCustomSetting(frameBufferEmulation, forceDepthBufferClear);
	WriteCustomSetting(frameBufferEmulation, copyAuxToRDRAM);
	WriteCustomSetting(frameBufferEmulation, copyFromRDRAM);
	WriteCustomSetting(frameBufferEmulation, copyToRDRAM);
	WriteCustomSetting(frameBufferEmulation, copyDepthToRDRAM);
	WriteCustomSetting(frameBufferEmulation, fbInfoDisabled);
	WriteCustomSetting(frameBufferEmulation, fbInfoReadColorChunk);
	WriteCustomSetting(frameBufferEmulation, fbInfoReadDepthChunk);
	WriteCustomSetting(frameBufferEmulation, copyDepthToMainDepthBuffer);
	WriteCustomSetting(frameBufferEmulation, enableOverscan);
	WriteCustomSetting2(frameBufferEmulation, overscanPalLeft, overscanPAL.left);
	WriteCustomSetting2(frameBufferEmulation, overscanPalRight, overscanPAL.right);
	WriteCustomSetting2(frameBufferEmulation, overscanPalTop, overscanPAL.top);
	WriteCustomSetting2(frameBufferEmulation, overscanPalBottom, overscanPAL.bottom);
	WriteCustomSetting2(frameBufferEmulation, overscanNtscLeft, overscanNTSC.left);
	WriteCustomSetting2(frameBufferEmulation, overscanNtscRight, overscanNTSC.right);
	WriteCustomSetting2(frameBufferEmulation, overscanNtscTop, overscanNTSC.top);
	WriteCustomSetting2(frameBufferEmulation, overscanNtscBottom, overscanNTSC.bottom);
	settings.endGroup();

	settings.beginGroup("textureFilter");
	WriteCustomSetting(textureFilter, txFilterMode);
	WriteCustomSetting(textureFilter, txEnhancementMode);
	WriteCustomSetting(textureFilter, txDeposterize);
	WriteCustomSetting(textureFilter, txFilterIgnoreBG);
	WriteCustomSetting(textureFilter, txCacheSize);
	WriteCustomSetting(textureFilter, txEnhancedTextureFileStorage);
	WriteCustomSetting(textureFilter, txHiresTextureFileStorage);
	WriteCustomSetting(textureFilter, txNoTextureFileStorage);
	WriteCustomSetting(textureFilter, txHiresVramLimit);
	WriteCustomSetting(textureFilter, txHiresEnable);
	WriteCustomSetting(textureFilter, txHiresFullAlphaChannel);
	WriteCustomSetting(textureFilter, txHresAltCRC);
	WriteCustomSetting(textureFilter, txStrongCRC);
	WriteCustomSetting(textureFilter, txForce16bpp);
	WriteCustomSetting(textureFilter, txCacheCompression);
	WriteCustomSetting(textureFilter, txSaveCache);
	WriteCustomSettingS(txPath);
	WriteCustomSettingS(txCachePath);
	WriteCustomSettingS(txDumpPath);
	settings.endGroup();

	settings.beginGroup("gammaCorrection");
	WriteCustomSetting(gammaCorrection, force);
	WriteCustomSettingF(gammaCorrection, level);
	settings.endGroup();

	settings.beginGroup("onScreenDisplay");
	WriteCustomSetting2(onScreenDisplay, showFPS, fps);
	WriteCustomSetting2(onScreenDisplay, showVIS, vis);
	WriteCustomSetting2(onScreenDisplay, showPercent, percent);
	WriteCustomSetting2(onScreenDisplay, showInternalResolution, internalResolution);
	WriteCustomSetting2(onScreenDisplay, showRenderingResolution, renderingResolution);
	WriteCustomSetting2(onScreenDisplay, showStatistics, statistics);
	WriteCustomSetting2(onScreenDisplay, osdPos, pos);
	settings.endGroup();

	settings.beginGroup("hotkeys");
	for (u32 idx = 0; idx < Config::HotKey::hkTotal; ++idx) {
		if (origConfig.hotkeys.keys[idx] != config.hotkeys.keys[idx] ||
			origConfig.hotkeys.keys[idx] != settings.value(Config::hotkeyIniName(idx), config.hotkeys.keys[idx]).toInt()) {
			settings.setValue(Config::hotkeyIniName(idx), config.hotkeys.keys[idx]);
		}
		if (origConfig.hotkeys.enabledKeys[idx] != config.hotkeys.enabledKeys[idx] ||
			origConfig.hotkeys.enabledKeys[idx] != settings.value(Config::enabledHotkeyIniName(idx), config.hotkeys.enabledKeys[idx]).toInt()) {
			settings.setValue(Config::enabledHotkeyIniName(idx), config.hotkeys.enabledKeys[idx]);
		}
	}
	settings.endGroup();

	settings.endGroup();
}

QString getTranslationFile()
{
	return config.translationFile.c_str();
}

QStringList getProfiles(const QString & _strIniFolder)
{
	QSettings settings(_strIniFolder + "/" + strIniFileName, QSettings::IniFormat);
	return settings.childGroups();
}

QString getCurrentProfile(const QString & _strIniFolder)
{
	QSettings settings(_strIniFolder + "/" + strIniFileName, QSettings::IniFormat);
	return settings.value("profile", strUserProfile).toString();
}

void changeProfile(const QString & _strIniFolder, const QString & _strSharedIniFolder, const QString & _strProfile)
{
	{
		QSettings settings(_strIniFolder + "/" + strIniFileName, QSettings::IniFormat);
		settings.setValue("profile", _strProfile);
	}
	loadSettings(_strIniFolder, _strSharedIniFolder);
}

void addProfile(const QString & _strIniFolder, const QString & _strProfile)
{
	{
		QSettings settings(_strIniFolder + "/" + strIniFileName, QSettings::IniFormat);
		settings.setValue("profile", _strProfile);
	}
	writeSettings(_strIniFolder);
}

void removeProfile(const QString & _strIniFolder, const QString & _strProfile)
{
	QSettings settings(_strIniFolder + "/" + strIniFileName, QSettings::IniFormat);
	settings.remove(_strProfile);
}
