#pragma once

namespace muyuy
{
    namespace utils
    {

        template <typename T>
        class Singleton
        {
        protected:
            static T *_singleton_reference;

            Singleton()
            {
            }

            virtual ~Singleton()
            {
            }

        public:
            static T *singletonCreate()
            {
                if (_singleton_reference == nullptr)
                    _singleton_reference = new T();
                return _singleton_reference;
            }

            static void singletonDestroy()
            {
                if (_singleton_reference != nullptr)
                    delete _singleton_reference;
                _singleton_reference = nullptr;
            }

            static const T *singletonGetReference()
            {
                return _singleton_reference;
            }

            virtual bool singletonInitialize() = 0;

        private:
            Singleton(const Singleton &s);
            Singleton &operator=(const Singleton &s);
        };

        template <typename T>
        T *Singleton<T>::_singleton_reference = nullptr;

    }
}
