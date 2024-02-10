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
        //" "
        BlankToken,
        IdToken,
        //int
        IntToken,
        //return
        RetToken,
        //{
        OBraceToken,
        //}
        CBraceToken,
        //(
        OpToken,
        //)
        CpToken,
        //;
        SemiToken,
        //-
        NegaToken,
        //~
        BComplementToken,
        //!
        LogiNegaToken,
        //+
        AddToken,
        //*
        MultiToken,
        // /
        DivToken,
        //others
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
                return SyntaxToken(SyntaxKind::NumberToken,start,text);
            }
            if(isblank(Current()))
            {
                int start=_position;

                while(isblank(Current()))
                    Next();
                int length=_position-start;
                string text=_text.substr(start,length);
                return SyntaxToken(SyntaxKind::BlankToken,start,text);
            }
            if(isalpha(Current()))
            {
                int start=_position;

                while(isalnum(Current()))
                    Next();
                int length=_position-start;
                string text=_text.substr(start,length);
                if(text=="int")
                    return SyntaxToken(SyntaxKind::IntToken,start,text);
                if(text=="return")
                    return SyntaxToken(SyntaxKind::RetToken,start,text);
                else return SyntaxToken(SyntaxKind::IdToken,start,text);
            }
            if(Current()=='{') return SyntaxToken(SyntaxKind::OBraceToken,_position++,"{");
            else if(Current()=='}') return SyntaxToken(SyntaxKind::CBraceToken,_position++,"}");
            else if(Current()=='(') return SyntaxToken(SyntaxKind::OpToken,_position++,"(");
            else if(Current()==')') return SyntaxToken(SyntaxKind::CpToken,_position++,")");
            else if(Current()==';') return SyntaxToken(SyntaxKind::SemiToken,_position++,";");
            else if(Current()=='-') return SyntaxToken(SyntaxKind::NegaToken,_position++,"-");
            else if(Current()=='~') return SyntaxToken(SyntaxKind::BComplementToken,_position++,"~");
            else if(Current()=='!') return SyntaxToken(SyntaxKind::LogiNegaToken,_position++,"!");
            else if(Current()=='+') return SyntaxToken(SyntaxKind::AddToken,_position++,"+");
            else if(Current()=='*') return SyntaxToken(SyntaxKind::MultiToken,_position++,"*");
            else if(Current()=='/') return SyntaxToken(SyntaxKind::DivToken,_position++,"/");
            else return SyntaxToken(SyntaxKind::BadToken,_position++,_text.substr(_position-1,1));
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
        Exp()= default;
        Exp(string text)
        {
            _text=text;
        }
        string _text;
        int _val;
        Exp* _next= nullptr;
        bool _isdigit= false;
    };

    class Statement
    {
    public:
        Statement()= default;
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
        Function()= default;
        Function(Statement* statement,string name)
        {
            _statement=statement;
            _name=name;
        }
        Statement* _statement;
        //函数名称
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

    bool isUnaryOp(SyntaxToken* token)
    {
        SyntaxKind kind=token->_kind;
        return kind==SyntaxKind::LogiNegaToken
        ||kind==SyntaxKind::BComplementToken
        ||kind==SyntaxKind::NegaToken;
    }

    //语法分析器
    class Parser{
    public:
        Parser(Lexer* lexer)
        {
            _lexer=lexer;
        }

        //parse the expression
        Exp* parse_exp()
        {
            SyntaxToken p=_lexer->NextToken();

            if (p._kind==SyntaxKind::NumberToken)
            {
                Exp* exp=new Exp(p._text);
                exp->_val= stoi(exp->_text);
                exp->_isdigit= true;
                return exp;
            }else if(mc::isUnaryOp(&p))
            {
                //get the unary operator
                Exp* exp=new Exp(p._text);
                Exp* inner_exp=parse_exp();
                exp->_next=inner_exp;
                return exp;
            }else
            {
                fail("exp error1");
            }


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

    void exp_out(Exp* exp,ofstream &fout)
    {
        if(exp==NULL) return;
        exp_out(exp->_next,fout);
        if(exp->_text=="-")
            fout<<"neg     %eax"<<endl;
        else if(exp->_text=="~")
            fout<<"not     %eax"<<endl;
        else if(exp->_text=="!")
        {
            fout<<"cmpl    $0, %eax"<<endl;
            fout<<"movl    $0, %eax"<<endl;
            fout<<"sete    %al"<<endl;
        }
        else if(exp->_isdigit)
            fout<<" movl    $"<<exp->_text<<","<<" %eax"<<endl;
    }


    void aOut(Program* program,ofstream &fout)
    {
        string name=program->_function->_name;
        fout<<" .globl"<<" "<<name<<endl;
        fout<<name<<":"<<endl;

        Exp* exp=program->_function->_statement->_exp;
        exp_out(exp,fout);
        fout<<"ret";

        fout.close();
    }

    __attribute__((unused)) void pretty_print(Program* program)
    {
        cout<<"program:"<<program->_function->_name<<endl;
        Exp* exp=program->_function->_statement->_exp;
        while(exp!=NULL)
        {

            cout << exp->_text<<endl;
            exp= exp->_next;
        }


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

        string Filename=filename;
        int length=Filename.length();
        Filename=Filename.substr(0,length-2);
        ofstream fout(Filename+".s");
        //print

//        fout<<" .globl"<<" "<<name<<endl;
//        fout<<name<<":"<<endl;
//        fout<<" movl    $"<<val<<","<<" %eax"<<endl;
//        fout<<"ret";
//        fout.close();
//        mc::pretty_print(program);
        mc::aOut(program,fout);
        string command="gcc ";

        command.append(Filename+".s -o ").append(Filename);
        system(command.c_str());
    }

    return 0;
}


