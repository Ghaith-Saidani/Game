#include "Resource Menu/Header/Constantes.h"
#include "Resource Menu/Header/Background.h"
#include "Resource Menu/Header/Music.h"
#include "Resource Menu/Header/Buttons.h"
#include "Resource Menu/Header/Text.h"
#include "Resource Menu/Header/Menu.h"
////////////////////////////////////////
#include "Resource Perso/personnage.h"
#include "Resource Back/background.h"
#include "Resource Back/collisionparfaite.h"
#include "Resource Entite/ennemy.h"
#include "Resource MiniMap/minimap.h"
#include "Resource Enigme5/enigme.h"
#include "Resource Enigme 6/EnigmeSF.h"
////////////////////////////////////////

void NewGame(SDL_Surface *screen, int *Mode)
{
    SDL_Init(SDL_INIT_EVERYTHING);
    FILE *fichier = NULL;
    fichier = fopen("Resource MiniMap/nomficher.txt", "a+");
    TTF_Init();
    TTF_Font *police = NULL;
    SDL_Color Color = {255, 255, 255};
    police = TTF_OpenFont("Resource MiniMap/pol.ttf", 50);

    SDL_Event event;
    /////////////////////
    Ennemi E;
    Enigme e;
    /////////////////////

    // Mounir's Updates:
    enigme eSF;
    eSF.num_enigme = -1;
    int s, r = 0, run = 1, running = 1, alea;
    char image[30] = "";
    int time = 0; //, r = 0;
    ///////////////////////
    ////wided decl/////

    background ba, bm;

    ////////////////////
    personne S;
    Uint8 *keys;
    Uint32 dt = 1, t_prev = 1;
    int impulsion = 6;
    int i;
    SDL_Rect camera;
    minimap m;
    int temps;
    /////////////////////
    int die = 0;
    int continuer = 1;
    int boucle = 1;
    int random;
    int Game;
    SDL_WM_SetCaption("NEW GAME", NULL);

    /////////////////////
    // wided init////

    initialisation_back(&ba); // initback
    initBackMasque(&bm);      // pour la collision parfaite

    // initialiserperso(&perso);
    //////////
    initperso(&S);
    initEnnemi(&E);
    init_map(&m);
    Init_Enigme(&e, "Resource Enigme5/questions.txt", "Resource Enigme5/reponses.txt", "Resource Enigme5/vraireponses.txt");
    init_enigme(&eSF, "Resource Enigme 6/enigme.txt");

    /////////////////////

    while (continuer)
    {

        /////////////////////
        t_prev = SDL_GetTicks();
        /////////////////////////////////////////

        // deplacerIA(&E, p.position_perso);

        //////////////////////////////////////////

        SDL_PollEvent(&event);
        switch (event.type)
        {

        case SDL_QUIT:
            continuer = 0;
            SaveScreenMode(screen, Mode);
            break;
        case SDL_KEYDOWN:
            switch (event.key.keysym.sym)
            {

            case SDLK_ESCAPE:
                continuer = 0;
                SaveScreenMode(screen, Mode);
                break;
            case SDLK_RIGHT:

                if (collisionparfaite(screen, S) == 10)
                {
                    scrolling(&ba, 0); // scrollingymin;
                    scrolling(&bm, 0);
                }

                break;
            case SDLK_LEFT:

                if (collisionparfaite(screen, S) == 10)
                {
                    scrolling(&ba, 1); // scrolllingysar;
                    scrolling(&bm, 1);
                }

                break;
            case SDLK_UP:

                if (collisionparfaite(screen, S) == 10)
                {
                    scrolling(&ba, 2); // scrollingymin;
                    scrolling(&bm, 2);
                }
                break;
            case SDLK_DOWN:

                if (collisionparfaite(screen, S) == 10)
                {
                    scrolling(&ba, 3); // scrollingymin;
                    scrolling(&bm, 3);
                }
                break;
            }
            break;
        }

        //////////////////////////////////////
        keys = SDL_GetKeyState(NULL);
        if (keys[SDLK_RIGHT] == 0 && keys[SDLK_LEFT] == 0 && keys[SDLK_j] == 0 && keys[SDLK_UP] == 0 && keys[SDLK_w] == 0)
        {
            S.direction = -1;
            S.deplacement = -1;
        }
        if (keys[SDLK_RIGHT] == 1)
        {
            S.deplacement = 1;
            S.direction = 0;
        }
        if (keys[SDLK_j])
        {
            S.direction = 2;
        }
        if (keys[SDLK_w])
        {
            S.direction = 4;
        }
        if (keys[SDLK_a])
        {
            S.acceleration = 0;
        }
        if (keys[SDLK_LEFT])
        {
            S.deplacement = 0;
            S.direction = 1;
        }
        if (keys[SDLK_UP] && S.etat == ETAT_SOL)
        {
            Saute(&S, impulsion);
            S.direction = 3;
        }

        //////////////////////////////////////
        afficheBack(bm, screen);
        collision(screen, &S);
        animerBackground(&ba);
        afficheBack2(ba, screen);
        afficherminimap(m, screen);
        affichertemp(&temps, screen, police);
        MAJMinimap(S.position_perso, &m, camera, 20);
        afficherperso(&S, screen);
        afficherEnnemi(E, screen);
        afficherEnnemi2(E, screen);
        animerEnnemi(&E);
        deplacer(&E);
        animerperso(&S);
        deplacerperso(&S, dt);
        Updateperso(&S, keys);
        dt = SDL_GetTicks() - t_prev;
        SDL_Flip(screen);
        SDL_Delay(10);
        // die = 0;
        die = collisionBB(S, E);
        if (die)
        {
            while (boucle == 1)
            {
                random = rand() % 2;
                switch (random)
                {
                case 0:
                    boucle = Play_Enigme(&e, screen, &Game);
                    if (Game)
                    {
                        printf("you win!!!");
                        //////////////////// IF CORRECT DONT LOSE LIFE
                    }
                    else if (!Game)
                    {
                        printf("you lost!!! %d ", Game);
                        //////////////////// IF INCORRECT LOSE LIFE
                    }
                    break;
                case 1:
                    afficherEnigme(eSF, screen);
                    afficher_resultat(screen, eSF.reponsevrai, r, &eSF);
                    boucle = 0;
                    break;
                }
            }
        }
    }
    /////////////////////////////////////
    for (i = 0; i < 20; i++)
    {
        SDL_FreeSurface(S.sprite[i]);
    }
    SDL_FreeSurface(S.jeu.HUD_etoiles);
    SDL_FreeSurface(S.jeu.HUD_vie);

    freeEnnemy(E);
    fclose(fichier);
}

