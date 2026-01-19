#include <Geode/Geode.hpp>
#include <Geode/modify/EditorUI.hpp>

using namespace geode::prelude;

class $modify(MyEditorUI, EditorUI) {
    struct Fields {
        CCMenuItemSpriteExtra* decorateBtn = nullptr;
    };

    bool init(LevelEditorLayer* editor) {
        if (!EditorUI::init(editor)) return false;

        auto editBar = this->getChildByID("edit-button-bar");
        if (!editBar) return true;

        auto menu = editBar->getChildByType<CCMenu>(0);
        if (!menu) return true;

        auto sprite = CCSprite::createWithSpriteFrameName("GJ_button_01.png");
        sprite->setScale(0.7f);
        
        auto label = CCLabelBMFont::create("D", "bigFont.fnt");
        label->setScale(0.5f);
        label->setPosition(sprite->getContentSize() / 2);
        sprite->addChild(label);

        m_fields->decorateBtn = CCMenuItemSpriteExtra::create(
            sprite,
            this,
            menu_selector(MyEditorUI::onDecorate)
        );
        menu->addChild(m_fields->decorateBtn);
        menu->updateLayout();

        return true;
    }

    void onDecorate(CCObject*) {
        auto selected = this->getSelectedObjects();
        if (!selected || selected->count() == 0) {
            FLAlertLayer::create("No Selection", "Select objects first!", "OK")->show();
            return;
        }

        CCObject* obj;
        CCARRAY_FOREACH(selected, obj) {
            auto gameObj = static_cast<GameObject*>(obj);
            if (!gameObj) continue;

            auto pos = gameObj->getPosition();

            // Add glow
            auto glow = GameObject::createWithKey(106);
            if (glow) {
                glow->setPosition(pos);
                glow->setScale(1.5f);
                glow->setZOrder(-1);
                glow->setObjectColor(ccc3(100, 150, 255));
                m_editorLayer->m_objectLayer->addChild(glow);
                m_editorLayer->addToSection(glow);
            }

            // Add corner decorations
            for (int i = 0; i < 4; i++) {
                auto decor = GameObject::createWithKey(1764);
                if (!decor) continue;
                
                float x = (i % 2) ? 20.f : -20.f;
                float y = (i < 2) ? 20.f : -20.f;
                
                decor->setPosition(ccp(pos.x + x, pos.y + y));
                decor->setScale(0.6f);
                decor->setZOrder(2);
                decor->setObjectColor(ccc3(200, 200, 255));
                m_editorLayer->m_objectLayer->addChild(decor);
                m_editorLayer->addToSection(decor);
            }
        }

        Notification::create("Decorated!", NotificationIcon::Success)->show();
    }
};
