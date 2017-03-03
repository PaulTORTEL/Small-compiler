#ifndef ANALYSER_H
#define ANALYSER_H


class Analyser
{

    private:

    public:
        Analyser();
        virtual ~Analyser();

        bool readGrammar(const char* filename);

};

#endif // ANALYSER_H
