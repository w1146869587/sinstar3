//stamp:0ea13dc2f872089a
/*<------------------------------------------------------------------------------------------------->*/
/*该文件由uiresbuilder生成，请不要手动修改*/
/*<------------------------------------------------------------------------------------------------->*/
#pragma once
#include <res.mgr/snamedvalue.h>
#define ROBJ_IN_CPP \
namespace SOUI \
{\
    const _R R;\
    const _UIRES UIRES;\
}
namespace SOUI
{
	class _UIRES{
		public:
		class _UIDEF{
			public:
			_UIDEF(){
				XML_INIT = _T("UIDEF:XML_INIT");
			}
			const TCHAR * XML_INIT;
		}UIDEF;
		class _LAYOUT{
			public:
			_LAYOUT(){
				wnd_composition = _T("LAYOUT:wnd_composition");
				wnd_status_bar = _T("LAYOUT:wnd_status_bar");
				dlg_config = _T("LAYOUT:dlg_config");
				wnd_tip = _T("LAYOUT:wnd_tip");
			}
			const TCHAR * wnd_composition;
			const TCHAR * wnd_status_bar;
			const TCHAR * dlg_config;
			const TCHAR * wnd_tip;
		}LAYOUT;
		class _values{
			public:
			_values(){
				string = _T("values:string");
				color = _T("values:color");
				skin = _T("values:skin");
			}
			const TCHAR * string;
			const TCHAR * color;
			const TCHAR * skin;
		}values;
		class _IMG{
			public:
			_IMG(){
				logo_32 = _T("IMG:logo_32");
				png_charmode = _T("IMG:png_charmode");
				png_charmode_full = _T("IMG:png_charmode_full");
				png_charmode_half = _T("IMG:png_charmode_half");
				png_status_open = _T("IMG:png_status_open");
				png_status_close = _T("IMG:png_status_close");
				png_composition = _T("IMG:png_composition");
				btn_extend = _T("IMG:btn_extend");
				btn_shrink = _T("IMG:btn_shrink");
				btn_makeword = _T("IMG:btn_makeword");
				btn_prevpage = _T("IMG:btn_prevpage");
				btn_nextpage = _T("IMG:btn_nextpage");
				btn_query = _T("IMG:btn_query");
				btn_record_enable = _T("IMG:btn_record_enable");
				btn_record_disable = _T("IMG:btn_record_disable");
				btn_sound_enable = _T("IMG:btn_sound_enable");
				btn_sound_disable = _T("IMG:btn_sound_disable");
				SYS_SCROLLBAR = _T("IMG:SYS_SCROLLBAR");
				png_lines = _T("IMG:png_lines");
			}
			const TCHAR * logo_32;
			const TCHAR * png_charmode;
			const TCHAR * png_charmode_full;
			const TCHAR * png_charmode_half;
			const TCHAR * png_status_open;
			const TCHAR * png_status_close;
			const TCHAR * png_composition;
			const TCHAR * btn_extend;
			const TCHAR * btn_shrink;
			const TCHAR * btn_makeword;
			const TCHAR * btn_prevpage;
			const TCHAR * btn_nextpage;
			const TCHAR * btn_query;
			const TCHAR * btn_record_enable;
			const TCHAR * btn_record_disable;
			const TCHAR * btn_sound_enable;
			const TCHAR * btn_sound_disable;
			const TCHAR * SYS_SCROLLBAR;
			const TCHAR * png_lines;
		}IMG;
		class _ICON{
			public:
			_ICON(){
				ICON_LOGO = _T("ICON:ICON_LOGO");
			}
			const TCHAR * ICON_LOGO;
		}ICON;
		class _smenu{
			public:
			_smenu(){
				context = _T("smenu:context");
			}
			const TCHAR * context;
		}smenu;
	};
	const SNamedID::NAMEDVALUE namedXmlID[]={
		{L"_name_start",65535},
		{L"ass_mode_english_prompt",402},
		{L"ass_mode_none",400},
		{L"ass_mode_phrase",401},
		{L"ass_mode_phrase_prompt",403},
		{L"btn_charmode",65563},
		{L"btn_close",2},
		{L"btn_makeword",65564},
		{L"btn_nextpage",65551},
		{L"btn_prevpage",65549},
		{L"btn_record",65565},
		{L"btn_sound",65566},
		{L"btn_status_extend",65560},
		{L"btn_status_shrink",65561},
		{L"cand_auto_input",65571},
		{L"cand_container",65550},
		{L"cand_english",65552},
		{L"cand_normal",65548},
		{L"cand_phrase",65554},
		{L"cand_py_phrase_first",65572},
		{L"cand_tip",65555},
		{L"chk_auto_comp_promp",65582},
		{L"chk_auto_dot",65580},
		{L"chk_auto_select_cand",65581},
		{L"chk_disable_number_to_select_cand",65587},
		{L"chk_enable_23cand_hotkey",65584},
		{L"chk_full_skip_simple",65590},
		{L"chk_sent_input",65583},
		{L"chk_show_op_tip",65573},
		{L"comp_lineime",65543},
		{L"comp_normal",65536},
		{L"comp_sent",65544},
		{L"comp_spell",65539},
		{L"comp_start",400},
		{L"comp_umode",65541},
		{L"config",100},
		{L"edit_to_sentmode",65570},
		{L"edit_to_umode",65568},
		{L"enter_for_clear",110},
		{L"enter_for_input",111},
		{L"follow_caret",102},
		{L"forcast_always",601},
		{L"forcast_disable",600},
		{L"forcast_no_cand",602},
		{L"gbk_show_as_gb",702},
		{L"gbk_show_manual",701},
		{L"gbk_show_only",700},
		{L"help",112},
		{L"hide_statusbar",101},
		{L"hk_2_cand",65585},
		{L"hk_3_cand",65586},
		{L"hk_input_en",65579},
		{L"hk_make_phrase",65575},
		{L"hk_show_comp",65577},
		{L"hk_show_statusbar",65578},
		{L"hk_show_table",65576},
		{L"hk_switch_py",65574},
		{L"hk_turn_next",65589},
		{L"hk_turn_prev",65588},
		{L"ime_switch_disabled",102},
		{L"ime_switch_left_shift",100},
		{L"ime_switch_right_shift",101},
		{L"img_logo",65558},
		{L"input_big5",103},
		{L"key_map",110},
		{L"key_speed",111},
		{L"key_to_sentmode",65569},
		{L"key_to_umode",65567},
		{L"menu_blend",65594},
		{L"menu_comp",65593},
		{L"menu_server",65595},
		{L"menu_skin",200},
		{L"py_switch_disabled",122},
		{L"py_switch_left_ctrl",120},
		{L"py_switch_right_ctrl",121},
		{L"rate_adjust_auto",501},
		{L"rate_adjust_disable",500},
		{L"rate_adjust_fast",502},
		{L"sent_input",65545},
		{L"sent_left",65546},
		{L"sent_right",65547},
		{L"skin_cloud",202},
		{L"skin_def",220},
		{L"skin_mgr",201},
		{L"sound_beep",802},
		{L"sound_disable",800},
		{L"sound_wave",801},
		{L"spell_all",504},
		{L"spell_one",501},
		{L"spell_three",503},
		{L"spell_two",502},
		{L"status_base",65557},
		{L"status_bg",65556},
		{L"status_extend",65562},
		{L"svr_showicon",601},
		{L"txt_auto_complete",65542},
		{L"txt_comp",65559},
		{L"txt_comps",65537},
		{L"txt_en_header",65553},
		{L"txt_temp_spell_flag",65540},
		{L"txt_tip",65538},
		{L"txt_tip_content",65592},
		{L"txt_tip_title",65591},
		{L"userdef",505}		};
	class _R{
	public:
		class _name{
		public:
		_name(){
			_name_start = namedXmlID[0].strName;
			ass_mode_english_prompt = namedXmlID[1].strName;
			ass_mode_none = namedXmlID[2].strName;
			ass_mode_phrase = namedXmlID[3].strName;
			ass_mode_phrase_prompt = namedXmlID[4].strName;
			btn_charmode = namedXmlID[5].strName;
			btn_close = namedXmlID[6].strName;
			btn_makeword = namedXmlID[7].strName;
			btn_nextpage = namedXmlID[8].strName;
			btn_prevpage = namedXmlID[9].strName;
			btn_record = namedXmlID[10].strName;
			btn_sound = namedXmlID[11].strName;
			btn_status_extend = namedXmlID[12].strName;
			btn_status_shrink = namedXmlID[13].strName;
			cand_auto_input = namedXmlID[14].strName;
			cand_container = namedXmlID[15].strName;
			cand_english = namedXmlID[16].strName;
			cand_normal = namedXmlID[17].strName;
			cand_phrase = namedXmlID[18].strName;
			cand_py_phrase_first = namedXmlID[19].strName;
			cand_tip = namedXmlID[20].strName;
			chk_auto_comp_promp = namedXmlID[21].strName;
			chk_auto_dot = namedXmlID[22].strName;
			chk_auto_select_cand = namedXmlID[23].strName;
			chk_disable_number_to_select_cand = namedXmlID[24].strName;
			chk_enable_23cand_hotkey = namedXmlID[25].strName;
			chk_full_skip_simple = namedXmlID[26].strName;
			chk_sent_input = namedXmlID[27].strName;
			chk_show_op_tip = namedXmlID[28].strName;
			comp_lineime = namedXmlID[29].strName;
			comp_normal = namedXmlID[30].strName;
			comp_sent = namedXmlID[31].strName;
			comp_spell = namedXmlID[32].strName;
			comp_start = namedXmlID[33].strName;
			comp_umode = namedXmlID[34].strName;
			config = namedXmlID[35].strName;
			edit_to_sentmode = namedXmlID[36].strName;
			edit_to_umode = namedXmlID[37].strName;
			enter_for_clear = namedXmlID[38].strName;
			enter_for_input = namedXmlID[39].strName;
			follow_caret = namedXmlID[40].strName;
			forcast_always = namedXmlID[41].strName;
			forcast_disable = namedXmlID[42].strName;
			forcast_no_cand = namedXmlID[43].strName;
			gbk_show_as_gb = namedXmlID[44].strName;
			gbk_show_manual = namedXmlID[45].strName;
			gbk_show_only = namedXmlID[46].strName;
			help = namedXmlID[47].strName;
			hide_statusbar = namedXmlID[48].strName;
			hk_2_cand = namedXmlID[49].strName;
			hk_3_cand = namedXmlID[50].strName;
			hk_input_en = namedXmlID[51].strName;
			hk_make_phrase = namedXmlID[52].strName;
			hk_show_comp = namedXmlID[53].strName;
			hk_show_statusbar = namedXmlID[54].strName;
			hk_show_table = namedXmlID[55].strName;
			hk_switch_py = namedXmlID[56].strName;
			hk_turn_next = namedXmlID[57].strName;
			hk_turn_prev = namedXmlID[58].strName;
			ime_switch_disabled = namedXmlID[59].strName;
			ime_switch_left_shift = namedXmlID[60].strName;
			ime_switch_right_shift = namedXmlID[61].strName;
			img_logo = namedXmlID[62].strName;
			input_big5 = namedXmlID[63].strName;
			key_map = namedXmlID[64].strName;
			key_speed = namedXmlID[65].strName;
			key_to_sentmode = namedXmlID[66].strName;
			key_to_umode = namedXmlID[67].strName;
			menu_blend = namedXmlID[68].strName;
			menu_comp = namedXmlID[69].strName;
			menu_server = namedXmlID[70].strName;
			menu_skin = namedXmlID[71].strName;
			py_switch_disabled = namedXmlID[72].strName;
			py_switch_left_ctrl = namedXmlID[73].strName;
			py_switch_right_ctrl = namedXmlID[74].strName;
			rate_adjust_auto = namedXmlID[75].strName;
			rate_adjust_disable = namedXmlID[76].strName;
			rate_adjust_fast = namedXmlID[77].strName;
			sent_input = namedXmlID[78].strName;
			sent_left = namedXmlID[79].strName;
			sent_right = namedXmlID[80].strName;
			skin_cloud = namedXmlID[81].strName;
			skin_def = namedXmlID[82].strName;
			skin_mgr = namedXmlID[83].strName;
			sound_beep = namedXmlID[84].strName;
			sound_disable = namedXmlID[85].strName;
			sound_wave = namedXmlID[86].strName;
			spell_all = namedXmlID[87].strName;
			spell_one = namedXmlID[88].strName;
			spell_three = namedXmlID[89].strName;
			spell_two = namedXmlID[90].strName;
			status_base = namedXmlID[91].strName;
			status_bg = namedXmlID[92].strName;
			status_extend = namedXmlID[93].strName;
			svr_showicon = namedXmlID[94].strName;
			txt_auto_complete = namedXmlID[95].strName;
			txt_comp = namedXmlID[96].strName;
			txt_comps = namedXmlID[97].strName;
			txt_en_header = namedXmlID[98].strName;
			txt_temp_spell_flag = namedXmlID[99].strName;
			txt_tip = namedXmlID[100].strName;
			txt_tip_content = namedXmlID[101].strName;
			txt_tip_title = namedXmlID[102].strName;
			userdef = namedXmlID[103].strName;
		}
		 const wchar_t * _name_start;
		 const wchar_t * ass_mode_english_prompt;
		 const wchar_t * ass_mode_none;
		 const wchar_t * ass_mode_phrase;
		 const wchar_t * ass_mode_phrase_prompt;
		 const wchar_t * btn_charmode;
		 const wchar_t * btn_close;
		 const wchar_t * btn_makeword;
		 const wchar_t * btn_nextpage;
		 const wchar_t * btn_prevpage;
		 const wchar_t * btn_record;
		 const wchar_t * btn_sound;
		 const wchar_t * btn_status_extend;
		 const wchar_t * btn_status_shrink;
		 const wchar_t * cand_auto_input;
		 const wchar_t * cand_container;
		 const wchar_t * cand_english;
		 const wchar_t * cand_normal;
		 const wchar_t * cand_phrase;
		 const wchar_t * cand_py_phrase_first;
		 const wchar_t * cand_tip;
		 const wchar_t * chk_auto_comp_promp;
		 const wchar_t * chk_auto_dot;
		 const wchar_t * chk_auto_select_cand;
		 const wchar_t * chk_disable_number_to_select_cand;
		 const wchar_t * chk_enable_23cand_hotkey;
		 const wchar_t * chk_full_skip_simple;
		 const wchar_t * chk_sent_input;
		 const wchar_t * chk_show_op_tip;
		 const wchar_t * comp_lineime;
		 const wchar_t * comp_normal;
		 const wchar_t * comp_sent;
		 const wchar_t * comp_spell;
		 const wchar_t * comp_start;
		 const wchar_t * comp_umode;
		 const wchar_t * config;
		 const wchar_t * edit_to_sentmode;
		 const wchar_t * edit_to_umode;
		 const wchar_t * enter_for_clear;
		 const wchar_t * enter_for_input;
		 const wchar_t * follow_caret;
		 const wchar_t * forcast_always;
		 const wchar_t * forcast_disable;
		 const wchar_t * forcast_no_cand;
		 const wchar_t * gbk_show_as_gb;
		 const wchar_t * gbk_show_manual;
		 const wchar_t * gbk_show_only;
		 const wchar_t * help;
		 const wchar_t * hide_statusbar;
		 const wchar_t * hk_2_cand;
		 const wchar_t * hk_3_cand;
		 const wchar_t * hk_input_en;
		 const wchar_t * hk_make_phrase;
		 const wchar_t * hk_show_comp;
		 const wchar_t * hk_show_statusbar;
		 const wchar_t * hk_show_table;
		 const wchar_t * hk_switch_py;
		 const wchar_t * hk_turn_next;
		 const wchar_t * hk_turn_prev;
		 const wchar_t * ime_switch_disabled;
		 const wchar_t * ime_switch_left_shift;
		 const wchar_t * ime_switch_right_shift;
		 const wchar_t * img_logo;
		 const wchar_t * input_big5;
		 const wchar_t * key_map;
		 const wchar_t * key_speed;
		 const wchar_t * key_to_sentmode;
		 const wchar_t * key_to_umode;
		 const wchar_t * menu_blend;
		 const wchar_t * menu_comp;
		 const wchar_t * menu_server;
		 const wchar_t * menu_skin;
		 const wchar_t * py_switch_disabled;
		 const wchar_t * py_switch_left_ctrl;
		 const wchar_t * py_switch_right_ctrl;
		 const wchar_t * rate_adjust_auto;
		 const wchar_t * rate_adjust_disable;
		 const wchar_t * rate_adjust_fast;
		 const wchar_t * sent_input;
		 const wchar_t * sent_left;
		 const wchar_t * sent_right;
		 const wchar_t * skin_cloud;
		 const wchar_t * skin_def;
		 const wchar_t * skin_mgr;
		 const wchar_t * sound_beep;
		 const wchar_t * sound_disable;
		 const wchar_t * sound_wave;
		 const wchar_t * spell_all;
		 const wchar_t * spell_one;
		 const wchar_t * spell_three;
		 const wchar_t * spell_two;
		 const wchar_t * status_base;
		 const wchar_t * status_bg;
		 const wchar_t * status_extend;
		 const wchar_t * svr_showicon;
		 const wchar_t * txt_auto_complete;
		 const wchar_t * txt_comp;
		 const wchar_t * txt_comps;
		 const wchar_t * txt_en_header;
		 const wchar_t * txt_temp_spell_flag;
		 const wchar_t * txt_tip;
		 const wchar_t * txt_tip_content;
		 const wchar_t * txt_tip_title;
		 const wchar_t * userdef;
		}name;

