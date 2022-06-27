/* See LICENSE file for copyright and license details. */

/* appearance */
static const unsigned int borderpx  = 2;  /* border pixel of windows */
static const unsigned int gappx     = 5;  /* gaps between windows */
static const unsigned int snap      = 32; /* snap pixel */
static const int showbar            = 1;  /* 0 means no bar */
static const int topbar             = 1;  /* 0 means bottom bar */
static const char *fonts[]          = { "Liberation Mono:size=14" };
static const char dmenufont[]       = "Liberation Mono:size=14";
static const char col_gray1[]       = "#222222";
static const char col_gray2[]       = "#444444";
static const char col_gray3[]       = "#bbbbbb";
static const char col_gray4[]       = "#eeeeee";
static const char col_cyan[]        = "#005577";
/* monokai */
static const char norm_fg[]         = "#FF6188";
static const char norm_bg[]         = "#272822";
static const char norm_border[]     = "#75715e";
static const char sel_fg[]          = "#A9DC76";
static const char sel_bg[]          = "#272822";
static const char sel_border[]      = "#f9f8f5";
static const char urg_fg[]          = "#f9f8f5";
static const char urg_bg[]          = "#FF6188";
static const char urg_border[]      = "#FF6188";
static const char title_fg[]        = "#78DCE8";
static const char title_bg[]        = "#272822";
static const char col_borderbar[]   = "#75715e";
/* schemes */
static const char *colors[][3] = {
    /* scheme       fg              bg          border */
//  [SchemeNorm]    = { col_gray3,  col_gray1,  col_gray2 },
//  [SchemeSel]     = { col_gray4,  col_cyan,   col_cyan },

    /* monokai */
    [SchemeNorm]    = { norm_fg,    norm_bg,    norm_border },
    [SchemeSel]     = { sel_fg,     sel_bg,     sel_border },
//  [SchemeHid]     = { sel_border, norm_bg,    sel_border },
//  [SchemeUrg]     = { urg_fg,     urg_bg,     urg_border },
//  [SchemeTitle]   = { title_fg,   title_bg,   norm_border },
};

static const char *const autostart[] = {
    "sh", "-c", "sleep 1 && xdotool key super+shift+7", NULL,
	NULL /* terminate */
};

/* tagging */
//static const char *tags[]         = { "1",  "2",  "3",  "4",  "5",  "6",  "7",  "8",  "9",  "0"  };
//static const char *tags[]         = { "一", "二", "三", "四", "五", "六", "七", "八", "九", "零" };
static const char *tags[]           = { "",  "",  "",  "",  "",  "",  "",  "",  "",  "" };
static const char *defaulttagapps[] = { "stabbed", "stranger", "librewolf", "emacsbg", "libreoffice", "betterbird", "keepassxc", "disco", "steam", NULL };

/* lockfile */
static char lockfile[] = "/tmp/dwm.lock";

static const Rule rules[] = {
    /* xprop(1):
    *     WM_CLASS(STRING) = instance, class
    *     WM_NAME(STRING) = title
    */
    /* class        instance    title                               tags mask   isfloating  monitor float x,y,w,h       floatborderpx */
    { "Betterbird", NULL,       NULL,                               1 << 5,     0,          -1,      -1, -1, -1, -1,    -1 },
    { "discord",    NULL,       NULL,                               1 << 7,     0,          -1,      -1, -1, -1, -1,    -1 },
    { "Firefox",    NULL,       "Picture-in-Picture",               0,          1,          -1,      -1, -1, -1, -1,    -1 },
    { "Gimp",       NULL,       NULL,                               0,          1,          -1,      -1, -1, -1, -1,    -1 },
    { "Gimp",       NULL,       NULL,                               0,          1,          -1,      -1, -1, -1, -1,    -1 },
    { "Gimp",       NULL,       "GNU Image Manipulation Program",   0,          0,          -1,      -1, -1, -1, -1,    -1 },
    { "Godot",      NULL,       NULL,                               0,          1,          -1,      -1, -1, -1, -1,    -1 },
    { "Steam",      NULL,       NULL,                               1 << 8,     1,          -1,      -1, -1, -1, -1,    -1 },
    { NULL,         NULL,       "Steam",                            1 << 8,     0,          -1,      -1, -1, -1, -1,    -1 },
    { "Steam",      NULL,       "Music Player",                     1 << 8,     0,          -1,      -1, -1, -1, -1,    -1 },
    { "Steam",      NULL,       "Steam - News",                     1 << 8,     1,          -1,      -1, -1, -1, -1,    -1 },
    { "Steam",      NULL,       "Steam Guard",                      1 << 8,     1,          -1,      -1, -1, -1, -1,    -1 },
};

