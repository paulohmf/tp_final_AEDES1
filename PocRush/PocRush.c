#include <stdio.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <allegro5/allegro_image.h>
#include <stdlib.h>
#include <math.h>

#define NUMCARS 10

enum KEYS{ RIGHT , LEFT , UP, DOWN};

const float FPS = 100;  
const int SCREEN_W = 960;
const int SCREEN_H = 540;

//-----------------------------------------Presentes do Pedro---------------------------------------------------//
int SKY_H;

float CAR_W = 100;
float CAR_H = 20;
float WHEEL_W = 30;
float WHEEL_H = 40;

float TRACK_TOP_W;
float TRACK_BOTTOM_W;

float TRACK_TOP_LEFT_X;
float TRACK_TOP_LEFT_Y;
float TRACK_BOTTOM_LEFT_X;
float TRACK_BOTTOM_LEFT_Y;

float TRACK_TOP_RIGHT_X;
float TRACK_TOP_RIGHT_Y;
float TRACK_BOTTOM_RIGHT_X;
float TRACK_BOTTOM_RIGHT_Y;

float THETA;
//----------------------------------------------------------------------------------------------------------------//
//---------------------------------------------------Minhas gambiarras--------------------------------------------//
//BARRA CONTROLADA PELO USUARIO//
int x_bar=(960/2-50); //SCREEN_W/2-70
int x2_bar=(960/2+50);
int y_bar=(540-40); //(SCREEN_H-40
int y2_bar=(540);
//
//vetor que salva as cores dos inimigos
int cor[9][3];
//Cont eh o tempo em segundos & Blocos e o numero de blocos passados
float cont=0.0;
int blocos=0;

//Strings de Score e Highscore
char my_text[20];
char text[20];

//CORAÇÃO DO JOG0
int playing = 1;
//
//Auxiliar para variar a trajetoria dos Batmans
int varia=1;
int limite=0;
int played = 0;



/*
const float FPS = 100;  
const int SCREEN_W = 640*1.5;
const int SCREEN_H = 540;

int SKY_H = SCREEN_H/4;

const float CAR_W = 100;
const float CAR_H = 50;

const int TRACK_TOP_W = SCREEN_W/10;
const int TRACK_BOTTOM_W = SCREEN_W/5;

const int TRACK_TOP_LEFT_X = SCREEN_W/2 - TRACK_TOP_W/2;
const int TRACK_TOP_LEFT_Y = SKY_H;
const int TRACK_BOTTOM_LEFT_X = SCREEN_W/2 - TRACK_BOTTOM_W/2;
const int TRACK_BOTTOM_LEFT_Y = SCREEN_H;
*/

//--------------------------------------STRUCTURES DE INIMIGO E BONUS (BATMANS)---------------------------------------//
typedef struct Inimigo {
	float pos_x,pos_x2;
	float pos_y,pos_y2;
	int RGB[3];
} Inimigo;

typedef struct Batman{
	float pos_x,pos_x2;
	float pos_y,pos_y2;
} Batman;
//--------------------------------------------------------------------------------------------------------------------//
//Presente do Pedro
void init_global_vars() {
	
	SKY_H = SCREEN_H/4;

	TRACK_TOP_W = SCREEN_W/100;
	TRACK_BOTTOM_W = SCREEN_W/1.2;
	
	TRACK_TOP_LEFT_X = SCREEN_W/2 - TRACK_TOP_W/2;
	TRACK_TOP_LEFT_Y = SKY_H;
	TRACK_BOTTOM_LEFT_X = SCREEN_W/2 - TRACK_BOTTOM_W/2;
	TRACK_BOTTOM_LEFT_Y = SCREEN_H;
	
	TRACK_TOP_RIGHT_X = SCREEN_W/2 + TRACK_TOP_W/2;
	TRACK_TOP_RIGHT_Y = SKY_H;
	TRACK_BOTTOM_RIGHT_X = SCREEN_W/2 + TRACK_BOTTOM_W/2;
	TRACK_BOTTOM_RIGHT_Y = SCREEN_H;

	//track angle
	THETA = atan((TRACK_BOTTOM_LEFT_Y - TRACK_TOP_LEFT_Y)/(TRACK_TOP_LEFT_X - TRACK_BOTTOM_LEFT_X));
	
	
}
//

