#include <Geode/Geode.hpp>
#include <Geode/modify/EditorUI.hpp>

using namespace geode::prelude;

class $modify(MyEditorUI, EditorUI) {
    bool init(LevelEditorLayer* editor) {
        if (!EditorUI::init(editor)) return false;

        // Create a standalone menu at the top-right
        auto winSize = CCDirector::sharedDirector()->getWinSize();
        auto menu = CCMenu::create();
        menu->setPosition(winSize.width - 50, winSize.height - 50);
        menu->setID("decorator-menu"_spr);
        
        // Create button with bright colors to be very visible
        auto buttonSprite = ButtonSprite::create(
            "DECORATE",
            "bigFont.fnt",
            "GJ_button_04.png",
            0.8f
        );
        
        auto button = CCMenuItemSpriteExtra::create(
            buttonSprite,
            this,
            menu_selector(MyEditorUI::onDecorate)
        );
        
        menu->addChild(button);
        this->addChild(menu, 999);

        return true;
    }

    void onDecorate(CCObject*) {
        auto selected = this->getSelectedObjects();
        if (!selected || selected->count() == 0) {
            FLAlertLayer::create(
                "Level Decorator",
                "Please select objects first!",
                "OK"
            )->show();
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
                m_editorLayer->m_objectLayer->addChild(glow);
                m_editorLayer->addToSection(glow);
            }

            // Add 4 corner decorations
            for (int i = 0; i < 4; i++) {
                auto decor = GameObject::createWithKey(1764);
                if (!decor) continue;
                
                float xOffset = (i % 2 == 0) ? -20.0f : 20.0f;
                float yOffset = (i < 2) ? 20.0f : -20.0f;
                
                decor->setPosition(ccp(pos.x + xOffset, pos.y + yOffset));
                decor->setScale(0.6f);
                decor->setZOrder(2);
                m_editorLayer->m_objectLayer->addChild(decor);
                m_editorLayer->addToSection(decor);
            }
        }
        
        // Show success popup
        FLAlertLayer::create(
            "Success!",
            "Decorations applied!",
            "OK"
        )->show();
    }
};
