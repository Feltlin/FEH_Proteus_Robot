#ifndef TEXT
#define TEXT

#undef A
#undef B

#include <FEHLCD.h>
#include <FEHUtility.h>

#include <vector>
#include <string>

//Text Class to display text and button.
//This entire text library and button detection was made by Junhao
class Text{
    public:

        //Dimensions of a single letter in the font.
        int width = 5;
        int height = 7;

        //Click detection. 0: Previous frame click, 1: Current fram click.
        bool click[2] = {false, false};

        //Width and height of the image.
        int imageWidth = 16;

        //Display text button. Return if the button is been clicked. After x and y all other parameter is default.
        bool button(std::string text, int textColor, int x, int y, int borderWidth = 1, int borderColor = 0xffffff, int boxColor = -1, int clickedTextColor = 0, int clickedBoxColor = 0xffffff){
            
            //Store where the touch is detected.
            int tx, ty;

            //Calculate the initial and final position of the border.
            int x0 = x - borderWidth, y0 = y - borderWidth;
            int x1 = x + (width + 1) * text.length() + borderWidth, y1 = y + (height + 1) + borderWidth;
            
            if(LCD.Touch(&tx, &ty)){

                //Draw the clicked version if it is clicked inside the button area.
                if(tx >= x0 && tx <= x1 && ty >= y0 && ty <= y1){

                    //Draw the highlight box.
                    LCD.SetFontColor(clickedBoxColor);
                    LCD.FillRectangle(x0 + 1, y0 + 1,  x1 - x0 - 1, y1 - y0 - 1);

                    //Draw the border.
                    drawBorder(borderColor, x0, y0, x1, y1);

                    //Display the highlight text.
                    display(text, clickedTextColor, x, y);

                    //Change the click state.
                    click[0] = click[1];
                    click[1] = true;
                    return true;
                }
            }

            //Draw the regular version if it is not clicked or clicked outside the button area.
            if(boxColor != -1){
                LCD.SetFontColor(boxColor);
                LCD.FillRectangle(x0, y0,  x1 - x0 + 1, y1 - y0 + 1);
            }
            display(text, textColor, x, y);
            drawBorder(borderColor, x0, y0, x1, y1);
            click[0] = click[1];
            click[1] = false;
            return false;
        }

        //Draw the border of the button.
        void drawBorder(int color, int xi, int yi, int xf, int yf){
            LCD.SetFontColor(color);
            LCD.DrawHorizontalLine(yi - 1, xi, xf + 1);
            LCD.DrawHorizontalLine(yf + 1, xi, xf + 1);
            LCD.DrawVerticalLine(xi - 1, yi, yf + 1);
            LCD.DrawVerticalLine(xf + 1, yi, yf + 1);
        }

        //Draw a rectangular box.
        void drawBox(int color, int xi, int yi, int xf, int yf){
            LCD.SetFontColor(color);
            LCD.FillRectangle(xi, yi, xf - xi + 1, yf - yi + 1);
        }

