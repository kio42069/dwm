/* See LICENSE file for copyright and license details. */

#include "fibonacci.c"
#include "movestack.c"

/* appearance */
static const unsigned int borderpx  = 3;        /* border pixel of windows */
static const unsigned int snap      = 32;       /* snap pixel */
static const int showbar            = 1;        /* 0 means no bar */
static const int topbar             = 1;        /* 0 means bottom bar */
static const char *fonts[]          = { "JetBrainsMono Nerd Font:size=11" };
static const char dmenufont[]       = "JetBrainsMono Nerd Font:size=10";
static const char col_gray1[]       = "#222222";
static const char col_gray2[]       = "#444444";
static const char col_gray3[]       = "#bbbbbb";
static const char col_gray4[]       = "#eeeeee";
static const char col_cyan[]        = "#005577";

static const char col_barbg[]   = "#1e1e2e";  // bar background
static const char col_text[]    = "#8888aa";  // normal text
static const char col_text_sel[] = "#ffffff"; // selected text

static const char col_border_inactive[] = "#313244";
static const char col_border_active[]   = "#89b4fa";
// static const char *colors[][3]      = {
// 	/*               fg         bg         border   */
// 	[SchemeNorm] = { col_gray3, col_gray1, col_gray2 },
// 	[SchemeSel]  = { col_gray4, col_cyan,  col_cyan  },
// };
static const char *colors[][3] = {
    /*               fg            bg         border */
    [SchemeNorm] = { col_text,     col_barbg, col_border_inactive },
    [SchemeSel]  = { col_text_sel, col_barbg, col_border_active   },
};

/* tagging */
static const char *tags[] = { "1", "2", "3", "4", "5", "6", "7", "8", "9" };

static const Rule rules[] = {
	/* xprop(1):
	 *	WM_CLASS(STRING) = instance, class
	 *	WM_NAME(STRING) = title
	 */
	/* class      instance    title       tags mask     isfloating   monitor */
	{ "Gimp",     NULL,       NULL,       0,            1,           -1 },
	{ "Firefox",  NULL,       NULL,       1 << 8,       0,           -1 },
};

/* layout(s) */
static const float mfact     = 0.5; /* factor of master area size [0.05..0.95] */
static const int nmaster     = 1;    /* number of clients in master area */
static const int resizehints = 1;    /* 1 means respect size hints in tiled resizals */
static const int lockfullscreen = 1; /* 1 will force focus on the fullscreen window */
static const int refreshrate = 120;  /* refresh rate (per second) for client move/resize */

static const Layout layouts[] = {
	/* symbol     arrange function */
	{ "[]=",      tile },    /* first entry is default */
	{ "><>",      NULL },    /* no layout function means floating behavior */
	{ "[M]",      monocle },
  { "[@]",      spiral },
  { "[/]",      dwindle },
};

