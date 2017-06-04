#ifndef SBEXAMPLES_APP_H
#define SBEXAMPLES_APP_H

namespace sb {
    class app {
    public:
        app() { }

        void render(double current_time) {
            do_render(current_time);
        }
        virtual ~app() { }

    private:
      virtual void do_render(double current_time) = 0;
    };
}

#endif //SBEXAMPLES_APP_H
