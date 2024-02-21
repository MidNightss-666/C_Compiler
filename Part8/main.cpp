//
// Created by LHX on 2024/2/10.
//

#include <iostream>
#include <string>
#include <istream>
#include <fstream>
#include <list>
#include <map>
#include <unordered_map>
#include <set>

using namespace std;


namespace mc
{
    class Lexer;

    void fail(const char* text,Lexer* lexer= nullptr)
    {
        fprintf(stderr,text);
        cout<<"\n";
        exit(EXIT_FAILURE);
    }
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
        //=
        AssignmentToken,
        //if
        IfToken,
        //else
        ElseToken,
        //:
        ColonToken,
        //?
        QToken,
        //for
        ForToken,
        //while
        WhileToken,
        //do
        DoToken,
        //break
        BreakToken,
        //continue
        ContinueToken,
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

        void Setposition(int p)
        {
            _position=p;
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
                if(text=="if")
                    return SyntaxToken(SyntaxKind::IfToken,start,text);
                if(text=="else")
                    return SyntaxToken(SyntaxKind::ElseToken,start,text);
                if(text=="for")
                    return SyntaxToken(SyntaxKind::ForToken,start,text);
                if(text=="while")
                    return SyntaxToken(SyntaxKind::WhileToken,start,text);
                if(text=="do")
                    return SyntaxToken(SyntaxKind::DoToken,start,text);
                if(text=="break")
                    return SyntaxToken(SyntaxKind::BreakToken,start,text);
                if(text=="continue")
                    return SyntaxToken(SyntaxKind::ContinueToken,start,text);
                else return SyntaxToken(SyntaxKind::IdToken,start,text);
            }
            if(Current()=='{') return SyntaxToken(SyntaxKind::OBraceToken,_position++,"{");
            else if(Current()=='}') return SyntaxToken(SyntaxKind::CBraceToken,_position++,"}");
            else if(Current()=='(') return SyntaxToken(SyntaxKind::OpToken,_position++,"(");
            else if(Current()==')') return SyntaxToken(SyntaxKind::CpToken,_position++,")");
            else if(Current()==';') return SyntaxToken(SyntaxKind::SemiToken,_position++,";");
            else if(Current()=='-') return SyntaxToken(SyntaxKind::NegaToken,_position++,"-");
            else if(Current()=='~') return SyntaxToken(SyntaxKind::BComplementToken,_position++,"~");
            else if(Current()==':') return SyntaxToken(SyntaxKind::ColonToken,_position++,":");
            else if(Current()=='?') return SyntaxToken(SyntaxKind::QToken,_position++,"?");
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
                return SyntaxToken(SyntaxKind::AssignmentToken,_position-1,"=");
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

    enum FactorKind
    {
        ExpFactor,UnaryFactor,NumberFactor,IdFactor
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

        FactorKind _kind;

//        ~Factor()
//        {
//            delete _exp;
//            delete _next;
//        }
    };

    class Term
    {
    public:
        SyntaxKind _kind;//* /
        Factor* _factor= nullptr;
        Term* _next= nullptr;

//        ~Term()
//        {
//            delete _factor;
//            delete _next;
//        }
    };

    class AddExp
    {
    public:
        SyntaxKind _kind=SyntaxKind::Unset;//+ -
        Term* _term= nullptr;
        AddExp* _next= nullptr;

//        ~AddExp()
//        {
//            delete _term;
//            delete _next;
//        }
    };

    class RelationalExp
    {
    public:
        SyntaxKind _kind=SyntaxKind::Unset;//"<" | ">" | "<=" | ">="
        AddExp* _addexp= nullptr;
        RelationalExp* _next= nullptr;

//        ~RelationalExp()
//        {
//            delete _addexp;
//            delete _next;
//        }
    };

    class EqualityExp
    {
    public:
        SyntaxKind _kind=SyntaxKind::Unset;// == !=
        RelationalExp* _relationalexp= nullptr;
        EqualityExp* _next= nullptr;

//        ~EqualityExp()
//        {
//            delete _relationalexp;
//            delete _next;
//        }
    };

    class LAndExp
    {
    public:
        SyntaxKind _kind=SyntaxKind::Unset;// &&
        EqualityExp* _equalityexp= nullptr;
        LAndExp* _next= nullptr;

//        ~LAndExp()
//        {
//            delete _equalityexp;
//            delete _next;
//        }
    };

