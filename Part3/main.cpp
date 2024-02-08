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
        SyntaxToken()=default;
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

        SyntaxToken Peek()
        {
            if(_pflag) return _ptoken;
            _ptoken=NextToken();
            _pflag= true;
            return _ptoken;
        }

        SyntaxToken NextToken()
        {
            if(_pflag)
            {
                _pflag= false;
                return _ptoken;
            }

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
        bool _pflag= false;
        SyntaxToken _ptoken;
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
    class Exp;

    class Factor
    {
    public:
        Factor()= default;
        Factor(string text)
        {
            _text=text;
        }
        string _text;
        int _val;
        Factor* _next= nullptr;
        bool _isdigit= false;
        Exp* _exp= nullptr;
    };

    class Term
    {
    public:
        SyntaxKind _kind;//* /
        Factor* _factor= nullptr;
        Term* _next= nullptr;
    };

    class Exp
    {
    public:
        SyntaxKind _kind;//+ -
        Term* _term= nullptr;
        Exp* _next= nullptr;
    };

    class Statement
    {
    public:
        Statement()= default;
        Statement(StatementType type, Exp* exp)
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

        Exp* parse_exp()
        {
            //<exp> ::= <term> { ("+" | "-") <term> }
            Exp* exp=new Exp();
            Term* term=parse_term();
            exp->_term=term;
            SyntaxToken p= _lexer->Peek();
            Exp* exp_iter=exp;
            while(p._kind==SyntaxKind::AddToken||p._kind==SyntaxKind::NegaToken)
            {
                _lexer->NextToken();
                Term* term1=parse_term();
                Exp* exp1=new Exp();
                exp1->_kind=p._kind;
                exp1->_term=term1;
                exp_iter->_next=exp1;
                exp_iter=exp_iter->_next;
                p=_lexer->Peek();
            }
            return exp;

        }

        Term* parse_term()
        {
            //<term> ::= <factor> { ("*" | "/") <factor> }
            Term* term=new Term();
            Factor* factor=parse_factor();
            term->_factor=factor;
            SyntaxToken p= _lexer->Peek();
            Term* term_iter=term;
            while(p._kind==SyntaxKind::MultiToken||p._kind==SyntaxKind::DivToken)
            {
                _lexer->NextToken();
                Factor* factor1=parse_factor();
                Term* term1=new Term();
                term1->_kind=p._kind;
                term1->_factor=factor1;
                term_iter->_next=term1;
                term_iter=term_iter->_next;
                p=_lexer->Peek();
            }
            return term;
        }

        //parse the factor
        Factor* parse_factor()
        {
            SyntaxToken p=_lexer->NextToken();

            if (p._kind==SyntaxKind::NumberToken)
            {
                //<factor> ::= <int>
                Factor* pFactor=new Factor(p._text);
                pFactor->_val= stoi(pFactor->_text);
                pFactor->_isdigit= true;
                return pFactor;
            }else if(mc::isUnaryOp(&p))
            {
                //<factor> ::= <unary_op> <factor>
                //get the unary operator
                Factor* pFactor=new Factor(p._text);
                Factor* inner_exp=parse_factor();
                pFactor->_next=inner_exp;
                return pFactor;
            }else if (p._kind==SyntaxKind::OpToken)
            {
                //<factor> ::= "(" <exp> ")"
                Exp* exp=parse_exp();
                p=_lexer->NextToken();
                if(p._kind!=SyntaxKind::CpToken)
                {
                    fail("factor error1");
                }

                Factor* factor=new Factor();
                factor->_exp=exp;
                return factor;
            }
            else
            {
                fail("factor error2");
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

//    <program> ::= <function>
//    <function> ::= "int" <id> "(" ")" "{" <statement> "}"
//    <statement> ::= "return" <exp> ";"
//    <exp> ::= <term> { ("+" | "-") <term> }
//    <term> ::= <factor> { ("*" | "/") <factor> }
//    <factor> ::= "(" <exp> ")" | <unary_op> <factor> | <int>
    void exp_out(Exp* exp, ofstream &fout);
    void term_out(Term* term,ofstream &fout);
    void factor_out(Factor* factor,ofstream &fout);

    void factor_out(Factor* factor,ofstream &fout)
    {
        if(factor== nullptr) return;
        //<factor> ::= "(" <exp> ")"
        if(factor->_exp!= nullptr)
            exp_out(factor->_exp,fout);
        else if(factor->_next!= nullptr)
            factor_out(factor->_next,fout);
        //<factor> ::=  <unary_op> <factor>
        if(factor->_text=="-")
            fout<<"neg     %eax"<<endl;
        else if(factor->_text=="~")
            fout<<"not     %eax"<<endl;
        else if(factor->_text=="!")
        {
            fout<<"cmpl    $0, %eax"<<endl;
            fout<<"movl    $0, %eax"<<endl;
            fout<<"sete    %al"<<endl;
        }
        //<factor> ::= <int>
        else if(factor->_isdigit)
            fout<<" movl    $"<<factor->_text<<","<<" %eax"<<endl;
        //
    }

    void term_out(Term* term,ofstream &fout)
    {
        if(term== nullptr) return;
        factor_out(term->_factor,fout);
        fout<<"    push %eax"<<endl;
        while (term->_next!= nullptr)
        {
            factor_out(term->_next->_factor,fout);
            fout<<"    pop %ecx"<<endl;
            if(term->_next->_kind==SyntaxKind::MultiToken)
                fout<<"    imul %ecx, %eax"<<endl;
            else if(term->_next->_kind==SyntaxKind::DivToken)
            {
                fout<<"    movl %ecx %edx"<<endl;
                fout<<"    movl %eax %ecx"<<endl;
                fout<<"    movl %edx %eax"<<endl;
                fout<<"    cdq"<<endl;
                fout<<"    idivl %ecx"<<endl;
            }
        }

    }
//    <exp> ::= <term> { ("+" | "-") <term> }
    void exp_out(Exp* exp, ofstream &fout)
    {
        if(exp== nullptr) return;
        term_out(exp->_term,fout);
        fout<<"    push %eax"<<endl;
        while (exp->_next!= nullptr)
        {
            term_out(exp->_next->_term,fout);
            fout<<"    pop %ecx"<<endl;
            if(exp->_next->_kind==SyntaxKind::AddToken)
                fout<<"    addl %ecx, %eax"<<endl;
            else if(exp->_next->_kind==SyntaxKind::NegaToken)
            {
                fout<<"subl %ecx, %eax"<<endl;
                fout<<"neg     %eax"<<endl;
            }
            exp=exp->_next;

        }



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

    void pretty_print(Program* program)
    {
        cout<<"program:"<<program->_function->_name<<endl;
        Exp* exp=program->_function->_statement->_exp;
        while(exp!=nullptr)
        {
//            exp= exp->_next;
//            cout << exp->_text<<endl;
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

//        cout<<name<<endl<<_val;
//        ofstream fout("return_2.s");
//        fout<<" .globl"<<" "<<name<<endl;
//        fout<<name<<":"<<endl;
//        fout<<" movl    $"<<val<<","<<" %eax"<<endl;
//        fout<<"ret";
//        fout.close();

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

//        cout<<text<<endl;

        mc::Lexer* lexer=new mc::Lexer(text);
        mc::Parser* parser=new mc::Parser(lexer);

        //parse the program
        mc::Program* program=parser->parse_program();

        //set the file name
        string Filename=filename;
        int length=Filename.length();
        Filename=Filename.substr(0,length-2);
        ofstream fout(Filename+".s");

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


