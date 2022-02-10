/* See LICENSE file for copyright and license details. */

/* appearance */
static const unsigned int borderpx  = 2;        /* border pixel of windows */
static const unsigned int gappx     = 5;	/* gaps between windows */
static const unsigned int snap      = 32;       /* snap pixel */
static const int showbar            = 1;        /* 0 means no bar */
static const int topbar             = 1;        /* 0 means bottom bar */
static const char *fonts[]          = { "monospace:size=12", "fontawesome:size=12" };
static const char dmenufont[]       = "monospace:size=12";
static const char col_gray1[]       = "#222222";
static const char col_gray2[]       = "#444444";
static const char col_gray3[]       = "#bbbbbb";
static const char col_gray4[]       = "#eeeeee";
static const char col_cyan[]        = "#005577";
static const char col_rosewater[]   = "#F5E0DC";
static const char col_flamingo[]    = "#F2CDCD";
static const char col_mauve[]       = "#DDB6F2";
static const char col_pink[]        = "#F5C2E7";
static const char col_maroon[]      = "#E8A2AF";
static const char col_red[]         = "#F28FAD";
static const char col_peach[]       = "#F8BD96";
static const char col_yellow[]      = "#FAE3B0";
static const char col_green[]       = "#ABE9B3";
static const char col_teal[]        = "#B5E8E0";
static const char col_blue[]        = "#96CDFB";
static const char col_sky[]         = "#89DCEB";
static const char col_lavender[]    = "#C9CBFF";
static const char col_black1[]      = "#161320";
static const char col_black2[]      = "#1A1826";
static const char col_black3[]      = "#1E1E2E";
static const char col_black4[]      = "#302D41";
static const char col_black5[]      = "#575268";
static const char col_gray5[]       = "#6E6C7E";
static const char col_gray6[]       = "#988BA2";
static const char col_gray7[]       = "#C3BAC6";
static const char col_white[]       = "#D9E0EE";
static const char *colors[][3]      = {
	/*		fg			bg			border */
/*	[SchemeNorm]  = { col_gray3,		col_gray1,		col_gray2 },*/
	[SchemeNorm]  = { col_gray3,		col_gray1,		col_gray2 },
	[SchemeSel]   = { col_gray4,		col_cyan,		col_cyan },
};

/* tagging */
/*static const char *tags[] = { "1", "2", "3", "4", "5", "6", "7", "8", "9" };*/
/*static const char *tags[] = { "一", "二", "三", "四", "五", "六", "七", "八", "九" };*/
static const char *tags[] = { "", "", "", "", "", "", "", "", "" };
static const char *defaulttagapps[] = { "st", "stranger", "firefox", "betterbird", "discord", "steam", "gimp", "libreoffice", "code" };

static const Rule rules[] = {
	/* xprop(1):
	 *	WM_CLASS(STRING) = instance, class
	 *	WM_NAME(STRING) = title
	 */
	/* class		instance    title					tags mask     isfloating   monitor */
	{ "Firefox",		NULL,       "Picture-in-Picture",			0,            1,           -1 },	
	{ "Gimp",		NULL,       NULL,					0,            1,           -1 },
	{ "Gimp",		NULL,       "GNU Image Manipulation Program",		0,            0,           -1 },
	{ "Steam",		NULL,       NULL,					0,            1,           -1 },
	{ "Steam",		NULL,       "Steam",					0,            0,           -1 },
};

/* layout(s) */
static const float mfact     = 0.55; /* factor of master area size [0.05..0.95] */
static const int nmaster     = 1;    /* number of clients in master area */
static const int resizehints = 1;    /* 1 means respect size hints in tiled resizals */
static const int lockfullscreen = 1; /* 1 will force focus on the fullscreen window */

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
static char dmenumon[2] = "0"; /* component of dmenucmd, manipulated in spawn() */
static const char *dmenucmd[] = { "dmenu_run", "-m", dmenumon, "-fn", dmenufont, "-nb", col_gray1, "-nf", col_gray3, "-sb", col_cyan, "-sf", col_gray4, NULL };
static const char *termcmd[]  = { "st", NULL };