//------------------------------Funções que Iniciam e Variam os INIMIGOS E BATMANS na tela--------------------------//
void Iniciar_Music(Batman *Batman){
	Batman->pos_x = SCREEN_W/2;
	Batman->pos_x2 = (SCREEN_W/2)+5;
	Batman->pos_y = (SCREEN_H/4)-1000;
	Batman->pos_y2 =  (SCREEN_H/4)+5-1000;

}

void variar_MusicY(Batman *Batman){
	if(Batman->pos_y<SCREEN_H){
		Batman->pos_y += 1;
		Batman->pos_y2 += 1;
	}else{
		Batman->pos_y = (SCREEN_H/4)-1000;
		Batman->pos_y2 = (SCREEN_H/4)+5-1000;
	}
}

void variar_taMusic(Batman *Batman){
	if(Batman->pos_y<SCREEN_H){
		Batman->pos_x -= 0.05;
		Batman->pos_x2 += 0.05;
		Batman->pos_y -= 0.05 ;
		Batman->pos_y2 += 0.05;
		Batman->pos_x += 0.1*(-3)*varia;
		Batman->pos_x2 += 0.1*(-3)*varia;
	}else{
		Batman->pos_x = SCREEN_W/2;
		Batman->pos_x2 = (SCREEN_W/2)+5;
	}
}


void iniciar_inimigos(Inimigo *foe, int R,int G,int B,int i){
	foe->pos_x = SCREEN_W/2;
	foe->pos_x2 = (SCREEN_W/2)+10;
	foe->pos_y = (SCREEN_H/4)-(140*i);
	foe->pos_y2 =  (SCREEN_H/4)+10-(140*i);

	foe->RGB[0] = R;
	foe->RGB[0] = G;
	foe->RGB[0] = B;

	cor[i][0]=R;
	cor[i][1]=G;
	cor[i][2]=B;
	
}

void variar_inimigosY(Inimigo *foe){
	if(foe->pos_y<SCREEN_H){
		foe->pos_y += 1;
		foe->pos_y2 += 1;
	}else{
		foe->pos_y = (SCREEN_H/4)-(140*9);
		foe->pos_y2 = (SCREEN_H/4)+10-(140*9);
	}
}

void variar_inimigosX(Inimigo *foe,int j){
	if(foe->pos_y<SCREEN_H){
		foe->pos_x += 0.1*j;
		foe->pos_x2 += 0.1*j;
	}else{
		foe->pos_x = SCREEN_W/2;
		foe->pos_x2 = (SCREEN_W/2)+10;
	}
}

void variar_tam(Inimigo *foe){
	if(foe->pos_y<SCREEN_H){
		foe->pos_x -= 0.1;
		foe->pos_x2 += 0.1;
		foe->pos_y -= 0.1 ;
		foe->pos_y2 += 0.1;
	}else{
		foe->pos_x = SCREEN_W/2;
		foe->pos_x2 = (SCREEN_W/2)+10;
		foe->pos_y = (SCREEN_H/4)-(50*9);
		foe->pos_y2 =  (SCREEN_H/4)+10-(50*9);
		blocos++;
	}
}
//--------------------------------------------------------------------------------------------------------------------//

