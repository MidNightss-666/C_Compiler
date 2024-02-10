//
// Created by LHX on 2024/2/10.
//

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
        //&&
        AndToken,
        // ||
        OrToken,
        //==
        EqualToken,
        //!=
        NequalToken,
        //<
        LthanToken,
        //<=
        LEthanToken,
        //>
        GthanToken,
        //>=
        GEthanToken,
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
        //Test Func
        int show_pos()
        {
            return _position;
        }

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
            else if (Current() == '!')
            {
                Next();
                if(Current()=='=')
                    return SyntaxToken(SyntaxKind::NequalToken,(_position++)-1,"!=");
                return SyntaxToken(SyntaxKind::LogiNegaToken, _position-1, "!");
            }
            else if (Current() == '+') return SyntaxToken(SyntaxKind::AddToken, _position++, "+");
            else if (Current() == '*') return SyntaxToken(SyntaxKind::MultiToken, _position++, "*");
            else if (Current() == '/') return SyntaxToken(SyntaxKind::DivToken, _position++, "/");
            else if (Current() == '&') {
                Next();
                if (Current() == '&') {
                    return SyntaxToken(SyntaxKind::AndToken, (_position++) - 1, "&&");
                }
            } else if (Current() == '|') {
                Next();
                if (Current() == '|') {
                    return SyntaxToken(SyntaxKind::OrToken, (_position++) - 1, "||");
                }
            } else if (Current() == '=') {
                Next();
                if (Current() == '=') {
                    return SyntaxToken(SyntaxKind::EqualToken, (_position++) - 1, "==");
                }
            }else if (Current() == '<') {
                Next();
                if (Current() == '=') {
                    return SyntaxToken(SyntaxKind::LEthanToken, (_position++) - 1, "<=");
                }
                return SyntaxToken(SyntaxKind::LthanToken, _position-1, "<");
            }else if (Current() == '>') {
                Next();
                if (Current() == '=') {
                    return SyntaxToken(SyntaxKind::GEthanToken, (_position++) - 1, ">=");
                }
                return SyntaxToken(SyntaxKind::GthanToken, _position-1, ">");
            }

            return SyntaxToken(SyntaxKind::BadToken,_position++,_text.substr(_position-1,1));
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
    class AddExp;

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
        AddExp* _exp= nullptr;
    };

    class Term
    {
    public:
        SyntaxKind _kind;//* /
        Factor* _factor= nullptr;
        Term* _next= nullptr;
    };

    class AddExp
    {
    public:
        SyntaxKind _kind;//+ -
        Term* _term= nullptr;
        AddExp* _next= nullptr;
    };

    class RelationalExp
    {
    public:
        SyntaxKind _kind;//"<" | ">" | "<=" | ">="
        AddExp* _addexp;
        RelationalExp* _next;
    };

    class EqualityExp
    {
    public:
        SyntaxKind _kind;// == !=
        RelationalExp* _relationalexp;
        EqualityExp* _next;
    };

    class LAndExp
    {
    public:
        SyntaxKind _kind;// &&
        EqualityExp* _equalityexp;
        LAndExp* _next;
    };

    class Exp
    {
    public:
        SyntaxKind _kind;// "||"
        LAndExp* _landexp;
        Exp* _next;
    };


    class Statement
    {
    public:
        Statement()= default;
        Statement(StatementType type, AddExp* exp)
        {
            _type=type;
            _exp=exp;
        }
        StatementType _type;
        AddExp* _exp;
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

        Exp* parse_Exp()
        {
            //<exp> ::= <logical-and-exp> { "||" <logical-and-exp> }
            Exp* exp=new Exp();
            LAndExp* lAndExp=parse_LAndExp();
            exp->_landexp=lAndExp;
            SyntaxToken p= _lexer->Peek();

            if(p._kind==SyntaxKind::BlankToken)
                p=_lexer->NextToken();
            p=_lexer->Peek();

            Exp* exp_iter=exp;
            while(p._kind==SyntaxKind::OrToken)
            {
                //another logical-and-exp

                _lexer->NextToken();
                LAndExp* lAndExp1=parse_LAndExp();
                Exp* exp1=new Exp();
                exp1->_kind=p._kind;
                exp1->_landexp=lAndExp;
                exp_iter->_next=exp1;
                exp_iter=exp_iter->_next;
                p=_lexer->Peek();

                if(p._kind==SyntaxKind::BlankToken)
                    p=_lexer->NextToken();
                p=_lexer->Peek();
            }
            return exp;
        }

        LAndExp* parse_LAndExp()
        {
            //<logical-and-exp> ::= <equality-exp> { "&&" <equality-exp> }
            LAndExp* lAndExp=new LAndExp();
            EqualityExp* equalityExp=parse_EqualityExp();
            lAndExp->_equalityexp=equalityExp;
            SyntaxToken p= _lexer->Peek();

            if(p._kind==SyntaxKind::BlankToken)
                p=_lexer->NextToken();
            p=_lexer->Peek();

            LAndExp* exp_iter=lAndExp;
            while(p._kind==SyntaxKind::AndToken)
            {
                //another equality-exp

                _lexer->NextToken();
                EqualityExp* equalityExp1=parse_EqualityExp();
                LAndExp* lAndExp1=new LAndExp();
                lAndExp1->_kind=p._kind;
                lAndExp1->_equalityexp=equalityExp1;
                exp_iter->_next=lAndExp1;
                exp_iter=exp_iter->_next;
                p=_lexer->Peek();

                if(p._kind==SyntaxKind::BlankToken)
                    p=_lexer->NextToken();
                p=_lexer->Peek();
            }
            return lAndExp;
        }

        EqualityExp* parse_EqualityExp()
        {
            //<equality-exp> ::= <relational-exp> { ("!=" | "==") <relational-exp> }
            EqualityExp* equalityExp=new EqualityExp();
            RelationalExp* relationalExp=parse_RelationalExp();
            equalityExp->_relationalexp=relationalExp;
            SyntaxToken p= _lexer->Peek();

            if(p._kind==SyntaxKind::BlankToken)
                p=_lexer->NextToken();
            p=_lexer->Peek();

            EqualityExp* exp_iter=equalityExp;
            while(p._kind==SyntaxKind::EqualToken||p._kind==SyntaxKind::NequalToken)
            {
                //another relational-exp

                _lexer->NextToken();
                RelationalExp* relationalExp1=parse_RelationalExp();
                EqualityExp* equalityExp1=new EqualityExp();
                equalityExp1->_kind=p._kind;
                equalityExp1->_relationalexp=relationalExp1;
                exp_iter->_next=equalityExp1;
                exp_iter=exp_iter->_next;
                p=_lexer->Peek();

                if(p._kind==SyntaxKind::BlankToken)
                    p=_lexer->NextToken();
                p=_lexer->Peek();
            }
            return equalityExp;



        }

        RelationalExp* parse_RelationalExp()
        {
            //<relational-exp> ::= <additive-exp> { ("<" | ">" | "<=" | ">=") <additive-exp> }
            RelationalExp* relationalExp=new RelationalExp();
            AddExp* addExp=parse_AddExp();
            relationalExp->_addexp=addExp;
            SyntaxToken p= _lexer->Peek();

            if(p._kind==SyntaxKind::BlankToken)
                p=_lexer->NextToken();
            p=_lexer->Peek();

            RelationalExp* exp_iter=relationalExp;
            while(p._kind==SyntaxKind::LthanToken||p._kind==SyntaxKind::GthanToken
                ||p._kind==SyntaxKind::LEthanToken||p._kind==SyntaxKind::GEthanToken)
            {
                //another additive-exp

                _lexer->NextToken();
                AddExp* addExp1=parse_AddExp();
                RelationalExp* relationalExp1=new RelationalExp();
                relationalExp1->_kind=p._kind;
                relationalExp1->_addexp=addExp1;
                exp_iter->_next=relationalExp1;
                exp_iter=exp_iter->_next;
                p=_lexer->Peek();

                if(p._kind==SyntaxKind::BlankToken)
                    p=_lexer->NextToken();
                p=_lexer->Peek();
            }
            return relationalExp;
        }

        AddExp* parse_AddExp()
        {
            //<exp> ::= <term> { ("+" | "-") <term> }
            AddExp* exp=new AddExp();
            Term* term=parse_term();
            exp->_term=term;
            SyntaxToken p= _lexer->Peek();

            if(p._kind==SyntaxKind::BlankToken)
                p=_lexer->NextToken();
            p=_lexer->Peek();

            AddExp* exp_iter=exp;
            while(p._kind==SyntaxKind::AddToken||p._kind==SyntaxKind::NegaToken)
            {
                _lexer->NextToken();
                Term* term1=parse_term();
                AddExp* exp1=new AddExp();
                exp1->_kind=p._kind;
                exp1->_term=term1;
                exp_iter->_next=exp1;
                exp_iter=exp_iter->_next;
                p=_lexer->Peek();

                if(p._kind==SyntaxKind::BlankToken)
                    p=_lexer->NextToken();
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

            if(p._kind==SyntaxKind::BlankToken)
                p=_lexer->NextToken();
            p=_lexer->Peek();

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

                if(p._kind==SyntaxKind::BlankToken)
                    p=_lexer->NextToken();
                p=_lexer->Peek();
            }
            return term;
        }

        //parse the factor
        Factor* parse_factor()
        {
            SyntaxToken p=_lexer->NextToken();
            if(p._kind==SyntaxKind::BlankToken)
                p=_lexer->NextToken();

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
                AddExp* exp=parse_AddExp();
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
                //test
                cout<<"pos:"<<_lexer->show_pos()<<" "<<p._position<<endl;
                cout<<"kind:"<<p._kind<<"text:"<<p._text;

                fail("factor error2");
            }
            return nullptr;

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

            AddExp* exp=parse_AddExp();

            p=_lexer->NextToken();
            if(p._kind==SyntaxKind::BlankToken)
                p=_lexer->NextToken();
//            //test
//            cout<<"pos:"<<_lexer->show_pos()<<" "<<p._position<<endl;
//            cout<<"kind:"<<p._kind<<"text:"<<p._text;

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
    void exp_out(AddExp* exp, ofstream &fout);
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

//    <term> ::= <factor> { ("*" | "/") <factor> }
    void term_out(Term* term,ofstream &fout)
    {
        if(term== nullptr) return;
        factor_out(term->_factor,fout);

        while (term->_next!= nullptr)
        {
            fout<<"    push %eax"<<endl;
            factor_out(term->_next->_factor,fout);
            fout<<"    pop %ecx"<<endl;
            if(term->_next->_kind==SyntaxKind::MultiToken)
                fout<<"    imul %ecx, %eax"<<endl;
            else if(term->_next->_kind==SyntaxKind::DivToken)
            {
                fout<<"    movl %ecx, %edx"<<endl;
                fout<<"    movl %eax, %ecx"<<endl;
                fout<<"    movl %edx, %eax"<<endl;
                fout<<"    cdq"<<endl;
                fout<<"    idivl %ecx"<<endl;
            }
            term=term->_next;
        }

    }
//    <exp> ::= <term> { ("+" | "-") <term> }
    void exp_out(AddExp* exp, ofstream &fout)
    {
        if(exp== nullptr) return;
        term_out(exp->_term,fout);

        while (exp->_next!= nullptr)
        {
            fout<<"    push %eax"<<endl;
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

        AddExp* exp=program->_function->_statement->_exp;
        exp_out(exp,fout);
        fout<<"ret";

        fout.close();
    }

    void pretty_print(Program* program)
    {
        cout<<"program:"<<program->_function->_name<<endl;
        AddExp* exp=program->_function->_statement->_exp;
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

        cout<<text<<endl;

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
//        system(command.c_str());
    }

    return 0;
}