        //Display the text.
        void display(std::string text, int color, int x, int y){

            //Iterate through the string.
            for(int index = 0; index < text.length(); ++index){

                //Use switch case to match each letter.
                switch(text[index]){
                    case 'A':
                        displayLetter(&A, color, x + index * (width + 1), y);
                        break;
                    case 'B':
                        displayLetter(&B, color, x + index * (width + 1), y);
                        break;
                    case 'C':
                        displayLetter(&C, color, x + index * (width + 1), y);
                        break;
                    case 'D':
                        displayLetter(&D, color, x + index * (width + 1), y);
                        break;
                    case 'E':
                        displayLetter(&E, color, x + index * (width + 1), y);
                        break;
                    case 'F':
                        displayLetter(&F, color, x + index * (width + 1), y);
                        break;
                    case 'G':
                        displayLetter(&G, color, x + index * (width + 1), y);
                        break;
                    case 'H':
                        displayLetter(&H, color, x + index * (width + 1), y);
                        break;
                    case 'I':
                        displayLetter(&I, color, x + index * (width + 1), y);
                        break;
                    case 'J':
                        displayLetter(&J, color, x + index * (width + 1), y);
                        break;
                    case 'K':
                        displayLetter(&K, color, x + index * (width + 1), y);
                        break;
                    case 'L':
                        displayLetter(&L, color, x + index * (width + 1), y);
                        break;
                    case 'M':
                        displayLetter(&M, color, x + index * (width + 1), y);
                        break;
                    case 'N':
                        displayLetter(&N, color, x + index * (width + 1), y);
                        break;
                    case 'O':
                        displayLetter(&O, color, x + index * (width + 1), y);
                        break;
                    case 'P':
                        displayLetter(&P, color, x + index * (width + 1), y);
                        break;
                    case 'Q':
                        displayLetter(&Q, color, x + index * (width + 1), y);
                        break;
                    case 'R':
                        displayLetter(&R, color, x + index * (width + 1), y);
                        break;
                    case 'S':
                        displayLetter(&S, color, x + index * (width + 1), y);
                        break;
                    case 'T':
                        displayLetter(&T, color, x + index * (width + 1), y);
                        break;
                    case 'U':
                        displayLetter(&U, color, x + index * (width + 1), y);
                        break;
                    case 'V':
                        displayLetter(&V, color, x + index * (width + 1), y);
                        break;
                    case 'W':
                        displayLetter(&W, color, x + index * (width + 1), y);
                        break;
                    case 'X':
                        displayLetter(&X, color, x + index * (width + 1), y);
                        break;
                    case 'Y':
                        displayLetter(&Y, color, x + index * (width + 1), y);
                        break;
                    case 'Z':
                        displayLetter(&Z, color, x + index * (width + 1), y);
                        break;
                    case 'a':
                        displayLetter(&a, color, x + index * (width + 1), y);
                        break;
                    case 'b':
                        displayLetter(&b, color, x + index * (width + 1), y);
                        break;
                    case 'c':
                        displayLetter(&c, color, x + index * (width + 1), y);
                        break;
                    case 'd':
                        displayLetter(&d, color, x + index * (width + 1), y);
                        break;
                    case 'e':
                        displayLetter(&e, color, x + index * (width + 1), y);
                        break;
                    case 'f':
                        displayLetter(&f, color, x + index * (width + 1), y);
                        break;
                    case 'g':
                        displayLetter(&g, color, x + index * (width + 1), y);
                        break;
                    case 'h':
                        displayLetter(&h, color, x + index * (width + 1), y);
                        break;
                    case 'i':
                        displayLetter(&i, color, x + index * (width + 1), y);
                        break;
                    case 'j':
                        displayLetter(&j, color, x + index * (width + 1), y);
                        break;
                    case 'k':
                        displayLetter(&k, color, x + index * (width + 1), y);
                        break;
                    case 'l':
                        displayLetter(&l, color, x + index * (width + 1), y);
                        break;
                    case 'm':
                        displayLetter(&m, color, x + index * (width + 1), y);
                        break;
                    case 'n':
                        displayLetter(&n, color, x + index * (width + 1), y);
                        break;
                    case 'o':
                        displayLetter(&o, color, x + index * (width + 1), y);
                        break;
                    case 'p':
                        displayLetter(&p, color, x + index * (width + 1), y);
                        break;
                    case 'q':
                        displayLetter(&q, color, x + index * (width + 1), y);
                        break;
                    case 'r':
                        displayLetter(&r, color, x + index * (width + 1), y);
                        break;
                    case 's':
                        displayLetter(&s, color, x + index * (width + 1), y);
                        break;
                    case 't':
                        displayLetter(&t, color, x + index * (width + 1), y);
                        break;
                    case 'u':
                        displayLetter(&u, color, x + index * (width + 1), y);
                        break;
                    case 'v':
                        displayLetter(&v, color, x + index * (width + 1), y);
                        break;
                    case 'w':
                        displayLetter(&w, color, x + index * (width + 1), y);
                        break;
                    case 'x':
                        displayLetter(&lx, color, x + index * (width + 1), y);
                        break;
                    case 'y':
                        displayLetter(&ly, color, x + index * (width + 1), y);
                        break;
                    case 'z':
                        displayLetter(&z, color, x + index * (width + 1), y);
                        break;
                    case '0':
                        displayLetter(&n0, color, x + index * (width + 1), y);
                        break;
                    case '1':
                        displayLetter(&n1, color, x + index * (width + 1), y);
                        break;
                    case '2':
                        displayLetter(&n2, color, x + index * (width + 1), y);
                        break;
                    case '3':
                        displayLetter(&n3, color, x + index * (width + 1), y);
                        break;
                    case '4':
                        displayLetter(&n4, color, x + index * (width + 1), y);
                        break;
                    case '5':
                        displayLetter(&n5, color, x + index * (width + 1), y);
                        break;
                    case '6':
                        displayLetter(&n6, color, x + index * (width + 1), y);
                        break;
                    case '7':
                        displayLetter(&n7, color, x + index * (width + 1), y);
                        break;
                    case '8':
                        displayLetter(&n8, color, x + index * (width + 1), y);
                        break;
                    case '9':
                        displayLetter(&n9, color, x + index * (width + 1), y);
                        break;
                    case '+':
                        displayLetter(&plus, color, x + index * (width + 1), y);
                        break;
                    case '-':
                        displayLetter(&minus, color, x + index * (width + 1), y);
                        break;
                    case '*':
                        displayLetter(&aster, color, x + index * (width + 1), y);
                        break;
                    case '/':
                        displayLetter(&slash, color, x + index * (width + 1), y);
                        break;
                    case '=':
                        displayLetter(&equal, color, x + index * (width + 1), y);
                        break;
                    case '%':
                        displayLetter(&percent, color, x + index * (width + 1), y);
                        break;
                    case '\"':
                        displayLetter(&dquote, color, x + index * (width + 1), y);
                        break;
                    case '\'':
                        displayLetter(&squote, color, x + index * (width + 1), y);
                        break;
                    case '#':
                        displayLetter(&hash, color, x + index * (width + 1), y);
                        break;
                    case '$':
                        displayLetter(&dollar, color, x + index * (width + 1), y);
                        break;
                    case '@':
                        displayLetter(&at, color, x + index * (width + 1), y);
                        break;
                    case '&':
                        displayLetter(&amper, color, x + index * (width + 1), y);
                        break;
                    case '_':
                        displayLetter(&under, color, x + index * (width + 1), y);
                        break;
                    case '(':
                        displayLetter(&LrBra, color, x + index * (width + 1), y);
                        break;
                    case ')':
                        displayLetter(&RrBra, color, x + index * (width + 1), y);
                        break;
                    case '{':
                        displayLetter(&LcBra, color, x + index * (width + 1), y);
                        break;
                    case '}':
                        displayLetter(&RcBra, color, x + index * (width + 1), y);
                        break;
                    case '<':
                        displayLetter(&LaBra, color, x + index * (width + 1), y);
                        break;
                    case '>':
                        displayLetter(&RaBra, color, x + index * (width + 1), y);
                        break;
                    case '[':
                        displayLetter(&LsBra, color, x + index * (width + 1), y);
                        break;
                    case ']':
                        displayLetter(&RsBra, color, x + index * (width + 1), y);
                        break;
                    case ',':
                        displayLetter(&comma, color, x + index * (width + 1), y);
                        break;
                    case '.':
                        displayLetter(&period, color, x + index * (width + 1), y);
                        break;
                    case ';':
                        displayLetter(&scolon, color, x + index * (width + 1), y);
                        break;
                    case ':':
                        displayLetter(&colon, color, x + index * (width + 1), y);
                        break;
                    case '?':
                        displayLetter(&quest, color, x + index * (width + 1), y);
                        break;
                    case '!':
                        displayLetter(&exclam, color, x + index * (width + 1), y);
                        break;
                    case '^':
                        displayLetter(&caret, color, x + index * (width + 1), y);
                        break;
                    case '~':
                        displayLetter(&tilde, color, x + index * (width + 1), y);
                        break;
                    case ' ':
                        break;
                    default:
                        displayLetter(&missing, color, x + index * (width + 1), y);
                }
            }
        }