void draw_scenario(ALLEGRO_DISPLAY *display) {
	
	//grass
	ALLEGRO_COLOR BKG_COLOR = al_map_rgb(105,105,105); 
	al_set_target_bitmap(al_get_backbuffer(display));
	al_clear_to_color(BKG_COLOR);   
	
	//sky
	al_draw_filled_rectangle(0, 0, SCREEN_W, SKY_H, al_map_rgb(0,0,0));
   
   //desenha a pista:
   al_draw_line(TRACK_TOP_LEFT_X, TRACK_TOP_LEFT_Y, TRACK_BOTTOM_LEFT_X, TRACK_BOTTOM_LEFT_Y, al_map_rgb(255,255,255), 10); 
   al_draw_line(TRACK_TOP_RIGHT_X, TRACK_TOP_RIGHT_Y, TRACK_BOTTOM_RIGHT_X, TRACK_BOTTOM_RIGHT_Y, al_map_rgb(255,255,255), 10); 

   //a barra do usuario é desenhada de cores diferentes dependendo do numero de escudos
	if(playing==1){
		//Nenhum escudo VINHO
		al_draw_filled_rectangle(x_bar,y_bar,x2_bar, y2_bar, al_map_rgb(255, 105, 180)); 
	}
	if(playing>=2){
		//Um escudo YELLOW
		al_draw_filled_rectangle(x_bar,y_bar,x2_bar-(5.0/6.0*100), y2_bar, al_map_rgb(231, 0, 0));
		al_draw_filled_rectangle(x_bar+(1.0/6.0*100),y_bar,x2_bar-(4.0/6.0*100), y2_bar, al_map_rgb(255, 140, 0)); 
		al_draw_filled_rectangle(x_bar+(2.0/6.0*100),y_bar,x2_bar-(3.0/6.0*100), y2_bar, al_map_rgb(255, 239, 0));
		al_draw_filled_rectangle(x_bar+(3.0/6.0*100),y_bar,x2_bar-(2.0/6.0*100), y2_bar, al_map_rgb(0, 129, 31));
		al_draw_filled_rectangle(x_bar+(4.0/6.0*100),y_bar,x2_bar-(1.0/6.0*100), y2_bar, al_map_rgb(0, 68, 255)); 
		al_draw_filled_rectangle(x_bar+(5.0/6.0*100),y_bar,x2_bar-(0.0/6.0*100), y2_bar, al_map_rgb(118, 0, 137));

	}/*
	if(playing>=3){
		//Mais de um escudo GOLDEN
		al_draw_filled_rectangle(x_bar,y_bar,x2_bar, y2_bar, al_map_rgb(0,0,0)); 
	}*/

}

