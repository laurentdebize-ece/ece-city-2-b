//
// Created by Louis Duvanel on 02/11/2022.
//

#include <unistd.h>
#include "include.h"
#include <allegro5/allegro_acodec.h>
#include <allegro5/allegro_audio.h>


void menuDebut(Principal *p) {

    assert(al_init());
    assert(al_init_image_addon());
    assert(al_init_primitives_addon());
    al_install_audio();
    al_init_acodec_addon();

    ///////////// initialisation des variables   ////////////////////////
    ALLEGRO_DISPLAY *display = NULL;
    ALLEGRO_EVENT_QUEUE *queue = NULL;
    ALLEGRO_BITMAP *image2;
    ALLEGRO_BITMAP *image3;
    ALLEGRO_BITMAP *image4;
    ALLEGRO_BITMAP *imageMenu = NULL;
    ALLEGRO_BITMAP *imageModeDeJeu = NULL;
    ALLEGRO_BITMAP *imageMode = NULL;

    ALLEGRO_SAMPLE *sample = NULL;
    ALLEGRO_SAMPLE_INSTANCE* songinstance = NULL;

    p->reprendrepartie = false;
    p->anciennepartiecharge = false;
    bool isEnd = false;
    bool isEndPlayerChoice = true;
    bool sourisDansRectangle = false;
    bool sourisDansRectangleCommuniste = false;
    bool sourisDansRectangleCapitaliste = false;
    int modeDeJeu;
    ALLEGRO_TIMER *timer = NULL;
    al_install_keyboard();
    al_install_mouse();
    al_reserve_samples(11);



    /////////////  verifications   ///////////////////////////////

    queue = al_create_event_queue();
    timer = al_create_timer(0.1);

    sample = al_load_sample("../music.wav");

    display = al_create_display(WIDTH, HIGH);
    //imageMenu = al_load_bitmap("../images/menu.png");
    image2 = al_load_bitmap("../images/100.png");
    image3 = al_load_bitmap("../images/102.png");
    image4 = al_load_bitmap("../images/103.png");
    imageModeDeJeu = al_load_bitmap("../images/CHOIX_du_mode.png");
    imageMenu = al_load_bitmap("../images/ECE CITY.png");
    imageMode = al_load_bitmap("../images/mode.png");

    assert(imageMenu);
    assert(imageModeDeJeu);
    assert(imageMode);
    assert(al_install_mouse());
    assert(al_install_keyboard());
    assert(queue);
    assert(display != NULL);

    songinstance= al_create_sample_instance(sample);
    al_set_sample_instance_playmode(songinstance,0);
    al_attach_sample_instance_to_mixer(songinstance,al_get_default_mixer());

    al_play_sample_instance(songinstance);
    al_set_window_title(display, "ECE City");
    al_clear_to_color(al_map_rgb(200, 180, 55));
    al_set_window_position(display, 0, 0);
    al_draw_scaled_bitmap(image2, 0, 0, 1024, 678, 0, 0, 1024, 678, 0);
    al_flip_display();
    sleep(1);

    al_draw_scaled_bitmap(image3, 0, 0, 1024, 678, 0, 0, 1024, 678, 0);
    al_flip_display();
    sleep(1);
    al_draw_scaled_bitmap(image4, 0, 0, 1024, 678, 0, 0, 1024, 678, 0);
    al_flip_display();
    sleep(1);

    al_draw_bitmap(imageMenu, 0, 0, 0);

    al_register_event_source(queue, al_get_display_event_source(display));
    al_register_event_source(queue, al_get_keyboard_event_source());
    al_register_event_source(queue, al_get_mouse_event_source());
    al_register_event_source(queue, al_get_timer_event_source(timer));
    al_set_target_backbuffer(display);

    al_start_timer(timer);
    al_flip_display();

    while (isEnd == false) {
        ALLEGRO_EVENT event = {0};
        al_wait_for_event(queue, &event);
        switch (event.type) {
            case ALLEGRO_EVENT_MOUSE_AXES:
                if (307 <= event.mouse.x && event.mouse.x <= 660 && 343 <= event.mouse.y && event.mouse.y <= 460) {
                    sourisDansRectangle = true;
                }
                else if (307 <= event.mouse.x && event.mouse.x <= 660 && 490 <= event.mouse.y && event.mouse.y <= 605){

                    p->reprendrepartie = true;
                } else {
                    sourisDansRectangle = false;
                    p->reprendrepartie = false ;
                }
                break;
            case ALLEGRO_EVENT_MOUSE_BUTTON_UP:
                if (sourisDansRectangle) {
                    isEnd = true;
                    isEndPlayerChoice = false;
                }
                if (p->reprendrepartie) {
                    p->anciennepartiecharge = true ;
                    isEnd = true;
                    isEndPlayerChoice = false;
                }
                break;
            case ALLEGRO_EVENT_DISPLAY_CLOSE :
                isEnd = true;
                p->leJeuEstLance = false;
                break;
            case ALLEGRO_EVENT_KEY_UP :
                switch (event.keyboard.keycode) {
                    case ALLEGRO_KEY_ESCAPE :
                        isEnd = true;
                        p->leJeuEstLance = false;
                        break;
                }
                break;
            case ALLEGRO_EVENT_TIMER:
                if (p->reprendrepartie) {
                    al_draw_bitmap(imageMenu, 0, 0, 0);
                    al_draw_filled_rounded_rectangle(303, 485, 670, 600, 30, 50, al_map_rgba(0,0, 170, 15));

                } else if (sourisDansRectangle) {
                    al_draw_bitmap(imageMenu, 0, 0, 0);
                    al_draw_filled_rounded_rectangle(303, 343, 660, 455, 30, 60, al_map_rgba(0,0, 170, 15));

                } else {
                    al_draw_bitmap(imageMenu, 0, 0, 0);
                }

                al_flip_display();
        }
    }
    while (isEndPlayerChoice == false) {
        al_draw_bitmap(imageModeDeJeu, 0, 0, 0);
        ALLEGRO_EVENT event = {0};
        al_wait_for_event(queue, &event);
        switch (event.type) {
            case ALLEGRO_EVENT_MOUSE_AXES:

                if (292 <= event.mouse.x && event.mouse.x <= 667 && 277 <= event.mouse.y && event.mouse.y <= 400) {
                    sourisDansRectangleCommuniste = true;
                } else if (292 <= event.mouse.x && event.mouse.x <= 667 && 453 <= event.mouse.y &&event.mouse.y <= 573) {
                    sourisDansRectangleCapitaliste = true;
                } else { sourisDansRectangleCommuniste = sourisDansRectangleCapitaliste = false; }
                break;
            case ALLEGRO_EVENT_DISPLAY_CLOSE :
                isEndPlayerChoice = true;
                p->leJeuEstLance = false;
                break;
            case ALLEGRO_EVENT_KEY_UP :
                switch (event.keyboard.keycode) {
                    case ALLEGRO_KEY_ESCAPE :
                        isEndPlayerChoice = true;
                        p->leJeuEstLance = false;
                        break;
                }
                break;
            case ALLEGRO_EVENT_MOUSE_BUTTON_UP:
                if (sourisDansRectangleCapitaliste) {
                    isEndPlayerChoice = true;
                    p->modeDeJeu = 1; // mode communiste
                } else if (sourisDansRectangleCommuniste) {
                    isEndPlayerChoice = true;
                   p-> modeDeJeu = 0; // mode capitaliste
                }
                break;
            case ALLEGRO_EVENT_TIMER:
                if (sourisDansRectangleCommuniste) {
                    al_draw_bitmap(imageModeDeJeu, 0, 0, 0);
                    al_draw_filled_rounded_rectangle(285, 277, 673, 400, 30, 60, al_map_rgba(0,0, 170, 15));
                } else if (sourisDansRectangleCapitaliste) {
                    al_draw_bitmap(imageModeDeJeu, 0, 0, 0);
                    al_draw_filled_rounded_rectangle(285, 453, 673, 573, 30, 60, al_map_rgba(0, 0, 170, 15));
                } else {
                    al_draw_bitmap(imageModeDeJeu, 0, 0, 0);
                }
                al_flip_display();
                break;
        }
    }

    al_destroy_display(display);
    al_destroy_event_queue(queue);
    al_destroy_bitmap(imageMode);
    al_destroy_bitmap(image2);
    al_destroy_bitmap(image3);
    al_destroy_bitmap(image4);
    al_destroy_bitmap(imageMenu);
    al_destroy_bitmap(imageModeDeJeu);
    al_destroy_sample(sample);
    al_destroy_sample_instance(songinstance);
    al_destroy_timer(timer);
}