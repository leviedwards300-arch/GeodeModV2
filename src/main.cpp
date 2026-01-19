#include <Geode/Geode.hpp>
#include <Geode/modify/EditorUI.hpp>

using namespace geode::prelude;

// Beautiful decoration templates
struct DecoTemplate {
    std::string name;
    std::vector<std::tuple<int, float, float, float, int>> objects; // id, x, y, scale, zOrder
};

class TemplatePopup : public Popup<> {
protected:
    std::function<void(int)> m_callback;
    
    bool setup(std::function<void(int)> callback) override {
        m_callback = callback;
        this->setTitle("Choose Template");
        
        auto menu = CCMenu::create();
        menu->setPosition(m_mainLayer->getContentSize() / 2);
        
        std::vector<std::string> templates = {
            "Neon Glow", "Crystal", "Pulse", "Outline", 
            "Particles", "Ethereal", "Tech", "Minimal"
        };
        
        float yStart = 100.f;
        for (int i = 0; i < templates.size(); i++) {
            auto btnSprite = ButtonSprite::create(
                templates[i].c_str(),
                "bigFont.fnt", 
                "GJ_button_01.png",
                0.7f
            );
            
            auto btn = CCMenuItemSpriteExtra::create(
                btnSprite,
                this,
                menu_selector(TemplatePopup::onTemplate)
            );
            btn->setTag(i);
            btn->setPosition(0, yStart - (i * 35.f));
            menu->addChild(btn);
        }
        
        m_mainLayer->addChild(menu);
        return true;
    }
    