        //Display a single letter.
        void displayLetter(std::vector<std::vector<int>> *letter, int color, int x, int y){
            for(int row = 0; row < letter->size(); ++row){
                for(int column = 0; column < (*letter)[row].size(); ++column){
                    if((*letter)[row][column] == 1){
                        LCD.SetFontColor(color);
                        LCD.DrawPixel(column + x, row + y);
                    }
                }
            }
        }


        //2D vector for store the shape of the letters.
        std::vector<std::vector<int>> A = 
        {
            {0, 1, 1, 1},
            {1, 0, 0, 0, 1},
            {1, 1, 1, 1, 1},
            {1, 0, 0, 0, 1},
            {1, 0, 0, 0, 1},
            {1, 0, 0, 0, 1},
            {1, 0, 0, 0, 1}
        };

        std::vector<std::vector<int>> B = 
        {
            {1, 1, 1, 1},
            {1, 0, 0, 0, 1},
            {1, 1, 1, 1},
            {1, 0, 0, 0, 1},
            {1, 0, 0, 0, 1},
            {1, 0, 0, 0, 1},
            {1, 1, 1, 1}
        };

        std::vector<std::vector<int>> C = 
        {
            {0, 1, 1, 1},
            {1, 0, 0, 0, 1},
            {1},
            {1},
            {1},
            {1, 0, 0, 0, 1},
            {0, 1, 1, 1}
        };

        std::vector<std::vector<int>> D = 
        {
            {1, 1, 1, 1},
            {1, 0, 0, 0, 1},
            {1, 0, 0, 0, 1},
            {1, 0, 0, 0, 1},
            {1, 0, 0, 0, 1},
            {1, 0, 0, 0, 1},
            {1, 1, 1, 1}
        };

        std::vector<std::vector<int>> E = 
        {
            {1, 1, 1, 1, 1},
            {1},
            {1, 1, 1},
            {1},
            {1},
            {1},
            {1, 1, 1, 1, 1}
        };

        std::vector<std::vector<int>> F = 
        {
            {1, 1, 1, 1, 1},
            {1},
            {1, 1, 1},
            {1},
            {1},
            {1},
            {1}
        };

