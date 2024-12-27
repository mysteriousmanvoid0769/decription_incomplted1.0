#ifndef DECRYPT_CPP
#define DECRYPT_CPP




#include "decrypt.hpp"


using namespace std;



// Класс для расшифровки текста на основе частот букв
class DECR_CLASS_ENG {
public:
    // Конструктор, принимающий зашифрованный текст
    DECR_CLASS_ENG(const string& encrypted_text) : encrypted_text(encrypted_text) {}
    
    // Основная функция для расшифровки текста
    void decrypt_byfreq() {
        // Подсчёт частоты символов в зашифрованном тексте
        unordered_map<char, double> letter_freq_EncrText = calculate_freq();

        // Сортируем частоты для английского языка и зашифрованного текста
        vector<LetterFreq> sorted_ENG = sort_by_freq(EnglishLang_letter_freq);
        vector<LetterFreq> sorted_Encrypted = sort_by_freq(letter_freq_EncrText);

        // Создаём мапу для сопоставления букв
        letter_mapping_byfreq.clear();  // Очищаем старое значение
        for (size_t i = 0; i < sorted_Encrypted.size(); ++i) {
            letter_mapping_byfreq[sorted_Encrypted[i].letter] = sorted_ENG[i].letter;
        }

        // Расшифровка текста без изменений пользователя
        decrypted_text_byfreq = "";  // Очищаем старое значение
        for (char c : encrypted_text) {
            if (isalpha(c)) {
                char upper_char = toupper(c);
                decrypted_text_byfreq += letter_mapping_byfreq[upper_char];  // Заменяем на соответствующую букву
            } else {
                decrypted_text_byfreq += c;  // Если не буква (например, пробел), оставляем как есть
            }
        }


        decrypted_text_byusermod = decrypted_text_byfreq;
    }
    

    void user_insert(char old_letter, char new_letter){

            // old_letter - Это буква полученная по Частотному Методу, но оказалась неправильно(неудачно) сопоставлена шифрованному тексту(то есть encrypted_text). Она не подошла по смыслу слов(а), и в какой-то степени счиатается полушифрованной xD

            old_letter = toupper(old_letter);
            new_letter = toupper(new_letter);


            letter_mapping_byusermod[old_letter] = new_letter;
    }

 
    void decrypt_byusermod() {
        decrypted_text_byusermod = decrypted_text_byfreq;
        
        for (const auto& entry : letter_mapping_byusermod) {
            for (size_t i = 0; i < decrypted_text_byusermod.size(); ++i) {
                if (isalpha(decrypted_text_byusermod[i]) && toupper(decrypted_text_byusermod[i]) == entry.first) {
                    decrypted_text_byusermod[i] = entry.second;
                }
            }
        }

    }   




    // Метод для получения расшифрованного текста
    string get_decrypted_text_byfreq(bool enable_output = false) const {
        if (enable_output) {
            cout << "\n\n\n----------------------------------------------[Decrypted Text by Frequence Algorithm BEGIN]----------------------------------------------:\n" 
                 << decrypted_text_byfreq << 
                 "\n\n\n----------------------------------------------[Decrypted Text by Frequence Algorithm END]----------------------------------------------\n\n\n";
        }
        return decrypted_text_byfreq;
    }
    

    string get_decrypted_text_byusermod(bool enable_output = false) const {
        if (enable_output) {
            cout << "\n\n\n----------------------------------------------[Decrypted Text by +User Insert BEGIN]----------------------------------------------:\n" 
                 << decrypted_text_byusermod << 
                 "\n\n\n----------------------------------------------[Decrypted Text by +User Insert END]----------------------------------------------\n\n\n";
        }
        return decrypted_text_byusermod;

    }


