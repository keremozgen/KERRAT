#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <gtk/gtk.h>
#include <pthread.h>
#include <sys/time.h>
/*
"pardus_gray": "#383838",
"pardus_orange": "#FF6C00",
"pardus_white": "#EEEEEE",
"green": "#8bc34a",
"blue": "#59abe3",
"red": "#fe0d00"
 bright yellow : #ffdc70
 dark yellow : #fba100
dark blue : #1256a0
 light red : #f2b094
 blue : @2d88e5
 */
GdkRGBA pardusGray = {((double)0x38/(256.0)),((double)0x38/(256.0)),((double)0x38/(256.0)),0.9};
GdkRGBA pardusOrange = {((double)0xff/(256.0)),((double)0x6c/(256.0)),((double)0x00/(256.0)),0.9};
GdkRGBA pardusGreen = {((double)0x8b/(256.0)),((double)0xc3/(256.0)),((double)0x4a/(256.0)),0.9};
GdkRGBA pardusBlue = {((double)0x59/(256.0)),((double)0xab/(256.0)),((double)0xe3/(256.0)),0.9};
GdkRGBA pardusRed = {((double)0xfe/(256.0)),((double)0x0d/(256.0)),((double)0x00/(256.0)),0.9};
GdkRGBA pardusBryellow = {((double)0xff/(256.0)),((double)0xdc/(256.0)),((double)0x70/(256.0)),0.9};
GdkRGBA pardusDryellow = {((double)0xfb/(256.0)),((double)0xa1/(256.0)),((double)0x00/(256.0)),0.9};
GdkRGBA pardusDrblue = {((double)0x12/(256.0)),((double)0x56/(256.0)),((double)0xa0/(256.0)),0.9};
GdkRGBA pardusLred = {((double)0xf2/(256.0)),((double)0xb0/(256.0)),((double)0x94/(256.0)),0.9};
/*GdkRGBA pardusGray = {0.0,0.093645485,0.093645485,0.093645485};
GdkRGBA pardusGray = {0.0,0.093645485,0.093645485,0.093645485};
GdkRGBA pardusGray = {0.0,0.093645485,0.093645485,0.093645485};
GdkRGBA pardusGray = {0.0,0.093645485,0.093645485,0.093645485};
GdkRGBA pardusGray = {0.0,0.093645485,0.093645485,0.093645485};*/


GtkWidget* window;
volatile GtkWidget* resWindow;
GtkWidget* button;
GtkWidget* button_box;
GtkEntryBuffer* input_buffer;	//INPUT TEXT
GtkWidget* question_label;

GtkWidget* buttonn;
GtkWidget* button0;
GtkWidget* buttonc;
GtkWidget* button1;
GtkWidget* button2;
GtkWidget* button3;
GtkWidget* button4;
GtkWidget* button5;
GtkWidget* button6;
GtkWidget* button7;
GtkWidget* button8;
GtkWidget* button9;

int total = 0;
int resetted = 0;
int wh = 0;
int ww = 0;
float ratio = 0.0f;
int sw = 0, sh = 0;
#define kace(x,y,f) gtk_container_add(GTK_CONTAINER(x), y); g_signal_connect(GTK_CONTAINER(y), "button_release_event", f, NULL); gtk_widget_set_hexpand(y, TRUE);
GtkCssProvider* cssProvider;

struct timeval volatile start = { 0 };

struct timeval volatile ftime = {0} , ltime = {0}, diftime = {0};
int c = 0;
static gboolean resizeWidget(GtkWidget* widget, GdkEvent* event, gpointer user_data) {

    if(event->type == GDK_CONFIGURE){
        c++;
        if(ftime.tv_sec == 0 && ftime.tv_usec == 0){
            gettimeofday(&ftime,NULL);
        }else{
            gettimeofday(&ltime,NULL);

            int tw, th;
            gtk_window_get_size(window, &tw, &th);
            if (!(tw == ww && th == wh)) {
                //    gtk_window_resize(window, sw, 1);
                GtkCssProvider* cssProvider = gtk_css_provider_new();
                char* cssFormat = "* {font-size: %dpx ;}";
                size_t cssLen = strlen(cssFormat) + 9;
                char* cssStr = calloc(cssLen + 1, sizeof(char));
                float nfs = (tw > th) ? ((float)tw/(float)sw) * 12.0 : ((float)th/(float)sh) * 12.0;
                snprintf(cssStr, cssLen, cssFormat, (int)nfs);
                gtk_css_provider_load_from_data(cssProvider, cssStr, -1, NULL);
                gtk_style_context_add_provider_for_screen(gdk_screen_get_default(),
                                                          GTK_STYLE_PROVIDER(cssProvider),
                                                          GTK_STYLE_PROVIDER_PRIORITY_USER);
                free(cssStr);
                int x = 0, y = 0;
                gtk_window_get_size(window, &ww, &wh);
            }
        }
    }
}