static Key keys[] = {
	/* modifier                     key        	function        argument */
	{ MODKEY,                       XK_p,      	spawn,          {.v = dmenucmd } },
	{ MODKEY|ShiftMask,             XK_Return, 	spawn,          {.v = termcmd } },
	{ MODKEY,                       XK_s,      	spawndefault,   {0} },
	{ MODKEY,                       XK_b,      	togglebar,      {0} },
	{ MODKEY,                       XK_j,      	focusstack,     {.i = +1 } },
	{ MODKEY,                       XK_k,      	focusstack,     {.i = -1 } },
	{ MODKEY,                       XK_i,      	incnmaster,     {.i = +1 } },
	{ MODKEY,                       XK_d,      	incnmaster,     {.i = -1 } },
	{ MODKEY,                       XK_h,      	setmfact,       {.f = -0.05} },
	{ MODKEY,                       XK_l,      	setmfact,       {.f = +0.05} },
	{ MODKEY,                       XK_Return, 	zoom,           {0} },
	{ MODKEY,                       XK_Tab,    	view,           {0} },
	{ MODKEY|ShiftMask,             XK_q,      	killclient,     {0} },
	{ MODKEY,                       XK_t,      	setlayout,      {.v = &layouts[0]} },
	{ MODKEY,                       XK_f,      	setlayout,      {.v = &layouts[1]} },
	{ MODKEY,                       XK_m,      	setlayout,      {.v = &layouts[2]} },
	{ MODKEY,                       XK_space,  	setlayout,      {0} },
	{ MODKEY|ShiftMask,             XK_space,  	togglefloating, {0} },
	{ MODKEY,                       XK_0,      	view,           {.ui = ~0 } },
	{ MODKEY|ShiftMask,             XK_0,      	tag,            {.ui = ~0 } },
	{ MODKEY,                       XK_comma,  	focusmon,       {.i = -1 } },
	{ MODKEY,                       XK_period, 	focusmon,       {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_comma,  	tagmon,         {.i = -1 } },
	{ MODKEY|ShiftMask,             XK_period, 	tagmon,         {.i = +1 } },
	{ MODKEY,                       XK_minus,  	setgaps,        {.i = -1 } },
	{ MODKEY,                       XK_equal,  	setgaps,        {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_equal,  	setgaps,        {.i = 0  } },
	TAGKEYS(                        XK_exclam,		        0)
	TAGKEYS(                        XK_quotedbl,			1)
	TAGKEYS(                        XK_sterling,			2)
	TAGKEYS(                        XK_dollar,			3)
	TAGKEYS(                        XK_percent,			4)
	TAGKEYS(                        XK_asciicircum,			5)
	TAGKEYS(                        XK_ampersand,			6)
	TAGKEYS(                        XK_asterisk,			7)
	TAGKEYS(                        XK_parenleft,			8)
	{ MODKEY|ShiftMask,             XK_BackSpace,	quit,           {0} },
};

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static Button buttons[] = {
	/* click                event mask      button          function        argument */
	{ ClkLtSymbol,          0,              Button1,        setlayout,      {0} },
	{ ClkLtSymbol,          0,              Button3,        setlayout,      {.v = &layouts[2]} },
	{ ClkWinTitle,          0,              Button2,        zoom,           {0} },
	{ ClkStatusText,        0,              Button2,        spawn,          {.v = termcmd } },
	{ ClkClientWin,         MODKEY,         Button1,        movemouse,      {0} },
	{ ClkClientWin,         MODKEY,         Button2,        togglefloating, {0} },
	{ ClkClientWin,         MODKEY,         Button3,        resizemouse,    {0} },
	{ ClkTagBar,            0,              Button1,        view,           {0} },
	{ ClkTagBar,            0,              Button3,        toggleview,     {0} },
	{ ClkTagBar,            MODKEY,         Button1,        tag,            {0} },
	{ ClkTagBar,            MODKEY,         Button3,        toggletag,      {0} },
};

