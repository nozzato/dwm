/* See LICENSE file for copyright and license details. */

#include <X11/XF86keysym.h>

#define SESSION_FILE "/tmp/dwm-session"

/* appearance */
static const unsigned int borderpx  = 1;        /* border pixel of windows */
static const unsigned int snap      = 32;       /* snap pixel */
static const int showbar            = 1;        /* 0 means no bar */
static const int topbar             = 1;        /* 0 means bottom bar */
static const char *fonts[]          = { "DejaVu Sans Mono:size=10", "Blobmoji:size=10", "Font Awesome 6 Free Solid:size=10" };
static const char *colors[][3]      = {
	/*                fg         bg         border   */
	[SchemeNorm]  = { "#a9b1d6", "#121116", "#292e42" },
	[SchemeSel]   = { "#121116", "#6e93d9", "#292e42" },
	[SchemeTitle] = { "#a9b1d6", "#121116", "#292e42" },
	[SchemeUrg]   = { "#121116", "#a9b1d6", "#292e42" },
};

static const char *const autostart[] = {
	NULL /* terminate */
};

typedef struct {
	const char *name;
	const void *cmd;
} Sp;
const char *spcmd1[] = { "st", "-c", "terminalsp", NULL };
const char *spcmd2[] = { "keepassxc", NULL };
static Sp scratchpads[] = {
	/* name             cmd   */
	{ "terminalsp",     spcmd1 },
	{ "keepassxcsp",    spcmd2 },
};

/* tagging */
static const char *tags[] = { "1", "2", "3", "4", "5", "6", "7", "8", "9" };
static const char *defaulttagapps[][9] = {
	{ NULL, NULL, NULL, NULL, NULL, "steam", "discord", "thunderbird", "ncmpcpp-st" },
	{ NULL, NULL, NULL, NULL, NULL, "heroic", NULL, "outlook-librewolf", NULL },
};

/* Lockfile */
static char lockfile[] = "/tmp/dwm.lock";

static const Rule rules[] = {
	/* xprop(1):
	 *	WM_CLASS(STRING) = instance, class
	 *	WM_NAME(STRING) = title
	 */
	/* class         instance       title                             tags mask  isfloating  monitor  float x,y,w,h         floatborderpx */
	{ "Gimp",        NULL,          NULL,                             0,         1,          -1,        -1,  -1,  -1,  -1,  -1 },
	{ "Gimp",        NULL,          "GNU Image Manipulation Program", 0,         0,          -1,        -1,  -1,  -1,  -1,  -1 },
	{ "MobyDroid",   NULL,          "Working...",                     0,         1,          -1,        -1,  -1,  -1,  -1,  -1 },
	{ "dotnet",      NULL,          "osu!",                           0,         1,          -1,       280, 165,1360, 768,  -1 },
	{ "st",          NULL,          NULL,                             0,         0,          -1,        -1,  -1,  -1,  -1,   1 },
	{ "Thunar",      NULL,          "File Operation Progress",        0,         1,          -1,       735, 494,  -1,  -1,  -1 },
	{ "Steam",       NULL,          NULL,                             1 << 5,    1,          -1,        -1,  -1,  -1,  -1,  -1 },
	{ "Steam",       NULL,          "Steam",                          1 << 5,    0,          -1,        -1,  -1,  -1,  -1,  -1 },
	{ "Steam",       NULL,          "Music Player",                   1 << 5,    0,          -1,        -1,  -1,  -1,  -1,  -1 },
	{ "discord",     NULL,          NULL,                             1 << 6,    0,          -1,        -1,  -1,  -1,  -1,  -1 },
	{ "thunderbird", NULL,          NULL,                             1 << 7,    0,          -1,        -1,  -1,  -1,  -1,  -1 },
	{ "ncmpcpp",     NULL,          NULL,                             1 << 8,    0,          -1,        -1,  -1,  -1,  -1,  -1 },
	{ "terminalsp",  NULL,          NULL,                             SPTAG(0),  1,          -1,         0,  19,1915, 154,  -1 },
	{ "KeePassXC",   NULL,          NULL,                             SPTAG(1),  0,          -1,        -1,  -1,  -1,  -1,  -1 },
};

