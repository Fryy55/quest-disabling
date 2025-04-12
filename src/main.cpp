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

		auto menu = this -> getChildByID("top-right-menu");
		menu -> addChild(button);
		menu -> updateLayout();

		if (locked) {
			Mod::get() -> setSavedValue<bool>("locked", false);
			onQuestsToggle(nullptr);
		}

		//this -> querySelector("")
		return true;
	}

	void checkQuestsStatus() {
		CreatorLayer::checkQuestsStatus();

		if (m_questsSprite -> getChildrenCount()) {
			m_questsSprite -> removeAllChildren();
			addExMark(m_questsSprite, Mod::get() -> getSavedValue("locked", false));
		}

		return;
	}

	void onQuestsToggle(CCObject* sender) {
		bool locked = !Mod::get() -> getSavedValue<bool>("locked", false);
		Mod::get() -> setSavedValue<bool>("locked", locked);

		auto questsButton = static_cast<CCMenuItemSpriteExtra*>(m_questsSprite -> m_pParent);
		auto sprite = (
			locked ?
				CCSpriteGrayscale::createWithSpriteFrameName("GJ_challengeBtn_001.png")
			:
				CCSprite::createWithSpriteFrameName("GJ_challengeBtn_001.png")
		);
		sprite -> setScale(0.8f);
		sprite -> setCascadeColorEnabled(true);
		sprite -> setColor({175, 175, 175});

		if (m_questsSprite -> getChildrenCount()) {
			addExMark(sprite, locked);

			m_questsSprite -> removeAllChildren();
		}

		questsButton -> removeAllChildren();
		questsButton -> addChildAtPosition(sprite, Anchor::Center);
		questsButton -> setEnabled(!locked);
		m_questsSprite = sprite;

		return;
	}

	void addExMark(CCSprite* sprite, bool locked) {
		auto exMark = (
			locked ?
				CCSpriteGrayscale::createWithSpriteFrameName("exMark_001.png")
			:
				CCSprite::createWithSpriteFrameName("exMark_001.png")
		);
		sprite -> addChild(exMark);
		exMark -> setPosition({20.f, 83.6f});
		exMark -> setScale(0.8f);

		return;
	}
};