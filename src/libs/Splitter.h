#include <string.h>

#ifndef Splitter_h
#define Splitter_h

class Splitter
{
  private:
    int length;
    String *results;

    void parts(String characters, char delimiter)
    {
      int total = 1;
      int charactersLength = (int)characters.length();

      for (int t = 0; t < charactersLength; t++)
      {
        if (characters[t] == delimiter)
        {
          total++;
        }
      }

      this->length = total;
    }

    void split(String characters, char delimiter)
    {
      this->results = new String[this->length];
      String accumulator = "";
      int charactersLength = (int)characters.length();
      int index = 0;

      for (int i = 0; i < charactersLength; i++)
      {
        if (characters[i] != delimiter)
        {
          accumulator = accumulator + characters[i];
        }

        bool lastCharacter = i + 1 == charactersLength;

        if (characters[i] == delimiter || lastCharacter)
        {
          this->results[index] = accumulator;
          accumulator = "";
          index++;
        }
      }
    }

  public:
    Splitter(String characters, char delimiter)
    {
      this->parts(characters, delimiter);
      this->split(characters, delimiter);
    }

    int size()
    {
      return this->length;
    }

    String *array()
    {
      return this->results;
    }

    void show() {
      Serial.println("Array content:");
      
      for (int i = 0; i < this->length; i++)
      {
        Serial.print("[");
        Serial.print(i);
        Serial.print("]: ");
        Serial.print(this->results[i]);
        Serial.println(" ");
      }
    }

    ~Splitter()
    {
      delete[] this->results;
    }
};

#endif /*Splitter_h*/