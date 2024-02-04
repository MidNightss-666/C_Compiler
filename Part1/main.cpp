#include <iostream>
#include <string>
#include <cstring>
#include <istream>
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
        int val;
    };

    class Statement
    {
    public:
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

        Exp parse_exp()
        {
            SyntaxToken p=_lexer->NextToken();
            if (p._kind!=SyntaxKind::IntToken)
                fail("exp error");

            return Exp(p._text);
        }

        Statement parse_statement()
        {
            SyntaxToken p=_lexer->NextToken();
            if(p._kind!=SyntaxKind::RetToken) fail("statement error");
            p=_lexer->NextToken();
            if(p._kind!=SyntaxKind::BlankToken) fail("statement error");
            Exp exp=parse_exp();
            p=_lexer->NextToken();
            if(p._kind!=SyntaxKind::SemiToken) fail("statement error");
            return Statement(StatementType::Return,&exp);
        }

        Function parse_function()
        {
            SyntaxToken p=_lexer->NextToken();
            if(p._kind!=SyntaxKind::IntToken) fail("function error");
            p=_lexer->NextToken();
            if(p._kind!=SyntaxKind::BlankToken) fail("function error");
            p=_lexer->NextToken();
            if(p._kind!=SyntaxKind::IdToken) fail("function error");

            string name=p._text;

            p=_lexer->NextToken();
            if(p._kind!=SyntaxKind::BlankToken) fail("function error");
            p=_lexer->NextToken();
            if(p._kind!=SyntaxKind::OpToken) fail("function error");
            p=_lexer->NextToken();
            if(p._kind!=SyntaxKind::CpToken) fail("function error");
            p=_lexer->NextToken();
            if(p._kind!=SyntaxKind::OBraceToken) fail("function error");

            Statement statement=parse_statement();

            p=_lexer->NextToken();
            if(p._kind!=SyntaxKind::CBraceToken) fail("function error");

            return Function(&statement,name);
        }

        Program parse_program()
        {
            Function function=parse_function();
            return Program(&function);
        }



    private:
        Lexer* _lexer;
        static void fail(const char* text)
        {
            fprintf(stderr,text);
            exit(EXIT_FAILURE);
        }




    };
}



int main(int argc,char* argv[]) {
    int test=1;
    if(test)
    {
        ifstream fin;
        fin.open("return_2.txt",ios::in);
        if(!fin.is_open())
        {
            cout<<"failed";
            exit(EXIT_FAILURE);
        }
        string buff;
        while(getline(fin,buff))
        {
            cout<<buff;
        }


    }else
    {
        if(argc<2)
        {
            cout<<"invalid usage"<<endl;
            return 1;
        }
        char* filename=argv[1];
        char c;
    }

    return 0;
}
