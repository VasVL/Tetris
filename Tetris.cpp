#include <iostream>
#include <string>
#include <chrono>
#include <windows.h>
#include <math.h>
#include <conio.h>


class Field
{
private:
    int score = 0;
    int lvl = 1;
    static const int fieldSize = 23;
    static const int gameFieldLength = 20;
    static const int gameFieldWidth = 10;

    //  Это поле отрисовываем
    std::string field[fieldSize] = 
    {
        "|          |            \n",
        "|          |            \n",
        "|          |            \n",
        "|          |            \n",
        "|          |            \n",
        "|          |            \n",
        "|          |            \n",
        "|          |            \n",
        "|          |            \n",
        "|          |    Next: ",
        "|          |          ",
        "|          |          ",
        "|          |          ",
        "|          |            \n",
        "|          |            \n",
        "|          |            \n",
        "|          |            \n",
        "|          |    Level:  ",
        "|          |            \n",
        "|          |    Score:  ",
        " TTTTTTTTTT             \n",
        "                        \n",
        "                        \n"
    };

    //  Это поле для добавления в кадр кучи внизу
    std::string fullField[fieldSize] =
    {
        "|          |            \n",
        "|          |            \n",
        "|          |            \n",
        "|          |            \n",
        "|          |            \n",
        "|          |            \n",
        "|          |            \n",
        "|          |            \n",
        "|          |            \n",
        "|          |    Next: ",
        "|          |          ",
        "|          |          ",
        "|          |          ",
        "|          |            \n",
        "|          |            \n",
        "|          |            \n",
        "|          |            \n",
        "|          |    Level:  ",
        "|          |            \n",
        "|          |    Score:  ",
        " TTTTTTTTTT             \n",
        "                        \n",
        "                        \n"
    };

public:
    Field()
    {
    }

    const std::string* getFullField() const
    {
        return fullField;
    }
    int getGameFieldLength()
    {
        return gameFieldLength;
    }
    int getGameFieldWidth()
    {
        return gameFieldWidth;
    }

    //   Рисует всё поле, со счётом, левелом с следующей фигурой. Фигуру принимает как параметр.
    void Print(std::string* figure)
    {
        int i = 0;      //  Номер строки
        for (auto line : field)
        {
            i++;
            //  
            switch (i)
            {
                //  В этих 4-х строках рисуется следующая фигура
            case 10:
                std::cout << line << figure[0];
                break;
            case 11:
                std::cout << line << figure[1];
                break;
            case 12:
                std::cout << line << figure[2];
                break;
            case 13:
                std::cout << line << figure[3];
                break;
                //  В 18-й Левел
            case 18:
                std::cout << line << lvl << '\n';
                break;
                //  В 20-й Счёт
            case 20:
                std::cout << line << score << '\n';
                break;
            default:
                std::cout << line;
                break;
            }
        }
    }

    //  Добавляет падающую фигуру на каждый кадр
    void AddFigure(int x, int y, std::string* fig, int strSize)
    {
        for (int figY = 0; figY < strSize; figY++)
        {
            for (int figX = 0; figX < strSize; figX++)
            {
                if (fig[figY][figX] == '%')
                {
                    field[y+figY][x+figX] = '%';
                }
            }                
        }
    }

    //  Очищает новый кадр от падающей фигуры
    void ClearField()
    {
        for (int fieY = 0; fieY < gameFieldLength; fieY++)
        {
            for (int fieX = 1; fieX < gameFieldWidth + 1; fieX++)   // +1 пушто рамочки
            {
                field[fieY][fieX] = fullField[fieY][fieX];
            }
        }
    }

    void setScore(int score)
    {
        Field::score = score;
    }
    void setLevel(int lvl)
    {
        Field::lvl = lvl;
    }