    class LOrExp
    {
    public:
        SyntaxKind _kind=SyntaxKind::Unset;// "||"
        LAndExp* _landexp= nullptr;
        LOrExp* _next= nullptr;

//        ~LOrExp()
//        {
//            delete _landexp;
//            delete _next;
//        }
    };

    enum ExpKind
    {
        AssignmentExp,ConditonalExp,
    };

    class ConditionExp;

    class Exp
    {
    public:
        string _id;
        Exp* _next= nullptr;
        ExpKind _kind;
        ConditionExp* _conditionExp;
    };

    enum ExpOptionKind
    {
        EmptyExpOption,NoEmpty
    };

    class ExpOption
    {
    public:
        ExpOptionKind _kind;
        Exp* _exp;
    };

    class ConditionExp
    {
    public:
        LOrExp* _lorExp= nullptr;
        Exp* _exp= nullptr;
        ConditionExp* _next= nullptr;
    };

    class Declaration
    {
    public:
        string _id;
        Exp* _exp= nullptr;
    };

    enum StatementType{
        ReturnType,ExpType,ConditionType,CompoundType,For_1,For_2,WhileStatement,DoWhileStatement,
        BreakStatement,ContinueStatement
    };

    class BlockItem;

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
        Statement* _statement1= nullptr;
        Statement* _statement2= nullptr;
        string _id;
        list<BlockItem*> _list;

