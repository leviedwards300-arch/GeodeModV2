#include <Geode/Geode.hpp>
#include <Geode/modify/EditorUI.hpp>

using namespace geode::prelude;

class $modify(MyEditorUI, EditorUI) {
    struct Fields {
        int currentTemplate = 0;
    };
    
    bool init(LevelEditorLayer* editor) {
        if (!EditorUI::init(editor)) return false;

        auto winSize = CCDirector::sharedDirector()->getWinSize();
        auto menu = CCMenu::create();
        menu->setPosition(0, 0);
        menu->setID("decorator-menu"_spr);
        
        // DECORATE button
        auto decorateBtn = ButtonSprite::create(
            "DECORATE",
            "bigFont.fnt",
            "GJ_button_04.png",
            0.65f
        );
        auto decorateItem = CCMenuItemSpriteExtra::create(
            decorateBtn,
            this,
            menu_selector(MyEditorUI::onDecorate)
        );
        decorateItem->setPosition(winSize.width - 80, winSize.height - 130);
        menu->addChild(decorateItem);
        
        // Template selector buttons
        std::vector<std::string> templates = {
            "Neon", "Crystal", "Pulse", "Outline"
        };
        
        float startY = winSize.height - 175;
        for (int i = 0; i < templates.size(); i++) {
            auto btn = ButtonSprite::create(
                templates[i].c_str(),
                "bigFont.fnt",
                "GJ_button_05.png",
                0.5f
            );
            auto item = CCMenuItemSpriteExtra::create(
                btn,
                this,
                menu_selector(MyEditorUI::onSelectTemplate)
            );
            item->setTag(i);
            item->setPosition(winSize.width - 80, startY - (i * 40));
            menu->addChild(item);
        }
        
        this->addChild(menu, 999);
        return true;
    }
    
    void onSelectTemplate(CCObject* sender) {
        m_fields->currentTemplate = sender->getTag();
        
        std::vector<std::string> names = {"Neon Glow", "Crystal", "Pulse", "Outline"};
        std::string msg = "Selected: " + names[m_fields->currentTemplate];
        
        FLAlertLayer::create("Template", msg.c_str(), "OK")->show();
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
            
            switch (m_fields->currentTemplate) {
                case 0: applyNeonGlow(pos); break;
                case 1: applyCrystal(pos); break;
                case 2: applyPulse(pos); break;
                case 3: applyOutline(pos); break;
                default: applyNeonGlow(pos);
            }
        }
        
        FLAlertLayer::create("Success!", "Decorations applied!", "OK")->show();
    }
    
    void applyNeonGlow(CCPoint pos) {
        // Multiple layered glows - neon effect
        addObject(106, pos.x, pos.y, 2.5f, -3);
        addObject(106, pos.x, pos.y, 2.0f, -2);
        addObject(106, pos.x, pos.y, 1.5f, -1);
        
        // Neon outline squares
        for (int i = 0; i < 4; i++) {
            float angle = i * 90.0f * M_PI / 180.0f;
            float x = cos(angle) * 35;
            float y = sin(angle) * 35;
            addObject(1764, pos.x + x, pos.y + y, 0.8f, 1);
        }
        
        // Corner accents
        addObject(1329, pos.x - 25, pos.y + 25, 0.6f, 2);
        addObject(1329, pos.x + 25, pos.y + 25, 0.6f, 2);
        addObject(1329, pos.x - 25, pos.y - 25, 0.6f, 2);
        addObject(1329, pos.x + 25, pos.y - 25, 0.6f, 2);
        
        // Extra particles for beauty
        for (int i = 0; i < 8; i++) {
            float angle = i * 45.0f * M_PI / 180.0f;
            float x = cos(angle) * 42;
            float y = sin(angle) * 42;
            addObject(1705, pos.x + x, pos.y + y, 0.35f, 3);
        }
    }
    
    void applyCrystal(CCPoint pos) {
        // Crystal glow
        addObject(106, pos.x, pos.y, 2.2f, -2);
        addObject(106, pos.x, pos.y, 1.6f, -1);
        
        // Crystal shards in circle
        for (int i = 0; i < 8; i++) {
            float angle = i * 45.0f * M_PI / 180.0f;
            float x = cos(angle) * 30;
            float y = sin(angle) * 30;
            addObject(1764, pos.x + x, pos.y + y, 0.7f, 1);
        }
        
        // Inner crystals
        addObject(1329, pos.x, pos.y + 15, 0.5f, 2);
        addObject(1329, pos.x, pos.y - 15, 0.5f, 2);
        addObject(1329, pos.x + 15, pos.y, 0.5f, 2);
        addObject(1329, pos.x - 15, pos.y, 0.5f, 2);
        
        // Sparkles
        for (int i = 0; i < 6; i++) {
            float angle = i * 60.0f * M_PI / 180.0f;
            float x = cos(angle) * 38;
            float y = sin(angle) * 38;
            addObject(1705, pos.x + x, pos.y + y, 0.3f, 3);
        }
    }
    
    void applyPulse(CCPoint pos) {
        // Pulsing rings
        addObject(106, pos.x, pos.y, 3.0f, -3);
        addObject(106, pos.x, pos.y, 2.2f, -2);
        addObject(106, pos.x, pos.y, 1.4f, -1);
        
        // Pulse particles in rings
        for (int ring = 0; ring < 2; ring++) {
            float radius = 30 + (ring * 15);
            for (int i = 0; i < 12; i++) {
                float angle = i * 30.0f * M_PI / 180.0f;
                float x = cos(angle) * radius;
                float y = sin(angle) * radius;
                addObject(1705, pos.x + x, pos.y + y, 0.4f - (ring * 0.1f), 1 + ring);
            }
        }
        
        // Center accents
        addObject(1329, pos.x, pos.y, 0.8f, 3);
    }
    
    void applyOutline(CCPoint pos) {
        // Thick outline effect
        addObject(915, pos.x, pos.y, 1.4f, -1);
        addObject(915, pos.x, pos.y, 1.2f, 0);
        
        // Glow behind
        addObject(106, pos.x, pos.y, 1.8f, -2);
        
        // Corner reinforcements
        for (int i = 0; i < 4; i++) {
            float x = (i % 2 == 0) ? -28.0f : 28.0f;
            float y = (i < 2) ? 28.0f : -28.0f;
            addObject(211, pos.x + x, pos.y + y, 0.6f, 1);
            addObject(1764, pos.x + x * 0.7f, pos.y + y * 0.7f, 0.5f, 2);
        }
        
        // Edge decorations
        addObject(1329, pos.x - 32, pos.y, 0.5f, 2);
        addObject(1329, pos.x + 32, pos.y, 0.5f, 2);
        addObject(1329, pos.x, pos.y - 32, 0.5f, 2);
        addObject(1329, pos.x, pos.y + 32, 0.5f, 2);
    }
    
    void addObject(int id, float x, float y, float scale, int zOrder) {
        auto obj = GameObject::createWithKey(id);
        if (!obj) return;
        
        obj->setPosition(ccp(x, y));
        obj->setScale(scale);
        obj->setZOrder(zOrder);
        
        m_editorLayer->m_objectLayer->addChild(obj);
        m_editorLayer->addToSection(obj);
    }
};