    //  Проверка на пересечение со стенами
    bool isWallCollision(int x, int y, std::string* fig, int strSize)
    {
        for (int figY = 0; figY < strSize; figY++)
        {
            for (int figX = 0; figX < strSize; figX++)
            {
                if ((fig[figY][figX] == '%') && ((field[y+figY][x+figX] == '|') || (field[y+figY][x+figX] == '%')))
                    return true;  
            }
        }
        return false;
    }

    //  Проверяем дошла ли фигура до низу, если дошла, добавляем её в кучу
    bool isDownCollision(int x, int y, std::string* fig, int strSize)
    {
        for (int figY = 0; figY < strSize; figY++)
        {
            for (int figX = 0; figX < strSize; figX++)
            {
                if ((fig[figY][figX] == '%') && (field[y + figY][x + figX] != ' '))
                {
                    for (int figY = 0; figY < strSize; figY++)
                    {
                        for (int figX = 0; figX < strSize; figX++)
                        {
                            if (fig[figY][figX] == '%')
                            {
                                fullField[y + figY-1][x + figX] = '%';
                            }
                        }
                    }
                    return true;
                }
            }
        }
        return false;
    }

    //  У даляет полные строки из кучи
    void BigBaDaBoom(int start)
    {
        for (int fieY = start; fieY > 1; fieY--)
        {
            for (int fieX = 1; fieX < gameFieldWidth + 1; fieX++)   //  
            {
                    fullField[fieY][fieX] = fullField[fieY - 1][fieX];  
            }
        }
    }
};



class Figure
{
public:
    enum class figureName
    {
        figI,
        figT,
        figCube,
        figSeven,
        figSevenL,
        figZ,
        figZL,
        figNumber,
        figIRot,
        figTRot,
        figTRotX2,
        figTRotX3,
        figSevenRot,
        figSevenRotX2,
        figSevenRotX3,
        figSevenLRot,
        figSevenLRotX2,
        figSevenLRotX3,
        figZRot,
        figZLRot
    };

private:
    static const int figureSize = 4;    //  4 - чтобы влезла палка
    std::string* figure = new std::string[figureSize];   
    int positionX;
    int positionY;
    figureName name;
    
   
    //  Это всё лучше бы вынести куда-нибудь отдельно
    std::string figI[figureSize] =
    {
        "%%%%\n",
        "    \n",
        "    \n",
        "    \n"
    };
    std::string figIRot[figureSize] =
    {
        " %  \n",
        " %  \n",
        " %  \n",
        " %  \n"
    };
    std::string figT[figureSize] =
    {
        " %  \n",
        "%%% \n",
        "    \n",
        "    \n"
    };
    std::string figTRot[figureSize] =
    {
        " %  \n",
        " %% \n",
        " %  \n",
        "    \n"
    };
    std::string figTRotX2[figureSize] =
    {
        "%%% \n",
        " %  \n",
        "    \n",
        "    \n"
    };
    std::string figTRotX3[figureSize] =
    {
        "  % \n",
        " %% \n",
        "  % \n",
        "    \n"
    };
    std::string figCube[figureSize] =
    {
        " %% \n",
        " %% \n",
        "    \n",
        "    \n"
    };
    std::string figSeven[figureSize] =
    {
        "%%% \n",
        "%   \n",
        "    \n",
        "    \n"
    };
    std::string figSevenRot[figureSize] =
    {
        " %% \n",
        "  % \n",
        "  % \n",
        "    \n"
    };
    std::string figSevenRotX2[figureSize] =
    {
        "  % \n",
        "%%% \n",
        "    \n",
        "    \n"
    };
    std::string figSevenRotX3[figureSize] =
    {
        " %  \n",
        " %  \n",
        " %% \n",
        "    \n"
    };
    std::string figSevenL[figureSize] =
    {
        "%%% \n",
        "  % \n",
        "    \n",
        "    \n"
    };
    std::string figSevenLRot[figureSize] =
    {
        "  % \n",
        "  % \n",
        " %% \n",
        "    \n"
    };
    std::string figSevenLRotX2[figureSize] =
    {
        "%   \n",
        "%%% \n",
        "    \n",
        "    \n"
    };
    std::string figSevenLRotX3[figureSize] =
    {
        " %% \n",
        " %  \n",
        " %  \n",
        "    \n"
    };
    std::string figZ[figureSize] =
    {
        "%%  \n",
        " %% \n",
        "    \n",
        "    \n"
    };
    std::string figZRot[figureSize] =
    {
        "  % \n",
        " %% \n",
        " %  \n",
        "    \n"
    };
    std::string figZL[figureSize] =
    {
        " %% \n",
        "%%  \n",
        "    \n",
        "    \n"
    };
    std::string figZLRot[figureSize] =
    {
        " %  \n",
        " %% \n",
        "  % \n",
        "    \n"
    };