        std::vector<std::vector<int>> G = 
        {
            {0, 1, 1, 1, 1},
            {1},
            {1, 0, 0, 1, 1},
            {1, 0, 0, 0, 1},
            {1, 0, 0, 0, 1},
            {1, 0, 0, 0, 1},
            {0, 1, 1, 1}
        };

        std::vector<std::vector<int>> H = 
        {
            {1, 0, 0, 0, 1},
            {1, 0, 0, 0, 1},
            {1, 1, 1, 1, 1},
            {1, 0, 0, 0, 1},
            {1, 0, 0, 0, 1},
            {1, 0, 0, 0, 1},
            {1, 0, 0, 0, 1}
        };

        std::vector<std::vector<int>> I = 
        {
            {0, 1, 1, 1},
            {0, 0, 1},
            {0, 0, 1},
            {0, 0, 1},
            {0, 0, 1},
            {0, 0, 1},
            {0, 1, 1, 1}
        };

        std::vector<std::vector<int>> J = 
        {
            {0, 0, 0, 0, 1},
            {0, 0, 0, 0, 1},
            {0, 0, 0, 0, 1},
            {0, 0, 0, 0, 1},
            {0, 0, 0, 0, 1},
            {1, 0, 0, 0, 1},
            {0, 1, 1, 1}
        };

        std::vector<std::vector<int>> K = 
        {
            {1, 0, 0, 0, 1},
            {1, 0, 0, 1},
            {1, 1, 1},
            {1, 0, 0, 1},
            {1, 0, 0, 0, 1},
            {1, 0, 0, 0, 1},
            {1, 0, 0, 0, 1}
        };

        std::vector<std::vector<int>> L = 
        {
            {1},
            {1},
            {1},
            {1},
            {1},
            {1},
            {1, 1, 1, 1, 1}
        };

        std::vector<std::vector<int>> M = 
        {
            {1, 0, 0, 0, 1},
            {1, 1, 0, 1, 1},
            {1, 0, 1, 0, 1},
            {1, 0, 0, 0, 1},
            {1, 0, 0, 0, 1},
            {1, 0, 0, 0, 1},
            {1, 0, 0, 0, 1}
        };

        std::vector<std::vector<int>> N = 
        {
            {1, 0, 0, 0, 1},
            {1, 1, 0, 0, 1},
            {1, 0, 1, 0, 1},
            {1, 0, 0, 1, 1},
            {1, 0, 0, 0, 1},
            {1, 0, 0, 0, 1},
            {1, 0, 0, 0, 1}
        };

        std::vector<std::vector<int>> O = 
        {
            {0, 1, 1, 1},
            {1, 0, 0, 0, 1},
            {1, 0, 0, 0, 1},
            {1, 0, 0, 0, 1},
            {1, 0, 0, 0, 1},
            {1, 0, 0, 0, 1},
            {0, 1, 1, 1}
        };

        std::vector<std::vector<int>> P = 
        {
            {1, 1, 1, 1},
            {1, 0, 0, 0, 1},
            {1, 1, 1, 1},
            {1},
            {1},
            {1},
            {1}
        };

        std::vector<std::vector<int>> Q = 
        {
            {0, 1, 1, 1},
            {1, 0, 0, 0, 1},
            {1, 0, 0, 0, 1},
            {1, 0, 0, 0, 1},
            {1, 0, 0, 0, 1},
            {1, 0, 0, 1},
            {0, 1, 1, 0, 1}
        };

        std::vector<std::vector<int>> R = 
        {
            {1, 1, 1, 1},
            {1, 0, 0, 0, 1},
            {1, 1, 1, 1},
            {1, 0, 0, 0, 1},
            {1, 0, 0, 0, 1},
            {1, 0, 0, 0, 1},
            {1, 0, 0, 0, 1}
        };

        std::vector<std::vector<int>> S = 
        {
            {0, 1, 1, 1, 1},
            {1},
            {0, 1, 1, 1},
            {0, 0, 0, 0, 1},
            {0, 0, 0, 0, 1},
            {1, 0, 0, 0, 1},
            {0, 1, 1, 1}
        };

        std::vector<std::vector<int>> T = 
        {
            {1, 1, 1, 1, 1},
            {0, 0, 1},
            {0, 0, 1},
            {0, 0, 1},
            {0, 0, 1},
            {0, 0, 1},
            {0, 0, 1}
        };

        std::vector<std::vector<int>> U = 
        {
            {1, 0, 0, 0, 1},
            {1, 0, 0, 0, 1},
            {1, 0, 0, 0, 1},
            {1, 0, 0, 0, 1},
            {1, 0, 0, 0, 1},
            {1, 0, 0, 0, 1},
            {0, 1, 1, 1}
        };

