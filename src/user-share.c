#ifdef HAVE_CONFIG_H
#include "config.h"
#endif
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <shadow.h>
#include <pwquality.h>
#include "user-share.h"


/******************************************************************************
* 函数:             MessageReport
*
* 说明:  显示信息对话框，用来显示用户操作后失败提醒
*
* 输入:
*        @Title          消息标题
*        @Msg            消息
*        @nType          消息类型
* 返回: 
*       用户操作状态
*
* 作者:  zhuyaliang  25/05/2018
******************************************************************************/
int MessageReport(const char *Title,const char *Msg,int nType)
{
    GtkWidget *dialog = NULL;
    int nRet;

    switch(nType)
    {
        case ERROR:
        {
            dialog = gtk_message_dialog_new(GTK_WINDOW(WindowLogin),
                                            GTK_DIALOG_DESTROY_WITH_PARENT,
                                            GTK_MESSAGE_ERROR,
                                            GTK_BUTTONS_OK,
                                            "%s",Title);
            break;
        }
        case WARING:
        {
            dialog = gtk_message_dialog_new(GTK_WINDOW(WindowLogin),
                                            GTK_DIALOG_DESTROY_WITH_PARENT,
                                            GTK_MESSAGE_WARNING,
                                            GTK_BUTTONS_OK,
                                            "%s",Title);
            break;
        }
        case INFOR:
        {
            dialog = gtk_message_dialog_new(GTK_WINDOW(WindowLogin),
                                            GTK_DIALOG_DESTROY_WITH_PARENT,
                                            GTK_MESSAGE_INFO,
                                            GTK_BUTTONS_OK,
                                            "%s",Title);
            break;
        }
        case QUESTION:
        {
            dialog = gtk_message_dialog_new(GTK_WINDOW(WindowLogin),
                                            GTK_DIALOG_DESTROY_WITH_PARENT,
                                            GTK_MESSAGE_QUESTION,
                                            GTK_BUTTONS_YES_NO,
                                            "%s",Title);
            break;
        }
        default :
            break;

    }
    gtk_message_dialog_format_secondary_markup(GTK_MESSAGE_DIALOG(dialog),TYPEMSG,Msg);
    gtk_window_set_title(GTK_WINDOW(dialog),_("Message"));
    nRet =  gtk_dialog_run(GTK_DIALOG(dialog));
    gtk_widget_destroy(dialog);
    return nRet;
}


