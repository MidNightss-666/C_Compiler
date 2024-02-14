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
        //unset
        Unset,
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

        ~Factor()
        {
            delete _exp;
            delete _next;
        }
    };

    class Term
    {
    public:
        SyntaxKind _kind;//* /
        Factor* _factor= nullptr;
        Term* _next= nullptr;

        ~Term()
        {
            delete _factor;
            delete _next;
        }
    };

    class AddExp
    {
    public:
        SyntaxKind _kind=SyntaxKind::Unset;//+ -
        Term* _term= nullptr;
        AddExp* _next= nullptr;

        ~AddExp()
        {
            delete _term;
            delete _next;
        }
    };

    class RelationalExp
    {
    public:
        SyntaxKind _kind=SyntaxKind::Unset;//"<" | ">" | "<=" | ">="
        AddExp* _addexp= nullptr;
        RelationalExp* _next= nullptr;

        ~RelationalExp()
        {
            delete _addexp;
            delete _next;
        }
    };

    class EqualityExp
    {
    public:
        SyntaxKind _kind=SyntaxKind::Unset;// == !=
        RelationalExp* _relationalexp= nullptr;
        EqualityExp* _next= nullptr;

        ~EqualityExp()
        {
            delete _relationalexp;
            delete _next;
        }
    };

    class LAndExp
    {
    public:
        SyntaxKind _kind=SyntaxKind::Unset;// &&
        EqualityExp* _equalityexp= nullptr;
        LAndExp* _next= nullptr;

        ~LAndExp()
        {
            delete _equalityexp;
            delete _next;
        }
    };

    class Exp
    {
    public:
        SyntaxKind _kind=SyntaxKind::Unset;// "||"
        LAndExp* _landexp= nullptr;
        Exp* _next= nullptr;

        ~Exp()
        {
            delete _landexp;
            delete _next;
        }
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
        Exp* _exp= nullptr;

        ~Statement()
        {
            delete _exp;
        }
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
        Statement* _statement= nullptr;
        //函数名称
        string _name;
        ~Function()
        {
            delete _statement;
        }
    };

    class Program
    {
    public:
        Program(Function* function)
        {
            _function=function;
        }
        Function* _function= nullptr;

        ~Program()
        {

            delete _function;
        }
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
                Exp* exp=parse_Exp();
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

            Exp* exp=parse_Exp();

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

    string clause_counter()
    {
        static int cnt=0;
        string ret="_clause";
        ret.append(to_string(cnt));
        cnt++;
        return ret;
    }

    string return_counter()
    {
        static int cnt=0;
        string ret="_return";
        ret.append(to_string(cnt));
        cnt++;
        return ret;
    }


//<program> ::= <function>
//<function> ::= "int" <id> "(" ")" "{" <statement> "}"
//<statement> ::= "return" <exp> ";"
//<exp> ::= <logical-and-exp> { "||" <logical-and-exp> }
//<logical-and-exp> ::= <equality-exp> { "&&" <equality-exp> }
//<equality-exp> ::= <relational-exp> { ("!=" | "==") <relational-exp> }
//<relational-exp> ::= <additive-exp> { ("<" | ">" | "<=" | ">=") <additive-exp> }
//<additive-exp> ::= <term> { ("+" | "-") <term> }
//<term> ::= <factor> { ("*" | "/") <factor> }
//<factor> ::= "(" <exp> ")" | <unary_op> <factor> | <int>
//<unary_op> ::= "!" | "~" | "-"

    void exp_out(Exp* exp, ofstream &fout);
    void landexp_out(LAndExp* exp, ofstream &fout);
    void equalityexp_out(EqualityExp* exp, ofstream &fout);
    void relationalexp_out(RelationalExp* exp, ofstream &fout);
    void addexp_out(AddExp* addExp, ofstream &fout);
    void term_out(Term* term,ofstream &fout);
    void factor_out(Factor* factor,ofstream &fout);

//<exp> ::= <logical-and-exp> { "||" <logical-and-exp> }
    void exp_out(Exp* exp, ofstream &fout)
    {
        if (exp== nullptr) return;
        landexp_out(exp->_landexp,fout);

        string return_flag=return_counter();
        if (exp->_next!= nullptr)
        {
            while (exp->_next!= nullptr)
            {
                fout<<"    cmpl $0, %eax"<<endl; //cmp e1 and false

                string clause_flag=clause_counter();


                fout<<"    je "<<clause_flag<<endl; //jump if e1 equals false
                fout<<"    movl $1, %eax"<<endl; //else set ret true
                fout<<"    jmp "<<return_flag<<endl;//jump to _end
                fout<<clause_flag<<":"<<endl;
                landexp_out(exp->_next->_landexp,fout);//next bool expression
                exp=exp->_next;
            }
            //last bool expression finished, and don't jump to the end

            fout<<"    cmpl $0, %eax"<<endl;//test the ans of the last bool expression
            fout<<"    movl $0, %eax"<<endl;
            fout<<"    setne %al"<<endl;//set %eax true if not equal to false (of course!)
            fout<<return_flag<<":"<<endl;
        }


    }

//<logical-and-exp> ::= <equality-exp> { "&&" <equality-exp> }
    void landexp_out(LAndExp* exp, ofstream &fout)
    {
        if (exp== nullptr) return;
        equalityexp_out(exp->_equalityexp,fout);

        string return_flag=return_counter();
        if (exp->_next!= nullptr)
        {
            while (exp->_next!= nullptr)
            {
                fout<<"    cmpl $0, %eax"<<endl;

                string clause_flag=clause_counter();

                fout<<"    jne "<<clause_flag<<endl; //jump to the next expression e2 if e1 is true
                fout<<"    jmp "<<return_flag<<endl;//otherwise jump to the end
                fout<<clause_flag<<":"<<endl;
                equalityexp_out(exp->_next->_equalityexp,fout);//next bool expression
                exp=exp->_next;
            }
            //last bool expression finished, and don't jump to the end yet

            fout<<"    cmpl $0, %eax"<<endl;//test the ans of the last bool expression
            fout<<"    movl $0, %eax"<<endl;
            fout<<"    setne %al"<<endl;//set %eax true if not equal to false
            fout<<return_flag<<":"<<endl;

        }

    }

//<equality-exp> ::= <relational-exp> { ("!=" | "==") <relational-exp> }
    void equalityexp_out(EqualityExp* exp, ofstream &fout)
    {
        if (exp== nullptr) return;
        relationalexp_out(exp->_relationalexp,fout);
        while(exp->_next!= nullptr)
        {
            fout<<"    push %eax"<<endl;
            relationalexp_out(exp->_next->_relationalexp,fout);
            fout<<"    pop %ecx"<<endl;
            if(exp->_next->_kind==SyntaxKind::EqualToken)
            {
                fout<<"     cmpl   %eax, %ecx"<<endl;
                fout<<"     movl   $0, %eax"<<endl;
                fout<<"     sete   %al"<<endl;
            }else if (exp->_next->_kind==SyntaxKind::NequalToken)
            {
                fout<<"     cmpl   %eax, %ecx"<<endl;
                fout<<"     movl   $0, %eax"<<endl;
                fout<<"     setne   %al"<<endl;
            }

            exp=exp->_next;
        }

    }

//<relational-exp> ::= <additive-exp> { ("<" | ">" | "<=" | ">=") <additive-exp> }
    void relationalexp_out(RelationalExp* exp, ofstream &fout)
    {
        if (exp== nullptr) return;
        addexp_out(exp->_addexp,fout);
        while(exp->_next!= nullptr)
        {
            fout<<"    push %eax"<<endl;
            addexp_out(exp->_next->_addexp,fout);
            fout<<"    pop %ecx"<<endl;
            if(exp->_next->_kind==SyntaxKind::LthanToken)
            {
                //"<"
                fout<<"     cmpl   %eax, %ecx"<<endl;
                fout<<"     movl   $0, %eax"<<endl;
                fout<<"     setl   %al"<<endl;
            }else if (exp->_next->_kind==SyntaxKind::LEthanToken)
            {
                //"<="
                fout<<"     cmpl   %eax, %ecx"<<endl;
                fout<<"     movl   $0, %eax"<<endl;
                fout<<"     setle   %al"<<endl;
            }else if (exp->_next->_kind==SyntaxKind::GthanToken)
            {
                //">"
                fout<<"     cmpl   %eax, %ecx"<<endl;
                fout<<"     movl   $0, %eax"<<endl;
                fout<<"     setg   %al"<<endl;
            }else if (exp->_next->_kind==SyntaxKind::GEthanToken)
            {
                //">="
                fout<<"     cmpl   %eax, %ecx"<<endl;
                fout<<"     movl   $0, %eax"<<endl;
                fout<<"     setge   %al"<<endl;
            }

            exp=exp->_next;
        }

    }

//<factor> ::= "(" <exp> ")" | <unary_op> <factor> | <int>
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
//    <additive-exp> ::= <term> { ("+" | "-") <term> }
    void addexp_out(AddExp* addExp, ofstream &fout)
    {
        if(addExp== nullptr) return;
        term_out(addExp->_term,fout);

        while (addExp->_next!= nullptr)
        {
            fout<<"    push %eax"<<endl;
            term_out(addExp->_next->_term,fout);
            fout<<"    pop %ecx"<<endl;
            if(addExp->_next->_kind==SyntaxKind::AddToken)
                fout<<"    addl %ecx, %eax"<<endl;
            else if(addExp->_next->_kind==SyntaxKind::NegaToken)
            {
                fout<<"subl %ecx, %eax"<<endl;
                fout<<"neg     %eax"<<endl;
            }
            addExp=addExp->_next;

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

    __attribute__((unused)) void pretty_print(Program* program)
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

    mc::aOut(program,fout);
    string command="gcc ";

    command.append(Filename+".s -o ").append(Filename);
//        system(command.c_str());

    delete program;
    return 0;
}


