#include "userprog/syscall.h"
#include <stdio.h>
#include <syscall-nr.h>
#include "threads/interrupt.h"
#include "threads/thread.h"
#include "threads/loader.h"
#include "userprog/gdt.h"
#include "threads/flags.h"
#include "intrinsic.h"

void syscall_entry (void);
void syscall_handler (struct intr_frame *);

/* System call.
 *
 * Previously system call services was handled by the interrupt handler
 * (e.g. int 0x80 in linux). However, in x86-64, the manufacturer supplies
 * efficient path for requesting the system call, the `syscall` instruction.
 *
 * The syscall instruction works by reading the values from the the Model
 * Specific Register (MSR). For the details, see the manual. */

#define MSR_STAR 0xc0000081         /* Segment selector msr */
#define MSR_LSTAR 0xc0000082        /* Long mode SYSCALL target */
#define MSR_SYSCALL_MASK 0xc0000084 /* Mask for the eflags */

void
syscall_init (void) {
	write_msr(MSR_STAR, ((uint64_t)SEL_UCSEG - 0x10) << 48  |
			((uint64_t)SEL_KCSEG) << 32);
	write_msr(MSR_LSTAR, (uint64_t) syscall_entry);

	/* The interrupt service rountine should not serve any interrupts
	 * until the syscall_entry swaps the userland stack to the kernel
	 * mode stack. Therefore, we masked the FLAG_FL. */
	write_msr(MSR_SYSCALL_MASK,
			FLAG_IF | FLAG_TF | FLAG_DF | FLAG_IOPL | FLAG_AC | FLAG_NT);
}

/* The main system call interface */
void
syscall_handler (struct intr_frame *f UNUSED) {
	// TODO: Your implementation goes here.
	printf ("system call!\n");
    /* 유저 스택에 저장되어 있는 시스템 콜 넘버를 이용해 시스템 콜
    핸들러 구현 */
    /* 스택 포인터가 유저 영역인지 확인 */
    /* 저장된 인자 값이 포인터일 경우 유저 영역의 주소인지 확인 */
    /* 0 : halt */
    /* 1 : exit */
    /*
        procedure syscall_handler (interrupt frame) {
            get stack pointer from interrupt frame
            get system call number from stack
            switch (system call number){
            case the number is halt:
            call halt function;
            break;
            case the number is exit:
            call exit function;
            break;
            …
            default
            call thread_exit function;
        }
    */
    // switch (f->R.rax)
    // {
    // case SYS_HALT:
    //     halt();
    //     break;
    // case SYS_EXIT:
    //     exit(f->R.rdi);
    //     break;
    // case SYS_FORK:
    //     f->R.rax = process_fork(f->R.rdi, f); // 인자 이렇게 맞았나?
    //     break;
    // case SYS_EXEC:
	// 	// writable = is_kernel_vaddr(f->R.rdi); //is_writable((uint64_t *)f->R.rdi);
	// 	// fn_copy = palloc_get_page(0);
	// 	// if (fn_copy == NULL)
	// 	// 	exit(-1);
	// 	// siz = strlen(f->R.rdi);
	// 	// strlcpy(fn_copy, f->R.rdi, siz); // Kernel panic; fn_copy는 kernel virtual addr라 write가 안되는건가?

	// 	if (exec(f->R.rdi) == -1)
	// 		exit(-1);
	// 	break;
	// case SYS_WAIT:
	// 	f->R.rax = process_wait(f->R.rdi);
	// 	break;
	// case SYS_CREATE:
	// 	f->R.rax = create(f->R.rdi, f->R.rsi);
	// 	break;
	// case SYS_REMOVE:
	// 	f->R.rax = remove(f->R.rdi);
	// 	break;
	// case SYS_OPEN:
	// 	f->R.rax = open(f->R.rdi);
	// 	break;
	// case SYS_FILESIZE:
	// 	f->R.rax = filesize(f->R.rdi);
	// 	break;
	// case SYS_READ:
	// 	f->R.rax = read(f->R.rdi, f->R.rsi, f->R.rdx);
	// 	break;
	// case SYS_WRITE:
	// 	f->R.rax = write(f->R.rdi, f->R.rsi, f->R.rdx);
	// 	break;
	// case SYS_SEEK:
	// 	seek(f->R.rdi, f->R.rsi);
	// 	break;
	// case SYS_TELL:
	// 	f->R.rax = tell(f->R.rdi);
	// 	break;
	// case SYS_CLOSE:
	// 	close(f->R.rdi);
	// 	break;
    // default:
    //     //thread_exit();
    //     exit(-1);
    //     break;
    // }
	thread_exit (); // 이거 주석처리하는거야?
}

