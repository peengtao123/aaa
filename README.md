# 简单浏览器 (Simple Browser)

一个基于 GTK3 和 WebKit2GTK 构建的轻量级桌面浏览器应用。

## 📋 项目简介

这是一个使用 C++ 开发的简单浏览器,集成了 GTK3 图形界面库和 WebKit2GTK 网页渲染引擎,提供了基本的网页浏览功能。

## ✨ 主要功能

### 1. **地址栏导航**
- 支持在地址栏输入 URL 并点击"前往"按钮加载网页
- 自动补全协议前缀:如果输入的 URL 没有 `http://` 或 `https://`,会自动添加 `http://`
- 支持在地址栏按回车键快速加载页面

### 2. **超链接跳转**
- 完整支持页面内超链接点击跳转
- 通过 `decide-policy` 信号处理导航策略决策
- 自动处理新窗口请求,在当前窗口打开而非弹出新窗口

### 3. **标题同步**
- 网页标题改变时,自动更新窗口标题
- 实时显示当前页面的标题信息

### 4. **默认首页**
- 启动时自动加载百度首页 (https://www.baidu.com)
- 可自定义修改为其他默认页面

### 5. **滚动支持**
- WebView 包裹在 ScrolledWindow 中
- 支持自动显示滚动条,方便浏览长页面

## 🛠️ 技术栈

- **编程语言**: C/C++
- **GUI 框架**: GTK+ 3.0
- **网页引擎**: WebKit2GTK 4.0
- **构建工具**: CMake (>= 3.10.0)
- **可选依赖**: OpenCV (用于图像处理扩展)

## 📦 依赖安装

在 Ubuntu/Debian 系统上安装所需依赖:

```bash
sudo apt update
sudo apt install -y \
    build-essential \
    cmake \
    libgtk-3-dev \
    libwebkit2gtk-4.0-dev \
    pkg-config \
    libopencv-dev
```

## 🔧 编译与运行

### 编译步骤

```bash
# 创建构建目录
mkdir -p build
cd build

# 配置项目
cmake ..

# 编译
make
```

### 运行程序

```bash
./aaa
```

## 📁 项目结构

```
.
├── main.cpp          # 主程序源代码
├── CMakeLists.txt    # CMake 构建配置文件
├── Dockerfile        # Docker 配置文件 (待完善)
└── README.md         # 项目说明文档
```

## 💡 核心实现

### 导航策略处理

WebKit WebView 默认会拦截所有导航请求,包括超链接点击。项目通过以下方式解决:

```cpp
// 连接 decide-policy 信号
g_signal_connect(web_view, "decide-policy", G_CALLBACK(on_decide_policy), NULL);

// 处理导航决策
static gboolean on_decide_policy(WebKitWebView *web_view, 
                                  WebKitPolicyDecision *decision,
                                  WebKitPolicyDecisionType decision_type, 
                                  gpointer user_data) {
    if (decision_type == WEBKIT_POLICY_DECISION_TYPE_NAVIGATION_ACTION) {
        webkit_policy_decision_use(decision);  // 允许导航
        return TRUE;
    }
    return FALSE;
}
```

### 新窗口处理

对于尝试在新窗口打开的链接 (`target="_blank"`),会重定向到当前窗口:

```cpp
if (decision_type == WEBKIT_POLICY_DECISION_TYPE_NEW_WINDOW_ACTION) {
    // 获取 URL 并在当前窗口加载
    webkit_web_view_load_uri(web_view, uri);
    webkit_policy_decision_ignore(decision);
    return TRUE;
}
```

## 🐛 已知问题

1. **Dockerfile 配置不完整**: 当前 Dockerfile 仅包含 `From nginx`,无法正确部署 C++ 应用
2. **缺少高级功能**: 如书签管理、历史记录、多标签页等
3. **安全性**: 未实现 HTTPS 证书验证等安全机制

## 🚀 未来改进方向

- [ ] 添加前进/后退按钮
- [ ] 实现刷新和停止加载功能
- [ ] 支持多标签页浏览
- [ ] 添加书签管理
- [ ] 实现浏览历史记录
- [ ] 支持下载管理
- [ ] 添加开发者工具
- [ ] 优化 Docker 部署配置

## 📝 许可证

本项目仅供学习和参考使用。

## 🤝 贡献

欢迎提交 Issue 和 Pull Request!

---

**注意**: 这是一个基础的浏览器实现,适合学习 GTK3 和 WebKit2GTK 的开发。如需生产环境使用,建议使用成熟的浏览器方案。
