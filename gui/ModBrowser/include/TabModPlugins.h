#ifndef SIMPLEMODMANAGER_TABMODPLUGINS_H
#define SIMPLEMODMANAGER_TABMODPLUGINS_H

#include <borealis.hpp>

class TabModPlugins : public brls::List
{

public:
	TabModPlugins();

	std::map<std::string, brls::ListItem *> &getModsListItems();

	void draw(NVGcontext *vg, int x, int y, unsigned width, unsigned height, brls::Style *style, brls::FrameContext *ctx) override;

	std::string remove_extension(const std::string &filename);
	std::string get_extension(const std::string &filename);
	brls::Image *load_image_cache(std::string filename);

	std::string base64_encode(const std::string &in);
	std::string base64_decode(const std::string &in);

private:
	brls::Dialog *dialog;
	std::map<std::string, brls::ListItem *> _modsListItems_;
	int frameCounter;
};
extern std::map<std::string, brls::Image *> cached_thumbs;

#endif //SIMPLEMODMANAGER_TABMODPLUGINS_H