/* window swallowing */
static const int swaldecay = 3;
static const int swalretroactive = 1;
static const char swalsymbol[] = "";

/* layout(s) */
static const float mfact         = 0.55; /* factor of master area size [0.05..0.95] */
static const int nmaster         = 1;    /* number of clients in master area */
static const int resizehints     = 0;    /* 1 means respect size hints in tiled resizals */
static const int lockfullscreen  = 1;    /* 1 will force focus on the fullscreen window */
static const int attachdirection = 4;    /* 0 default, 1 above, 2 aside, 3 below, 4 bottom, 5 top */

static const Layout layouts[] = {
	/* symbol     arrange function */
	{ "[]=",      tile },    /* first entry is default */
	{ "><>",      NULL },    /* no layout function means floating behavior */
	{ "[M]",      monocle },
};

/* key definitions */
#define MODKEY Mod4Mask
#define TAGKEYS(KEY,TAG) \
	{ MODKEY,                       KEY,      view,           {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask,           KEY,      toggleview,     {.ui = 1 << TAG} }, \
	{ MODKEY|ShiftMask,             KEY,      tag,            {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask|ShiftMask, KEY,      toggletag,      {.ui = 1 << TAG} },

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }

/* commands */
static const char *runcmd[] = { "dmenu_run_plus", NULL };
static const char *runsudocmd[] = { "dmenu_run_plus", "-A", NULL };
static const char *clipboardcmd[] = { "clipmenu", NULL };
static const char *emojicmd[] = { "emojimenu", NULL };
static const char *calculatorcmd[] = { "=", NULL };
static const char *terminalcmd[] = { "st", NULL };
static const char *explorercmd[] = { "thunar", NULL };
static const char *centercmd[] = { "center", NULL };
static const char *killcmd[] = { "xkill", NULL };
static const char *lockcmd[] = { "sh", "-c", "alock -b image:file=$HOME/app/dwm/lock_screen.png -c blank -i frame:width=1", NULL };
static const char *vpnonoffcmd[] = { "vpn-toggle", NULL };
static const char *screenshotcmd[] = { "maimshot", NULL };
static const char *audiomutecmd[] = { "pactl", "set-sink-mute", "@DEFAULT_SINK@", "toggle", NULL };
static const char *audioraisevolumecmd[] = { "pactl", "set-sink-volume", "@DEFAULT_SINK@", "+2%", NULL };
static const char *audiolowervolumecmd[] = { "pactl", "set-sink-volume", "@DEFAULT_SINK@", "-2%", NULL };
static const char *audioplaypausecmd[] = { "playerctl", "play-pause", NULL };
static const char *audionextcmd[] = { "playerctl", "next", NULL };
static const char *audioprevcmd[] = { "playerctl", "previous", NULL };
static const char *audiostopcmd[] = { "mpc", "stop", NULL };
static const char *audiolooponcmd[] = { "sh", "-c", "mpc repeat on && mpc single on", NULL };
static const char *audioloopoffcmd[] = { "sh", "-c", "mpc repeat off && mpc single off", NULL };
static const char *audiopresetoncmd[] = { "pulseaudio-equalizer", "enable", NULL };
static const char *audiopresetoffcmd[] = { "pulseaudio-equalizer", "disable", NULL };
static const char *monbrightnessupcmd[] = { "light", "-A", "4", NULL };
static const char *monbrightnessdowncmd[] = { "light", "-U", "4", NULL };
static const char *kbdlightonoffcmd[] = { "kbd-backlight-toggle", NULL };

static const Key keys[] = {
	/* modifier                     key                         function        argument */
	{ MODKEY,                       XK_p,                       spawn,          {.v = runcmd } },
	{ MODKEY|ShiftMask,             XK_p,                       spawn,          {.v = runsudocmd } },
	{ MODKEY,                       XK_v,                       spawn,          {.v = clipboardcmd } },
	{ MODKEY,                       XK_semicolon,               spawn,          {.v = emojicmd } },
	{ MODKEY,                       XK_equal,                   spawn,          {.v = calculatorcmd } },
	{ MODKEY|ShiftMask,             XK_Return,                  spawn,          {.v = terminalcmd } },
	{ MODKEY,                       XK_grave,                   togglescratch,  {.ui = 0 } },
	{ MODKEY|ShiftMask,             XK_e,                       spawn,          {.v = explorercmd } },
	{ MODKEY,                       XK_x,                       togglescratch,  {.ui = 1 } },
	{ MODKEY,                       XK_c,                       togglescratch,  {.ui = 2 } },
	{ MODKEY,                       XK_i,                       spawn,          {.v = centercmd } },
	{ MODKEY,                       XK_Escape,                  spawn,          {.v = killcmd } },
	{ MODKEY|ShiftMask,             XK_a,                       spawn,          {.v = lockcmd } },
	{ MODKEY|ShiftMask,             XK_v,                       spawn,          {.v = vpnonoffcmd } },
	{ 0,                            XK_Print,                   spawn,          {.v = screenshotcmd } },
	{ 0,                            XF86XK_AudioMute,           spawn,          {.v = audiomutecmd } },
	{ 0,                            XF86XK_AudioRaiseVolume,    spawn,          {.v = audioraisevolumecmd } },
	{ 0,                            XF86XK_AudioLowerVolume,    spawn,          {.v = audiolowervolumecmd } },
	{ 0,                            XF86XK_AudioPlay,           spawn,          {.v = audioplaypausecmd } },
	{ 0,                            XF86XK_AudioNext,           spawn,          {.v = audionextcmd } },
	{ 0,                            XF86XK_AudioPrev,           spawn,          {.v = audioprevcmd } },
	{ ShiftMask,                    XF86XK_AudioPlay,           spawn,          {.v = audiostopcmd } },
	{ ShiftMask,                    XF86XK_AudioNext,           spawn,          {.v = audiolooponcmd } },
	{ ShiftMask,                    XF86XK_AudioPrev,           spawn,          {.v = audioloopoffcmd } },
	{ ShiftMask,                    XF86XK_AudioRaiseVolume,    spawn,          {.v = audiopresetoncmd } },
	{ ShiftMask,                    XF86XK_AudioLowerVolume,    spawn,          {.v = audiopresetoffcmd } },
	{ 0,                            XF86XK_MonBrightnessUp,     spawn,          {.v = monbrightnessupcmd } },
	{ 0,                            XF86XK_MonBrightnessDown,   spawn,          {.v = monbrightnessdowncmd } },
	{ ShiftMask,                    XF86XK_AudioMute,           spawn,          {.v = kbdlightonoffcmd } },

	{ MODKEY,                       XK_j,                       focusstack,     {.i = +1 } },
	{ MODKEY,                       XK_k,                       focusstack,     {.i = -1 } },
	{ MODKEY,                       XK_n,                       incnmaster,     {.i = +1 } },
	{ MODKEY,                       XK_m,                       incnmaster,     {.i = -1 } },
	{ MODKEY,                       XK_h,                       setmfact,       {.f = -0.05} },
	{ MODKEY,                       XK_l,                       setmfact,       {.f = +0.05} },
//	{ MODKEY,                       XK_n,                       focusstack,     {.i = +1 } },
//	{ MODKEY,                       XK_e,                       focusstack,     {.i = -1 } },
//	{ MODKEY,                       XK_y,                       incnmaster,     {.i = +1 } },
//	{ MODKEY,                       XK_o,                       incnmaster,     {.i = -1 } },
//	{ MODKEY,                       XK_k,                       setmfact,       {.f = -0.05} },
//	{ MODKEY,                       XK_l,                       setmfact,       {.f = +0.05} },

	{ MODKEY,                       XK_s,                       spawndefault,   {0} },
	{ MODKEY|ShiftMask,             XK_s,                       spawndefault,   {1} },
	{ MODKEY,                       XK_b,                       togglebar,      {0} },
	{ MODKEY,                       XK_Return,                  zoom,           {0} },
	{ MODKEY,                       XK_Tab,                     view,           {0} },
	{ MODKEY|ShiftMask,             XK_q,                       killclient,     {0} },
	{ MODKEY,                       XK_t,                       setlayout,      {.v = &layouts[0]} },
	{ MODKEY,                       XK_f,                       setlayout,      {.v = &layouts[1]} },
	{ MODKEY,                       XK_g,                       setlayout,      {.v = &layouts[2]} },
	{ MODKEY|ShiftMask,             XK_g,                       togglefullscr,  {0} },
	{ MODKEY,                       XK_space,                   setlayout,      {0} },
	{ MODKEY|ShiftMask,             XK_space,                   togglefloating, {0} },
	{ MODKEY|ShiftMask,             XK_0,                       tag,            {.ui = ~0 } },
	{ MODKEY,                       XK_comma,                   focusmon,       {.i = -1 } },
	{ MODKEY,                       XK_period,                  focusmon,       {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_comma,                   tagmon,         {.i = -1 } },
	{ MODKEY|ShiftMask,             XK_period,                  tagmon,         {.i = +1 } },
	{ MODKEY,                       XK_u,                       swalstopsel,    {0} },
	{ MODKEY|ShiftMask,             XK_c,                       quit,           {0} },
	{ MODKEY|ShiftMask,             XK_r,                       quit,           {1} }, 
	TAGKEYS(                        XK_1,                                       0)
	TAGKEYS(                        XK_2,                                       1)
	TAGKEYS(                        XK_3,                                       2)
	TAGKEYS(                        XK_4,                                       3)
	TAGKEYS(                        XK_5,                                       4)
	TAGKEYS(                        XK_6,                                       5)
	TAGKEYS(                        XK_7,                                       6)
	TAGKEYS(                        XK_8,                                       7)
	TAGKEYS(                        XK_9,                                       8)
	TAGKEYS(                        XK_exclam,                                  0)
	TAGKEYS(                        XK_quotedbl,                                1)
	TAGKEYS(                        XK_sterling,                                2)
	TAGKEYS(                        XK_dollar,                                  3)
	TAGKEYS(                        XK_percent,                                 4)
	TAGKEYS(                        XK_asciicircum,                             5)
	TAGKEYS(                        XK_ampersand,                               6)
	TAGKEYS(                        XK_asterisk,                                7)
	TAGKEYS(                        XK_parenleft,                               8)
};

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static const Button buttons[] = {
	/* click                event mask        button          function        argument */
	{ ClkLtSymbol,          0,                Button1,        setlayout,      {0} },
	{ ClkLtSymbol,          0,                Button3,        setlayout,      {.v = &layouts[2]} },
	{ ClkWinTitle,          0,                Button2,        zoom,           {0} },
	{ ClkStatusText,        0,                Button2,        spawn,          {.v = terminalcmd } },
	{ ClkClientWin,         MODKEY,           Button1,        movemouse,      {0} },
	{ ClkClientWin,         MODKEY,           Button2,        togglefloating, {0} },
	{ ClkClientWin,         MODKEY,           Button3,        resizemouse,    {0} },
	{ ClkClientWin,         MODKEY|ShiftMask, Button1,        swalmouse,      {0} },
	{ ClkTagBar,            0,                Button1,        view,           {0} },
	{ ClkTagBar,            0,                Button3,        toggleview,     {0} },
	{ ClkTagBar,            MODKEY,           Button1,        tag,            {0} },
	{ ClkTagBar,            MODKEY,           Button3,        toggletag,      {0} },
};