/* window swallowing */
static const int swaldecay = 3;
static const int swalretroactive = 1;
static const char swalsymbol[] = "";

/* layout(s) */
static const float mfact            = 0.55; /* factor of master area size [0.05..0.95] */
static const int nmaster            = 1;    /* number of clients in master area */
static const int resizehints        = 1;    /* 1 means respect size hints in tiled resizals */
static const int attachdirection    = 5;    /* 0 default, 1 above, 2 aside, 3 below, 4 bottom, 5 top */
static const int lockfullscreen     = 1;    /* 1 will force focus on the fullscreen window */

static const Layout layouts[] = {
    /* symbol   arrange     function */
    { "[]=",    tile },     /* first entry is default */
    { "><>",    NULL },     /* no layout function means floating behavior */
    { "[M]",    monocle },
};

/* key definitions */
#define MODKEY Mod4Mask
#define TAGKEYS(KEY,TAG) \
    { MODKEY,                       KEY,    view,       {.ui = 1 << TAG} }, \
    { MODKEY|ControlMask,           KEY,    toggleview, {.ui = 1 << TAG} }, \
    { MODKEY|ShiftMask,             KEY,    tag,        {.ui = 1 << TAG} }, \
    { MODKEY|ControlMask|ShiftMask, KEY,    toggletag,  {.ui = 1 << TAG} }, 

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }

/* commands */
static char dmenumon[2]             = "0";  /* component of dmenucmd, manipulated in spawn() */
static const char *dmenucmd[]       = { "dmenu_run", topbar ? NULL : "-b", NULL };
static const char *dcalccmd[]       = { "dcalc", NULL };
static const char *clipmenucmd[]    = { "clipmenu", NULL };
static const char *termcmd[]        = { "tabbed", "-c", "-r", "2", "st", "-w", "''", NULL };
static const char *xkillcmd[]       = { "xkill", NULL };
static const char scratchpadname[]  = "scratchpad";
static const char *scratchpadcmd[]  = { "st", "-t", scratchpadname, "-g", "120x34", NULL };