    void ChangeFigure(std::string* newFigure, figureName newName)
    {
        for (int i = 0; i < figureSize; i++)
            figure[i] = newFigure[i];
        name = newName;
    }

public:
    //  Самому стыдно
    Figure(figureName fName, int x = 4, int y = 0) : name(fName), positionX(x), positionY(y)
    {
        switch (fName)
        {
        case figureName::figI:
            for (int i = 0; i < figureSize; i++)
                figure[i] = figI[i];
            break;
        case figureName::figT:
            for (int i = 0; i < figureSize; i++)
                figure[i] = figT[i];
            break;
        case figureName::figCube:
            for (int i = 0; i < figureSize; i++)
                figure[i] = figCube[i];
            break;
        case figureName::figSeven:
            for (int i = 0; i < figureSize; i++)
                figure[i] = figSeven[i];
            break;
        case figureName::figSevenL:
            for (int i = 0; i < figureSize; i++)
                figure[i] = figSevenL[i];
            break;
        case figureName::figZ:
            for (int i = 0; i < figureSize; i++)
                figure[i] = figZ[i];
            break;
        case figureName::figZL:
            for (int i = 0; i < figureSize; i++)
                figure[i] = figZL[i];
            break;
        default:
            for (int i = 0; i < figureSize; i++)
                figure[i] = "%%%%";
            break;
        }       
    }
    ~Figure()   //  Проверить деструктор, потому что кажется, что что-то не так
    {           
        delete[] figure;
    }

    std::string* getFigure()
    {
        return figure;
    }
    int getPositionX()
    {
        return positionX;
    }
    int getPositionY()
    {
        return positionY;
    }
    int getSize()
    {
        return figureSize;
    }
    figureName getFigureName()
    {
        return name;
    }

    void setPosition(int x, int y)
    {
        positionX = x;
        positionY = y;
    }
    void setFigure(std::string* fig)
    {
        figure = fig;
    }
    void setFigureName(figureName fig)
    {
        name = fig;
    }