void Settings(SDL_Surface *screen, int *Mode, int *niv_volume, int *volume)
{
    SDL_Init(SDL_INIT_EVERYTHING);
    //////////////////////////////
    SDL_Event event;
    Buttons B;
    Background Set;
    int continuer = 2;
    //////////////////////////////
    SDL_WM_SetCaption("SETTINGS", NULL);
    initBackSet(&Set);
    initButSet(&B);
    //////////////////////////////
    SDL_EnableKeyRepeat(0, 50);
    while (continuer)
    {
        SDL_PollEvent(&event);
        switch (event.type)
        {
        case SDL_QUIT:
            continuer = 0;
            SaveScreenMode(screen, Mode);
            break;

        case SDL_KEYDOWN:
            switch (event.key.keysym.sym)
            {
            case SDLK_ESCAPE:
                continuer = 0;
                SDL_BlitSurface(B.Quit, NULL, screen, &B.positionQuit);
                SDL_Flip(screen);
                SaveScreenMode(screen, Mode);
                break;
            case SDLK_LEFT:
                volumeDown(niv_volume, volume);
                SDL_BlitSurface(B.Moins, NULL, screen, &B.positionMoins);
                SDL_Flip(screen);
                break;
            case SDLK_RIGHT:
                volumeUp(niv_volume, volume);
                SDL_BlitSurface(B.Plus, NULL, screen, &B.positionPlus);
                SDL_Flip(screen);
                break;
            case SDLK_f:
                SetFull(screen, Mode);

                break;
            case SDLK_n:
                SetNorm(screen, Mode);

                break;
            }
            break;
        case SDL_MOUSEBUTTONUP:
            if (event.button.button == SDL_BUTTON_LEFT)
            {
                if (event.button.x > 260 && event.button.x < 380 && event.button.y > 210 && event.button.y < 330)
                {
                    volumeDown(niv_volume, volume);
                    SDL_BlitSurface(B.Moins, NULL, screen, &B.positionMoins);
                    SDL_Flip(screen);
                }
                else if (event.button.x > 610 && event.button.x < 730 && event.button.y > 210 && event.button.y < 330)
                {
                    volumeUp(niv_volume, volume);
                    SDL_BlitSurface(B.Plus, NULL, screen, &B.positionPlus);
                    SDL_Flip(screen);
                }
                else if (event.button.x > 410 && event.button.x < 494 && event.button.y > 440 && event.button.y < 528)
                {
                    SetFull(screen, Mode);
                }
                else if (event.button.x > 495 && event.button.x < 578 && event.button.y > 440 && event.button.y < 528)
                {
                    SetNorm(screen, Mode);
                }
                else if (event.button.x > 445 && event.button.x < 566 && event.button.y > 530 && event.button.y < 649)
                {
                    continuer = 0;
                    SDL_BlitSurface(B.Quit, NULL, screen, &B.positionQuit);
                    SaveScreenMode(screen, Mode);
                    break;
                }
            }
            break;
        }
        AfficherSet(Set, screen);
        AfficherButOption(B, screen);
        AfficherButMoins(B, screen);
        AfficherButPlus(B, screen);
        AfficherButScreen(B, screen, Mode);
        SDL_Flip(screen);
    }
    freeBackSet(Set);
    freeButSet(B);
}

void SaveScreenMode(SDL_Surface *screen, int *Mode)
{
    if (*Mode == 0)
    {
        SDL_Flip(screen);
        screen = SDL_SetVideoMode(SCREEN_W, SCREEN_H, 32, SDL_HWSURFACE | SDL_DOUBLEBUF);
    }
    else
    {
        SDL_Flip(screen);
        screen = SDL_SetVideoMode(SCREEN_W, SCREEN_H, 32, SDL_HWSURFACE | SDL_FULLSCREEN);
    }
    SDL_WM_SetCaption("Game Menu", NULL);
}

void SetFull(SDL_Surface *screen, int *Mode)
{
    *Mode = 1;
    screen = SDL_SetVideoMode(SCREEN_W, SCREEN_H, 32, SDL_HWSURFACE | SDL_FULLSCREEN);
}
void SetNorm(SDL_Surface *screen, int *Mode)
{
    *Mode = 0;
    screen = SDL_SetVideoMode(SCREEN_W, SCREEN_H, 32, SDL_HWSURFACE | SDL_ANYFORMAT);
}
