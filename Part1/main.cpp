#include <iostream>

using namespace std;


namespace mc
{
    class Solve{


    };


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
        SemiToken,
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

    //语法分析器
    class Parser{
    public:
        Parser()
        {

        }




    private:


    };
}



int main(int argc,char* argv[]) {
    if(argc<2)
    {
        cout<<"invalid usage"<<endl;
        return 1;
    }
    char* filename=argv[1];
    char c;

}
