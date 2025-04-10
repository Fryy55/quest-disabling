#include <Geode/Geode.hpp>
#include <Geode/modify/CreatorLayer.hpp>

using namespace geode::prelude;


class $modify(HCreatorLayer, CreatorLayer) {
	bool init() {
		if (!CreatorLayer::init())
			return false;

		bool locked = Mod::get() -> getSavedValue<bool>("locked", false);

		auto button = CCMenuItemToggler::create(
			CCSprite::createWithSpriteFrameName("GJ_lock_open_001.png"),
			CCSprite::createWithSpriteFrameName("GJ_lock_001.png"),
			this,
			menu_selector(HCreatorLayer::onQuestsToggle)
		);
		button -> toggle(locked);
		button -> setID("quests-toggle"_spr);

		if (locked) {
			Mod::get() -> setSavedValue<bool>("locked", false);
			onQuestsToggle(nullptr);
		}

		auto menu = this -> getChildByID("top-right-menu");
		menu -> addChild(button);
		menu -> updateLayout();

		return true;
	}

	void onQuestsToggle(CCObject* sender) {
		bool locked = !Mod::get() -> getSavedValue<bool>("locked", false);
		Mod::get() -> setSavedValue<bool>("locked", locked);

		auto questsButton = static_cast<CCMenuItemSpriteExtra*>(
			this -> getChildByID("creator-buttons-menu") -> getChildByID("quests-button")
		);
		auto sprite =(
			locked ?
				CCSpriteGrayscale::createWithSpriteFrameName("GJ_challengeBtn_001.png")
			:
				CCSprite::createWithSpriteFrameName("GJ_challengeBtn_001.png")
		);
		sprite -> setScale(0.8f);
		questsButton -> setSprite(sprite);
		questsButton -> setEnabled(!locked);

		return;
	}
};