        std::vector<std::vector<int>> V = 
        {
            {1, 0, 0, 0, 1},
            {1, 0, 0, 0, 1},
            {1, 0, 0, 0, 1},
            {1, 0, 0, 0, 1},
            {0, 1, 0, 1},
            {0, 1, 0, 1},
            {0, 0, 1}
        };

        std::vector<std::vector<int>> W = 
        {
            {1, 0, 0, 0, 1},
            {1, 0, 0, 0, 1},
            {1, 0, 0, 0, 1},
            {1, 0, 0, 0, 1},
            {1, 0, 1, 0, 1},
            {1, 1, 0, 1, 1},
            {1, 0, 0, 0, 1}
        };

        std::vector<std::vector<int>> X = 
        {
            {1, 0, 0, 0, 1},
            {0, 1, 0, 1, 0},
            {0, 0, 1, 0, 0},
            {0, 1, 0, 1, 0},
            {1, 0, 0, 0, 1},
            {1, 0, 0, 0, 1},
            {1, 0, 0, 0, 1}
        };

        std::vector<std::vector<int>> Y = 
        {
            {1, 0, 0, 0, 1},
            {0, 1, 0, 1},
            {0, 0, 1},
            {0, 0, 1},
            {0, 0, 1},
            {0, 0, 1},
            {0, 0, 1}
        };

        std::vector<std::vector<int>> Z = 
        {
            {1, 1, 1, 1, 1},
            {0, 0, 0, 0, 1},
            {0, 0, 0, 1},
            {0, 0, 1},
            {0, 1},
            {1},
            {1, 1, 1, 1, 1}
        };

        std::vector<std::vector<int>> a = 
        {
            {0},
            {0},
            {0, 1, 1, 1},
            {0, 0, 0, 0, 1},
            {0, 1, 1, 1, 1},
            {1, 0, 0, 0, 1},
            {0, 1, 1, 1, 1}
        };

        std::vector<std::vector<int>> b = 
        {
            {1},
            {1},
            {1, 0, 1, 1},
            {1, 1, 0, 0, 1},
            {1, 0, 0, 0, 1},
            {1, 0, 0, 0, 1},
            {1, 1, 1, 1}
        };

        std::vector<std::vector<int>> c = 
        {
            {0},
            {0},
            {0, 1, 1, 1},
            {1, 0, 0, 0, 1},
            {1},
            {1, 0, 0, 0, 1},
            {0, 1, 1, 1}
        };

        std::vector<std::vector<int>> d = 
        {
            {0, 0, 0, 0, 1},
            {0, 0, 0, 0, 1},
            {0, 1, 1, 0, 1},
            {1, 0, 0, 1, 1},
            {1, 0, 0, 0, 1},
            {1, 0, 0, 0, 1},
            {0, 1, 1, 1, 1}
        };

        std::vector<std::vector<int>> e = 
        {
            {0},
            {0},
            {0, 1, 1, 1},
            {1, 0, 0, 0, 1},
            {1, 1, 1, 1, 1},
            {1},
            {0, 1, 1, 1}
        };

        std::vector<std::vector<int>> f = 
        {
            {0, 0, 0, 1, 1},
            {0, 0, 1},
            {0, 1, 1, 1, 1},
            {0, 0, 1},
            {0, 0, 1},
            {0, 0, 1},
            {0, 0, 1}
        };

        std::vector<std::vector<int>> g = 
        {
            {0},
            {0},
            {0, 1, 1, 1, 1},
            {1, 0, 0, 0, 1},
            {1, 0, 0, 0, 1},
            {0, 1, 1, 1, 1},
            {0, 0, 0, 0, 1},
            {1, 1, 1, 1}
        };

        std::vector<std::vector<int>> h = 
        {
            {1},
            {1},
            {1, 0, 1, 1},
            {1, 1, 0, 0, 1},
            {1, 0, 0, 0, 1},
            {1, 0, 0, 0, 1},
            {1, 0, 0, 0, 1}
        };

        std::vector<std::vector<int>> i = 
        {
            {0, 0, 1},
            {0},
            {0, 0, 1},
            {0, 0, 1},
            {0, 0, 1},
            {0, 0, 1},
            {0, 0, 1}
        };

        std::vector<std::vector<int>> j = 
        {
            {0, 0, 0, 0, 1},
            {0},
            {0, 0, 0, 0, 1},
            {0, 0, 0, 0, 1},
            {1, 0, 0, 0, 1},
            {1, 0, 0, 0, 1},
            {0, 1, 1, 1}
        };

        std::vector<std::vector<int>> k = 
        {
            {1},
            {1},
            {1, 0, 0, 1},
            {1, 0, 1},
            {1, 1},
            {1, 0, 1},
            {1, 0, 0, 1}
        };

        std::vector<std::vector<int>> l = 
        {
            {0, 0, 1},
            {0, 0, 1},
            {0, 0, 1},
            {0, 0, 1},
            {0, 0, 1},
            {0, 0, 1},
            {0, 0, 0, 1}
        };