    unordered_map<char, char> get_letter_mapping_byfreq(bool enable_output = false) const{
        
        if(enable_output){
            for (const auto& entry : letter_mapping_byfreq) {
                cout << "Original: " << entry.first << " -> Mapped: " << entry.second << endl;
            }
        }

        return letter_mapping_byfreq;
    }

    
    unordered_map<char, char> get_letter_mapping_byusermod(bool enable_output = false) const{
            
        if(enable_output){
            for (const auto& entry : letter_mapping_byusermod) {
                cout << "Original: " << entry.first << " -> Mapped: " << entry.second << endl;
            }
        }

        return letter_mapping_byusermod;
    }


private:
    string encrypted_text;  // Зашифрованный текст

    string decrypted_text_byfreq;  // Расшифрованный текст основанный на Частотном Методе (без изменений пользователя)

    string decrypted_text_byusermod;  // Расшифрованный текст (с учётом изменений пользователя)
    
    
    unordered_map<char, char> letter_mapping_byfreq; // Мапа для сопоставления букв основаная на Частотном Методе

    
    unordered_map<char, char> letter_mapping_byusermod; // Мапа для сохранения изменений, сделанных пользователем

    
    static const unordered_map<char, double> EnglishLang_letter_freq; // Статическое константное поле для частот букв в английском языке

    
    unordered_map<char, double> calculate_freq() {
        unordered_map<char, unsigned int> letter_count;
        unsigned int total_letters = 0;

        // Подсчёт частоты букв
        for (char c : encrypted_text) {
            if (isalpha(c)) {
                char upper_char = toupper(c);  // Переводим символ в верхний регистр
                letter_count[upper_char]++;
                total_letters++;
            }
        }

        // Преобразуем подсчитанные частоты в проценты
        unordered_map<char, double> letter_freq;
        for (const auto& entry : letter_count) {
            letter_freq[entry.first] = (double(entry.second) / total_letters) * 100;
        }

        return letter_freq;
    } // Вспомогательная функция для подсчёта частоты букв в тексте

    vector<LetterFreq> sort_by_freq(const unordered_map<char, double>& letter_freq) {
        vector<LetterFreq> sorted_freq;
        for (const auto& entry : letter_freq) {
            sorted_freq.push_back({entry.first, entry.second});
        }
        sort(sorted_freq.begin(), sorted_freq.end(), [](const LetterFreq& a, const LetterFreq& b) {
            return a.freq > b.freq;
        });
        return sorted_freq;
    } // Вспомогательная функция для сортировки частот

};

// Инициализация статического поля для частот букв в английском языке
const unordered_map<char, double> DECR_CLASS_ENG::EnglishLang_letter_freq = {
    {'E', 12.70}, {'T', 9.06}, {'A', 8.17}, {'O', 7.51}, {'I', 6.97},
    {'N', 6.75}, {'S', 6.33}, {'R', 6.03}, {'H', 5.92}, {'D', 4.25},
    {'L', 4.03}, {'U', 3.64}, {'C', 3.33}, {'M', 3.01}, {'F', 2.23},
    {'P', 2.02}, {'G', 1.93}, {'Y', 1.77}, {'B', 1.49}, {'V', 0.98},
    {'K', 0.77}, {'X', 0.15}, {'J', 0.15}, {'Q', 0.10}, {'Z', 0.07},
    {'W', 2.36}
};




