/* ------------------------------------------------------------------------------------ *
 *                                                                                      *
 * EPITECH PROJECT - Sun, Nov, 2024                                                     *
 * Title           - GameEngine                                                         *
 * Description     -                                                                    *
 *     Script                                                                           *
 *                                                                                      *
 * ------------------------------------------------------------------------------------ *
 *                                                                                      *
 *         ░        ░       ░░        ░        ░        ░░      ░░  ░░░░  ░             *
 *         ▒  ▒▒▒▒▒▒▒  ▒▒▒▒  ▒▒▒▒  ▒▒▒▒▒▒▒  ▒▒▒▒  ▒▒▒▒▒▒▒  ▒▒▒▒  ▒  ▒▒▒▒  ▒             *
 *         ▓      ▓▓▓       ▓▓▓▓▓  ▓▓▓▓▓▓▓  ▓▓▓▓      ▓▓▓  ▓▓▓▓▓▓▓        ▓             *
 *         █  ███████  ██████████  ███████  ████  ███████  ████  █  ████  █             *
 *         █        █  ███████        ████  ████        ██      ██  ████  █             *
 *                                                                                      *
 * ------------------------------------------------------------------------------------ */

#ifndef B_CPP_500_STG_5_1_RTYPE_NOE_TRITSCH_SCRIPT_HPP
    #define B_CPP_500_STG_5_1_RTYPE_NOE_TRITSCH_SCRIPT_HPP

    #include <unordered_map>
    #include <string>
    #ifdef _WIN32
        #include <Python.h>
        #include <basetsd.h>
        typedef SSIZE_T ssize_t;
    #else
        #include <python3.11/Python.h>
    #endif

namespace ecs
{
    namespace cmp
    {

        struct Script {
            using scriptData = std::unordered_map<std::string, int>;

            std::unordered_map<std::string, std::pair<PyObject *, bool>> _scriptModules;
            scriptData scriptVariables;

            Script(const std::string &scriptFilepath = "", const scriptData &variables = {})
            {
                if (!scriptFilepath.empty())
                    addScript(scriptFilepath);
                for (auto &[id, value] : variables)
                    scriptVariables[id] = value;
            }

            ~Script()
            {
                //for (auto &&[pyObj, _] : _scriptModules) TODO: delete all allocated objects
                //    Py_DECREF(pyObj);
                _scriptModules.clear();
            }

            PyObject *mapToDict(const scriptData &map)
            {
                PyObject *dict = PyDict_New();

                for (auto &[name, value] : map)
                    PyDict_SetItemString(dict, name.c_str(), PyLong_FromLong(value));
                return dict;
            }

            void tupleToMap(PyObject *tuple, scriptData &map)
            {
                if (!PyDict_Check(tuple))
                    return;
                PyObject *key;
                PyObject *value;
                ssize_t i = 0;
                std::string keyStr;

                while (PyDict_Next(tuple, &i, &key, &value)) {
                    keyStr = std::string(PyUnicode_AsUTF8(key));
                    if (PyLong_Check(value)) {
                        int valueOutput = static_cast<int>(PyLong_AsLong(value));
                        map[keyStr] = valueOutput;
                    }
                }
            }

            void addScript(const std::string &scriptFilepath)
            {
                if (_scriptModules.contains(scriptFilepath))
                    return;

                PyObject *name = PyUnicode_DecodeFSDefault(scriptFilepath.c_str());
                if (!name)
                    return;

                PyObject *module = PyImport_Import(name);
                Py_DECREF(name);

                if (!module) {
                    PyErr_Print();
                    return;
                }

                _scriptModules[scriptFilepath] = std::pair(module, true);
            }

            bool removeScript(const std::string &scriptFilepath)
            {
                if (!_scriptModules.contains(scriptFilepath))
                    return false;
                _scriptModules.erase(scriptFilepath);

                return true;
            }

            void runScripts(const std::string &functionName = "run")
            {
                for (auto &[name, data] : _scriptModules) {
                    PyObject *func = PyObject_GetAttrString(data.first, functionName.c_str());
                    if (func && PyCallable_Check(func)) {
                        PyObject *tupleArgs = mapToDict(scriptVariables);
                        PyObject *output = PyObject_CallObject(func, PyTuple_Pack(1, tupleArgs));
                        if (output && PyDict_Check(output))
                            tupleToMap(output, scriptVariables);
                        else
                            PyErr_Print();
                        Py_DECREF(tupleArgs);
                    }
                    Py_XDECREF(func);
                }
            }
        };
    };
};

#endif /* B_CPP_500_STG_5_1_RTYPE_NOE_TRITSCH_SCRIPT_HPP */