    void onTemplate(CCObject* sender) {
        if (m_callback) m_callback(sender->getTag());
        this->onClose(sender);
    }
    
public:
    static TemplatePopup* create(std::function<void(int)> callback) {
        auto ret = new TemplatePopup();
        if (ret && ret->initAnchored(300.f, 350.f, callback)) {
            ret->autorelease();
            return ret;
        }
        CC_SAFE_DELETE(ret);
        return nullptr;
    }
};

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
        
        // TEMPLATES button
        auto templatesBtn = ButtonSprite::create(
            "TEMPLATES",
            "bigFont.fnt",
            "GJ_button_05.png",
            0.6f
        );
        auto templatesItem = CCMenuItemSpriteExtra::create(
            templatesBtn,
            this,
            menu_selector(MyEditorUI::onShowTemplates)
        );
        templatesItem->setPosition(winSize.width - 80, winSize.height - 175);
        menu->addChild(templatesItem);
        
        this->addChild(menu, 999);
        return true;
    }
    
    void onShowTemplates(CCObject*) {
        auto popup = TemplatePopup::create([this](int idx) {
            m_fields->currentTemplate = idx;
            FLAlertLayer::create(
                "Template Selected",
                "Now select objects and click DECORATE!",
                "OK"
            )->show();
        });
        popup->show();
    }
    
    void onDecorate(CCObject*) {
        auto selected = this->getSelectedObjects();
        if (!selected || selected->count() == 0) {
            FLAlertLayer::create("No Selection", "Select objects first!", "OK")->show();
            return;
        }
        
        applyTemplate(m_fields->currentTemplate, selected);
        
        FLAlertLayer::create("Success!", "Decorations applied!", "OK")->show();
    }
    
    void applyTemplate(int templateIdx, CCArray* selected) {
        CCObject* obj;
        CCARRAY_FOREACH(selected, obj) {
            auto gameObj = static_cast<GameObject*>(obj);
            if (!gameObj) continue;
            
            auto pos = gameObj->getPosition();
            
            switch (templateIdx) {
                case 0: applyNeonGlow(pos); break;
                case 1: applyCrystal(pos); break;
                case 2: applyPulse(pos); break;
                case 3: applyOutline(pos); break;
                case 4: applyParticles(pos); break;
                case 5: applyEthereal(pos); break;
                case 6: applyTech(pos); break;
                case 7: applyMinimal(pos); break;
                default: applyNeonGlow(pos);
            }
        }
    }
    
    void applyNeonGlow(CCPoint pos) {
        // Multiple layered glows - neon effect
        addObject(106, pos.x, pos.y, 2.5f, -3); // Outer glow
        addObject(106, pos.x, pos.y, 2.0f, -2); // Mid glow
        addObject(106, pos.x, pos.y, 1.5f, -1); // Inner glow
        
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
    }
    
    void applyCrystal(CCPoint pos) {
        // Crystal glow
        addObject(106, pos.x, pos.y, 2.0f, -2);
        
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
    }
    
    void applyPulse(CCPoint pos) {
        // Pulsing rings
        addObject(106, pos.x, pos.y, 3.0f, -3);
        addObject(106, pos.x, pos.y, 2.2f, -2);
        addObject(106, pos.x, pos.y, 1.4f, -1);
        
        // Pulse particles
        for (int i = 0; i < 12; i++) {
            float angle = i * 30.0f * M_PI / 180.0f;
            float x = cos(angle) * 40;
            float y = sin(angle) * 40;
            addObject(1705, pos.x + x, pos.y + y, 0.4f, 1);
        }
    }
    
    void applyOutline(CCPoint pos) {
        // Thick outline effect
        addObject(915, pos.x, pos.y, 1.3f, -1);
        addObject(915, pos.x, pos.y, 1.15f, 0);
        
        // Corner reinforcements
        for (int i = 0; i < 4; i++) {
            float x = (i % 2 == 0) ? -28.0f : 28.0f;
            float y = (i < 2) ? 28.0f : -28.0f;
            addObject(211, pos.x + x, pos.y + y, 0.6f, 1);
        }
        
        // Edge decorations
        addObject(1764, pos.x - 30, pos.y, 0.5f, 2);
        addObject(1764, pos.x + 30, pos.y, 0.5f, 2);
        addObject(1764, pos.x, pos.y - 30, 0.5f, 2);
        addObject(1764, pos.x, pos.y + 30, 0.5f, 2);
    }
    
    void applyParticles(CCPoint pos) {
        // Particle cloud
        for (int i = 0; i < 16; i++) {
            float angle = i * 22.5f * M_PI / 180.0f;
            float distance = 25 + (i % 3) * 10;
            float x = cos(angle) * distance;
            float y = sin(angle) * distance;
            addObject(1705, pos.x + x, pos.y + y, 0.3f + (i % 3) * 0.1f, 1);
        }
        
        // Center glow
        addObject(106, pos.x, pos.y, 1.8f, -1);
    }
    
    void applyEthereal(CCPoint pos) {
        // Ethereal wisps
        addObject(106, pos.x, pos.y, 2.8f, -3);
        addObject(106, pos.x - 15, pos.y + 15, 1.5f, -2);
        addObject(106, pos.x + 15, pos.y - 15, 1.5f, -2);
        
        // Floating elements
        for (int i = 0; i < 6; i++) {
            float angle = i * 60.0f * M_PI / 180.0f;
            float x = cos(angle) * 35;
            float y = sin(angle) * 35;
            addObject(1705, pos.x + x, pos.y + y, 0.5f, 1);
            addObject(1329, pos.x + x * 0.6f, pos.y + y * 0.6f, 0.4f, 2);
        }
    }
    
    void applyTech(CCPoint pos) {
        // Tech grid
        addObject(915, pos.x, pos.y, 1.4f, -1);
        
        // Circuit nodes
        for (int i = 0; i < 4; i++) {
            float x = (i % 2 == 0) ? -30.0f : 30.0f;
            float y = (i < 2) ? 30.0f : -30.0f;
            addObject(1764, pos.x + x, pos.y + y, 0.7f, 1);
            addObject(211, pos.x + x * 0.5f, pos.y + y * 0.5f, 0.4f, 2);
        }
        
        // Tech glow
        addObject(106, pos.x, pos.y, 1.6f, -2);
    }
    
    void applyMinimal(CCPoint pos) {
        // Subtle glow
        addObject(106, pos.x, pos.y, 1.3f, -1);
        
        // Corner dots
        addObject(1705, pos.x - 20, pos.y + 20, 0.3f, 1);
        addObject(1705, pos.x + 20, pos.y + 20, 0.3f, 1);
        addObject(1705, pos.x - 20, pos.y - 20, 0.3f, 1);
        addObject(1705, pos.x + 20, pos.y - 20, 0.3f, 1);
        
        // Thin outline
        addObject(915, pos.x, pos.y, 1.1f, 0);
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
