/* See LICENSE file for copyright and license details. */

#define SESSION_FILE "/tmp/dwm-session"

/* appearance */
static const unsigned int borderpx  = 1;        /* border pixel of windows */
static const unsigned int snap      = 32;       /* snap pixel */
static const int showbar            = 1;        /* 0 means no bar */
static const int topbar             = 1;        /* 0 means bottom bar */
static const char *fonts[]          = { "DejaVuSansMono Nerd Font:size=10" };
static const char *colors[][3]      = {
	/*                fg         bg         border   */
	[SchemeNorm]  = { "#cccccc", "#1c1c1c", "#303030" },
	[SchemeSel]   = { "#cccccc", "#00875f", "#303030" },
	[SchemeTitle] = { "#cccccc", "#1c1c1c", "#303030" },
};

typedef struct {
	const char *name;
	const void *cmd;
} Sp;
const char *spcmd1[] = {"st", "-c", "scratchpad", "-t", "scratchpad", "-g", "120x34", NULL };
const char *spcmd2[] = {"keepassxc", NULL };
const char *spcmd3[] = {"veracrypt", NULL };
static Sp scratchpads[] = {
	/* name          cmd   */
	{ "scratchpad",  spcmd1 },
	{ "keepassxc",   spcmd2 },
	{ "veracrypt",   spcmd3 },
};

/* tagging */
static const char *tags[] = { "1", "2", "3", "4", "5", "6", "7", "8", "9" };
static const char *defaulttagapps[] = { NULL, NULL, NULL, NULL, "ncmpcppt", "thunderbird", "discord", "steam", NULL };

/* Lockfile */
static char lockfile[] = "/tmp/dwm.lock";