/******************************************************************************
* 函数:             SetLableFontType
*
* 说明:  设置Lable的显示样式，包括颜色字体
*
* 输入:
*        @GtkWidget      需要设置的lable控件
*        @Color          控件颜色
*        @FontSize       文字字体
*        @Word           显示内容
* 返回:
*
* 作者:  zhuyaliang  25/05/2018
******************************************************************************/
void SetLableFontType(GtkWidget *Lable ,const char *Color,int FontSzie,const char *Word)        
{
    char LableTypeBuf[200] = { 0 };
    
    sprintf(LableTypeBuf,"<span foreground=\'%s\'weight=\'light\'font_desc=\'%d\'>%s</span>",
                             Color,FontSzie,Word);
    gtk_label_set_markup(GTK_LABEL(Lable),LableTypeBuf);

}        
/******************************************************************************
* 函数:             UserListAppend
*
* 说明:  向列表中添加用户信息
*
* 输入:
*        @list        列表
*        @UserIcon    头像
*        @UserName    名字
*        @Color       字体颜色
*        @Index       用户标号
*        Iter
* 返回:
*
* 作者:  zhuyaliang  09/05/2018
******************************************************************************/
void UserListAppend(GtkWidget *list,
                    const gchar *UserIcon,
                    const gchar *UserName,
                    const gchar *Color,
                    int Index,
                    GtkTreeIter *Iter)
{
    GtkListStore *store;
    GtkTreeIter iter;
    GdkPixbuf *face;

    /*set user icon size 50 * 50 */
    face = SetUserFaceSize (UserIcon, 50);
    store = GTK_LIST_STORE(gtk_tree_view_get_model(GTK_TREE_VIEW(list)));

    gtk_list_store_append(store, &iter);
    gtk_list_store_set(store, &iter,COL_USER_FACE, face,  //icon 
                                    INT_COLUMN,Index,     //count
                                    LIST_TEXT, UserName,  //name
                                    LIST_COLOR,Color,     //text color
                                    LIST_FRONT,1985,-1);   //text front
    *Iter = iter;

}
/******************************************************************************
* 函数:              SetUserFaceSize   
*        
* 说明:  设置照片尺寸
*        
* 输入:  		
*        @PicName  照片名字
*        @Size     尺寸
*        
* 返回:  
*        
* 作者:  zhuyaliang  09/05/2018
******************************************************************************/ 
GdkPixbuf * SetUserFaceSize (const char  *PicName, int Size)
{
    GdkPixbuf *pixbuf;
    char Path[128] = { 0 };
    if(PicName == NULL)
    {
        sprintf(Path, FACEDIR"/Default.png");
    }
    else
        sprintf(Path, "%s",PicName);

    pixbuf = gdk_pixbuf_new_from_file_at_size (Path, Size, Size, NULL);

    return pixbuf;
}
/******************************************************************************
* 函数:              UpdateInterface
*
* 说明:  更新显示界面
*
* 输入:
*        @Cnt  需要更新的用户标号
*
* 返回:
*
* 作者:  zhuyaliang  09/05/2018
******************************************************************************/
void UpdateInterface(int Cnt,UserAdmin *ua)
{
    GtkWidget *image;
    GdkPixbuf *pb, *pb2;
    
    //切换用户时有些选项会变动，会引起信号响应，需要设置标志位，Chang = 1时或略信号响应处理
    Change = 1;           
    pb = gdk_pixbuf_new_from_file(ua->ul[Cnt].UserIcon,NULL);
    pb2 = gdk_pixbuf_scale_simple (pb,96,96, GDK_INTERP_BILINEAR);
    image = gtk_image_new_from_pixbuf(pb2);
    
    gtk_button_set_image(GTK_BUTTON(ua->ButtonFace),
                         image);

    gtk_entry_set_text(GTK_ENTRY(ua->EntryName),
                       ua->ul[Cnt].RealName); 

    gtk_combo_box_set_active(GTK_COMBO_BOX(ua->ComUserType),
                             ua->ul[Cnt].UserType);

    gtk_combo_box_set_active(GTK_COMBO_BOX(ua->ComUserLanguage),
                             ua->ul[Cnt].LangType);

    gtk_button_set_label(GTK_BUTTON(ua->ButtonPass),
                         ua->ul[Cnt].PassText);
    
    gtk_switch_set_active(GTK_SWITCH(ua->SwitchAutoLogin),
                          ua->ul[Cnt].LoginType);

    gtk_button_set_label (GTK_BUTTON(ua->ButtonUserTime),
                          ua->ul[Cnt].UserTime);
    Change = 0;

} 
static pwquality_settings_t * get_pwq (void)
{
    static pwquality_settings_t *settings;

    if (settings == NULL)
   	{
    	char *err = NULL;
        settings = pwquality_default_settings ();
        pwquality_set_int_value (settings, PWQ_SETTING_MAX_SEQUENCE, 4);
        if (pwquality_read_config (settings, NULL, (gpointer)&err) < 0)
       	{
        	return NULL;
        }
    }

    return settings;
}

