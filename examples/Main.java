public class Main {
	static int x=5;

	int member;
	int printMember(){
		printInt(member);
		return member;
	}
	native static void println(String s);
	native static void printInt(int s);
	static int add(int a,int b){
		return a+b;
	}
	static int factorial(int n){
		if(n<=1)return 1;
		return factorial(n-1)*n;
	}
    public static void main(String[] args) {
		x=5;
		printInt(x);
		x=factorial(x);
		printInt(x);
		int a=0;
		println("Fibonacchi\n");
		int b=1;
		printInt(a);
		for(int i=0;i<8;i++){
			int temp;
			temp=b;
			b=b+a;
			a=temp;
			printInt(a);
		}
		printInt(b);
		println("5 + 10 = ");
		printInt(add(5,10));
		println("9! = ");
		printInt(factorial(9));
		Main main=new Main();
		main.member=10;
		main.printMember();
    }
}