void createRandomQuestion() {
    int numberCount = (rand() % 3) + 2;
    int totalLen = numberCount + ((numberCount - 1) * 3) + 1;
    int numbers[5] = { -1,-1,-1,-1,-1 };
    char* ops = "+-*";
    char operators[4] = { ' ', ' ', ' ', ' ' };

    for (int i = 0; i < numberCount; i++)
    {
        numbers[i] = (rand() % 9) + 1;
    }
    for (int i = 0; i < numberCount - 1; i++)
    {
        operators[i] = ops[rand() % 3];
    }
    int answer = 0;


    char* questionString = calloc(totalLen + 1, sizeof(char));
    memset(questionString, ' ', totalLen);

    for (int i = 0; i < numberCount; i++)
    {
        questionString[i * 4] = '0' + numbers[i];
    }
    for (int i = 0; i < numberCount - 1; i++)
    {
        questionString[i * 4 + 2] = operators[i];
    }

    printf("%s is the string %d\n", questionString, numberCount);
    gtk_label_set_text(question_label, questionString);
    free(questionString);
    for (int i = 0; i < numberCount - 1; i++)
        for (int i = 0; i < numberCount - 1; i++)
        {
            if (operators[i] == '*') {
                numbers[i] *= numbers[i + 1];
                memmove(numbers + i + 1, numbers + i + 2, (numberCount - i - 2) * sizeof(int));
                memmove(operators + i, operators + i + 1, (numberCount - i - 2) * sizeof(char));
                numbers[numberCount - 1] = 0;
                operators[numberCount - 2] = '+';
            }
        }

    total = numbers[0];
    for (int i = 0; i < numberCount - 1; i++)
    {
        if (operators[i] == '+') {
            total += numbers[i + 1];
        }
        else if (operators[i] == '-') {
            total -= numbers[i + 1];
        }
        else if (operators[i] != 0) {
            printf("Unexpected operator error %c %d\n", operators[i], (int)operators[i]);
            for (int i = 0; i < numberCount - 1; i++)
            {
                printf("%d %c ", numbers[i], operators[i]);
            }printf("%d\n", numbers[numberCount - 1]);
            exit(0);
        }
    }
    printf("%d is the sum\n", total);

    //free(questionString);
}


void click() {
    printf("Button clicked\n");
    gchar* input = gtk_entry_buffer_get_text(input_buffer);
    struct timeval f,s,diff = {0};
    gettimeofday(&f,NULL);
    if (input) {
        int inp = (int)strtol(input, NULL, 10);
        printf("%s is the input text %d is he input %d is the total\n", input, inp, total);
        if (inp == total) {
            gtk_widget_override_background_color (question_label,GTK_STATE_FLAG_NORMAL,&pardusGreen);
            createRandomQuestion();
            resetted = 1;
        }else{
            gtk_widget_override_background_color (question_label,GTK_STATE_FLAG_NORMAL,&pardusRed);
        }
    }
}

gboolean keypress (GtkWidget *widget, GdkEventKey *event, gpointer data) {
    if (event->keyval == GDK_KEY_KP_Enter){
        click();
        return TRUE;
    }
    return FALSE;
}

static gboolean
buttonClick(GtkWidget* widget, GdkEventButton* event)
{
    GdkRGBA g;
    g.alpha = 0.0;g.red=0.1;g.blue=0.1;g.green=0.9;

    gtk_widget_override_background_color (question_label,GTK_STATE_FLAG_NORMAL,&g);
    if (event->button == 1) {
        gchar* input = gtk_button_get_label(widget);
        if (input[0] == 'C') {
            gtk_entry_buffer_set_text(input_buffer, "", 0);
        }
        else {

            if (resetted) {
                gtk_entry_buffer_set_text(input_buffer, "", 0);
                resetted = 0;
            }
            printf("Button pressed %s input\n", input);
            const gchar* buttonLbl = gtk_button_get_label(widget);
            const gchar* input_buf = gtk_entry_buffer_get_text(input_buffer);
            printf("%d is the strlen\n", (int)strlen(input_buf));
            gtk_entry_buffer_insert_text(input_buffer, (guint)strlen(input_buf), buttonLbl, 1);
        }

    }

    return TRUE;
}

