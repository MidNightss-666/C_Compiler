#include <iostream>
#include <string>
#include <cstring>
#include <istream>
#include <ostream>
#include <fstream>

using namespace std;


namespace mc
{
    enum SyntaxKind{
        NumberToken,
        BlankToken,
        IdToken,
        IntToken,
        RetToken,
        OBraceToken,
        CBraceToken,
        OpToken,
        CpToken,
        SemiToken,//;
        BadToken
    };

    class SyntaxToken{
    public:
        SyntaxToken(SyntaxKind kind,int position,string text=NULL)
        {
            _kind=kind;
            _position=position;
            _text=text;
        }
        SyntaxKind _kind;
        int _position;
        string _text;
    };

    //词法分析器
    class Lexer{
    public:
        Lexer(string text)
        {
            _text=text;
            _position=0;
        }

        void Next()
        {
            _position++;
        }

        SyntaxToken NextToken()
        {
            if(isdigit(Current()))
            {
                int start=_position;

                while(isdigit(Current()))
                    Next();
                int length=_position-start;
                string text=_text.substr(start,length);
                return SyntaxToken(NumberToken,start,text);
            }
            if(isblank(Current()))
            {
                int start=_position;

                while(isblank(Current()))
                    Next();
                int length=_position-start;
                string text=_text.substr(start,length);
                return SyntaxToken(BlankToken,start,text);
            }
            if(isalpha(Current()))
            {
                int start=_position;

                while(isalnum(Current()))
                    Next();
                int length=_position-start;
                string text=_text.substr(start,length);
                if(text=="int")
                    return SyntaxToken(IntToken,start,text);
                if(text=="return")
                    return SyntaxToken(RetToken,start,text);
                else return SyntaxToken(IdToken,start,text);
            }
            if(Current()=='{') return SyntaxToken(OBraceToken,_position++,"{");
            else if(Current()=='}') return SyntaxToken(CBraceToken,_position++,"}");
            else if(Current()=='(') return SyntaxToken(OpToken,_position++,"(");
            else if(Current()==')') return SyntaxToken(CpToken,_position++,")");
            else if(Current()==';') return SyntaxToken(SemiToken,_position++,";");
            else return SyntaxToken(BadToken,_position++,_text.substr(_position-1,1));
        }
    private:
        string _text;
        int _position;
        char Current()
        {
            if(_position>=_text.length())
                return '\0';
            return _text[_position];
        }
    };

    enum StatementType{
        Return
    };

    class Exp
    {
    public:
        Exp(string text)
        {
            _text=text;
        }
        string _text;
        int _val;
    };

    class Statement
    {
    public:
        Statement(){}
        Statement(StatementType type,Exp* exp)
        {
            _type=type;
            _exp=exp;
        }
        StatementType _type;
        Exp* _exp;
    };

    class Function
    {
    public:
        Function(){}
        Function(Statement* statement,string name)
        {
            _statement=statement;
            _name=name;
        }
        Statement* _statement;
        string _name;
    };

    class Program
    {
    public:
        Program(Function* function)
        {
            _function=function;
        }
        Function* _function;
    };

    //语法分析器
    class Parser{
    public:
        Parser(Lexer* lexer)
        {
            _lexer=lexer;
        }

        //parse the number
        Exp* parse_exp()
        {
            SyntaxToken p=_lexer->NextToken();
            if (p._kind!=SyntaxKind::NumberToken)
                fail("exp error1");

            Exp* exp=new Exp(p._text);
            return exp;
        }

        //parse the statement
        Statement* parse_statement()
        {
            SyntaxToken p=_lexer->NextToken();

            if(p._kind==SyntaxKind::BlankToken)
                p=_lexer->NextToken();

            if(p._kind!=SyntaxKind::RetToken) fail("statement error1");
            p=_lexer->NextToken();
            if(p._kind!=SyntaxKind::BlankToken) fail("statement error2");
            Exp* exp=parse_exp();
            p=_lexer->NextToken();
            if(p._kind==SyntaxKind::BlankToken)
                p=_lexer->NextToken();
            if(p._kind!=SyntaxKind::SemiToken) fail("statement error3");

            Statement* statement=new Statement(StatementType::Return,exp);
            return statement;
        }