		class _id{
		public:
		const static int _name_start	=	65535;
		const static int ass_mode_english_prompt	=	402;
		const static int ass_mode_none	=	400;
		const static int ass_mode_phrase	=	401;
		const static int ass_mode_phrase_prompt	=	403;
		const static int btn_charmode	=	65563;
		const static int btn_close	=	2;
		const static int btn_makeword	=	65564;
		const static int btn_nextpage	=	65551;
		const static int btn_prevpage	=	65549;
		const static int btn_record	=	65565;
		const static int btn_sound	=	65566;
		const static int btn_status_extend	=	65560;
		const static int btn_status_shrink	=	65561;
		const static int cand_auto_input	=	65571;
		const static int cand_container	=	65550;
		const static int cand_english	=	65552;
		const static int cand_normal	=	65548;
		const static int cand_phrase	=	65554;
		const static int cand_py_phrase_first	=	65572;
		const static int cand_tip	=	65555;
		const static int chk_auto_comp_promp	=	65582;
		const static int chk_auto_dot	=	65580;
		const static int chk_auto_select_cand	=	65581;
		const static int chk_disable_number_to_select_cand	=	65587;
		const static int chk_enable_23cand_hotkey	=	65584;
		const static int chk_full_skip_simple	=	65590;
		const static int chk_sent_input	=	65583;
		const static int chk_show_op_tip	=	65573;
		const static int comp_lineime	=	65543;
		const static int comp_normal	=	65536;
		const static int comp_sent	=	65544;
		const static int comp_spell	=	65539;
		const static int comp_start	=	400;
		const static int comp_umode	=	65541;
		const static int config	=	100;
		const static int edit_to_sentmode	=	65570;
		const static int edit_to_umode	=	65568;
		const static int enter_for_clear	=	110;
		const static int enter_for_input	=	111;
		const static int follow_caret	=	102;
		const static int forcast_always	=	601;
		const static int forcast_disable	=	600;
		const static int forcast_no_cand	=	602;
		const static int gbk_show_as_gb	=	702;
		const static int gbk_show_manual	=	701;
		const static int gbk_show_only	=	700;
		const static int help	=	112;
		const static int hide_statusbar	=	101;
		const static int hk_2_cand	=	65585;
		const static int hk_3_cand	=	65586;
		const static int hk_input_en	=	65579;
		const static int hk_make_phrase	=	65575;
		const static int hk_show_comp	=	65577;
		const static int hk_show_statusbar	=	65578;
		const static int hk_show_table	=	65576;
		const static int hk_switch_py	=	65574;
		const static int hk_turn_next	=	65589;
		const static int hk_turn_prev	=	65588;
		const static int ime_switch_disabled	=	102;
		const static int ime_switch_left_shift	=	100;
		const static int ime_switch_right_shift	=	101;
		const static int img_logo	=	65558;
		const static int input_big5	=	103;
		const static int key_map	=	110;
		const static int key_speed	=	111;
		const static int key_to_sentmode	=	65569;
		const static int key_to_umode	=	65567;
		const static int menu_blend	=	65594;
		const static int menu_comp	=	65593;
		const static int menu_server	=	65595;
		const static int menu_skin	=	200;
		const static int py_switch_disabled	=	122;
		const static int py_switch_left_ctrl	=	120;
		const static int py_switch_right_ctrl	=	121;
		const static int rate_adjust_auto	=	501;
		const static int rate_adjust_disable	=	500;
		const static int rate_adjust_fast	=	502;
		const static int sent_input	=	65545;
		const static int sent_left	=	65546;
		const static int sent_right	=	65547;
		const static int skin_cloud	=	202;
		const static int skin_def	=	220;
		const static int skin_mgr	=	201;
		const static int sound_beep	=	802;
		const static int sound_disable	=	800;
		const static int sound_wave	=	801;
		const static int spell_all	=	504;
		const static int spell_one	=	501;
		const static int spell_three	=	503;
		const static int spell_two	=	502;
		const static int status_base	=	65557;
		const static int status_bg	=	65556;
		const static int status_extend	=	65562;
		const static int svr_showicon	=	601;
		const static int txt_auto_complete	=	65542;
		const static int txt_comp	=	65559;
		const static int txt_comps	=	65537;
		const static int txt_en_header	=	65553;
		const static int txt_temp_spell_flag	=	65540;
		const static int txt_tip	=	65538;
		const static int txt_tip_content	=	65592;
		const static int txt_tip_title	=	65591;
		const static int userdef	=	505;
		}id;

		class _string{
		public:
		const static int btn_close	=	0;
		const static int cfg_about	=	1;
		const static int cfg_associate	=	2;
		const static int cfg_candidate	=	3;
		const static int cfg_habit	=	4;
		const static int cfg_hotkey	=	5;
		const static int cfg_misc	=	6;
		const static int txt_config	=	7;
		}string;

		class _color{
		public:
		const static int blue	=	0;
		const static int cand_comp	=	1;
		const static int cand_index	=	2;
		const static int cand_phonetic	=	3;
		const static int cand_text	=	4;
		const static int cand_wild	=	5;
		const static int comp_auto_complete	=	6;
		const static int comp_umode_prefix	=	7;
		const static int configborder	=	8;
		const static int gray	=	9;
		const static int green	=	10;
		const static int red	=	11;
		const static int sent_input	=	12;
		const static int sent_left	=	13;
		const static int sent_right	=	14;
		const static int white	=	15;
		}color;

	};

#ifdef R_IN_CPP
	 extern const _R R;
	 extern const _UIRES UIRES;
#else
	 extern const __declspec(selectany) _R & R = _R();
	 extern const __declspec(selectany) _UIRES & UIRES = _UIRES();
#endif//R_IN_CPP
}