/******************************************************************************
* 函数:              OpenNote
*
* 说明:  打开提示
*
* 输入:
*        @note   提示信息
*
* 返回:
*
* 作者:  zhuyaliang  09/05/2018
******************************************************************************/
static int pw_min_length (void)
{
	int value = 0;
	if (pwquality_get_int_value (get_pwq (), PWQ_SETTING_MIN_LENGTH, &value) < 0)
	{
    	return -1;
    }
    return value;
}
/******************************************************************************
* 函数:              OpenNote
*
* 说明:  打开提示
*
* 输入:
*        @note   提示信息
*
* 返回:
*
* 作者:  zhuyaliang  09/05/2018
******************************************************************************/
static const gchar *pw_error_hint (gint error)
{
        switch (error) {
        case PWQ_ERROR_SAME_PASSWORD:
                return _("The new password needs to be different from the old one");
        case PWQ_ERROR_CASE_CHANGES_ONLY:
                return _("Try changing some letters and numbers");
        case PWQ_ERROR_TOO_SIMILAR:
                return _("Try changing the password a bit more");
        case PWQ_ERROR_USER_CHECK:
                return _("A password without your user name would be stronger");
        case PWQ_ERROR_GECOS_CHECK:
                return _("Try to avoid using your name in the password");
        case PWQ_ERROR_BAD_WORDS:
                return _("Try to avoid some of the words included in the password");
        case PWQ_ERROR_ROTATED:
                return _("Try changing the password a bit more");
        case PWQ_ERROR_CRACKLIB_CHECK:
                return _("Try to avoid common words");
        case PWQ_ERROR_PALINDROME:
                return _("Try to avoid reordering existing words");
        case PWQ_ERROR_MIN_DIGITS:
                return _("Try to use more numbers");
        case PWQ_ERROR_MIN_UPPERS:
                return _("Try to use more uppercase letters");
        case PWQ_ERROR_MIN_LOWERS:
                return _("Try to use more lowercase letters");
        case PWQ_ERROR_MIN_OTHERS:
                return _("Try to use more special characters, like punctuation");
        case PWQ_ERROR_MIN_CLASSES:
                return _("Try to use a mixture of letters, numbers and punctuation");
        case PWQ_ERROR_MAX_CONSECUTIVE:
                return _("Try to avoid repeating the same character");
        case PWQ_ERROR_MAX_CLASS_REPEAT:
                return _("Try to avoid repeating the same type of character: you need     to mix up letters, numbers and punctuation.");
        case PWQ_ERROR_MAX_SEQUENCE:
                return _("Try to avoid sequences like 1234 or abcd");
        case PWQ_ERROR_MIN_LENGTH:
                return _("Password needs to be longer. Try to add more letters, numbe    rs and punctuation");
        case PWQ_ERROR_EMPTY_PASSWORD:
                return ("     ");
        default:
                return NULL;
        }
}


/******************************************************************************
* 函数:              GetPassStrength
*
* 说明:  获得设置密码的密码强度
*
* 输入:
*        
*
* 返回:  新密码强度等级
*
* 作者:  zhuyaliang  09/05/2018
******************************************************************************/
int GetPassStrength (const char  *password,
                     const char  *old_password,
                     const char  *username,
                     const char **message)
{
	int rv, level, length = 0;
    double strength = 0.0;
    void *auxerror;

    rv = pwquality_check (get_pwq (),
                          password, 
                          old_password, 
                          username,
                          &auxerror);

    if (password != NULL)
    	length = strlen (password);
    strength = 0.01 * rv;

    if (rv < 0)
   	{
    	level = (length > 0) ? 1 : 0;
    }
    else if (strength < 0.50)
   	{
        level = 2;
    }
    else if (strength < 0.75)
   	{
        level = 3;
    }
    else if (strength < 0.90)
   	{
        level = 4;
    }
    else
   	{
        level = 5;
    }

    if ( length < pw_min_length())
    {        
    	*message = pw_error_hint (PWQ_ERROR_MIN_LENGTH);
         level = 0;
    }
    else
    {        
        *message = pw_error_hint (rv);
    }    
	return level;
}

/******************************************************************************
* 函数:              CheckPassword
*
* 说明:  定时器循环检测设置密码是否合理响应函数800ms进入一次
*
* 输入:
*        @note   提示信息
*
* 返回:
*
* 作者:  zhuyaliang  09/05/2018
******************************************************************************/
gboolean CheckPassword(gpointer data)
{
	UserAdmin *ua = (UserAdmin *)data;
    const char *s;
	int Level;
    const char *Message;

	s = gtk_entry_get_text(GTK_ENTRY(ua->NewPassEntry));
    Level = GetPassStrength (s, NULL,NULL,&Message);
    gtk_level_bar_set_value (GTK_LEVEL_BAR (ua->LevelBar), Level);

  	if(Message == NULL && Level > 1)
  	{
  		 OffNote(ua->LabelPassNote,ua);
  		 return TRUE;
  	}
    OpenNote(ua->LabelPassNote,Message,ua);
    return TRUE;

}
/******************************************************************************
* 函数:              AutoGenera
*
* 说明:  自动生成密码
*
* 输入:
*        @note   提示信息
*
* 返回:
*
* 作者:  zhuyaliang  09/05/2018
******************************************************************************/
void AutoGenera (GtkEntry            *entry,
                 GtkEntryIconPosition icon_pos,
                 GdkEvent            *event,
                 gpointer             data)
{

    char *NewPassWord;
    UserAdmin *ua = (UserAdmin *)data;

   	pwquality_generate (get_pwq (), 0, &NewPassWord);
    gtk_entry_set_text(GTK_ENTRY(ua->NewPassEntry),NewPassWord);
    gtk_entry_set_text(GTK_ENTRY(ua->CheckPassEntry),NewPassWord);

}