    //  Направление поворота: r - по часовой, l - против часовой
    void Rotate(figureName fName, char direction)
    {
        switch (fName)
        {
        case figureName::figI:
            if (positionY < 17) //  Чтоб под пол не провалиться
                ChangeFigure(figIRot, figureName::figIRot);
            break;
        case figureName::figIRot:
            if ((positionX > 0) && (positionX < 8))    //  Чтоб не вылезти за стены
                ChangeFigure(figI, figureName::figI);
            break;
        case figureName::figT:
            if (direction == 'r')
                ChangeFigure(figTRot, figureName::figTRot);
            else
                ChangeFigure(figTRotX3, figureName::figTRotX3);
            break;
        case figureName::figTRot:
            if (positionX > 0)
            {
                if (direction == 'r')
                    ChangeFigure(figTRotX2, figureName::figTRotX2);
                else
                    ChangeFigure(figT, figureName::figT);
            }
            break;
        case figureName::figTRotX2:
            if (direction == 'r')
                ChangeFigure(figTRotX3, figureName::figTRotX3);
            else
                ChangeFigure(figTRot, figureName::figTRot);
            break;
        case figureName::figTRotX3:
            if (positionX > 0)
            {
                if (direction == 'r')
                    ChangeFigure(figT, figureName::figT);
                else
                    ChangeFigure(figTRotX2, figureName::figTRotX2);
            }
            break;
        case figureName::figSeven:
            if (direction == 'r')
                ChangeFigure(figSevenRot, figureName::figSevenRot);
            else
                ChangeFigure(figSevenRotX3, figureName::figSevenRotX3);
            break;
        case figureName::figSevenRot:
            if (positionX > 0)
            {
                if (direction == 'r')
                    ChangeFigure(figSevenRotX2, figureName::figSevenRotX2);
                else
                    ChangeFigure(figSeven, figureName::figSeven);
            }
            break;
        case figureName::figSevenRotX2:
            if (direction == 'r')
                ChangeFigure(figSevenRotX3, figureName::figSevenRotX3);
            else
                ChangeFigure(figSevenRot, figureName::figSevenRot);
            break;
        case figureName::figSevenRotX3:
            if (positionX > 0)
            {
                if (direction == 'r')
                    ChangeFigure(figSeven, figureName::figSeven);
                else
                    ChangeFigure(figSevenRotX2, figureName::figSevenRotX2);
            }
            break;
        case figureName::figSevenL:
            if (direction == 'r')
                ChangeFigure(figSevenLRot, figureName::figSevenLRot);
            else
                ChangeFigure(figSevenLRotX3, figureName::figSevenLRotX3);
            break;
        case figureName::figSevenLRot:
            if (positionX > 0)
            {
                if (direction == 'r')
                    ChangeFigure(figSevenLRotX2, figureName::figSevenLRotX2);
                else
                    ChangeFigure(figSevenL, figureName::figSevenL);
            }
            break;
        case figureName::figSevenLRotX2:
            if (direction == 'r')
                ChangeFigure(figSevenLRotX3, figureName::figSevenLRotX3);
            else
                ChangeFigure(figSevenLRot, figureName::figSevenLRot);
            break;
        case figureName::figSevenLRotX3:
            if (positionX > 0)
            {
                if (direction == 'r')
                    ChangeFigure(figSevenL, figureName::figSevenL);
                else
                    ChangeFigure(figSevenLRotX2, figureName::figSevenLRotX2);
            }
            break;
        case figureName::figZ:
            ChangeFigure(figZRot, figureName::figZRot);
            break;
        case figureName::figZRot:
            if (positionX > 0)
                ChangeFigure(figZ, figureName::figZ);
            break;
        case figureName::figZL:
            ChangeFigure(figZLRot, figureName::figZLRot);
            break;
        case figureName::figZLRot:
            if (positionX > 0)
                ChangeFigure(figZL, figureName::figZL);
            break;
        default:
            break;
        }
    }
};