//-------------------------------------------------------------------------------------------------------------------//


 
int main(int argc, char **argv){
	
	//srand(time(NULL));
	srand(2);
	ALLEGRO_DISPLAY *display = NULL;
	ALLEGRO_EVENT_QUEUE *event_queue = NULL;
	ALLEGRO_TIMER *timer = NULL;
	//Auxiliar para animar o Unicornio que eu coloquei quando estava bêbado
	int uni=0;

	/*POWER UPS sao chamados de BATMANs - a ideia original era tocar so um NANANANANAANANANNANANANANANANANANANANNAANNA
	mas acabei transformando eles em powerups*/
	Batman Batman;
	Iniciar_Music(&Batman);

	//Vetor com 9 inimigos e inicialização
	Inimigo foe[10];
	int i;
	int R=0,G=0,B=0;
	for(i=0;i<9;i++){
		iniciar_inimigos(&foe[i],R,G,B,i);
		R+=25;
		G+=25;
		B+=25;
	}


	
	init_global_vars();

	
   
	//----------------------- rotinas de inicializacao ---------------------------------------
   if(!al_init()) {
      fprintf(stderr, "failed to initialize allegro!\n");
      return -1;
   }
   
    if(!al_init_primitives_addon()){
		fprintf(stderr, "failed to initialize primitives!\n");
        return -1;
    }
 
   timer = al_create_timer(1.0 / FPS);
   if(!timer) {
      fprintf(stderr, "failed to create timer!\n");
      return -1;
   }
 
   display = al_create_display(SCREEN_W, SCREEN_H);
   if(!display) {
      fprintf(stderr, "failed to create display!\n");
      al_destroy_timer(timer);
      return -1;
   }
   //-----------------------------------Animação do Unicornio---------------------------------//
   	al_init_image_addon();
   	ALLEGRO_BITMAP *image = al_load_bitmap("uni1.png");
   	ALLEGRO_BITMAP *image2 = al_load_bitmap("uni2.png");
   	ALLEGRO_BITMAP *image3 = al_load_bitmap("menu1.png");
   	ALLEGRO_BITMAP *image4 = al_load_bitmap("menu2.png");
   	ALLEGRO_BITMAP *image5 = al_load_bitmap("poxa.png");


	//inicializa o modulo allegro que carrega as fontes
    al_init_font_addon();
	//inicializa o modulo allegro que entende arquivos tff de fontes
    al_init_ttf_addon();
    //Modulos necessarios para a musica
    al_install_audio();	
    al_init_acodec_addon();

    //---------------------------------Relacionados a escrever na tela------------------------------//
	//carrega o arquivo arial.ttf da fonte Arial e define que sera usado o tamanho 32 (segundo parametro)
    ALLEGRO_FONT *size_32 = al_load_font("arial.ttf", 32, 1);   	

	sprintf(my_text, "Score: %d", blocos); //cont
		 
	//al_draw_text(size_32, al_map_rgb(128, 200, 30), 10, 10, 0, my_text);


 

   event_queue = al_create_event_queue();
   if(!event_queue) {
      fprintf(stderr, "failed to create event_queue!\n");
      al_destroy_display(display);
      al_destroy_timer(timer);
      return -1;
   }
   
   al_install_keyboard();
   
	
	//registra na fila de eventos que eu quero identificar quando a tela foi alterada
   al_register_event_source(event_queue, al_get_display_event_source(display));
	//registra na fila de eventos que eu quero identificar quando o tempo alterou de t para t+1
   al_register_event_source(event_queue, al_get_timer_event_source(timer));
   
   al_register_event_source(event_queue, al_get_keyboard_event_source());   

   //------------------------------------Inicialização da Background Music-----------------------------//
    al_reserve_samples(4);
   	ALLEGRO_SAMPLE *song = al_load_sample("menu2.ogg");
   	ALLEGRO_SAMPLE *soundEffect = al_load_sample("yuke.wav");
   	ALLEGRO_SAMPLE *soundEffect2 = al_load_sample("ressucita.wav");
   	ALLEGRO_SAMPLE *soundEffect3 = al_load_sample("button-09.wav");
	ALLEGRO_SAMPLE_INSTANCE *songInstance = al_create_sample_instance(song);

   	ALLEGRO_SAMPLE *song3 = al_load_sample("gameover.ogg");
	ALLEGRO_SAMPLE_INSTANCE *songInstance3 = al_create_sample_instance(song3);

	ALLEGRO_SAMPLE *song2 = al_load_sample("battle1.ogg");
	ALLEGRO_SAMPLE_INSTANCE *songInstance2 = al_create_sample_instance(song2);
   

  	al_set_sample_instance_playmode(songInstance, ALLEGRO_PLAYMODE_LOOP);
  	al_attach_sample_instance_to_mixer(songInstance, al_get_default_mixer());


  	al_play_sample_instance(songInstance);
  	
	//reinicializa a tela
   	al_flip_display();
	//inicia o temporizador
   	al_start_timer(timer);

   //-------------------------------------Keyboard------------------------------------------------------//
   bool keys[4] = {false,false,false,false};
   int menu = 1;
   
	//enquanto playing for verdadeiro, faca:
   while(playing) {
      ALLEGRO_EVENT ev;
	  //espera por um evento e o armazena na variavel de evento ev
      al_wait_for_event(event_queue, &ev);

		if(ev.type == ALLEGRO_EVENT_KEY_DOWN) {
			//verifica qual tecla foi
			switch(ev.keyboard.keycode) {
				//se a tecla for o W
				case ALLEGRO_KEY_W:
					keys[UP] = true;
				break;
				//se a tecla for o S
				case ALLEGRO_KEY_S:
					keys[DOWN] = true;
				break;

				case ALLEGRO_KEY_A:
					keys[LEFT] = true;

				break;

				case ALLEGRO_KEY_D:
					keys[RIGHT] = true;
				break;

				case ALLEGRO_KEY_ESCAPE:
					playing = 0;
				break;

				case ALLEGRO_KEY_ENTER:
					menu = 0;
					if(limite==0){
						al_destroy_bitmap(image3);
						al_destroy_sample(song);
		   				al_destroy_sample_instance(songInstance);


		  				al_set_sample_instance_playmode(songInstance2, ALLEGRO_PLAYMODE_LOOP);
		  				al_attach_sample_instance_to_mixer(songInstance2, al_get_default_mixer());


		  				al_play_sample_instance(songInstance2);
		  				limite++;
		  			}
					break;
				}

		}	 
		else if(ev.type == ALLEGRO_EVENT_KEY_UP) {
			//verifica qual tecla foi
			switch(ev.keyboard.keycode) {
				case ALLEGRO_KEY_W:
					keys[UP] = false;
				break;
				//se a tecla for o S
				case ALLEGRO_KEY_S:
					keys[DOWN] = false;
				break;

				case ALLEGRO_KEY_A:
					keys[LEFT] = false;
				break;

				case ALLEGRO_KEY_D:
					keys[RIGHT] = false;
							
				break;
				}

			}

	
	//se o tipo de evento for um evento do temporizador, ou seja, se o tempo passou de t para t+1
    else if(ev.type == ALLEGRO_EVENT_TIMER) {
		if(menu==0){
			//Desenha o cenário
			draw_scenario(display);

			//Animação do unicornio que eu coloquei quando tava doido
			if(uni<=(FPS/2)){
				al_draw_bitmap(image, 960-(32*4), (SCREEN_H/4)-(24*4), 0);
				uni++;
			}else{
				al_draw_bitmap(image2, 960-(32*4), (SCREEN_H/4)-(24*4), 0);
				uni++;
				if(uni==FPS){
					uni=0;
				}
			}
			if((blocos%15==0)&&(blocos>0)){
				al_play_sample(soundEffect, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, 0);
				al_draw_filled_rectangle(0,80,SCREEN_W-128,87,al_map_rgb(255,0,212));

			}

			//Contador de tempo
			cont+=(1/FPS);

			//Score
			sprintf(my_text, "Score: %d", blocos);//blocos


			//Movimentação dos PowerUps na tela  (BAATMANNNNNN !!!!!!!!!!!!)
			if(Batman.pos_y2>(SCREEN_H/4)){
				al_draw_filled_rectangle(Batman.pos_x,Batman.pos_y,Batman.pos_x2,Batman.pos_y2, al_map_rgb(57,255,20));	
			}
			variar_MusicY(&Batman);
			if(Batman.pos_y2>(SCREEN_H/4)){
				variar_taMusic(&Batman);	
			}
			//
				
			//Movimentação dos Inimigos na tela
			int j,aux;
			for(j=0,aux=0;j<9;j++,aux++){

				variar_inimigosY(&foe[j]);
				//Inimigos invisiveis fora da pista
				foe[j].RGB[0] = 0;
				foe[j].RGB[1] = 0;
				foe[j].RGB[2] = 0;

				if(foe[j].pos_y2>(SCREEN_H/4)){
					//Inimigos visiveis dentro da pista
					foe[j].RGB[0] = cor[j][0];
					foe[j].RGB[1] = cor[j][1];
					foe[j].RGB[2] = cor[j][2];

					if(j%2==0){
						aux=aux*(-1);
					}
					variar_inimigosX(&foe[j],aux);
					if(aux<=0){
						aux=aux*(-1);
					}
					variar_tam(&foe[j]);
				}
			}

			/* PARA TESTAR UM INIMIGO
			variar_inimigosY(&foe[0]);
			variar_inimigosX(&foe[0],1);
			variar_tam(&foe[0]);
			*/

			//Desenha os inimigos na Tela
			int i;
			for(i=0;i<9;i++){
				al_draw_filled_rectangle(foe[i].pos_x,foe[i].pos_y,foe[i].pos_x2,foe[i].pos_y2, al_map_rgb(foe[i].RGB[0],foe[i].RGB[1],foe[i].RGB[2]));
			}

			//Desenha seu score na tela
			al_draw_text(size_32, al_map_rgb(255, 105, 180), 10, 10, 0, my_text);

			//Movimentação do Jogador baseado em teclas apertadas
			if(x_bar>TRACK_BOTTOM_LEFT_X){
				x_bar -= keys[LEFT]*2;
				x2_bar -= keys[LEFT]*2;
			}
			if(x2_bar<TRACK_BOTTOM_RIGHT_X){
				x_bar += keys[RIGHT]*2;
				x2_bar += keys[RIGHT]*2;
			}
			//

			//HITBOX do powerUp shield e seu efeito
			if((Batman.pos_y2>=y_bar)&&(Batman.pos_y<=y2_bar)){
					if((Batman.pos_x<=x2_bar)&&(Batman.pos_x>=x_bar)){
						Batman.pos_y = (SCREEN_H/4)-1100;
						Batman.pos_y2 = (SCREEN_H/4)+5-1100;
						Batman.pos_x = SCREEN_W/2;
						Batman.pos_x2 = (SCREEN_W/2)+5;
						varia= varia*(-1);
						if(playing<3){
							playing++;
							al_play_sample(soundEffect3, 0.9, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, 0);
						}
					}else{
						if((Batman.pos_x2>=x_bar)&&(Batman.pos_x2<=x2_bar)){
							Batman.pos_y = (SCREEN_H/4)-1100;
							Batman.pos_y2 = (SCREEN_H/4)+5-1100;
							Batman.pos_x = SCREEN_W/2;
							Batman.pos_x2 = (SCREEN_W/2)+5;
							varia= varia*(-1);
							if(playing<3){
								playing++;
								al_play_sample(soundEffect3, 0.9, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, 0);
							}
						}
					}
				
			}
			
			//-----------------------------------------------HITBOX dos inimigos----------------------------------------//
			int x;
			for(x=0;x<9;x++){
				if((foe[x].pos_y2>=y_bar)&&(foe[x].pos_y<=y2_bar)){

					if((foe[x].pos_x<=x2_bar)&&(foe[x].pos_x>=x_bar)){
						
	  						foe[x].pos_x = SCREEN_W/2;
							foe[x].pos_x2 = (SCREEN_W/2)+10;


							int gamb=(foe[x].pos_y2-foe[x].pos_y);
							foe[x].pos_y = (SCREEN_H/4)-(50*9)-40-(gamb);
							foe[x].pos_y2 =  (SCREEN_H/4)+10-(50*9)-40-(gamb);

							playing--;
							
							//Troca a musica caso perca
							if(playing==0){
								//game over music
								al_destroy_sample(song2);
	   							al_destroy_sample_instance(songInstance2);

	  							al_set_sample_instance_playmode(songInstance3, ALLEGRO_PLAYMODE_ONCE);
	  							al_attach_sample_instance_to_mixer(songInstance3, al_get_default_mixer());


	  							al_play_sample_instance(songInstance3);
	  							//playing=0;
	  							//
							}else{
								//Caso tenha shield, aumenta o placar
								blocos++;
								//RESSUCITAAAAAAA
								al_play_sample(soundEffect2, 1.5, 0.0, 1.1, ALLEGRO_PLAYMODE_ONCE, 0);

							}
					}else{
						if((foe[x].pos_x2>=x_bar)&&(foe[x].pos_x2<=x2_bar)){

							foe[x].pos_x = SCREEN_W/2;
							foe[x].pos_x2 = (SCREEN_W/2)+10;

							int gamb=(foe[x].pos_y2-foe[x].pos_y);
							foe[x].pos_y = (SCREEN_H/4)-(50*9)-40-(gamb);
							foe[x].pos_y2 =  (SCREEN_H/4)+10-(50*9)-40-(gamb);


							playing--;
							if(playing==0){
								//game over music
								al_destroy_sample(song2);
	   							al_destroy_sample_instance(songInstance2);

	  							al_set_sample_instance_playmode(songInstance3, ALLEGRO_PLAYMODE_ONCE);
	  							al_attach_sample_instance_to_mixer(songInstance3, al_get_default_mixer());


	  							al_play_sample_instance(songInstance3);
	  							//
							}else{
								blocos++;
								//RESSUCITAAAAAAA
								al_play_sample(soundEffect2, 1.5, 0.0, 1.1, ALLEGRO_PLAYMODE_ONCE, 0);
							}
						}
					}
					played =1;
					/*if(playing<=0){
						playing=0;
					}*/
				}
			}
		}else{
			if(uni<=(FPS/2)){
				al_draw_bitmap(image3, 0, 0,0);
				uni++;
			}else{
				al_draw_bitmap(image4, 0, 0,0);
				uni++;
				if(uni==FPS){
					uni=0;
				}
			}
			al_flip_display();

		}
		//-----------------------------------------------------------------------------------------------------------//
		//reinicializo a tela
		al_flip_display();
		
    }
	//se o tipo de evento for o fechamento da tela (clique no x da janela)
	else if(ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
		playing = 0;

	}
	
  }
   //fim do while
	 	//-----------------------------------ARQUIVO PARA PUXA E SALVAR O SCORES E HIGHSCORES---------------------------//
	if(played){	
	  	FILE *arq;
	  	arq = fopen("recorde.txt","r");

		int highscore;
	  	fscanf(arq,"%d",&highscore);

	  	if(blocos>highscore){
	  		highscore=blocos;
	  		arq = fopen("recorde.txt","w");
	  		fprintf(arq,"%d",highscore);
	  	}
	  	fclose(arq);

	  	//desenha o high score na tela
	  	al_draw_filled_rectangle(0, 0, SCREEN_W, SCREEN_H, al_map_rgb(0,0,0));
	  	al_draw_bitmap(image5, 0, 0,0);
	  	sprintf(text, "High Score: %d",highscore);
			 
		al_draw_text(size_32, al_map_rgb(255, 105, 180), SCREEN_H/2, SCREEN_W/2, 0, text);

		if(blocos==highscore){
			sprintf(my_text, "New High Score: %d",highscore);
		}else{
		//desenha seu score na tela
		sprintf(my_text, "Score: %d",blocos);
		}

		al_draw_text(size_32, al_map_rgb(255, 105, 180), SCREEN_H/2, SCREEN_W/2-60, 0, my_text);
		//--------------------------------------------------------------------------------------------------------------//
		
		//reinicializa a tela
		al_flip_display();	
	    al_rest(5);	
	}

  
   al_destroy_timer(timer);
   al_destroy_display(display);
   al_destroy_event_queue(event_queue);
   al_destroy_sample(song);
   al_destroy_sample_instance(songInstance);
   al_destroy_sample(song2);
   al_destroy_sample_instance(songInstance2);
   al_destroy_sample(song3);
   al_destroy_sample_instance(songInstance3);
   al_destroy_sample(soundEffect);
   al_destroy_sample(soundEffect2);
   al_destroy_sample(soundEffect3);
   al_destroy_bitmap(image);
   al_destroy_bitmap(image2);
   al_destroy_bitmap(image3);
   al_destroy_bitmap(image4);
   al_destroy_bitmap(image5);


   
 
   return 0;
}