/******************************************************************************
* 函数:              OpenNote
*
* 说明:  打开提示,锁控件
*
* 输入:
*        @note   提示信息
*
* 返回:
*
* 作者:  zhuyaliang  09/05/2018
******************************************************************************/
void OpenNote(GtkWidget *label,const char *note,UserAdmin *ua)
{
    SetLableFontType(label,"red",10,note);
    gtk_widget_set_sensitive(ua->ButtonConfirm, FALSE);
}  
     
/******************************************************************************
* 函数:              OffNote
*
* 说明:  关闭提示，解锁确认按钮
*
* 输入:
*
* 返回:
*
* 作者:  zhuyaliang  09/05/2018
******************************************************************************/
void OffNote(GtkWidget *label,UserAdmin *ua)
{
    gtk_label_set_markup(GTK_LABEL(label),NULL);
    gtk_widget_set_sensitive(ua->ButtonConfirm, TRUE);
}   

/******************************************************************************
* 函数:              SetComboLanguageType
*
* 说明: 创建语言下拉框
*
* 输入:
*        @s1 语言类型
*        @s2
*
* 返回:
*
* 作者:  zhuyaliang  09/05/2018
******************************************************************************/
GtkWidget *SetComboLanguageType(const char *s1,const char *s2)
{
    GtkListStore    *Store;
    GtkTreeIter     Iter;
    GtkCellRenderer *Renderer;
    GtkWidget *ComboUser;

    Store = gtk_list_store_new(1,G_TYPE_STRING);
    gtk_list_store_append(Store,&Iter);
    gtk_list_store_set(Store,&Iter,0,s1,-1);
    gtk_list_store_append(Store,&Iter);
    gtk_list_store_set(Store,&Iter,0,s2,-1);

    ComboUser = gtk_combo_box_new_with_model(GTK_TREE_MODEL(Store));
    g_object_unref(G_OBJECT(Store));
    Renderer = gtk_cell_renderer_text_new();
    gtk_cell_layout_pack_start(GTK_CELL_LAYOUT(ComboUser),Renderer,TRUE);
    gtk_cell_layout_set_attributes(GTK_CELL_LAYOUT(ComboUser),Renderer,"text",0,NULL);

    return ComboUser;
}
/******************************************************************************
* 函数:              SetComboType
*
* 说明: 创建用户类型下拉框
*
* 输入:
*        @s1 用户类型
*        @s2
*
* 返回:
*
* 作者:  zhuyaliang  09/05/2018
******************************************************************************/
GtkWidget *SetComboUserType(const char *s1,const char *s2)
{
    GtkListStore    *Store;
    GtkTreeIter     Iter;
    GtkCellRenderer *Renderer;
    GtkWidget *ComboUser;

    Store = gtk_list_store_new(1,G_TYPE_STRING);
    gtk_list_store_append(Store,&Iter);
    gtk_list_store_set(Store,&Iter,0,s1,-1);
    gtk_list_store_append(Store,&Iter);
    gtk_list_store_set(Store,&Iter,0,s2,-1);

    ComboUser = gtk_combo_box_new_with_model(GTK_TREE_MODEL(Store));
    g_object_unref(G_OBJECT(Store));
    Renderer = gtk_cell_renderer_text_new();
    gtk_cell_layout_pack_start(GTK_CELL_LAYOUT(ComboUser),Renderer,TRUE);
    gtk_cell_layout_set_attributes(GTK_CELL_LAYOUT(ComboUser),Renderer,"text",0,NULL);

    return ComboUser;
}