        //parse the function
        Function* parse_function()
        {
            //int main() {    return 2;}
            SyntaxToken p=_lexer->NextToken();
            if(p._kind==SyntaxKind::BlankToken)
                p=_lexer->NextToken();
            if(p._kind!=SyntaxKind::IntToken) fail("function error1");
            p=_lexer->NextToken();
            if(p._kind!=SyntaxKind::BlankToken) fail("function error2");
            p=_lexer->NextToken();
            if(p._kind!=SyntaxKind::IdToken) fail("function error3");

            //function name
            string name=p._text;

            p=_lexer->NextToken();
            if(p._kind==SyntaxKind::BlankToken)
                p=_lexer->NextToken();

            if(p._kind!=SyntaxKind::OpToken) fail("function error4");
            p=_lexer->NextToken();

//            cout<<"TYPE:"<<p._kind<<endl;
            if(p._kind==SyntaxKind::BlankToken)
                p=_lexer->NextToken();

            if(p._kind!=SyntaxKind::CpToken) fail("function error5");
            p=_lexer->NextToken();

            if(p._kind==SyntaxKind::BlankToken)
                p=_lexer->NextToken();
            if(p._kind!=SyntaxKind::OBraceToken) fail("function error6");


            Statement* statement=parse_statement();

            p=_lexer->NextToken();
            if(p._kind==SyntaxKind::BlankToken)
                p=_lexer->NextToken();
            if(p._kind!=SyntaxKind::CBraceToken) fail("function error7");

            Function* function=new Function(statement,name);


            return function;
        }

        //parse the program
        Program* parse_program()
        {
            Function* function=parse_function();
            Program* program=new Program(function);

            return program;
        }

    private:
        Lexer* _lexer;
        static void fail(const char* text)
        {
            fprintf(stderr,text);
            exit(EXIT_FAILURE);
        }

    };

    __attribute__((unused)) void pretty_print()
    {

    }
}



int main(int argc,char* argv[]) {
    int test=0;
    if(test)
    {
        ifstream fin;
        fin.open(R"(F:\C_Compiler\Part1\return_2.c)",ios::in);
        if(!fin.is_open())
        {
            cout<<"failed when opening file";
            exit(EXIT_FAILURE);
        }
        string buff;
        string text="";
        while(getline(fin,buff))
        {
            text.append(buff);
        }
        fin.close();

        mc::Lexer* lexer=new mc::Lexer(text);
        mc::Parser* parser=new mc::Parser(lexer);

        //get info of the program
        mc::Program* program=parser->parse_program();

        string name=program->_function->_name;
        string val=program->_function->_statement->_exp->_text;

//        cout<<name<<endl<<_val;
        ofstream fout("return_2.s");
        fout<<" .globl"<<" "<<name<<endl;
        fout<<name<<":"<<endl;
        fout<<" movl    $"<<val<<","<<" %eax"<<endl;
        fout<<"ret";
        fout.close();

    }else
    {
        if(argc<2)
        {
            cout<<"invalid usage"<<endl;
            return 1;
        }
        const char* filename=argv[1];
        ifstream fin;
        fin.open(filename,ios::in);
        if(!fin.is_open())
        {
            cout<<"failed when opening file";
            exit(EXIT_FAILURE);
        }
        string buff;
        string text="";
        while(getline(fin,buff))
        {
            text.append(buff+" ");
        }
        fin.close();

        cout<<text<<endl;

        mc::Lexer* lexer=new mc::Lexer(text);
        mc::Parser* parser=new mc::Parser(lexer);

        //get info of the program
        mc::Program* program=parser->parse_program();

        string name=program->_function->_name;
        string val=program->_function->_statement->_exp->_text;

//        ofstream fout("return_2.s");
//        fout<<" .globl"<<" "<<"_"<<name<<endl;
//        fout<<"_"<<name<<":"<<endl;
//        fout<<" movl    $"<<_val<<","<<" %eax"<<endl;
//        fout<<"ret";
//        fout.close();

        string Filename=filename;
        int length=Filename.length();
        Filename=Filename.substr(0,length-2);
//        cout<<Filename;
        ofstream fout(Filename+".s");
        fout<<" .globl"<<" "<<name<<endl;
        fout<<name<<":"<<endl;
        fout<<" movl    $"<<val<<","<<" %eax"<<endl;
        fout<<"ret";
        fout.close();
        string command="gcc ";

        command.append(Filename+".s -o ").append(Filename);
        system(command.c_str());
    }

    return 0;
}