void colorFallback(){
    usleep(1000);
    gtk_widget_override_background_color (button,GTK_STATE_FLAG_NORMAL,&pardusOrange);
    gtk_widget_override_background_color (buttonn,GTK_STATE_FLAG_NORMAL,&pardusGray);
    gtk_widget_override_background_color (button0,GTK_STATE_FLAG_NORMAL,&pardusGray);
    gtk_widget_override_background_color (buttonc,GTK_STATE_FLAG_NORMAL,&pardusGray);
    gtk_widget_override_background_color (button1,GTK_STATE_FLAG_NORMAL,&pardusDryellow);
    gtk_widget_override_background_color (button2,GTK_STATE_FLAG_NORMAL,&pardusGray);
    gtk_widget_override_background_color (button3,GTK_STATE_FLAG_NORMAL,&pardusDryellow);
    gtk_widget_override_background_color (button4,GTK_STATE_FLAG_NORMAL,&pardusDryellow);
    gtk_widget_override_background_color (button5,GTK_STATE_FLAG_NORMAL,&pardusGray);
    gtk_widget_override_background_color (button6,GTK_STATE_FLAG_NORMAL,&pardusDryellow);
    gtk_widget_override_background_color (button7,GTK_STATE_FLAG_NORMAL,&pardusDryellow);
    gtk_widget_override_background_color (button8,GTK_STATE_FLAG_NORMAL,&pardusGray);
    gtk_widget_override_background_color (button9,GTK_STATE_FLAG_NORMAL,&pardusDryellow);
}