// void check_address(void *uaddr){
//     /* 포인터가 가리키는 주소가 유저영역의 주소인지 확인 */
//     /* 잘못된 접근일 경우 프로세스 종료 */

//     /* 주소 값이 유저 영역에서 사용하는 주소 값인지 확인 하는 함수
//     Pintos에서는 시스템 콜이 접근할 수 있는 주소를 0x8048000~0xc0000000으로 제한함
//     유저 영역을 벗어난 영역일 경우 프로세스 종료(exit(-1)) */

//     struct thread *cur = thread_current();
//     // is_user_vaddr : 유저영역인지 확인하는 변수
//     // pm14_get_page : 
//     if (uaddr == NULL || !(is_user_vaddr(uaddr)) || pm14_get_page(cur->pm14, uaddr) == NULL)
//     {
//         exit(-1);
//     };
    
// }

// void get_argument(void *esp, int *arg, int count)
// {
//     /* 유저 스택에 저장된 인자값들을 커널로 저장 */
//     /* 인자가 저장된 위치가 유저영역인지 확인 */

//     /* 유저 스택에 있는 인자들을 커널에 저장하는 함수
//     스택 포인터(esp)에 count(인자의 개수) 만큼의 데이터를 arg에 저장 */

//     /* 유저 스택에 있는 인자들을 커널에 저장하는 함수
//     스택 포인터(esp)에 count(인자의 개수) 만큼의 데이터를 arg에 저장 */

//     /*
//     스택에서 인자들을 4byte 크기로 꺼내어 arg 배열에 순차적으로 저장
//     count 개수 만큼의 인자를 스택에서 가져옴
//     */

// }

// void halt(void)
// {
//     // pintos를 종료시키는 시스템 콜
//     /*
//     void shutdown_power_off(void) pintos를 종료시키는 함수
//      ㄴ 이거로 종료시키기 - 없는듯?
//     */
//     power_off();
// }

// void exit(int status)
// {
//     /*
//     현재 프로세스를 종료시키는 시스템 콜
//     종료 시 "프로세스 이름: exit(status)" 출력(Process Termination Message )
//     정상적으로 종료시 status 는 0
//     status: 프로그램이 정상적으로 종료됐는지 확인
//     */

//    /* 실행중인 스레드 구조체를 가져옴 */
//     /* 프로세스 종료 메시지 출력,
//     출력 양식: “프로세스이름: exit(종료상태)” */
//     /* 스레드 종료 */
//     struct thread *cur = thread_current();
//     cur->exit_status = status;

//     printf("%s: exit(%d)\n", thread_name(), status);
//     thread_exit();
// }

// bool create(const char *file, unsigned initial_size)
// {
//     // 파일 생성하는 시스템 콜
//     // 성공일 경우 true, 실패일 경우 false
//     // file : 생성할 파일의 이름 및 경로 정보
//     // initial_size : 생성할 파일의 크기

//     /* 파일 이름과 크기에 해당하는 파일 생성 */
//     /* 파일 생성 성공 시 true 반환, 실패 시 false 반환 */

//     check_address(file);
//     return filesys_create(file, initial_size);
// }

// bool remove(const char *file)
// {
//     // 파일을 삭제하는 시스템 콜
//     // file: 제거할 파일의 이름 및 경로 정보
//     // 성공 일 경우 true, 실패 일 경우 false return 
//     /* 파일 이름에 해당하는 파일을 제거 */
//     /* 파일 제거 성공 시 true 반환, 실패 시 false 반환 */

//     check_address(file);
//     return filesys_remove(file);
// }

/*

USER_STACK +----------------------------------+
           |             user stack           |
           |                 |                |
           |                 |                |
           |                 V                |
           |           grows downward         |
           |                                  |
           |                                  |
           |                                  |
           |                                  |
           |           grows upward           |
           |                 ^                |
           |                 |                |
           |                 |                |
           +----------------------------------+
           | uninitialized data segment (BSS) |
           +----------------------------------+
           |     initialized data segment     |
           +----------------------------------+
           |            code segment          |
 0x400000  +----------------------------------+
           |                                  |
           |                                  |
           |                                  |
           |                                  |
           |                                  |
       0   +----------------------------------+

*/