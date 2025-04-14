#include <Geode/Geode.hpp>
#include <Geode/modify/CreatorLayer.hpp>

using namespace geode::prelude;


class $modify(HCreatorLayer, CreatorLayer) {
	struct Fields {
		CCSpriteGrayscale* m_lockedQuestsSprite;
		CCSpriteGrayscale* m_lockedQuestsExMarkSprite;
	};

	bool init() {
		if (!CreatorLayer::init())
			return false;

		bool locked = Mod::get() -> getSavedValue<bool>("locked", false);

		// Toggle button
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

		// Locked sprite
		auto sprite = CCSprite::createWithSpriteFrameName("GJ_challengeBtn_001.png");
		auto exMark = CCSprite::createWithSpriteFrameName("exMark_001.png");
		auto lockSprite = CCSprite::createWithSpriteFrameName("GJ_lock_001.png");

		sprite -> setScale(0.8f);
		sprite -> setColor(ccGRAY);
		sprite -> setID("locked-sprite"_spr);
		exMark -> setPosition({20.f, 83.6f});
		exMark -> setScale(0.8f);
		exMark -> setColor(ccGRAY);
		exMark -> setID("ex-mark-locked-sprite"_spr);
		// TODO: values below
		lockSprite -> setPosition({});
		lockSprite -> setScale();
		lockSprite -> setID("lock-sprite"_spr);

		sprite -> addChild(exMark);
		sprite -> addChild(lockSprite);

		m_fields -> m_lockedQuestsSprite = sprite;
		m_fields -> m_lockedQuestsExMarkSprite = exMark;

		auto questsButton = static_cast<CCMenuItemSpriteExtra*>(m_questsSprite -> m_pParent);
		sprite -> setVisible(false);
		questsButton -> addChildAtPosition(sprite, Anchor::Center);

		// Lock if locked waow
		if (locked) {
			Mod::get() -> setSavedValue<bool>("locked", false);
			onQuestsToggle(nullptr);
		}

		return true;
	}


	void onQuestsToggle(CCObject* sender) {
		bool locked = !Mod::get() -> getSavedValue<bool>("locked", false);
		Mod::get() -> setSavedValue<bool>("locked", locked);

		m_fields -> m_lockedQuestsSprite -> setVisible(locked);
		m_questsSprite -> setVisible(!locked);
		m_fields -> m_lockedQuestsExMarkSprite -> setVisible(m_questsSprite -> getChildrenCount());
		static_cast<CCMenuItemSpriteExtra*>(m_questsSprite -> m_pParent) -> setEnabled(!locked);

		return;
	}
};
