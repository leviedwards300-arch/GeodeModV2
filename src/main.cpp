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
        auto decorateBtn = ButtonSprite::create("DECORATE", "bigFont.fnt", "GJ_button_04.png", 0.65f);
        auto decorateItem = CCMenuItemSpriteExtra::create(decorateBtn, this, menu_selector(MyEditorUI::onDecorate));
        decorateItem->setPosition(winSize.width - 80, winSize.height - 130);
        menu->addChild(decorateItem);
        
        // Template buttons
        std::vector<std::string> templates = {"Neon", "Crystal", "Pulse", "Outline"};
        float startY = winSize.height - 175;
        for (int i = 0; i < templates.size(); i++) {
            auto btn = ButtonSprite::create(templates[i].c_str(), "bigFont.fnt", "GJ_button_05.png", 0.5f);
            auto item = CCMenuItemSpriteExtra::create(btn, this, menu_selector(MyEditorUI::onSelectTemplate));
            item->setTag(i);
            item->setPosition(winSize.width - 80, startY - (i * 40));
            menu->addChild(item);
        }
        
        this->addChild(menu, 999);
        return true;
    }
    
    void onSelectTemplate(CCObject* sender) {
        m_fields->currentTemplate = sender->getTag();
        std::vector<std::string> names = {"Neon Glow", "Crystal", "Pulse Wave", "Ethereal Outline"};
        FLAlertLayer::create("Template", names[m_fields->currentTemplate].c_str(), "OK")->show();
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
                case 2: applyPulseWave(pos); break;
                case 3: applyEtherealOutline(pos); break;
            }
        }
        
        FLAlertLayer::create("Success!", "Decorations applied!", "OK")->show();
    }
    
    void applyNeonGlow(CCPoint pos) {
        // Neon purple/pink glow layers with color
        auto glow1 = addGlow(pos.x, pos.y, 3.5f, -4);
        setColor(glow1, 180, 50, 255); // Purple
        
        auto glow2 = addGlow(pos.x, pos.y, 2.8f, -3);
        setColor(glow2, 255, 80, 200); // Pink
        
        auto glow3 = addGlow(pos.x, pos.y, 2.0f, -2);
        setColor(glow3, 255, 120, 255); // Light purple
        
        auto glow4 = addGlow(pos.x, pos.y, 1.2f, -1);
        setColor(glow4, 255, 180, 255); // Very light pink
        
        // Neon outline squares with cyan color
        for (int i = 0; i < 8; i++) {
            float angle = i * 45.0f * M_PI / 180.0f;
            float radius = 40.0f;
            float x = pos.x + cos(angle) * radius;
            float y = pos.y + sin(angle) * radius;
            
            auto square = addObject(211, x, y, 0.7f, 2); // Outline square
            setColor(square, 0, 255, 255); // Cyan
        }
        
        // Floating particles with color gradient
        for (int i = 0; i < 12; i++) {
            float angle = i * 30.0f * M_PI / 180.0f;
            float radius = 50.0f + (i % 3) * 8;
            float x = pos.x + cos(angle) * radius;
            float y = pos.y + sin(angle) * radius;
            
            auto particle = addObject(1705, x, y, 0.3f + (i % 3) * 0.1f, 3);
            if (i % 3 == 0) setColor(particle, 255, 100, 255);
            else if (i % 3 == 1) setColor(particle, 150, 100, 255);
            else setColor(particle, 100, 200, 255);
        }
        
        // Inner accent rings
        for (int ring = 0; ring < 2; ring++) {
            float radius = 15.0f + ring * 10;
            for (int i = 0; i < 6; i++) {
                float angle = i * 60.0f * M_PI / 180.0f;
                float x = pos.x + cos(angle) * radius;
                float y = pos.y + sin(angle) * radius;
                
                auto accent = addObject(1329, x, y, 0.4f, 4);
                setColor(accent, 255, 150, 255);
            }
        }
    }
    
    void applyCrystal(CCPoint pos) {
        // Blue/white crystal glow
        auto glow1 = addGlow(pos.x, pos.y, 3.0f, -3);
        setColor(glow1, 150, 200, 255);
        
        auto glow2 = addGlow(pos.x, pos.y, 2.0f, -2);
        setColor(glow2, 200, 230, 255);
        
        auto glow3 = addGlow(pos.x, pos.y, 1.3f, -1);
        setColor(glow3, 230, 245, 255);
        
        // Crystal shards in symmetrical pattern
        for (int i = 0; i < 8; i++) {
            float angle = i * 45.0f * M_PI / 180.0f;
            float radius = 35.0f;
            float x = pos.x + cos(angle) * radius;
            float y = pos.y + sin(angle) * radius;
            
            auto shard = addObject(1764, x, y, 0.8f, 1);
            setColor(shard, 180 + i * 8, 220, 255);
        }
        
        // Secondary smaller crystals
        for (int i = 0; i < 8; i++) {
            float angle = (i * 45.0f + 22.5f) * M_PI / 180.0f;
            float radius = 25.0f;
            float x = pos.x + cos(angle) * radius;
            float y = pos.y + sin(angle) * radius;
            
            auto crystal = addObject(1329, x, y, 0.5f, 2);
            setColor(crystal, 200, 235, 255);
        }
        
        // Sparkle particles
        for (int i = 0; i < 16; i++) {
            float angle = i * 22.5f * M_PI / 180.0f;
            float radius = 42.0f + (i % 4) * 5;
            float x = pos.x + cos(angle) * radius;
            float y = pos.y + sin(angle) * radius;
            
            auto sparkle = addObject(1705, x, y, 0.25f + (i % 4) * 0.05f, 3);
            setColor(sparkle, 220 + i * 2, 240, 255);
        }
        
        // Center diamond formation
        float diamondDist = 18.0f;
        auto top = addObject(1764, pos.x, pos.y + diamondDist, 0.6f, 4);
        auto bottom = addObject(1764, pos.x, pos.y - diamondDist, 0.6f, 4);
        auto left = addObject(1764, pos.x - diamondDist, pos.y, 0.6f, 4);
        auto right = addObject(1764, pos.x + diamondDist, pos.y, 0.6f, 4);
        setColor(top, 240, 250, 255);
        setColor(bottom, 240, 250, 255);
        setColor(left, 240, 250, 255);
        setColor(right, 240, 250, 255);
    }
    
    void applyPulseWave(CCPoint pos) {
        // Multiple pulse rings with gradient
        for (int ring = 0; ring < 5; ring++) {
            float scale = 3.5f - ring * 0.6f;
            auto pulse = addGlow(pos.x, pos.y, scale, -5 + ring);
            
            int r = 100 + ring * 30;
            int g = 150 + ring * 20;
            int b = 255;
            setColor(pulse, r, g, b);
        }
        
        // Rotating particle waves
        for (int wave = 0; wave < 3; wave++) {
            int particlesPerWave = 16;
            float radius = 35.0f + wave * 15;
            
            for (int i = 0; i < particlesPerWave; i++) {
                float angle = (i * (360.0f / particlesPerWave) + wave * 15) * M_PI / 180.0f;
                float x = pos.x + cos(angle) * radius;
                float y = pos.y + sin(angle) * radius;
                
                auto particle = addObject(1705, x, y, 0.35f - wave * 0.08f, 1 + wave);
                setColor(particle, 150 - wave * 30, 200, 255);
            }
        }
        
        // Energy core
        for (int i = 0; i < 4; i++) {
            float angle = i * 90.0f * M_PI / 180.0f;
            float x = pos.x + cos(angle) * 12;
            float y = pos.y + sin(angle) * 12;
            
            auto core = addObject(1329, x, y, 0.7f, 5);
            setColor(core, 255, 255, 255);
        }
        
        // Outer ring accents
        for (int i = 0; i < 12; i++) {
            float angle = i * 30.0f * M_PI / 180.0f;
            float x = pos.x + cos(angle) * 55;
            float y = pos.y + sin(angle) * 55;
            
            auto accent = addObject(211, x, y, 0.4f, 4);
            setColor(accent, 80, 180, 255);
        }
    }
    
    void applyEtherealOutline(CCPoint pos) {
        // Soft ethereal glow layers
        auto glow1 = addGlow(pos.x, pos.y, 3.2f, -4);
        setColor(glow1, 200, 180, 255);
        
        auto glow2 = addGlow(pos.x, pos.y, 2.4f, -3);
        setColor(glow2, 220, 200, 255);
        
        auto glow3 = addGlow(pos.x, pos.y, 1.6f, -2);
        setColor(glow3, 240, 220, 255);
        
        // Outlined border
        auto outline1 = addObject(915, pos.x, pos.y, 1.5f, -1);
        setColor(outline1, 180, 160, 255);
        
        auto outline2 = addObject(915, pos.x, pos.y, 1.3f, 0);
        setColor(outline2, 220, 200, 255);
        
        // Corner wisps
        for (int i = 0; i < 4; i++) {
            float x = pos.x + (i % 2 == 0 ? -32 : 32);
            float y = pos.y + (i < 2 ? 32 : -32);
            
            // Main corner piece
            auto corner = addObject(1764, x, y, 0.7f, 1);
            setColor(corner, 200, 180, 255);
            
            // Floating wisps around corner
            for (int j = 0; j < 3; j++) {
                float offsetAngle = (j * 120.0f + i * 30) * M_PI / 180.0f;
                float wx = x + cos(offsetAngle) * 10;
                float wy = y + sin(offsetAngle) * 10;
                
                auto wisp = addObject(1705, wx, wy, 0.3f, 2);
                setColor(wisp, 220 + j * 10, 200, 255);
            }
        }
        
        // Floating particles in orbit
        for (int i = 0; i < 20; i++) {
            float angle = i * 18.0f * M_PI / 180.0f;
            float radius = 38.0f + sin(i * 0.5f) * 8;
            float x = pos.x + cos(angle) * radius;
            float y = pos.y + sin(angle) * radius;
            
            auto particle = addObject(1705, x, y, 0.25f + (i % 5) * 0.04f, 3);
            int colorShift = i * 3;
            setColor(particle, 200 + colorShift, 180 + colorShift, 255);
        }
        
        // Edge accent markers
        float edgeDist = 40.0f;
        auto topAccent = addObject(1329, pos.x, pos.y + edgeDist, 0.5f, 4);
        auto bottomAccent = addObject(1329, pos.x, pos.y - edgeDist, 0.5f, 4);
        auto leftAccent = addObject(1329, pos.x - edgeDist, pos.y, 0.5f, 4);
        auto rightAccent = addObject(1329, pos.x + edgeDist, pos.y, 0.5f, 4);
        setColor(topAccent, 230, 210, 255);
        setColor(bottomAccent, 230, 210, 255);
        setColor(leftAccent, 230, 210, 255);
        setColor(rightAccent, 230, 210, 255);
    }
    
    GameObject* addGlow(float x, float y, float scale, int zOrder) {
        auto glow = GameObject::createWithKey(106);
        if (!glow) return nullptr;
        glow->setPosition(ccp(x, y));
        glow->setScale(scale);
        glow->setZOrder(zOrder);
        m_editorLayer->m_objectLayer->addChild(glow);
        m_editorLayer->addToSection(glow);
        return glow;
    }
    
    GameObject* addObject(int id, float x, float y, float scale, int zOrder) {
        auto obj = GameObject::createWithKey(id);
        if (!obj) return nullptr;
        obj->setPosition(ccp(x, y));
        obj->setScale(scale);
        obj->setZOrder(zOrder);
        m_editorLayer->m_objectLayer->addChild(obj);
        m_editorLayer->addToSection(obj);
        return obj;
    }
    
    void setColor(GameObject* obj, int r, int g, int b) {
        if (!obj) return;
        obj->setObjectColor(ccc3(r, g, b));
    }
};
