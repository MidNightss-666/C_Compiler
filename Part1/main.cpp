#include <iostream>

using namespace std;


namespace mc
{
    class Solve{


    };

    enum SyntaxKind{
        NumberToken
    };

    class SyntaxToken{
    public:
        SyntaxToken(SyntaxKind kind,int position,string text)
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
