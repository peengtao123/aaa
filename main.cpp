#include <gtk/gtk.h>
#include <webkit2/webkit2.h>

// 导航到地址栏中的 URL
static void on_go_button_clicked(GtkWidget *button, gpointer user_data) {
    GtkWidget *entry = GTK_WIDGET(g_object_get_data(G_OBJECT(button), "entry"));
    WebKitWebView *web_view = WEBKIT_WEB_VIEW(g_object_get_data(G_OBJECT(button), "web_view"));
    
    const gchar *uri = gtk_entry_get_text(GTK_ENTRY(entry));
    if (uri && *uri) {
        // 如果用户没有输入协议，默认添加 http://
        gchar *full_uri = g_strdup(uri);
        if (!g_str_has_prefix(uri, "http://") && !g_str_has_prefix(uri, "https://")) {
            g_free(full_uri);
            full_uri = g_strconcat("http://", uri, NULL);
        }
        
        webkit_web_view_load_uri(web_view, full_uri);
        g_free(full_uri);
    }
}

// 当页面标题改变时，更新窗口标题
static void on_title_changed(WebKitWebView *web_view, GParamSpec *pspec, gpointer user_data) {
    GtkWidget *window = GTK_WIDGET(user_data);
    const gchar *title = webkit_web_view_get_title(web_view);
    if (title) {
        gtk_window_set_title(GTK_WINDOW(window), title);
    } else {
        gtk_window_set_title(GTK_WINDOW(window), "aaaa");
    }
}

static void on_activate(GtkApplication *app) {
    // 创建新窗口
    GtkWidget *window = gtk_application_window_new(app);
    gtk_window_set_title(GTK_WINDOW(window), "aaaa");
    gtk_window_set_default_size(GTK_WINDOW(window), 1024, 768);

    // 创建垂直盒子容器
    GtkWidget *vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    gtk_container_add(GTK_CONTAINER(window), vbox);

    // 创建地址栏盒子 (水平)
    GtkWidget *hbox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);
    gtk_container_set_border_width(GTK_CONTAINER(hbox), 5);
    gtk_box_pack_start(GTK_BOX(vbox), hbox, FALSE, FALSE, 0);

    // 创建地址输入框
    GtkWidget *entry = gtk_entry_new();
    gtk_entry_set_text(GTK_ENTRY(entry), "https://www.baidu.com");
    gtk_box_pack_start(GTK_BOX(hbox), entry, TRUE, TRUE, 0);

    // 创建“前往”按钮
    GtkWidget *go_button = gtk_button_new_with_label("go");
    gtk_box_pack_start(GTK_BOX(hbox), go_button, FALSE, FALSE, 0);

    // 创建 WebView
    WebKitWebView *web_view = WEBKIT_WEB_VIEW(webkit_web_view_new());
    
    // 将 entry 和 web_view 存储到按钮中，以便在回调中访问
    g_object_set_data(G_OBJECT(go_button), "entry", entry);
    g_object_set_data(G_OBJECT(go_button), "web_view", web_view);

    // 连接信号
    g_signal_connect(go_button, "clicked", G_CALLBACK(on_go_button_clicked), NULL);
    g_signal_connect(web_view, "notify::title", G_CALLBACK(on_title_changed), window);
    
    // 支持在 Entry 中按回车键跳转
    g_signal_connect_swapped(entry, "activate", G_CALLBACK(on_go_button_clicked), go_button);

    // 将 WebView 添加到滚动窗口中（虽然 WebView 自带滚动，但有时包裹在 ScrolledWindow 中更灵活）
    GtkWidget *scrolled_window = gtk_scrolled_window_new(NULL, NULL);
    gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scrolled_window), GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
    gtk_container_add(GTK_CONTAINER(scrolled_window), GTK_WIDGET(web_view));
    
    gtk_box_pack_start(GTK_BOX(vbox), scrolled_window, TRUE, TRUE, 0);

    // 加载初始页面
    webkit_web_view_load_uri(web_view, "https://www.google.com");

    // 显示所有控件
    gtk_widget_show_all(window);
}

int main(int argc, char **argv) {
    // 初始化 GTK
    gtk_init(&argc, &argv);

    // 创建应用程序
    GtkApplication *app = gtk_application_new("com.example.simplebrowser", G_APPLICATION_FLAGS_NONE);
    
    g_signal_connect(app, "activate", G_CALLBACK(on_activate), NULL);
    
    int status = g_application_run(G_APPLICATION(app), argc, argv);
    
    g_object_unref(app);
    
    return status;
}