        ExpOption* _initExp;
        ExpOption* _controlExp;
        ExpOption* _postExp;
        Declaration* _declaration;
        Statement* _loopStatement;
        ExpOption* _expOption;
    };

    enum BlockType
    {
        StatementBlock,DeclarationBlock
    };

    class BlockItem
    {
    public:
        BlockType _type;
        Statement* _statement= nullptr;
        Declaration* _declaration= nullptr;
    };

    class Function
    {
    public:

        list<BlockItem*>_list;
        //func name
        string _name;

    };

    class Program
    {
    public:
        Program(Function* function)
        {
            _function=function;
        }
        Function* _function= nullptr;

//        ~Program()
//        {
//
//            delete _function;
//        }
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
            //<exp> ::= <id> "=" <exp> | <conditional-exp>

            SyntaxToken p=_lexer->Peek();
            if(p._kind==SyntaxKind::BlankToken)
            {
                _lexer->NextToken();
                p=_lexer->Peek();
            }
            Exp* exp=new Exp();
            if(p._kind==SyntaxKind::IdToken)
            {
                int position=p._position;
                string NAME=p._text;
                _lexer->NextToken();
                p=_lexer->NextToken();
                if(p._kind==SyntaxKind::BlankToken)
                    p=_lexer->NextToken();
                if (p._kind==SyntaxKind::AssignmentToken)
                {
                    cout<<"   1   "<<endl;
                    Exp* next_exp=parse_Exp();
                    exp->_kind=ExpKind::AssignmentExp;
                    exp->_next=next_exp;
                    exp->_id=NAME;
                    //test
                    cout<<"   2   "<<endl;
                }else
                {
                    cout<<"   3   "<<endl;
                    _lexer->Setposition(position );
                    exp->_conditionExp=parse_conditionExp();
                    cout<<"   4   "<<endl;
                    exp->_kind=ExpKind::ConditonalExp;
                }
            }else
            {
                //test
                cout<<"pos:"<<_lexer->show_pos()<<" "<<p._position<<endl;
                cout<<"kind:"<<p._kind<<" "<<"text:"<<p._text<<endl;


                exp->_kind=ExpKind::ConditonalExp;
                exp->_conditionExp=parse_conditionExp();
            }

            return exp;
        }

        ExpOption* parse_ExpOption()
        {
            //<exp-option> ::= <exp> | ""
            ExpOption* ret=new ExpOption();
            SyntaxToken p=_lexer->Peek();
            if (p._kind==SyntaxKind::BlankToken)
            {
                _lexer->NextToken();
                p=_lexer->Peek();
            }
            if (p._kind==SyntaxKind::SemiToken
                ||p._kind==SyntaxKind::CpToken)
            {
                ret->_kind=ExpOptionKind::EmptyExpOption;
            }else
            {
                ret->_kind=ExpOptionKind::NoEmpty;
                ret->_exp=parse_Exp();
            }
            return ret;
        }

        ConditionExp* parse_conditionExp()
        {
            //<conditional-exp> ::= <logical-or-exp> [ "?" <exp> ":" <conditional-exp> ]
            ConditionExp* ret=new ConditionExp();
            ret->_lorExp=parse_LOrExp();
            //test
            cout<<"condition test"<<endl;

            SyntaxToken p=_lexer->Peek();
            if(p._kind==SyntaxKind::BlankToken)
            {
                _lexer->NextToken();
                p=_lexer->Peek();
            }
            if(p._kind==SyntaxKind::QToken)
            {
                //exist conditional expression
                _lexer->NextToken();
                ret->_exp=parse_Exp();

                p=_lexer->NextToken();
                if(p._kind==SyntaxKind::BlankToken)
                    p=_lexer->NextToken();
                if(p._kind!=SyntaxKind::ColonToken) fail("condition error");
                ret->_next=parse_conditionExp();

            }
            return ret;
        }

        LOrExp* parse_LOrExp()
        {
            //<exp> ::= <logical-and-exp> { "||" <logical-and-exp> }
            LOrExp* exp=new LOrExp();
            LAndExp* lAndExp=parse_LAndExp();
            exp->_landexp=lAndExp;
            SyntaxToken p= _lexer->Peek();

            if(p._kind==SyntaxKind::BlankToken)
                p=_lexer->NextToken();
            p=_lexer->Peek();

            LOrExp* exp_iter=exp;
            while(p._kind==SyntaxKind::OrToken)
            {
                //another logical-and-exp

                _lexer->NextToken();
                LAndExp* lAndExp1=parse_LAndExp();
                LOrExp* exp1=new LOrExp();
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
                pFactor->_kind=FactorKind::NumberFactor;
                return pFactor;
            }else if(mc::isUnaryOp(&p))
            {
                //<factor> ::= <unary_op> <factor>
                //get the unary operator
                Factor* pFactor=new Factor(p._text);
                Factor* inner_exp=parse_factor();
                pFactor->_next=inner_exp;
                pFactor->_kind=FactorKind::UnaryFactor;
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
                factor->_kind=FactorKind::ExpFactor;
                return factor;
            }else if (p._kind==SyntaxKind::IdToken)
            {
                //<factor> ::= <id>
                Factor* factor=new Factor();
                factor->_text=p._text;
                factor->_kind=FactorKind::IdFactor;
                return factor;
            }
            else
            {
                //test
                cout<<"pos:"<<_lexer->show_pos()<<" "<<p._position<<endl;
                cout<<"kind:"<<p._kind<<" "<<"text:"<<p._text<<endl;

                fail("factor error2");
            }
            return nullptr;

        }

        //parse the statement
        Statement* parse_statement()
        {
//            <statement> ::= "return" <exp> ";"
//                          | <exp-option> ";"
//                          | "if" "(" <exp> ")" <statement> [ "else" <statement> ]
//                          | "{" { <block-item> } "}
//                          | "for" "(" <exp-option> ";" <exp-option> ";" <exp-option> ")" <statement>
//                          | "for" "(" <declaration> <exp-option> ";" <exp-option> ")" <statement>
//                          | "while" "(" <exp> ")" <statement>
//                          | "do" <statement> "while" "(" <exp> ")" ";"
//                          | "break" ";"
//                          | "continue" ";"
            SyntaxToken p=_lexer->Peek();

            if(p._kind==SyntaxKind::BlankToken)
            {
                _lexer->NextToken();
                p=_lexer->Peek();
            }


            if (p._kind==SyntaxKind::RetToken)
            {
                //<statement> ::= "return" <exp> ";"
                _lexer->NextToken();
                p=_lexer->NextToken();
                if(p._kind!=SyntaxKind::BlankToken) fail("statement error1");
                Exp* exp=parse_Exp();

                p=_lexer->NextToken();
                if(p._kind==SyntaxKind::BlankToken)
                    p=_lexer->NextToken();

                if(p._kind!=SyntaxKind::SemiToken) fail("statement error2");

                Statement* statement=new Statement(StatementType::ReturnType,exp);
                return statement;
            }
            else if (p._kind==SyntaxKind::IfToken)
            {
//                "if" "(" <exp> ")" <statement> [ "else" <statement> ];

                Statement* ret=new Statement();
                ret->_type=StatementType::ConditionType;
                _lexer->NextToken();
                p=_lexer->NextToken();
                if(p._kind==SyntaxKind::BlankToken)
                    p=_lexer->NextToken();
                if(p._kind!=SyntaxKind::OpToken) fail("statement error3");
                ret->_exp=parse_Exp();
                p=_lexer->NextToken();
                if(p._kind==SyntaxKind::BlankToken)
                    p=_lexer->NextToken();
                if(p._kind!=SyntaxKind::CpToken) fail("statement error4");

                ret->_statement1=parse_statement();

                p=_lexer->Peek();
                if(p._kind==SyntaxKind::BlankToken)
                {
                    _lexer->NextToken();
                    p=_lexer->Peek();
                }

                if(p._kind==SyntaxKind::ElseToken)
                {
                    _lexer->NextToken();
                    p=_lexer->NextToken();
                    if(p._kind!=SyntaxKind::BlankToken) fail("statement error5");
                    ret->_statement2=parse_statement();

//                    p=_lexer->NextToken();
//                    if(p._kind==SyntaxKind::BlankToken)
//                        p=_lexer->NextToken();
//
//                    if(p._kind==SyntaxKind::SemiToken) fail("statement error2");
                }
                return ret;
            }
            else if (p._kind==SyntaxKind::OBraceToken)
            {
                Statement* ret=new Statement();
//                "{" { <block-item> } "}
                _lexer->NextToken();
                p=_lexer->Peek();
                if(p._kind==SyntaxKind::BlankToken)
                {
                    _lexer->NextToken();
                    p=_lexer->Peek();
                }
                while (p._kind!=SyntaxKind::CBraceToken)
                {
                    BlockItem* next_item=parse_blockItem();
                    ret->_list.push_back(next_item);

                    p=_lexer->Peek();
                    if(p._kind==SyntaxKind::BlankToken)
                    {
                        _lexer->NextToken();
                        p=_lexer->Peek();
                    }
                }
                _lexer->NextToken();

                ret->_type=StatementType::CompoundType;
                return ret;
            }
            else if (p._kind==SyntaxKind::ForToken)
            {
//            "for" "(" <exp-option> ";" <exp-option> ";" <exp-option> ")" <statement>
//            "for" "(" <declaration> <exp-option> ";" <exp-option> ")" <statement>
                _lexer->NextToken();
                p=_lexer->NextToken();
                if(p._kind==SyntaxKind::BlankToken)
                    p=_lexer->NextToken();
                if(p._kind!=SyntaxKind::OpToken) fail("expect a (");
                Statement* ret=new Statement();
                p=_lexer->Peek();
                if(p._kind==SyntaxKind::BlankToken)
                {
                    _lexer->NextToken();
                    p=_lexer->Peek();
                }
                if(p._kind==SyntaxKind::IntToken)
                {
                    ret->_type=StatementType::For_2;
                    ret->_declaration=parse_declaration();

                }
                else
                {
                    ret->_type=StatementType::For_1;
                    ret->_initExp=parse_ExpOption();

                    p=_lexer->NextToken();
                    if(p._kind==SyntaxKind::BlankToken)
                        p=_lexer->NextToken();
                    if (p._kind!=SyntaxKind::SemiToken) fail("expect a ;");
                }
                ret->_controlExp=parse_ExpOption();
                p=_lexer->NextToken();
                if(p._kind==SyntaxKind::BlankToken)
                    p=_lexer->NextToken();
                if (p._kind!=SyntaxKind::SemiToken) fail("expect a ;");
                ret->_postExp=parse_ExpOption();
                p=_lexer->NextToken();
                if(p._kind==SyntaxKind::BlankToken)
                    p=_lexer->NextToken();
                if(p._kind!=SyntaxKind::CpToken) fail("expect a )");
                ret->_loopStatement=parse_statement();
                return ret;
            }
            else if (p._kind==SyntaxKind::WhileToken)
            {
                //"while" "(" <exp> ")" <statement>
                Statement* ret=new Statement();

                _lexer->NextToken();
                p=_lexer->NextToken();
                if(p._kind==SyntaxKind::BlankToken)
                    p=_lexer->NextToken();
                if (p._kind!=SyntaxKind::OpToken) fail("expect a (");
                ret->_exp=parse_Exp();

                p=_lexer->NextToken();
                if(p._kind==SyntaxKind::BlankToken)
                    p=_lexer->NextToken();
                if (p._kind!=SyntaxKind::CpToken) fail("expect a )");

                ret->_loopStatement=parse_statement();
                ret->_type=StatementType::WhileStatement;
                return ret;

            }
            else if (p._kind==SyntaxKind::DoToken)
            {
                //"do" <statement> "while" "(" <exp> ")" ";"
                _lexer->NextToken();
                Statement* ret=new Statement();
                ret->_loopStatement=parse_statement();
                p=_lexer->NextToken();
                if(p._kind==SyntaxKind::BlankToken)
                    p=_lexer->NextToken();
                if(p._kind!=SyntaxKind::WhileToken) fail("single do detected");

                p=_lexer->NextToken();
                if(p._kind==SyntaxKind::BlankToken)
                    p=_lexer->NextToken();
                if (p._kind!=SyntaxKind::OpToken) fail("expect a (");
                ret->_exp=parse_Exp();

                p=_lexer->NextToken();
                if(p._kind==SyntaxKind::BlankToken)
                    p=_lexer->NextToken();
                if (p._kind!=SyntaxKind::CpToken) fail("expect a )");
                p=_lexer->NextToken();
                if(p._kind==SyntaxKind::BlankToken)
                    p=_lexer->NextToken();
                if (p._kind!=SemiToken) fail("expect a ;");
                ret->_type=StatementType::DoWhileStatement;
                return ret;
            }
            else if (p._kind==SyntaxKind::BreakToken)
            {
                //"break" ";"
                _lexer->NextToken();
                p=_lexer->NextToken();
                if(p._kind==SyntaxKind::BlankToken)
                    p=_lexer->NextToken();
                if(p._kind!=SyntaxKind::SemiToken) fail("expect a ;");
                Statement* ret=new Statement();
                ret->_type=StatementType::BreakStatement;
                return ret;

            }
            else if (p._kind==SyntaxKind::ContinueToken)
            {
                //"continue" ";"
                _lexer->NextToken();
                p=_lexer->NextToken();
                if(p._kind==SyntaxKind::BlankToken)
                    p=_lexer->NextToken();
                if(p._kind!=SyntaxKind::SemiToken) fail("expect a ;");
                Statement* ret=new Statement();
                ret->_type=StatementType::ContinueStatement;
                return ret;
            }
            else
            {
//              <exp-option> ";"
                ExpOption* expOption=parse_ExpOption();
                p=_lexer->NextToken();

                if(p._kind==SyntaxKind::BlankToken)
                    p=_lexer->NextToken();

                if(p._kind!=SyntaxKind::SemiToken) fail("statement error6");

                Statement* statement = new Statement();
                statement->_expOption=expOption;
                statement->_type=StatementType::ExpType;

                return statement;

            }

            fail("statement error7");
            return nullptr;
        }

        Declaration* parse_declaration()
        {
            //<declaration> ::= "int" <id> [ = <exp> ] ";"

            Declaration* ret=new Declaration();
            SyntaxToken p=_lexer->NextToken();

            if(p._kind==SyntaxKind::BlankToken)
                p=_lexer->NextToken();

            if(p._kind!=SyntaxKind::IntToken) fail("declare error");
            p=_lexer->NextToken();

            if(p._kind!=SyntaxKind::BlankToken) fail("declare error");
            p=_lexer->NextToken();

            if(p._kind!=SyntaxKind::IdToken) fail("declare error");
            ret->_id=p._text;

            p=_lexer->NextToken();
            if(p._kind==SyntaxKind::BlankToken)
                p=_lexer->NextToken();
            //test
            cout<<"declare test!"<<endl;

            //no init
            if(p._kind==SyntaxKind::SemiToken)
                return ret;
            //exist assignment
            if(p._kind==SyntaxKind::AssignmentToken)
            {
                ret->_exp=parse_Exp();
                p=_lexer->NextToken();
                if(p._kind==SyntaxKind::BlankToken)
                    p=_lexer->NextToken();
                if(p._kind==SyntaxKind::SemiToken)
                    return ret;
            }
            fail("declare error");
            return nullptr;
        }

        BlockItem* parse_blockItem()
        {
            //<block-item> ::= <statement> | <declaration>
            BlockItem* ret=new BlockItem();
            SyntaxToken p=_lexer->Peek();

            if(p._kind==SyntaxKind::BlankToken)
            {
                _lexer->NextToken();
                p=_lexer->Peek();
            }
            if(p._kind==SyntaxKind::IntToken)
            {
                ret->_type=BlockType::DeclarationBlock;
                ret->_declaration=parse_declaration();
            }else
            {
                ret->_type=BlockType::StatementBlock;
                ret->_statement=parse_statement();
            }
            return ret;
        }

        //parse the function
        Function* parse_function()
        {
            //int main() {    return 2;}
            //<function> ::= "int" <id> "(" ")" "{" { <block-item> } "}"

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


//            Statement* statement=parse_statement();
//
            p=_lexer->Peek();
            if(p._kind==SyntaxKind::BlankToken)
            {
                p=_lexer->NextToken();
                p=_lexer->Peek();
            }


            Function* ret=new Function();
//            ret->_list.push_back(statement);


            while (p._kind!=SyntaxKind::CBraceToken)
            {
                BlockItem* next_block=parse_blockItem();

                ret->_list.push_back(next_block);

                p=_lexer->Peek();
                if(p._kind==SyntaxKind::BlankToken)
                {
                    p=_lexer->NextToken();
                    p=_lexer->Peek();
                }

            }

            _lexer->NextToken();

            if(p._kind!=SyntaxKind::CBraceToken) fail("function error7");

            ret->_name=name;
            return ret;
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

//    <program> ::= <function>
//    <function> ::= "int" <id> "(" ")" "{" { <block-item> } "}"
//    <block-item> ::= <statement> | <declaration>
//    <declaration> ::= "int" <id> [ = <exp> ] ";"
//    <statement> ::= "return" <exp> ";"
//                    | <exp-option> ";"
//                    | "if" "(" <exp> ")" <statement> [ "else" <statement> ]
//                    | "{" { <block-item> } "}
//                    | "for" "(" <exp-option> ";" <exp-option> ";" <exp-option> ")" <statement>
//                    | "for" "(" <declaration> <exp-option> ";" <exp-option> ")" <statement>
//                    | "while" "(" <exp> ")" <statement>
//                    | "do" <statement> "while" "(" <exp> ")" ";"
//                    | "break" ";"
//                    | "continue" ";"
//    <exp-option> ::= <exp> | ""
//    <exp> ::= <id> "=" <exp> | <conditional-exp>
//    <conditional-exp> ::= <logical-or-exp> [ "?" <exp> ":" <conditional-exp> ]
//    <logical-or-exp> ::= <logical-and-exp> { "||" <logical-and-exp> }
//    <logical-and-exp> ::= <equality-exp> { "&&" <equality-exp> }
//    <equality-exp> ::= <relational-exp> { ("!=" | "==") <relational-exp> }
//    <relational-exp> ::= <additive-exp> { ("<" | ">" | "<=" | ">=") <additive-exp> }
//    <additive-exp> ::= <term> { ("+" | "-") <term> }
//    <term> ::= <factor> { ("*" | "/") <factor> }
//    <factor> ::= "(" <exp> ")" | <unary_op> <factor> | <int> | <id>
//    <unary_op> ::= "!" | "~" | "-"

    class Varmap
    {
    public:
        bool contains(string s)
        {
            return _map.find(s)!=_map.end();
        }

        int find(string s)
        {
            if(!contains(s))
                fail((s+"is not declared").c_str());
            return _map[s];
        }

        Varmap* copy()
        {
            unordered_map<string,int> copy_map(_map);
            Varmap* ret=new Varmap();
            ret->set_map(copy_map);
        }

        Varmap* push(string s,int stack_index)
        {
            unordered_map<string,int> copy_map(_map);
            copy_map[s]=stack_index;
            Varmap* ret=new Varmap();
            ret->set_map(copy_map);

            return ret;
        }


    private:
        unordered_map<string,int> _map;
        void set_map(unordered_map<string,int> map)
        {
            _map=map;
        }
    };

    class Assembler
    {
    public:
        Assembler(string filename)
        {
            string Filename=filename;
            int length=Filename.length();
            Filename=Filename.substr(0,length-2);
            fout.open(Filename+".s");
            if (!fout.is_open())
            {
                fail("fail to create file");
            }
        }

        //    <function> ::= "int" <id> "(" ")" "{" { <block-item> } "}"
        void function_out(Function* function)
        {
            //init the varmap
            Varmap* varmap=new Varmap();

            string name=function->_name;
            fout<<" .globl"<<" "<<name<<endl;
            fout<<name<<":"<<endl;
            fout<<"    push %ebp"<<endl;
            fout<<"    movl %esp, %ebp"<<endl;
            auto slist=&function->_list;

            int cnt=0;
            for(auto x : *slist)
            {
                if(x->_type==BlockType::StatementBlock)
                {
                    if (x->_statement->_type==StatementType::ReturnType)
                        cnt++;
                    statement_out(x->_statement,varmap);
                }else if (x->_type==BlockType::DeclarationBlock)
                {
                    varmap=declaration_out(x->_declaration,varmap);
                }
            }
            if(!cnt)
            {
                fout<<"    movl    $0, %eax"<<endl;
                fout<<"    movl %ebp, %esp"<<endl;
                fout<<"    pop %ebp"<<endl;
                fout<<"ret"<<endl;
            }
        }

//        <statement> ::= "return" <exp> ";"
//                        | <exp> ";"
//                        | "if" "(" <exp> ")" <statement> [ "else" <statement> ]
//                        | "{" { <block-item> } "}"
        void statement_out(Statement* statement,Varmap* varmap)
        {
            if (statement->_type==StatementType::ConditionType)
            {
                exp_out(statement->_exp,varmap);
                string flag1=clause_counter();
                fout<<"    cmpl $0, %eax"<<endl;
                fout<<"    je "<<flag1<<endl;
                statement_out(statement->_statement1,varmap);
                if(statement->_statement2!= nullptr)
                {
                    string flag2=clause_counter();
                    fout<<"    jmp  "<<flag2<<endl;
                    fout<<flag1<<":"<<endl;
                    statement_out(statement->_statement2,varmap);
                    fout<<flag2<<":"<<endl;
                }else
                    fout<<flag1<<":"<<endl;

            }
            else if (statement->_type==StatementType::ExpType)
            {
                exp_out(statement->_exp,varmap);

            }
            else if (statement->_type==StatementType::ReturnType)
            {
                exp_out(statement->_exp,varmap);

                fout<<"    movl %ebp, %esp"<<endl;
                fout<<"    pop %ebp"<<endl;
                fout<<"ret"<<endl;
            }
            else if (statement->_type==StatementType::CompoundType)
            {
                //generate block

                set<string>current_scope;

                list<BlockItem*> blockList=statement->_list;
                for(auto item : blockList)
                {
                    if(item->_type==BlockType::StatementBlock)
                    {
                        statement_out(item->_statement,varmap);
                    }else if (item->_type==BlockType::DeclarationBlock)
                    {
                        varmap=declaration_out(item->_declaration,varmap,&current_scope);
                    }
                }

                int bytes_to_deallocate = 4 * current_scope.size();
                stack_index+=bytes_to_deallocate;
                fout<<"    addl $"<<bytes_to_deallocate<<", %esp"<<endl;

            }
        }

        //    <exp-option> ::= <exp> | ""
        void expOption_out(ExpOption* expOption,Varmap* varmap)
        {

        }


        //<declaration> ::= "int" <id> [ = <exp> ] ";"
        Varmap* declaration_out(Declaration* declaration,Varmap* varmap,set<string>* current_scope= nullptr)
        {
            stack_index-=4;
            if(current_scope== nullptr)
            {
                if (varmap->contains(declaration->_id))
                {
                    string text="redeclaration of ";
                    text+=declaration->_id;
                    fail(text.c_str());
                }
                fout<<"    movl    $0, %eax"<<endl;
                if(declaration->_exp != nullptr)
                {
                    exp_out(declaration->_exp,varmap);
                }
                fout<<"    pushl %eax"<<endl;
                varmap=varmap->push(declaration->_id,stack_index);
                return varmap;
            }else
            {
                if(current_scope->find(declaration->_id)!=current_scope->end())
                {
                    string text="redeclaration of ";
                    text+=declaration->_id;
                    fail(text.c_str());
                }
                fout<<"    movl    $0, %eax"<<endl;
                if(declaration->_exp != nullptr)
                {
                    exp_out(declaration->_exp,varmap);
                }
                fout<<"    pushl %eax"<<endl;
                varmap=varmap->push(declaration->_id,stack_index);
                current_scope->insert(declaration->_id);
                return varmap;
            }

        }

//    <exp> ::= <id> "=" <exp> | <conditional-exp>
        void exp_out(Exp* exp,Varmap* varmap)
        {
            if (exp->_kind==ExpKind::AssignmentExp)
            {
                exp_out(exp->_next,varmap);

                if(!varmap->contains(exp->_id))
                    fail((exp->_id+" is not declared").c_str());
                int offset=varmap->find(exp->_id);
                fout<<"    movl %eax, "<<offset<<"(%ebp)"<<endl;
            }else if(exp->_kind==ExpKind::ConditonalExp)
            {
                conditionalExp_out(exp->_conditionExp,varmap);
            }
        }

//        <conditional-exp> ::= <logical-or-exp> [ "?" <exp> ":" <conditional-exp> ]
        void conditionalExp_out(ConditionExp* exp,Varmap* varmap)
        {
            LOrExp_out(exp->_lorExp,varmap);
            if (exp->_exp!= nullptr)
            {
                string flag1=clause_counter();
                string flag2=clause_counter();

                fout<<"    cmpl $0, %eax"<<endl;
                fout<<"    je "<<flag1<<endl;
                exp_out(exp->_exp,varmap);
                fout<<"    jmp  "<<flag2<<endl;
                fout<<flag1<<":"<<endl;
                conditionalExp_out(exp->_next,varmap);
                fout<<flag2<<":"<<endl;
            }
        }

//<logical-or-exp> ::= <logical-and-exp> { "||" <logical-and-exp> }
        void LOrExp_out(LOrExp* exp,Varmap* varmap)
        {
            if (exp== nullptr) return;
            landexp_out(exp->_landexp,varmap);

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
                    landexp_out(exp->_next->_landexp,varmap);//next bool expression
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
        void landexp_out(LAndExp* exp,Varmap* varmap)
        {
            if (exp== nullptr) return;
            equalityexp_out(exp->_equalityexp,varmap);

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
                    equalityexp_out(exp->_next->_equalityexp,varmap);//next bool expression
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
        void equalityexp_out(EqualityExp* exp,Varmap* varmap)
        {
            if (exp== nullptr) return;
            relationalexp_out(exp->_relationalexp,varmap);
            while(exp->_next!= nullptr)
            {
                fout<<"    push %eax"<<endl;
                relationalexp_out(exp->_next->_relationalexp,varmap);
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
        void relationalexp_out(RelationalExp* exp,Varmap* varmap)
        {
            if (exp== nullptr) return;
            addexp_out(exp->_addexp,varmap);
            while(exp->_next!= nullptr)
            {
                fout<<"    push %eax"<<endl;
                addexp_out(exp->_next->_addexp,varmap);
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

//<factor> ::= "(" <exp> ")" | <unary_op> <factor> | <int> | <id>
        void factor_out(Factor* factor,Varmap* varmap)
        {
            if(factor== nullptr) return;
            //<factor> ::= "(" <exp> ")"
            if(factor->_kind==FactorKind::ExpFactor)
                exp_out(factor->_exp,varmap);
            else if(factor->_kind==FactorKind::UnaryFactor)
                factor_out(factor->_next,varmap);
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
            else if(factor->_kind==FactorKind::NumberFactor)
                fout<<" movl    $"<<factor->_text<<","<<" %eax"<<endl;
            else if (factor->_kind==FactorKind::IdFactor)
            {
                int offset=varmap->find(factor->_text);
                fout<<"    movl "<<offset<<"(%ebp), %eax"<<endl;
            }
        }

//    <term> ::= <factor> { ("*" | "/") <factor> }
        void term_out(Term* term,Varmap* varmap)
        {
            if(term== nullptr) return;
            factor_out(term->_factor,varmap);

            while (term->_next!= nullptr)
            {
                fout<<"    push %eax"<<endl;
                factor_out(term->_next->_factor,varmap);
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
        void addexp_out(AddExp* addExp,Varmap* varmap)
        {
            if(addExp== nullptr) return;
            term_out(addExp->_term,varmap);

            while (addExp->_next!= nullptr)
            {
                fout<<"    push %eax"<<endl;
                term_out(addExp->_next->_term,varmap);
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


        void aOut(Program* program)
        {
            if (program== nullptr) return;

            function_out(program->_function);


            fout.close();
        }

    private:
        ofstream fout;
        int stack_index=0;
    };

    __attribute__((unused)) void pretty_print(Program* program)
    {
//        cout<<"program:"<<program->_function->_name<<endl;
//        Exp* exp=program->_function->_statement->_exp;
//        while(exp!=nullptr)
//        {
////            exp= exp->_next;
////            cout << exp->_text<<endl;
//        }

    }

}


int main(int argc,char* argv[]) {
    int test=0;
    if(test)
    {
        string test_text="int main() {     int a = 1;     {         int a = 2;     }     {         return a;     } }";

        mc::Lexer* lexer=new mc::Lexer(test_text);
        mc::Parser* parser=new mc::Parser(lexer);

        //parse the program
        mc::Program* program=parser->parse_program();

        //set the file name
//    string Filename=filename;
//    int length=Filename.length();
//    Filename=Filename.substr(0,length-2);

        mc::Assembler* assembler=new mc::Assembler("test.s");

        assembler->aOut(program);
    }
    else
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
//    string Filename=filename;
//    int length=Filename.length();
//    Filename=Filename.substr(0,length-2);

        mc::Assembler* assembler=new mc::Assembler(filename);

        assembler->aOut(program);

//    string command="gcc ";
//
//    command.append(Filename+".s -o ").append(Filename);
//        system(command.c_str());

    }




    return 0;
}