        std::vector<std::vector<int>> m = 
        {
            {0},
            {0},
            {1, 1, 0, 1},
            {1, 0, 1, 0, 1},
            {1, 0, 1, 0, 1},
            {1, 0, 0, 0, 1},
            {1, 0, 0, 0, 1}
        };

        std::vector<std::vector<int>> n = 
        {
            {0},
            {0},
            {1, 1, 1, 1},
            {1, 0, 0, 0, 1},
            {1, 0, 0, 0, 1},
            {1, 0, 0, 0, 1},
            {1, 0, 0, 0, 1}
        };

        std::vector<std::vector<int>> o = 
        {
            {0},
            {0},
            {0, 1, 1, 1},
            {1, 0, 0, 0, 1},
            {1, 0, 0, 0, 1},
            {1, 0, 0, 0, 1},
            {0, 1, 1, 1}
        };

        std::vector<std::vector<int>> p = 
        {
            {0},
            {0},
            {1, 0, 1, 1},
            {1, 1, 0, 0, 1},
            {1, 0, 0, 0, 1},
            {1, 1, 1, 1},
            {1},
            {1}
        };

        std::vector<std::vector<int>> q = 
        {
            {0},
            {0},
            {0, 1, 1, 0, 1},
            {1, 0, 0, 1, 1},
            {1, 0, 0, 0, 1},
            {0, 1, 1, 1, 1},
            {0, 0, 0, 0, 1},
            {0, 0, 0, 0, 1}
        };

        std::vector<std::vector<int>> r = 
        {
            {0},
            {0},
            {1, 0, 1, 1},
            {1, 1, 0, 0, 1},
            {1},
            {1},
            {1}
        };

        std::vector<std::vector<int>> s = 
        {
            {0},
            {0},
            {0, 1, 1, 1, 1},
            {1},
            {0, 1, 1, 1},
            {0, 0, 0, 0, 1},
            {1, 1, 1, 1}
        };

        std::vector<std::vector<int>> t = 
        {
            {0, 0, 1},
            {0, 1, 1, 1},
            {0, 0, 1},
            {0, 0, 1},
            {0, 0, 1},
            {0, 0, 1},
            {0, 0, 0, 1}
        };

        std::vector<std::vector<int>> u = 
        {
            {0},
            {0},
            {1, 0, 0, 0, 1},
            {1, 0, 0, 0, 1},
            {1, 0, 0, 0, 1},
            {1, 0, 0, 0, 1},
            {0, 1, 1, 1, 1}
        };

        std::vector<std::vector<int>> v = 
        {
            {0},
            {0},
            {1, 0, 0, 0, 1},
            {1, 0, 0, 0, 1},
            {1, 0, 0, 0, 1},
            {0, 1, 0, 1},
            {0, 0, 1}
        };

        std::vector<std::vector<int>> w = 
        {
            {0},
            {0},
            {1, 0, 0, 0, 1},
            {1, 0, 0, 0, 1},
            {1, 0, 1, 0, 1},
            {1, 0, 1, 0, 1},
            {0, 1, 1, 1, 1}
        };

        std::vector<std::vector<int>> lx = 
        {
            {0},
            {0},
            {1, 0, 0, 0, 1},
            {0, 1, 0, 1},
            {0, 0, 1},
            {0, 1, 0, 1},
            {1, 0, 0, 0, 1}
        };

        std::vector<std::vector<int>> ly = 
        {
            {0},
            {0},
            {1, 0, 0, 0, 1},
            {1, 0, 0, 0, 1},
            {1, 0, 0, 0, 1},
            {0, 1, 1, 1, 1},
            {0, 0, 0, 0, 1},
            {1, 1, 1, 1}
        };

        std::vector<std::vector<int>> z = 
        {
            {0},
            {0},
            {1, 1, 1, 1, 1},
            {0, 0, 0, 1},
            {0, 0, 1},
            {0, 1},
            {1, 1, 1, 1, 1}
        };

        std::vector<std::vector<int>> n0 = 
        {
            {0, 1, 1, 1},
            {1, 0, 0, 0, 1},
            {1, 0, 0, 1, 1},
            {1, 0, 1, 0, 1},
            {1, 1, 0, 0, 1},
            {1, 0, 0, 0, 1},
            {0, 1, 1, 1}
        };

        std::vector<std::vector<int>> n1 = 
        {
            {0, 0, 1},
            {0, 1, 1},
            {0, 0, 1},
            {0, 0, 1},
            {0, 0, 1},
            {0, 0, 1},
            {1, 1, 1, 1, 1}
        };

        std::vector<std::vector<int>> n2 = 
        {
            {0, 1, 1, 1},
            {1, 0, 0, 0, 1},
            {0, 0, 0, 0, 1},
            {0, 0, 1, 1},
            {0, 1},
            {1},
            {1, 1, 1, 1, 1}
        };

