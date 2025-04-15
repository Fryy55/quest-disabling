#include <Geode/Geode.hpp>
#include <Geode/modify/CreatorLayer.hpp>
#include <Geode/modify/CCMenuItemSpriteExtra.hpp>

using namespace geode::prelude;


class ExMarkSprite final : public CCSprite {
public:
	static ExMarkSprite* createWithSpriteFrameName(char const* frameName) {
		auto ret = new ExMarkSprite;
		if (ret -> initWithSpriteFrameName(frameName)) {
			ret -> autorelease();
			return ret;
		}

		delete ret;
		return nullptr;
	}
	void setQuestsSprite(CCSprite* questsSprite) {
		m_questsSprite = questsSprite;

		return;
	}
private:
	CCSprite* m_questsSprite;

	void update(float dt) override {
		this -> setVisible(m_questsSprite -> getChildrenCount());

		return;
	}
};

class $modify(HCreatorLayer, CreatorLayer) {
	struct Fields {
		CCSprite* m_lockedQuestsSprite;
		CCSprite* m_lockedQuestsExMarkSprite;
	};

	$override bool init() {
		if (!CreatorLayer::init())
			return false;

		bool locked = Mod::get() -> getSavedValue<bool>("locked", false);

		// Toggle button
		auto lockButton = CCMenuItemToggler::create(
			CCSprite::createWithSpriteFrameName("GJ_lock_open_001.png"),
			CCSprite::createWithSpriteFrameName("GJ_lock_001.png"),
			this,
			menu_selector(HCreatorLayer::onQuestsToggle)
		);
		auto lockMenu = CCMenu::create();

		lockButton -> toggle(locked);
		auto offSpr = lockButton -> m_offButton -> getChildByType<CCSprite>(0);
		offSpr -> setPositionY(offSpr -> getPositionY() + 4.f);
		lockButton -> setID("quests-toggle"_spr);
		lockMenu -> setContentSize({30.f, 30.f});
		lockMenu -> setScale(0.75f);
		lockMenu -> setID("quests-toggle-menu"_spr);

		lockMenu -> addChildAtPosition(lockButton, Anchor::Center);

		// Locked sprite
		auto sprite = CCSprite::createWithSpriteFrameName("GJ_challengeBtn_001.png");
		auto exMark = ExMarkSprite::createWithSpriteFrameName("exMark_001.png");

		sprite -> addChild(exMark);

		sprite -> setScale(0.8f);
		sprite -> setCascadeColorEnabled(true);
		sprite -> setColor(ccGRAY);
		sprite -> setVisible(false);
		sprite -> setID("locked-sprite"_spr);
		exMark -> setPosition({20.f, 83.6f});
		exMark -> setScale(0.8f);
		exMark -> setQuestsSprite(m_questsSprite);
		exMark -> scheduleUpdate();
		exMark -> setID("ex-mark-locked-sprite"_spr);

		m_fields -> m_lockedQuestsSprite = sprite;
		m_fields -> m_lockedQuestsExMarkSprite = exMark;

		// Put stuff together
		auto questsButton = static_cast<CCMenuItemSpriteExtra*>(m_questsSprite -> m_pParent);
		questsButton -> addChildAtPosition(sprite, Anchor::Center);

		this -> addChild(lockMenu);
		auto buttonsMenu = this -> getChildByID("creator-buttons-menu");
		lockMenu -> setPosition(
			(buttonsMenu -> getPosition() - buttonsMenu -> getContentSize() / 2.f)
			+
			(questsButton -> getPosition() - questsButton -> getContentSize() / 2.f)
			+
			CCPoint(10.f, 10.f)
		);

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
		static_cast<CCMenuItemSpriteExtra*>(m_questsSprite -> m_pParent) -> setEnabled(!locked);

		return;
	}
};

class $modify(CCMenuItemSpriteExtra) {
	struct Fields {
		bool m_isQuests = false; // "My child will initialize fields in init"
		bool m_init = false;
	};

	$override void selected() {
		CCMenuItemSpriteExtra::selected();

		if (!(m_fields -> m_init)) {
			m_fields -> m_isQuests = (this -> getID() == "quests-button");
			m_fields -> m_init = true;
		}

		if (m_fields -> m_isQuests)
			this -> m_pParent -> m_pParent -> getChildByID("quests-toggle-menu"_spr) ->
				getChildByType<CCMenuItemToggler>(0) -> m_offButton -> runAction(CCFadeOut::create(0.3f));

		return;
	}

	$override void unselected() {
		CCMenuItemSpriteExtra::unselected();

		if (m_fields -> m_isQuests)
			this -> m_pParent -> m_pParent -> getChildByID("quests-toggle-menu"_spr) ->
				getChildByType<CCMenuItemToggler>(0) -> m_offButton -> runAction(CCFadeIn::create(0.3f));

		return;
	}
};