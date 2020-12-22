#include <cmath>
#include <cstdlib>
#include <iostream>
#include <string>

#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>

#include "al5poly/altypedef.hpp"
#include "al5poly/Animation.hpp"
#include "al5poly/AssetManager.hpp"
#include "al5poly/Camera.hpp"
#include "al5poly/Clock.hpp"
#include "al5poly/IAnimation.hpp"
#include "al5poly/IException.hpp"
#include "al5poly/IGameTime.hpp"
#include "al5poly/InputManager.hpp"
#include "al5poly/Player.hpp"
#include "al5poly/Renderer.hpp"

#define CHARACTERS() " \
        Squirrel(tm) (Name=???), \
        Puppy (Name=Watson), \
        Cat (Name=Lucky), \
        Rabbit (Name=???,Attributes=teen), \
        Mouse1, \
        Mouse2, \
        Mysterious Presence \
        "

const int SCREEN_W = 800;
const int SCREEN_H = 600;
const int PLAYER_W = 50;
const int PLAYER_H = 50;
const char * const PLAYER_ANIMATION_NAME = "player-animation";
const char * const PLAYER_BITMAP_NAME = "player-bitmap";

const char * const boolstr(bool);
al5poly::Player createPlayer(al5poly::AssetManager & assMan);

// The phrase that a 100% completion unlocks. Hehe, sneaky squirrels. The
// phrase is itself encoded though, requiring an additional secret item to
// decode it. Finding the secret item will have to require some hidden
// feature, like a false wall or something. E.g., Waterfall chamber hidden
// room.

static char phrase[] = "WORDS WORDS WORDS";

void initializeAllegro5(
        al5poly::ALLEGRO_DISPLAY_Ptr &,
        al5poly::ALLEGRO_TIMER_Ptr &,
        al5poly::ALLEGRO_EVENT_QUEUE_Ptr &);