        std::vector<std::vector<int>> n3 = 
        {
            {0, 1, 1, 1},
            {1, 0, 0, 0, 1},
            {0, 0, 0, 0, 1},
            {0, 0, 1, 1},
            {0, 0, 0, 0, 1},
            {1, 0, 0, 0, 1},
            {0, 1, 1, 1}
        };

        std::vector<std::vector<int>> n4 = 
        {
            {0, 0, 0, 1, 1},
            {0, 0, 1, 0, 1},
            {0, 1, 0, 0, 1},
            {1, 0, 0, 0, 1},
            {1, 1, 1, 1, 1},
            {0, 0, 0, 0, 1},
            {0, 0, 0, 0, 1}
        };

        std::vector<std::vector<int>> n5 = 
        {
            {1, 1, 1, 1, 1},
            {1},
            {1, 1, 1, 1},
            {0, 0, 0, 0, 1},
            {0, 0, 0, 0, 1},
            {0, 0, 0, 0, 1},
            {1, 1, 1, 1}
        };

        std::vector<std::vector<int>> n6 = 
        {
            {0, 0, 1, 1},
            {0, 1},
            {1},
            {1, 1, 1, 1},
            {1, 0, 0, 0, 1},
            {1, 0, 0, 0, 1},
            {0, 1, 1, 1}
        };

        std::vector<std::vector<int>> n7 = 
        {
            {1, 1, 1, 1, 1},
            {1, 0, 0, 0, 1},
            {0, 0, 0, 0, 1},
            {0, 0, 0, 1},
            {0, 0, 1},
            {0, 0, 1},
            {0, 0, 1}
        };

        std::vector<std::vector<int>> n8 = 
        {
            {0, 1, 1, 1},
            {1, 0, 0, 0, 1},
            {1, 0, 0, 0, 1},
            {0, 1, 1, 1},
            {1, 0, 0, 0, 1},
            {1, 0, 0, 0, 1},
            {0, 1, 1, 1}
        };

        std::vector<std::vector<int>> n9 = 
        {
            {0, 1, 1, 1},
            {1, 0, 0, 0, 1},
            {1, 0, 0, 0, 1},
            {0, 1, 1, 1, 1},
            {0, 0, 0, 0, 1},
            {0, 0, 0, 1},
            {0, 1, 1}
        };

        std::vector<std::vector<int>> plus = 
        {
            {0},
            {0, 0, 1},
            {0, 0, 1},
            {1, 1, 1, 1, 1},
            {0, 0, 1},
            {0, 0, 1}
        };

        std::vector<std::vector<int>> minus = 
        {
            {0},
            {0},
            {0},
            {1, 1, 1, 1, 1},
            {0},
            {0}
        };

        std::vector<std::vector<int>> aster = 
        {
            {0},
            {0},
            {0, 1, 0, 0, 1},
            {0, 0, 1, 1},
            {0, 1, 0, 0, 1},
            {0}
        };

        std::vector<std::vector<int>> slash = 
        {
            {0, 0, 0, 0, 1},
            {0, 0, 0, 1},
            {0, 0, 0, 1},
            {0, 0, 1},
            {0, 1},
            {0, 1},
            {1}
        };

        std::vector<std::vector<int>> equal = 
        {
            {0},
            {0},
            {1, 1, 1, 1, 1},
            {0},
            {0},
            {1, 1, 1, 1, 1},
            {0}
        };

        std::vector<std::vector<int>> percent = 
        {
            {1, 0, 0, 0, 1},
            {1, 0, 0, 1},
            {0, 0, 0, 1},
            {0, 0, 1},
            {0, 1},
            {0, 1, 0, 0, 1},
            {1, 0, 0, 0, 1}
        };

        std::vector<std::vector<int>> dquote = 
        {
            {0, 1, 0, 1},
            {0, 1, 0, 1},
            {1, 0, 1}
        };

        std::vector<std::vector<int>> squote = 
        {
            {0, 0, 1},
            {0, 0, 1},
            {0, 1}
        };

        std::vector<std::vector<int>> hash = 
        {
            {0, 1, 0, 1},
            {0, 1, 0, 1},
            {1, 1, 1, 1, 1},
            {0, 1, 0, 1},
            {1, 1, 1, 1, 1},
            {0, 1, 0, 1},
            {0, 1, 0, 1}
        };

        std::vector<std::vector<int>> dollar = 
        {
            {0, 0, 1},
            {0, 1, 1, 1, 1},
            {1},
            {0, 1, 1, 1},
            {0, 0, 0, 0, 1},
            {1, 1, 1, 1},
            {0, 0, 1}
        };

