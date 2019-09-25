# Makefile
# all:より下の3行のインデント、
# gtest-gen:より下の1行のインデント
# は半角空白になっていますがタブ1個にしてください
# target.cはCなのでgccで、それ以外はC++なのでg++でビルドします
# gtest_main.ccはGoogleTestが用意してくれているmain関数、
# gtest-all.ccがGoogleTest全部入りファイルです
# -lpthreadをつけることにも注意。
# makeかmake allしたらビルドして実行まで行います。
all:
			gcc -c ../../modules/ad05_01_module.c
			g++ ad05_01_test.cpp ../googletest-release-1.8.1/googletest/src/gtest_main.cc gtest/gtest-all.cc -I. -lpthread ad05_01_module.o
			./a.out
gtest-gen:
			python ../googletest-release-1.8.1/googletest/scripts/fuse_gtest_files.py ./
clean:
			rm -f ./a.out
			rm -f ./ad05_01_module.o
