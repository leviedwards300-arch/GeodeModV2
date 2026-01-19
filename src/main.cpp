#include <Geode/Geode.hpp>
#include <Geode/modify/EditorUI.hpp>

using namespace geode::prelude;

class $modify(MyEditorUI, EditorUI) {
    bool init(LevelEditorLayer* editor) {
        if (!EditorUI::init(editor)) return false;

        auto editBar = this->getChildByID("edit-button-bar");
        if (!editBar) return true;

        auto menu = editBar->getChildByType<CCMenu>(0);
        if (!menu) return true;

        auto sprite = CCSprite::createWithSpriteFrameName("GJ_button_01.png");
        if (!sprite) return true;
        sprite->setScale(0.7f);
        
        auto label = CCLabelBMFont::create("D", "bigFont.fnt");
        if (!label) return true;
        label->setScale(0.5f);
        label->setPosition(sprite->getContentSize() / 2);
        sprite->addChild(label);

        auto btn = CCMenuItemSpriteExtra::create(
            sprite,
            this,
            menu_selector(MyEditorUI::onDecorate)
        );
        if (!btn) return true;
        
        menu->addChild(btn);
        menu->updateLayout();

        return true;
    }

    void onDecorate(CCObject*) {
        auto selected = this->getSelectedObjects();
        if (!selected || selected->count() == 0) {
            return;
        }

        CCObject* obj;
        CCARRAY_FOREACH(selected, obj) {
            auto gameObj = static_cast<GameObject*>(obj);
            if (!gameObj) continue;

            auto pos = gameObj->getPosition();

            auto glow = GameObject::createWithKey(106);
            if (glow) {
                glow->setPosition(pos);
                glow->setScale(1.5f);
                glow->setZOrder(-1);
                m_editorLayer->m_objectLayer->addChild(glow);
                m_editorLayer->addToSection(glow);
            }

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
    }
};