static Key keys[] = {
    /* modifier                     key             function        argument */
    { MODKEY,                       XK_p,           spawn,          {.v = dmenucmd } },
    { MODKEY,                       XK_d,           spawn,          {.v = dmenucmd } },
    { MODKEY,                       XK_c,           spawn,          {.v = dcalccmd } },
    { MODKEY,                       XK_v,           spawn,          {.v = clipmenucmd} },
    { MODKEY|ShiftMask,             XK_Return,      spawn,          {.v = termcmd } },
    { MODKEY,                       XK_grave,       togglescratch,  {.v = scratchpadcmd } },
    { MODKEY,                       XK_Escape,      spawn,          {.v = xkillcmd } },
    { MODKEY,                       XK_s,           spawndefault,   {0} },
    { MODKEY,                       XK_b,           togglebar,      {0} },
    { MODKEY,                       XK_e,           focusstack,     {.i = +1 } },
    { MODKEY,                       XK_n,           focusstack,     {.i = -1 } },
    { MODKEY,                       XK_y,           incnmaster,     {.i = +1 } },
    { MODKEY,                       XK_o,           incnmaster,     {.i = -1 } },
    { MODKEY,                       XK_k,           setmfact,       {.f = -0.05} },
    { MODKEY,                       XK_l,           setmfact,       {.f = +0.05} },
    { MODKEY,                       XK_Return,      zoom,           {0} },
    { MODKEY,                       XK_Tab,         view,           {0} },
    { MODKEY,                       XK_u,           swalstopsel,    {0} },
    { MODKEY|ShiftMask,             XK_q,           killclient,     {0} },
    { MODKEY,                       XK_t,           setlayout,      {.v = &layouts[0]} },
    { MODKEY,                       XK_f,           setlayout,      {.v = &layouts[1]} },
    { MODKEY,                       XK_m,           setlayout,      {.v = &layouts[2]} },
    { MODKEY,                       XK_space,       setlayout,      {0} },
    { MODKEY|ShiftMask,             XK_space,       togglefloating, {0} },
    { MODKEY|ShiftMask,             XK_m,           togglefullscr,  {0} },
    { MODKEY,                       XK_0,           view,           {.ui = ~0 } },
    { MODKEY|ShiftMask,             XK_0,           tag,            {.ui = ~0 } },
    { MODKEY,                       XK_comma,       focusmon,       {.i = -1 } },
    { MODKEY,                       XK_period,      focusmon,       {.i = +1 } },
    { MODKEY|ShiftMask,             XK_comma,       tagmon,         {.i = -1 } },
    { MODKEY|ShiftMask,             XK_period,      tagmon,         {.i = +1 } },
    { MODKEY|ShiftMask,             XK_c,           quit,           {0} },
    { MODKEY|ShiftMask,             XK_r,           quit,           {1} },
    TAGKEYS(                        XK_1,                           0)
    TAGKEYS(                        XK_2,                           1)
    TAGKEYS(                        XK_3,                           2)
    TAGKEYS(                        XK_4,                           3)
    TAGKEYS(                        XK_5,                           4)
    TAGKEYS(                        XK_6,                           5)
    TAGKEYS(                        XK_7,                           6)
    TAGKEYS(                        XK_8,                           7)
    TAGKEYS(                        XK_9,                           8)
    TAGKEYS(                        XK_0,                           9)
    TAGKEYS(                        XK_exclam,                      0)
    TAGKEYS(                        XK_quotedbl,                    1)
    TAGKEYS(                        XK_sterling,                    2)
    TAGKEYS(                        XK_dollar,                      3)
    TAGKEYS(                        XK_percent,                     4)
    TAGKEYS(                        XK_asciicircum,                 5)
    TAGKEYS(                        XK_ampersand,                   6)
    TAGKEYS(                        XK_asterisk,                    7)
    TAGKEYS(                        XK_parenleft,                   8)
    TAGKEYS(                        XK_parenright,                  9)
};

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static Button buttons[] = {
    /* click            event mask          button      function        argument */
    { ClkLtSymbol,      0,                  Button1,    setlayout,      {0} },
    { ClkLtSymbol,      0,                  Button3,    setlayout,      {.v = &layouts[2]} },
    { ClkWinTitle,      0,                  Button2,    zoom,           {0} },
    { ClkStatusText,    0,                  Button2,    spawn,          {.v = termcmd } },
    { ClkClientWin,     MODKEY,             Button1,    movemouse,      {0} },
    { ClkClientWin,     MODKEY,             Button2,    togglefloating, {0} },
    { ClkClientWin,     MODKEY,             Button3,    resizemouse,    {0} },
    { ClkClientWin,     MODKEY|ShiftMask,   Button1,    swalmouse,      {0} },
    { ClkTagBar,        0,                  Button1,    view,           {0} },
    { ClkTagBar,        0,                  Button3,    toggleview,     {0} },
    { ClkTagBar,        MODKEY,             Button1,    tag,            {0} },
    { ClkTagBar,        MODKEY,             Button3,    toggletag,      {0} },
};