int main(int argc, char ** argv) try
{
    const char * const TITLE = "Squirrel(tm) (Copyright (C) 2020 Brandon McCaig)";

    al5poly::ALLEGRO_DISPLAY_Ptr display;
    al5poly::ALLEGRO_TIMER_Ptr timer;
    al5poly::ALLEGRO_EVENT_QUEUE_Ptr eventQueue;

    initializeAllegro5(display, timer, eventQueue);

    al_set_window_title(display.get(), TITLE);

    al5poly::AssetManager assMan;
    al5poly::Camera camera;
    al5poly::Clock clock;
    al5poly::InputManager inputMan;
    al5poly::Renderer renderer(display);

    al5poly::Player player(createPlayer(assMan));

    player.setCurrentAnimation(PLAYER_ANIMATION_NAME);
    player.beginAnimation(1, *clock.getGameTime());

    al_start_timer(timer.get());

    al_set_target_bitmap(al_get_backbuffer(display.get()));
    al_clear_to_color(al_map_rgb(0, 0, 0));

    while(true)
    {
        ALLEGRO_EVENT event;
        bool tick = false;

        al_wait_for_event(eventQueue.get(), &event);

        if(event.type == ALLEGRO_EVENT_TIMER)
        {
            clock.tick();
            tick = true;
        }
        else if(event.type == ALLEGRO_EVENT_KEY_DOWN)
        {
            int keycode = event.keyboard.keycode;

            if(keycode == ALLEGRO_KEY_ESCAPE)
            {
                break;
            }

            inputMan.keyPress(keycode);
        }
        else if(event.type == ALLEGRO_EVENT_KEY_UP)
        {
            inputMan.keyRelease(event.keyboard.keycode);
        }
        else if(event.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
        {
            break;
        }

        // Logic.
        if(tick)
        {
            inputMan.sendEvents(*clock.getGameTime());

            // Expand...
        }

        // Drawing.
        if(tick)
        {
            try
            {
                renderer.render(*clock.getGameTime(), camera, player);
                renderer.paint();
            }
            catch(al5poly::IException & ex)
            {
                std::cerr << ex.getMessage() << std::endl;
            }
        }
    }

    return 0;
}
catch(std::exception & ex)
{
    std::cerr << ex.what() << std::endl;

    return 1;
}
catch(...)
{
    try
    {
        std::exception_ptr p = std::current_exception();
        std::cerr << "An unrecognized unhandled exception of type {" << (p ? p.__cxa_exception_type()->name() : "null") << "} was encountered.\n";
    }
    catch(...)
    {
        std::cerr << "An unrecognized unhandled exception of unknown type was encountered.\n";
    }

    return 1;
}

const char * const boolstr(bool value)
{
    return value ? "true" : "false";
}

al5poly::Player createPlayer(al5poly::AssetManager & assMan)
{
    al5poly::ALLEGRO_BITMAP_Ptr bitmap(al_create_bitmap(PLAYER_W, PLAYER_H), al_destroy_bitmap);

    if (bitmap.get() == NULL)
    {
        al5poly::Exception("Failed to create a simple bitmap for the player.").raise();
    }

    float radius = std::fabs((PLAYER_W + PLAYER_H) / (float)2);

    ALLEGRO_COLOR color = assMan.createColor("squirrel", 114, 63, 32, 240);

    ALLEGRO_BITMAP * old_target = al_get_target_bitmap();
    al_set_target_bitmap(bitmap.get());
    al_draw_circle(SCREEN_W / (float)2, SCREEN_H / (float)2, radius, color, 1.0f);
    al_set_target_bitmap(old_target);

    assMan.addBitmap(PLAYER_BITMAP_NAME, bitmap);

    al5poly::ALLEGRO_BITMAP_Ptr_Vector sprites;

    sprites.push_back(bitmap);

    al5poly::IAnimation::Ptr animation(new al5poly::Animation(sprites));

    al5poly::IAnimation::StringMap animations;

    std::pair<std::string, al5poly::IAnimation::Ptr> pair = std::make_pair(PLAYER_ANIMATION_NAME, animation);

    animations.insert(pair);

    al5poly::Player player(animations);

    return player;
}

void initializeAllegro5(
        al5poly::ALLEGRO_DISPLAY_Ptr & display,
        al5poly::ALLEGRO_TIMER_Ptr & timer,
        al5poly::ALLEGRO_EVENT_QUEUE_Ptr & eventQueue)
{
    const int FPS = 30;

    if(!al_init())
        al5poly::Exception("Failed to initialize Allegro 5!").raise();

    al_set_new_display_flags(ALLEGRO_WINDOWED);

    al5poly::ALLEGRO_DISPLAY_Ptr d(
            al_create_display(SCREEN_W, SCREEN_H),
            al_destroy_display);

    if(!d)
        al5poly::Exception("Failed to create Allegro 5 display!").raise();

    display = d;

    if(!al_install_keyboard())
        al5poly::Exception("Failed to install Allegro 5 keyboard!").raise();

    if(!al_init_image_addon())
        al5poly::Exception("Failed to initialize image addon.").raise();

    if(!al_init_primitives_addon())
        al5poly::Exception("Failed to initialize primitives addon.").raise();

    al5poly::ALLEGRO_TIMER_Ptr t(
            al_create_timer(1.0 / FPS),
            al_destroy_timer);

    if(!t)
        al5poly::Exception("Failed to create Allegro 5 timer!").raise();

    timer = t;

    al5poly::ALLEGRO_EVENT_QUEUE_Ptr eQ(
            al_create_event_queue(),
            al_destroy_event_queue);

    if(!eQ)
        al5poly::Exception("Failed to create Allegro 5 event queue!").raise();

    eventQueue = eQ;

    al_register_event_source(
            eventQueue.get(),
            al_get_display_event_source(display.get()));

    al_register_event_source(
            eventQueue.get(),
            al_get_keyboard_event_source());

    al_register_event_source(
            eventQueue.get(),
            al_get_timer_event_source(timer.get()));
}