static void
activate(GtkApplication* app,
         gpointer        user_data)
{

    buttonn = gtk_button_new_with_label("-");
    button0 = gtk_button_new_with_label("0");
    buttonc = gtk_button_new_with_label("C");
    button1 = gtk_button_new_with_label("1");
    button2 = gtk_button_new_with_label("2");
    button3 = gtk_button_new_with_label("3");
    button4 = gtk_button_new_with_label("4");
    button5 = gtk_button_new_with_label("5");
    button6 = gtk_button_new_with_label("6");
    button7 = gtk_button_new_with_label("7");
    button8 = gtk_button_new_with_label("8");
    button9 = gtk_button_new_with_label("9");


    GtkWidget* button_grid_f;	//FIRST
    GtkWidget* button_grid_s;	//SECOND
    GtkWidget* button_grid_t;	//THIRD
    GtkWidget* button_grid_l;	//LAST
    //g_signal_connect(button0, "clicked", click, NULL);

    input_buffer = gtk_entry_buffer_new(NULL, 0);
    GtkWidget* input_entry = gtk_entry_new_with_buffer(input_buffer);

    window = gtk_application_window_new(app);
    gtk_window_set_title(GTK_WINDOW(window), "KERRAT");
    gtk_window_set_default_size(GTK_WINDOW(window), 200, 200);

    question_label = gtk_label_new(" DENEME - * / + ");

    button_box = gtk_button_box_new(GTK_ORIENTATION_VERTICAL);

    gtk_button_box_set_layout(button_box, GTK_BUTTONBOX_EXPAND);

    button_grid_f = gtk_button_box_new(GTK_ORIENTATION_HORIZONTAL); gtk_button_box_set_layout(button_grid_f, GTK_BUTTONBOX_EXPAND);
    button_grid_s = gtk_button_box_new(GTK_ORIENTATION_HORIZONTAL); gtk_button_box_set_layout(button_grid_s, GTK_BUTTONBOX_EXPAND);
    button_grid_t = gtk_button_box_new(GTK_ORIENTATION_HORIZONTAL); gtk_button_box_set_layout(button_grid_t, GTK_BUTTONBOX_EXPAND);
    button_grid_l = gtk_button_box_new(GTK_ORIENTATION_HORIZONTAL); gtk_button_box_set_layout(button_grid_l, GTK_BUTTONBOX_EXPAND);

    kace(GTK_CONTAINER(button_grid_f), button1, buttonClick);
    kace(GTK_CONTAINER(button_grid_f), button2, buttonClick);
    kace(GTK_CONTAINER(button_grid_f), button3, buttonClick);
    kace(GTK_CONTAINER(button_grid_s), button4, buttonClick);
    kace(GTK_CONTAINER(button_grid_s), button5, buttonClick);
    kace(GTK_CONTAINER(button_grid_s), button6, buttonClick);
    kace(GTK_CONTAINER(button_grid_t), button7, buttonClick);
    kace(GTK_CONTAINER(button_grid_t), button8, buttonClick);
    kace(GTK_CONTAINER(button_grid_t), button9, buttonClick);
    kace(GTK_CONTAINER(button_grid_l), buttonn, buttonClick);
    kace(GTK_CONTAINER(button_grid_l), button0, buttonClick);
    kace(GTK_CONTAINER(button_grid_l), buttonc, buttonClick);

    gtk_container_add(GTK_CONTAINER(window), button_box);

    button = gtk_button_new_with_label("   =   ");

    g_signal_connect(button, "clicked", click, NULL);
    //g_signal_connect_swapped(button, "clicked", G_CALLBACK(gtk_widget_destroy), window);
    gtk_container_add(GTK_CONTAINER(button_box), input_entry);
    gtk_container_add(GTK_CONTAINER(button_box), question_label);
    gtk_container_add(GTK_CONTAINER(button_box), button);
    gtk_container_add(GTK_CONTAINER(button_box), button_grid_f);
    gtk_container_add(GTK_CONTAINER(button_box), button_grid_s);
    gtk_container_add(GTK_CONTAINER(button_box), button_grid_t);
    gtk_container_add(GTK_CONTAINER(button_box), button_grid_l);

    gtk_widget_set_events(GTK_CONTAINER(button_box), GDK_BUTTON_PRESS_MASK);
    gtk_widget_add_events(window, GDK_KEY_PRESS_MASK);
    gtk_widget_add_events(GTK_WIDGET(window), GDK_CONFIGURE);
    g_signal_connect (window, "key_press_event", keypress, NULL);
    g_signal_connect(window, "configure-event", resizeWidget, NULL);


    createRandomQuestion();

    gtk_widget_show_all(window);
    g_object_set(window, "expand", TRUE, NULL);
    g_object_set(button_box, "expand", TRUE, NULL);
    gtk_window_get_size(window, &ww, &wh);


    sw = ww;
    sh = wh;
    ratio = (float)(wh) / (float)(ww);
    printf("%d width, %d height %lf\n", ww, wh, ratio);

    GdkGeometry hints;

    hints.base_width = ww;
    hints.base_height = wh;
    hints.min_width = ww;
    hints.min_height = wh;
    hints.min_aspect = 1.0;
    hints.max_aspect = 3.0;
    gtk_window_set_geometry_hints (window,
                                   NULL,
                                   &hints,
                                   GDK_HINT_MIN_SIZE |
                                   GDK_HINT_BASE_SIZE | GDK_HINT_ASPECT);

    gtk_widget_override_background_color (button,GTK_STATE_FLAG_NORMAL,&pardusOrange);
    gtk_widget_override_background_color (buttonn,GTK_STATE_FLAG_NORMAL,&pardusGray);
    gtk_widget_override_background_color (button0,GTK_STATE_FLAG_NORMAL,&pardusGray);
    gtk_widget_override_background_color (buttonc,GTK_STATE_FLAG_NORMAL,&pardusGray);
    gtk_widget_override_background_color (button1,GTK_STATE_FLAG_NORMAL,&pardusDryellow);
    gtk_widget_override_background_color (button2,GTK_STATE_FLAG_NORMAL,&pardusGray);
    gtk_widget_override_background_color (button3,GTK_STATE_FLAG_NORMAL,&pardusDryellow);
    gtk_widget_override_background_color (button4,GTK_STATE_FLAG_NORMAL,&pardusDryellow);
    gtk_widget_override_background_color (button5,GTK_STATE_FLAG_NORMAL,&pardusGray);
    gtk_widget_override_background_color (button6,GTK_STATE_FLAG_NORMAL,&pardusDryellow);
    gtk_widget_override_background_color (button7,GTK_STATE_FLAG_NORMAL,&pardusDryellow);
    gtk_widget_override_background_color (button8,GTK_STATE_FLAG_NORMAL,&pardusGray);
    gtk_widget_override_background_color (button9,GTK_STATE_FLAG_NORMAL,&pardusDryellow);

    pthread_t t;
    pthread_create(&t,NULL,colorFallback,NULL);

    pthread_detach(t);
}

int
main(int    argc,
     char** argv)
{
    srand(time(NULL));
    GtkApplication* app;
    int status;

    app = gtk_application_new("org.gtk.example", G_APPLICATION_FLAGS_NONE);
    g_signal_connect(app, "activate", G_CALLBACK(activate), NULL);

    status = g_application_run(G_APPLICATION(app), argc, argv);
    g_object_unref(app);

    return status;
}