/* key definitions */
#define ALTKEY Mod1Mask
#define MODKEY Mod4Mask
#define TAGKEYS(KEY,TAG) \
	{ MODKEY,                       KEY,      view,           {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask,           KEY,      toggleview,     {.ui = 1 << TAG} }, \
	{ MODKEY|ShiftMask,             KEY,      tag,            {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask|ShiftMask, KEY,      toggletag,      {.ui = 1 << TAG} },

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }

/* commands */
static char dmenumon[2] = "0"; /* component of dmenucmd, manipulated in spawn() */
static const char *dmenucmd[] = { "dmenu_run", "-b", "-i", "-m", dmenumon, "-fn", dmenufont, "-nb", "#000000", "-nf", "#ffffff", "-sb", "#b2d4fa", "-sf", "#000000", NULL };
static const char *termcmd[]  = { "st", NULL };
static const char *slock[] = {
  "sh", "-c",
  "slock & systemctl suspend", 
  NULL
};
static const char *librewolf[]  = { "librewolf", NULL };
static const char *firefoxcmd[]  = { "firefox", "-p", "main", NULL };
static const char *firefoxaltcmd[] = { "firefox", "-p", "alt", NULL };
static const char *screenshotcmd[] = { "/bin/sh", "-c", "maim -s | xclip -sel c -t image/png", NULL };
// static const char *volup[] = { "pamixer", "-i", "5;", "export", "VOL=$(pamixer --get-volume);", "notify-send", "$VOL", NULL };
static const char *volup[] = {
    "sh", "-c",
    "pamixer -i 5; VOL=$(pamixer --get-volume); notify-send -a Volume -r 32 \"$VOL\"",
    NULL
};
// static const char *voldown[] = { "pamixer", "-d", "5", NULL };
static const char *voldown[] = {
    "sh", "-c",
    "pamixer -d 5; VOL=$(pamixer --get-volume); notify-send -a Volume -r 32 \"$VOL\"",
    NULL
};
// static const char *voltog[] = { "pamixer", "-t", NULL };
static const char *voltog[] = {
    "sh", "-c",
    "MUTE=$(pamixer --get-mute); pamixer -t; notify-send -a Volume -r 32 \"$MUTE\"",
    NULL
};
// static const char *brightup[] = { "brightnessctl", "s", "5%+", NULL };
static const char *brightup[] = {
    "sh", "-c",
    "brightnessctl s 5%+; BRIGHT=${brightnessctl -m|cut -d, -f4}; notify-send -a Brightness -r 33 \"$BRIGHT\"",
    NULL
};
// static const char *brightdown[] = { "brightnessctl", "s", "5%-", NULL };
static const char *brightdown[] = {
    "sh", "-c",
    "brightnessctl s 5%-; BRIGHT=${brightnessctl -m|cut -d, -f4}; notify-send -a Brightness -r 33 \"$BRIGHT\"",
    NULL
};
// static const char *mictog[] = { "swayosd-client", "--input-volume", "mute-toggle", NULL };
static const char *mictog[] = {
    "sh", "-c",
    "pactl set-source-mute @DEFAULT_SOURCE@ toggle && pkill -RTMIN+10 dwmblocks;",
    NULL
};
static const char *filemgr[] = { "thunar", NULL };
static const char *snoop[] = {"systemctl", "suspend", NULL};
static const char *clipboard[] = { "clipmenu", NULL };
static const char *emoji[] = { "rofimoji", "--action", "copy", NULL };
static const char *redshift[] = { "/home/surt/garage/dwm/scripts/redshift.sh", NULL };
static const char *touchtoggle[] = { "/home/surt/garage/dwm/scripts/touchtog.sh", NULL };
static const char *powermenu[] = { "/home/surt/garage/dwm/scripts/powermenu", NULL };

void
shiftview(const Arg *arg)
{
    int i;
    unsigned int curtag = 0;
    unsigned int newtag;

    /* find current tag index */
    for (i = 0; i < LENGTH(tags); i++) {
        if (selmon->tagset[selmon->seltags] & (1 << i)) {
            curtag = i;
            break;
        }
    }

    /* compute new tag index */
    newtag = (curtag + arg->i + LENGTH(tags)) % LENGTH(tags);

    Arg a = {.ui = 1 << newtag};
    view(&a);
}

static const Key keys[] = {
	/* modifier                     key        function        argument */
	{ MODKEY,                       XK_a,      spawn,          {.v = dmenucmd } },
	{ MODKEY,                       XK_Return, spawn,          {.v = termcmd } },
	{ MODKEY,                       XK_b,      togglebar,      {0} },
	{ MODKEY,                       XK_Down,   focusstack,     {.i = +1 } },
	{ MODKEY,                       XK_Right,  focusstack,     {.i = +1 } },
	{ MODKEY,                       XK_j,      focusstack,     {.i = +1 } },
	{ MODKEY,                       XK_Up,     focusstack,     {.i = -1 } },
	{ MODKEY,                       XK_Left,   focusstack,     {.i = -1 } },
	{ MODKEY,                       XK_k,      focusstack,     {.i = -1 } },
	{ MODKEY,                       XK_i,      incnmaster,     {.i = +1 } },
	{ MODKEY,                       XK_o,      incnmaster,     {.i = -1 } },
	{ MODKEY,                       XK_semicolon,setmfact,     {.f = -0.05} },
	{ MODKEY,                       XK_apostrophe,setmfact,    {.f = +0.05} },
	{ MODKEY,                       XK_Return, zoom,           {0} },
	{ MODKEY,                       XK_Tab,    view,           {0} },
	{ MODKEY,                       XK_q,      killclient,     {0} },
	{ MODKEY,                       XK_t,      setlayout,      {.v = &layouts[0]} },
	{ MODKEY|ShiftMask,             XK_f,      setlayout,      {.v = &layouts[1]} },
	{ MODKEY,                       XK_m,      setlayout,      {.v = &layouts[2]} },
	{ MODKEY,                       XK_F11,    setlayout,      {.v = &layouts[3]} },
	{ MODKEY,                       XK_F12,    setlayout,      {.v = &layouts[4]} },
  { MODKEY|ShiftMask,             XK_Left,   movestack,      {.i = -1 } },
  { MODKEY|ShiftMask,             XK_Right,  movestack,      {.i = +1 } },
  { MODKEY,                       XK_f,      fullscreen,     {0} },
	{ MODKEY,                       XK_space,  togglefloating, {0} },
	{ MODKEY|ShiftMask,             XK_space,  setlayout,      {0} },
	{ MODKEY,                       XK_0,      view,           {.ui = ~0 } },
	{ MODKEY|ShiftMask,             XK_0,      tag,            {.ui = ~0 } },
	{ MODKEY,                       XK_comma,  focusmon,       {.i = -1 } },
	{ MODKEY,                       XK_period, focusmon,       {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_comma,  tagmon,         {.i = -1 } },
	{ MODKEY|ShiftMask,             XK_period, tagmon,         {.i = +1 } },
	TAGKEYS(                        XK_1,                      0)
	TAGKEYS(                        XK_2,                      1)
	TAGKEYS(                        XK_3,                      2)
	TAGKEYS(                        XK_4,                      3)
	TAGKEYS(                        XK_5,                      4)
	TAGKEYS(                        XK_6,                      5)
	TAGKEYS(                        XK_7,                      6)
	TAGKEYS(                        XK_8,                      7)
	TAGKEYS(                        XK_9,                      8)
	{ MODKEY|ShiftMask,             XK_q,      quit,           {0} },
  { MODKEY,                       XK_w,      spawn,          { .v = firefoxcmd} },
  { MODKEY,                       XK_r,      spawn,          { .v = firefoxaltcmd} },
  { MODKEY,                       XK_l,      spawn,          { .v = slock } },
  { MODKEY,                       XK_s,      spawn,          { .v = screenshotcmd } },
  { ALTKEY,                       XK_F1,     spawn,          { .v = voltog } },
  { ALTKEY,                       XK_F2,     spawn,          { .v = voldown } },
  { ALTKEY,                       XK_F3,     spawn,          { .v = volup } },
  { ALTKEY,                       XK_F4,     spawn,          { .v = mictog } },
  { ALTKEY,                       XK_F5,     spawn,          { .v = brightdown } },
  { ALTKEY,                       XK_F6,     spawn,          { .v = brightup } },
  { MODKEY,                       XK_e,      spawn,          { .v = filemgr } },
  { 0,                            XK_F10,    spawn,          { .v = snoop } },
  { MODKEY,                       XK_d,      spawn,          { .v = librewolf } },
  { MODKEY,                       XK_v,      spawn,          { .v = clipboard } },
  { MODKEY,                       XK_period, spawn,          { .v = emoji } },
  { MODKEY,                       XK_g,      spawn,          { .v = redshift } },
  { MODKEY,                       XK_Delete, spawn,          { .v = touchtoggle } },
  { MODKEY,                       XK_x,      spawn,          { .v = powermenu } },
  { MODKEY|Mod1Mask,              XK_Left,   shiftview,           { .i = -1 } },
  { MODKEY|Mod1Mask,              XK_Right,  shiftview,           { .i = +1 } },
  { MODKEY,                       XK_bracketleft,setcfact,        { .f = -0.25 } },
  { MODKEY,                       XK_bracketright,setcfact,       { .f = +0.25 } },
};

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static const Button buttons[] = {
	/* click                event mask      button          function        argument */
	{ ClkLtSymbol,          0,              Button1,        setlayout,      {0} },
	{ ClkLtSymbol,          0,              Button3,        setlayout,      {.v = &layouts[2]} },
	{ ClkWinTitle,          0,              Button2,        zoom,           {0} },
	{ ClkStatusText,        0,              Button2,        spawn,          {.v = termcmd } },
	{ ClkClientWin,         MODKEY,         Button1,        movemouse,      {0} },
	{ ClkClientWin,         MODKEY,         Button2,        togglefloating, {0} },
	{ ClkClientWin,         MODKEY,         Button3,        resizemouse,    {0} },
	{ ClkClientWin,         MODKEY|ShiftMask,Button1,       resizemouse,    {0} },
	{ ClkTagBar,            0,              Button1,        view,           {0} },
	{ ClkTagBar,            0,              Button3,        toggleview,     {0} },
	{ ClkTagBar,            MODKEY,         Button1,        tag,            {0} },
	{ ClkTagBar,            MODKEY,         Button3,        toggletag,      {0} },
};