string _virtual_encryption_ENG(bool enable_output = false) {
    
    string text_to_encrypt = "In the fast-paced world of today, the ability to manage one’s time effectively has become more crucial than ever before. Whether you are a student, a working professional, or someone balancing both, mastering time management is key to ensuring productivity, reducing stress, and achieving personal and professional success.\
Time management is the process of planning and controlling how much time to spend on specific activities. Good time management allows individuals to complete more in a shorter period, which leads to more free time, and better opportunities in both personal and professional lives. When managed properly, time becomes a resource that works for you, rather than a constraint that holds you back.\
The benefits of time management are clear and undeniable. One of the most significant advantages is the increase in productivity. Effective time management enables individuals to focus on what matters most, ensuring that time is spent efficiently. By organizing tasks and setting priorities, people can minimize distractions, avoid procrastination, and concentrate on achieving goals.\
Another major benefit of time management is stress reduction. When you don’t manage your time effectively, you may find yourself rushing to meet deadlines or struggling to complete tasks. This often leads to feelings of overwhelm, anxiety, and burnout. However, when you have a clear plan and stick to it, you are able to work steadily without feeling pressured. Planning your time ahead of time helps you allocate enough time to each task, reducing the feeling of urgency and stress.\
Moreover, proper time management also fosters a sense of control. When individuals fail to manage their time, they may feel as though they are constantly reacting to circumstances, putting out fires, or dealing with tasks as they come. By taking charge of their schedules, people can take proactive steps toward their goals, rather than simply responding to external demands. This sense of control not only makes life more manageable but also helps individuals feel empowered to make choices about how they want to spend their time.\
One of the first steps to effective time management is setting clear, achievable goals. Whether they are short-term or long-term goals, understanding what you want to achieve and setting a clear roadmap to get there is essential. This can be done by breaking large tasks into smaller, more manageable actions, and scheduling specific times to work on each one. Setting deadlines for yourself also helps you stay on track and focused.\
In addition to setting goals, prioritizing tasks is another important element of time management. Not all tasks are created equal, and it’s essential to focus on the most important or urgent ones first. This means identifying high-priority tasks and addressing them before moving on to less critical activities. By doing this, individuals can ensure that they are making progress on what matters most, rather than getting bogged down in less important tasks.\
A common time management tool is the creation of to-do lists. These lists can be a simple and effective way to stay organized and on top of tasks. To-do lists allow you to write down everything you need to accomplish and check things off as you complete them. This provides a sense of accomplishment and keeps you motivated. However, it’s important not to overwhelm yourself by putting too many tasks on your list. Instead, focus on what can realistically be completed within a given time frame.\
Time blocking is another useful technique for managing time. This method involves scheduling specific blocks of time for certain tasks or activities. For example, you could dedicate a two-hour block in the morning to work on a project, followed by a one-hour block for answering emails, and then a break. Time blocking helps individuals stay focused on one task at a time, reduces multitasking, and prevents distractions. It is an effective strategy for improving productivity and ensuring that tasks are completed on time.\
Another key element of time management is learning how to say no. Many people find themselves overcommitted because they have difficulty turning down requests or offers. However, saying yes to too many things can lead to burnout and a lack of focus. By learning to say no when necessary, individuals can protect their time and energy for what truly matters to them.\
One of the biggest obstacles to effective time management is procrastination. Procrastination is the act of delaying or postponing tasks, often until the last minute. It is a common behavior that many people struggle with, but it can be overcome with a few simple strategies. One method is the Pomodoro Technique, which involves working for a set period of time (typically 25 minutes), followed by a short break. This creates a sense of urgency and helps individuals stay focused. Another strategy is breaking tasks into smaller steps, making them feel less overwhelming and easier to begin.\
While time management is essential for success, it’s also important to allow flexibility in your schedule. Life is unpredictable, and sometimes things don’t go as planned. While it’s important to stick to your goals and deadlines, it’s equally important to adapt to changes and unforeseen circumstances. Being flexible and adjusting your schedule when necessary will help you avoid unnecessary stress and stay on track even when things don’t go according to plan.\
In today’s digital age, technology has made time management easier than ever. There are countless apps, tools, and software that can help individuals stay organized and manage their time more effectively. For example, task management apps like Trello or Asana can help you keep track of projects and deadlines. Calendar apps such as Google Calendar or Outlook can be used to schedule events and appointments. Time-tracking tools like Toggl can help you monitor how much time you spend on various tasks, allowing you to identify areas where you can improve efficiency.\
Another helpful resource is the use of reminders and notifications. Setting alarms or reminders on your phone or computer can help you stay on top of important tasks and deadlines. These tools can help reduce the mental load of remembering everything, allowing you to focus on the task at hand without worrying about forgetting something.\
Ultimately, time management is about making intentional choices and recognizing that time is a finite resource. Everyone has the same 24 hours in a day, and how you choose to use that time can make a huge difference in your life. By prioritizing important tasks, setting clear goals, eliminating distractions, and using time management tools effectively, individuals can create a structured, balanced, and productive lifestyle. As a result, they can achieve success without sacrificing their mental health or well-being.\
In conclusion, time management is not a one-size-fits-all concept; it is a skill that can be tailored to each person’s unique needs and preferences. Whether you are aiming to improve productivity at work, manage your personal life more effectively, or simply find more time for relaxation and hobbies, the principles of time management are invaluable. The sooner you start implementing time management techniques into your life, the sooner you will experience the benefits of increased efficiency, reduced stress, and a greater sense of control over your time.";
    

  // Текст, который нужно зашифровать

    // Мапа для сопоставления букв английского алфавита с буквами шифра
    unordered_map<char, char> letter_mapping = {
        {'A', 'Q'}, {'B', 'W'}, {'C', 'E'}, {'D', 'R'}, {'E', 'T'},
        {'F', 'Y'}, {'G', 'U'}, {'H', 'I'}, {'I', 'O'}, {'J', 'P'},
        {'K', 'A'}, {'L', 'S'}, {'M', 'D'}, {'N', 'F'}, {'O', 'G'},
        {'P', 'H'}, {'Q', 'J'}, {'R', 'K'}, {'S', 'L'}, {'T', 'Z'},
        {'U', 'X'}, {'V', 'C'}, {'W', 'V'}, {'X', 'B'}, {'Y', 'N'},
        {'Z', 'M'}
    };

    // Шифруем текст с использованием мапы
    string encrypted_text = "";
    for (char c : text_to_encrypt) {
        if (isalpha(c)) {  // Если символ — буква
            char upper_char = toupper(c);  // Переводим в верхний регистр
            encrypted_text += letter_mapping[upper_char];  // Заменяем на шифрованную букву
        } else {
            encrypted_text += c;  // Если не буква (например, пробел), оставляем как есть
        }
    }

    // Выводим зашифрованный текст
    if(enable_output){
        cout << "\n\n\n----------------------------------------------[Virtual Encrypted Text BEGIN]----------------------------------------------:\n" << encrypted_text << "\n\n\n----------------------------------------------[Virtual Encrypted Text END]----------------------------------------------\n\n\n";
    }


    return encrypted_text;
}