static const Rule rules[] = {
	/* xprop(1):
	 *	WM_CLASS(STRING) = instance, class
	 *	WM_NAME(STRING) = title
	 */
	/* class         instance  title                              tags mask  isfloating  monitor */
	{ "Gimp",        NULL,     NULL,                              0,         1,          -1 },
	{ "Gimp",        NULL,     "GNU Image Manipulation Program",  0,         0,          -1 },
	{ "ncmpcpp",     NULL,     NULL,                              1 << 4,    1,          -1 },
	{ "cava",        NULL,     NULL,                              1 << 4,    1,          -1 },
	{ "thunderbird", NULL,     NULL,                              1 << 5,    0,          -1 },
	{ "discord",     NULL,     NULL,                              1 << 6,    0,          -1 },
	{ "Steam",       NULL,     NULL,                              1 << 7,    1,          -1 },
	{ "Steam",       NULL,     "Steam",                           1 << 7,    0,          -1 },
	{ "scratchpad",  NULL,     NULL,                              SPTAG(0),  1,          -1 },
	{ "KeePassXC",   NULL,     NULL,                              SPTAG(1),  0,          -1 },
	{ "VeraCrypt",   NULL,     NULL,                              SPTAG(2),  1,          -1 },
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
static const int attachdirection = 5;    /* 0 default, 1 above, 2 aside, 3 below, 4 bottom, 5 top */

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
static const char *menucmd[] = { "dmenu-recent-aliases", NULL };
static const char *clipcmd[] = { "clipmenu", NULL };
static const char *calccmd[] = { "dcalc", NULL };
static const char *termcmd[] = { "st", NULL };
static const char *killcmd[] = { "xkill", NULL };
static const char *lockcmd[] = { "alock", "-bg", "none", "-cursor", "blank", NULL };
static const char *shotcmd[] = { "maimshot", NULL };

static const Key keys[] = {
	/* modifier                     key        function        argument */
	{ MODKEY,                       XK_p,      spawn,          {.v = menucmd } },
	{ MODKEY,                       XK_d,      spawn,          {.v = menucmd } },
	{ MODKEY,                       XK_v,      spawn,          {.v = clipcmd } },
	{ MODKEY,                       XK_c,      spawn,          {.v = calccmd } },
	{ MODKEY|ShiftMask,             XK_Return, spawn,          {.v = termcmd } },
	{ MODKEY,                       XK_Escape, spawn,          {.v = killcmd } },
	{ MODKEY|ShiftMask,             XK_a,      spawn,          {.v = lockcmd } },
	{ 0,                            XK_Print,  spawn,          {.v = shotcmd } },
	{ MODKEY,                       XK_s,      spawndefault,   {0} },
	{ MODKEY,                       XK_grave,  togglescratch,  {.ui = 0 } },
	{ MODKEY,                       XK_x,      togglescratch,  {.ui = 1 } },
	{ MODKEY,                       XK_z,      togglescratch,  {.ui = 2 } },
	{ MODKEY,                       XK_b,      togglebar,      {0} },
	/* qwerty */
	{ MODKEY,                       XK_k,      focusstack,     {.i = +1 } },
	{ MODKEY,                       XK_j,      focusstack,     {.i = -1 } },
	{ MODKEY,                       XK_h,      incnmaster,     {.i = +1 } },
	{ MODKEY,                       XK_l,      incnmaster,     {.i = -1 } },
	{ MODKEY,                       XK_n,      setmfact,       {.f = -0.05} },
	{ MODKEY,                       XK_m,      setmfact,       {.f = +0.05} },
	/* workman */
//	{ MODKEY,                       XK_e,      focusstack,     {.i = +1 } },
//	{ MODKEY,                       XK_n,      focusstack,     {.i = -1 } },
//	{ MODKEY,                       XK_y,      incnmaster,     {.i = +1 } },
//	{ MODKEY,                       XK_o,      incnmaster,     {.i = -1 } },
//	{ MODKEY,                       XK_k,      setmfact,       {.f = -0.05} },
//	{ MODKEY,                       XK_l,      setmfact,       {.f = +0.05} },
	{ MODKEY,                       XK_Return, zoom,           {0} },
	{ MODKEY,                       XK_Tab,    view,           {0} },
	{ MODKEY|ShiftMask,             XK_q,      killclient,     {0} },
	{ MODKEY,                       XK_t,      setlayout,      {.v = &layouts[0]} },
	{ MODKEY,                       XK_f,      setlayout,      {.v = &layouts[1]} },
	{ MODKEY,                       XK_g,      setlayout,      {.v = &layouts[2]} },
	{ MODKEY|ShiftMask,             XK_g,      togglefullscr,  {0} },
	{ MODKEY,                       XK_space,  setlayout,      {0} },
	{ MODKEY|ShiftMask,             XK_space,  togglefloating, {0} },
	{ MODKEY,                       XK_0,      view,           {.ui = ~0 } },
	{ MODKEY|ShiftMask,             XK_0,      tag,            {.ui = ~0 } },
	{ MODKEY,                       XK_comma,  focusmon,       {.i = -1 } },
	{ MODKEY,                       XK_period, focusmon,       {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_comma,  tagmon,         {.i = -1 } },
	{ MODKEY|ShiftMask,             XK_period, tagmon,         {.i = +1 } },
	{ MODKEY,                       XK_u,      swalstopsel,    {0} },
	{ MODKEY|ShiftMask,             XK_c,      quit,           {0} },
	{ MODKEY|ShiftMask,             XK_r,      quit,           {1} }, 
	TAGKEYS(                        XK_1,                      0)
	TAGKEYS(                        XK_2,                      1)
	TAGKEYS(                        XK_3,                      2)
	TAGKEYS(                        XK_4,                      3)
	TAGKEYS(                        XK_5,                      4)
	TAGKEYS(                        XK_6,                      5)
	TAGKEYS(                        XK_7,                      6)
	TAGKEYS(                        XK_8,                      7)
	TAGKEYS(                        XK_9,                      8)
	TAGKEYS(                        XK_exclam,                 0)
	TAGKEYS(                        XK_quotedbl,               1)
	TAGKEYS(                        XK_sterling,               2)
	TAGKEYS(                        XK_dollar,                 3)
	TAGKEYS(                        XK_percent,                4)
	TAGKEYS(                        XK_asciicircum,            5)
	TAGKEYS(                        XK_ampersand,              6)
	TAGKEYS(                        XK_asterisk,               7)
	TAGKEYS(                        XK_parenleft,              8)
	TAGKEYS(                        XK_parenright,             9)
};

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static const Button buttons[] = {
	/* click                event mask        button          function        argument */
	{ ClkLtSymbol,          0,                Button1,        setlayout,      {0} },
	{ ClkLtSymbol,          0,                Button3,        setlayout,      {.v = &layouts[2]} },
	{ ClkWinTitle,          0,                Button2,        zoom,           {0} },
	{ ClkStatusText,        0,                Button2,        spawn,          {.v = termcmd } },
	{ ClkClientWin,         MODKEY,           Button1,        movemouse,      {0} },
	{ ClkClientWin,         MODKEY,           Button2,        togglefloating, {0} },
	{ ClkClientWin,         MODKEY,           Button3,        resizemouse,    {0} },
	{ ClkClientWin,         MODKEY|ShiftMask, Button1,        swalmouse,      {0} },
	{ ClkTagBar,            0,                Button1,        view,           {0} },
	{ ClkTagBar,            0,                Button3,        toggleview,     {0} },
	{ ClkTagBar,            MODKEY,           Button1,        tag,            {0} },
	{ ClkTagBar,            MODKEY,           Button3,        toggletag,      {0} },
};