int main()
{
    bool isGame = true;

    Field tetris;

    //  Начальные координаты любой фигуры (примерно по-центру)
    int startPositionX = 4;
    int startPositionY = 0;

    int x = startPositionX;
    int y = startPositionY;

    //  Время на поворот, сдвиг и движение фигуры в милисекундах 
    int timeForRotate = 150;
    int timeForMove = 75;
    int gameSpeed = 300;
    int gameSpeedChange = 20;

    int score = 0;
    int level = 1;
    int maxLevel = 10;

    std::string fullString = "|%%%%%%%%%%|";

    long core = std::chrono::system_clock::now().time_since_epoch().count();
    srand(core);

    int nextFig = rand() % 7;   //  Фигур 7

    while (isGame)
    {      
        Figure figure(static_cast<Figure::figureName>(nextFig));
        nextFig = rand() % 7;

        auto timeStart = std::chrono::system_clock::now();
        auto timeCheckStartRot = std::chrono::system_clock::now();
        auto timeCheckStartMove = std::chrono::system_clock::now();
        auto timeEnd = std::chrono::system_clock::now();
        auto timeCheckEndRot = std::chrono::system_clock::now();
        auto timeCheckEndMove = std::chrono::system_clock::now();
        
        //  В этом цикле живёт одна фигура
        while (!(tetris.isDownCollision(x, y, figure.getFigure(), figure.getSize())))
        {
            timeCheckEndRot = std::chrono::system_clock::now();
            if (GetAsyncKeyState(65) && (std::chrono::duration_cast<std::chrono::milliseconds>(timeCheckEndRot - timeCheckStartRot).count() > timeForRotate))
            {       //      65 - A
                figure.Rotate(figure.getFigureName(), 'l');
                timeCheckStartRot = std::chrono::system_clock::now();
            }
            if (GetAsyncKeyState(68) && (std::chrono::duration_cast<std::chrono::milliseconds>(timeCheckEndRot - timeCheckStartRot).count() > timeForRotate))
            {       //      68 - D
                figure.Rotate(figure.getFigureName(), 'r');
                timeCheckStartRot = std::chrono::system_clock::now();
            }
            timeCheckEndMove = std::chrono::system_clock::now();
            if (GetAsyncKeyState(VK_LEFT) && (std::chrono::duration_cast<std::chrono::milliseconds>(timeCheckEndMove - timeCheckStartMove).count() > timeForMove))
            {
                if(!tetris.isWallCollision(x-1, y, figure.getFigure(), figure.getSize()))
                    figure.setPosition(--x, y);
                timeCheckStartMove = std::chrono::system_clock::now();
            }
            if (GetAsyncKeyState(VK_RIGHT) && (std::chrono::duration_cast<std::chrono::milliseconds>(timeCheckEndMove - timeCheckStartMove).count() > timeForMove))
            {
                if (!tetris.isWallCollision(x+1, y, figure.getFigure(), figure.getSize()))
                    figure.setPosition(++x, y);
                timeCheckStartMove = std::chrono::system_clock::now();
            }
            timeEnd = std::chrono::system_clock::now();
            if ((std::chrono::duration_cast<std::chrono::milliseconds>(abs(timeEnd - timeStart)).count() > gameSpeed))
            {
                timeStart = timeEnd;
                if (!(tetris.isDownCollision(x, y, figure.getFigure(), figure.getSize())))
                    figure.setPosition(x, y++);
            }

            //  Добавляем фигуру на кадр
            tetris.AddFigure(figure.getPositionX(), figure.getPositionY(), figure.getFigure(), figure.getSize());

            //  Рисуем кадр. nextFigure - сделующая фигура (логично)
            Figure nextFigure(static_cast<Figure::figureName>(nextFig));
            tetris.Print(nextFigure.getFigure());

            //  Удаляем фигуру с кадра
            tetris.ClearField();
            
            //  Минипуляции со счётом, уровнем и кадром при заполнении строки
            const std::string* test = tetris.getFullField();
            for(int i = 1; i < tetris.getGameFieldLength(); i++)
                if (!strncmp(test[i].c_str(), fullString.c_str(), tetris.getGameFieldWidth() + 1))  //  +1 - пушто рамочка
                {
                    tetris.BigBaDaBoom(i);
                    tetris.setScore(++score);
                    if ((!(score % 10)) && (level <= maxLevel))
                    {
                        tetris.setLevel(++level);
                        gameSpeed -= gameSpeedChange;
                    }
                }

            system("cls"); 
            
        }
        
        //  Проверяем не проиграли ли мы
        for (int i = 1; i < tetris.getGameFieldWidth() + 1; i++)        //  1, +1 - пушто рамочка
        {
            if (tetris.getFullField()[0][i] == '%')
            {
                isGame = false;
                break;
            }
        }

        y = startPositionY;
        x = startPositionX;
    }

    system("cls");
    std::cout << "Score: " << score << std::endl;
    _getch();

    return 0;
}