#endif //DECRYPT_CPP


/*

#include "decrypt.hpp"



DECR_CLASS_ENG::DECR_CLASS_ENG(const string& encrypted_text) : encrypted_text(encrypted_text) {}

void DECR_CLASS_ENG::decrypt_byfreq() {
    unordered_map<char, double> letter_freq_EncrText = calculate_freq();
    vector<LetterFreq> sorted_ENG = sort_by_freq(EnglishLang_letter_freq);
    vector<LetterFreq> sorted_Encrypted = sort_by_freq(letter_freq_EncrText);

    letter_mapping_byfreq.clear();
    for (size_t i = 0; i < sorted_Encrypted.size(); ++i) {
        letter_mapping_byfreq[sorted_Encrypted[i].letter] = sorted_ENG[i].letter;
    }

    decrypted_text_byfreq = "";
    for (char c : encrypted_text) {
        if (isalpha(c)) {
            char upper_char = toupper(c);
            decrypted_text_byfreq += letter_mapping_byfreq[upper_char];
        } else {
            decrypted_text_byfreq += c;
        }
    }

    decrypted_text_byusermod = decrypted_text_byfreq;
}

void DECR_CLASS_ENG::user_insert(char old_letter, char new_letter) {
    old_letter = toupper(old_letter);
    new_letter = toupper(new_letter);
    letter_mapping_byusermod[old_letter] = new_letter;
}

void DECR_CLASS_ENG::decrypt_byusermod() {
    decrypted_text_byusermod = decrypted_text_byfreq;
    
    for (const auto& entry : letter_mapping_byusermod) {
        for (size_t i = 0; i < decrypted_text_byusermod.size(); ++i) {
            if (isalpha(decrypted_text_byusermod[i]) && toupper(decrypted_text_byusermod[i]) == entry.first) {
                decrypted_text_byusermod[i] = entry.second;
            }
        }
    }
}

string DECR_CLASS_ENG::get_decrypted_text_byfreq(bool enable_output) const {
    if (enable_output) {
        cout << "\n\n\n----------------------------------------------[Decrypted Text by Frequence Algorithm BEGIN]----------------------------------------------:\n" 
             << decrypted_text_byfreq << 
             "\n\n\n----------------------------------------------[Decrypted Text by Frequence Algorithm END]----------------------------------------------\n\n\n";
    }
    return decrypted_text_byfreq;
}

string DECR_CLASS_ENG::get_decrypted_text_byusermod(bool enable_output) const {
    if (enable_output) {
        cout << "\n\n\n----------------------------------------------[Decrypted Text by +User Insert BEGIN]----------------------------------------------:\n" 
             << decrypted_text_byusermod << 
             "\n\n\n----------------------------------------------[Decrypted Text by +User Insert END]----------------------------------------------\n\n\n";
    }
    return decrypted_text_byusermod;
}

unordered_map<char, char> DECR_CLASS_ENG::get_letter_mapping_byfreq(bool enable_output) const {
    if (enable_output) {
        for (const auto& entry : letter_mapping_byfreq) {
            cout << "Original: " << entry.first << " -> Mapped: " << entry.second << endl;
        }
    }
    return letter_mapping_byfreq;
}

unordered_map<char, char> DECR_CLASS_ENG::get_letter_mapping_byusermod(bool enable_output) const {
    if (enable_output) {
        for (const auto& entry : letter_mapping_byusermod) {
            cout << "Original: " << entry.first << " -> Mapped: " << entry.second << endl;
        }
    }
    return letter_mapping_byusermod;
}

unordered_map<char, double> DECR_CLASS_ENG::calculate_freq() {
    unordered_map<char, unsigned int> letter_count;
    unsigned int total_letters = 0;

    for (char c : encrypted_text) {
        if (isalpha(c)) {
            char upper_char = toupper(c);
            letter_count[upper_char]++;
            total_letters++;
        }
    }

    unordered_map<char, double> letter_freq;
    for (const auto& entry : letter_count) {
        letter_freq[entry.first] = (double(entry.second) / total_letters) * 100;
    }

    return letter_freq;
}

vector<LetterFreq> DECR_CLASS_ENG::sort_by_freq(const unordered_map<char, double>& letter_freq) {
    vector<LetterFreq> sorted_freq;
    for (const auto& entry : letter_freq) {
        sorted_freq.push_back({entry.first, entry.second});
    }
    sort(sorted_freq.begin(), sorted_freq.end(), [](const LetterFreq& a, const LetterFreq& b) {
        return a.freq > b.freq;
    });
    return sorted_freq;
}

const unordered_map<char, double> DECR_CLASS_ENG::EnglishLang_letter_freq = {
    {'E', 12.70}, {'T', 9.06}, {'A', 8.17}, {'O', 7.51}, {'I', 6.97},
    {'N', 6.75}, {'S', 6.33}, {'R', 6.03}, {'H', 5.92}, {'D', 4.25},
    {'L', 4.03}, {'U', 3.64}, {'C', 3.33}, {'M', 3.01}, {'F', 2.23},
    {'P', 2.02}, {'G', 1.93}, {'Y', 1.77}, {'B', 1.49}, {'V', 0.98},
    {'K', 0.77}, {'X', 0.15}, {'J', 0.15}, {'Q', 0.10}, {'Z', 0.07},
    {'W', 2.36}
};


*/