        std::vector<std::vector<int>> at = 
        {
            {0, 1, 1, 1, 1},
            {1, 0, 0, 0, 0, 1},
            {1, 0, 1, 1, 0, 1},
            {1, 0, 1, 1, 0, 1},
            {1, 0, 1, 1, 1, 1},
            {1},
            {0, 1, 1, 1, 1, 1}
        };

        std::vector<std::vector<int>> amper = 
        {
            {0, 0, 1},
            {0, 1, 0, 1},
            {0, 0, 1},
            {0, 1, 1, 0, 1},
            {1, 0, 1, 1},
            {1, 0, 0, 1},
            {0, 1, 1, 0, 1}
        };

        std::vector<std::vector<int>> under = 
        {
            {0},
            {0},
            {0},
            {0},
            {0},
            {0},
            {0},
            {1, 1, 1, 1, 1}
        };

        std::vector<std::vector<int>> LrBra = 
        {
            {0, 0, 1, 1},
            {0, 1},
            {1},
            {1},
            {1},
            {0, 1},
            {0, 0, 1, 1}
        };

        std::vector<std::vector<int>> RrBra = 
        {
            {0, 1, 1},
            {0, 0, 0, 1},
            {0, 0, 0, 0, 1},
            {0, 0, 0, 0, 1},
            {0, 0, 0, 0, 1},
            {0, 0, 0, 1},
            {0, 1, 1}
        };

        std::vector<std::vector<int>> LcBra = 
        {
            {0, 0, 1, 1},
            {0, 1},
            {0, 1},
            {1},
            {0, 1},
            {0, 1},
            {0, 0, 1, 1}
        };

        std::vector<std::vector<int>> RcBra = 
        {
            {0, 1, 1},
            {0, 0, 0, 1},
            {0, 0, 0, 1},
            {0, 0, 0, 0, 1},
            {0, 0, 0, 1},
            {0, 0, 0, 1},
            {0, 1, 1}
        };

        std::vector<std::vector<int>> LaBra = 
        {
            {0, 0, 0, 1},
            {0, 0, 1},
            {0, 1},
            {1},
            {0, 1},
            {0, 0, 1},
            {0, 0, 0, 1}
        };

        std::vector<std::vector<int>> RaBra = 
        {
            {0, 1},
            {0, 0, 1},
            {0, 0, 0, 1},
            {0, 0, 0, 0, 1},
            {0, 0, 0, 1},
            {0, 0, 1},
            {0, 1}
        };

        std::vector<std::vector<int>> LsBra = 
        {
            {1, 1, 1},
            {1},
            {1},
            {1},
            {1},
            {1},
            {1, 1, 1}
        };

        std::vector<std::vector<int>> RsBra = 
        {
            {0, 0, 1, 1, 1},
            {0, 0, 0, 0, 1},
            {0, 0, 0, 0, 1},
            {0, 0, 0, 0, 1},
            {0, 0, 0, 0, 1},
            {0, 0, 0, 0, 1},
            {0, 0, 1, 1, 1}
        };

        std::vector<std::vector<int>> comma = 
        {
            {0},
            {0},
            {0},
            {0},
            {0},
            {0, 1},
            {0, 1},
            {0, 1}
        };

        std::vector<std::vector<int>> period = 
        {
            {0},
            {0},
            {0},
            {0},
            {0},
            {0, 1},
            {0, 1}
        };

        std::vector<std::vector<int>> scolon = 
        {
            {0},
            {0, 1},
            {0, 1},
            {0},
            {0},
            {0, 1},
            {0, 1},
            {0, 1}
        };

        std::vector<std::vector<int>> colon = 
        {
            {0},
            {0, 1},
            {0, 1},
            {0},
            {0},
            {0, 1},
            {0, 1}
        };

        std::vector<std::vector<int>> quest = 
        {
            {0, 1, 1, 1},
            {1, 0, 0, 0, 1},
            {0, 0, 0, 0, 1},
            {0, 0, 0, 1},
            {0, 0, 1},
            {0},
            {0, 0, 1}
        };

        std::vector<std::vector<int>> exclam = 
        {
            {0, 1},
            {0, 1},
            {0, 1},
            {0, 1},
            {0, 1},
            {0},
            {0, 1}
        };

        std::vector<std::vector<int>> caret = 
        {
            {0, 0, 1},
            {0, 1, 0, 1},
            {1, 0, 0, 0, 1}
        };

        std::vector<std::vector<int>> tilde = 
        {
            {0, 1, 1, 0, 0, 1},
            {1, 0, 0, 1, 1}
        };

        std::vector<std::vector<int>> missing = 
        {
            {1, 1, 1, 1, 1},
            {1, 1, 0, 0, 1},
            {1, 1, 0, 1, 1},
            {1, 1, 0, 0, 1},
            {1, 1, 0, 1, 1},
            {1, 1, 0, 1, 1},
            {1, 1, 1, 1, 1}
        };
};
#endif