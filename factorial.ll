; ModuleID = 'SimpleLang'
source_filename = "SimpleLang"

define i32 @fibonacci(i32 %n) {
entry:
  %n1 = alloca i32, align 4
  store i32 %n, ptr %n1, align 4
  %n2 = load i32, ptr %n1, align 4
  %cmptmp = icmp sle i32 %n2, 1
  br i1 %cmptmp, label %then, label %else

then:                                             ; preds = %entry
  %n3 = load i32, ptr %n1, align 4
  ret i32 %n3

else:                                             ; preds = %entry
  %n4 = load i32, ptr %n1, align 4
  %subtmp = sub i32 %n4, 1
  %calltmp = call i32 @fibonacci(i32 %subtmp)
  %n5 = load i32, ptr %n1, align 4
  %subtmp6 = sub i32 %n5, 2
  %calltmp7 = call i32 @fibonacci(i32 %subtmp6)
  %addtmp = add i32 %calltmp, %calltmp7
  ret i32 %addtmp

ifcont:                                           ; No predecessors!
  ret i32 0
}

define i32 @main() {
entry:
  %calltmp = call i32 @fibonacci(i32 10)
  ret i32 %calltmp
}
