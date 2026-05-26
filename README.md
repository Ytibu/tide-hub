# tide-hub --- 基于tide框架开发的一系列实体项目

## 编译与运行

```bash
# 编译tide框架
cmake -S . -B build/ && cmake --build build/
# make
sudo make install

# 生成模板项目(会创新不同于本项目的模板项目)
# sh generate.sh chat_room chat

# 编译本项目
cmake -S . -B build/ && cmake --build build/
# 二进制依赖
sh move.sh
# 启动项目
./bin/chat_room -s
```
