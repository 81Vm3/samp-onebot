#include "CCallback.h"

#include "CCallbackManager.h"
#include "logprintf.h"
#include "utf8.h"

Callback_t CCallback::Create(AMX *amx, const char *name, const char *format,
                             cell *params, cell param_offset) {
    if (amx == nullptr) {
        logprintf("invalid AMX");
        return nullptr;
    }

    if (params == nullptr) {
        logprintf("invalid parameters");
        return nullptr;
    }

    if (name == nullptr || strlen(name) == 0) {
        logprintf("empty name specified");
        return nullptr;
    }

    int cb_idx = -1;
    if (amx_FindPublic(amx, name, &cb_idx) != AMX_ERR_NONE || cb_idx < 0) {
        logprintf("callback \"%s\" does not exist", name);
        return nullptr;
    }

    const size_t num_params = (format == nullptr) ? 0 : strlen(format);
    if ((params[0] / sizeof(cell) - (param_offset - 1)) != num_params) {
        logprintf("parameter count does not match format specifier length");
        return nullptr;
    }

    ParamList_t param_list;
    if (num_params != 0) {
        cell param_idx = 0;
        cell *address_ptr = nullptr;
        cell *array_addr_ptr = nullptr;

        do {
            if (array_addr_ptr != nullptr && (*format) != 'd' &&
                (*format) != 'i') {
                logprintf("expected 'd'/'i' specifier for array size");
                return nullptr;
            }

            switch (*format) {
                case 'd':  // decimal
                case 'i':  // integer
                {
                    amx_GetAddr(amx, params[param_offset + param_idx],
                                &address_ptr);
                    cell value = *address_ptr;
                    // 前面传入了一个数组指针，说明当前的整数是数组的大小
                    if (array_addr_ptr != nullptr) {
                        if (value <= 0) {
                            logprintf("invalid array size '%d'", value);
                            return nullptr;
                        }

                        // 记得要释放copied_array
                        cell *copied_array =
                            static_cast<cell *>(malloc(value * sizeof(cell)));
                        memcpy(copied_array, array_addr_ptr,
                               value * sizeof(cell));

                        // origin method
                        /* param_list.push_front(std::make_tuple(
                            'a', std::make_tuple(copied_array, value))); */

                        // void method
                        // param_list.emplace_front(
                        //    'a', std::pair<cell *, cell>(copied_array,
                        //    value));
                        param_list.emplace_front(
                            'a', std::make_any<std::pair<cell *, cell>>(
                                     copied_array, value));
                        array_addr_ptr = nullptr;
                    }

                    param_list.emplace_front('c', std::make_any<cell>(value));
                } break;
                case 'f':  // float
                case 'b':  // bool
                    amx_GetAddr(amx, params[param_offset + param_idx],
                                &address_ptr);
                    param_list.emplace_front('c',
                                             std::make_any<cell>(*address_ptr));
                    break;
                case 's':  // string
                {
                    const char *str = nullptr;
                    amx_StrParam(amx, params[param_offset + param_idx], str);

                    param_list.emplace_front(
                        's', std::make_any<std::string>(str ? str : ""));
                } break;
                case 'a':  // array
                    amx_GetAddr(amx, params[param_offset + param_idx],
                                &array_addr_ptr);
                    break;
                case 'r':  // reference
                    amx_GetAddr(amx, params[param_offset + param_idx],
                                &address_ptr);

                    param_list.emplace_front(
                        'r', std::make_any<cell *>(address_ptr));
                    break;
                default:
                    logprintf("invalid format specifier ");
                    return nullptr;
            }
            param_idx++;
        } while (*(++format) != '\0');

        if (array_addr_ptr != nullptr) {
            logprintf("no array size specified after 'a' specifier");
            return nullptr;
        }
    }

    return std::make_shared<CCallback>(amx, cb_idx, std::move(param_list));
}

bool CCallback::Execute(ResultVector results) {
    // the user could unload a filterscript between CCallback creation and
    // execution, so we better check if the AMX instance is still valid
    logprintf("CCallback::Execute(ResultVector results)");

    if (CCallbackManager::IsValidAmx(m_AmxInstance) == false) {
        return false;
    }

    cell amx_address = -1;

    for (auto &i : m_Params) {
        cell tmp_addr;
        std::any param_val = i.second;
        char specifier = i.first;

        switch (specifier) {
            case 'c':  // cell
            {
                const cell value = std::any_cast<cell>(param_val);
                amx_Push(m_AmxInstance, value);
            } break;
            case 's':  // string
            {
                const std::string value = std::any_cast<std::string>(param_val);
                amx_PushString(m_AmxInstance, &tmp_addr, nullptr, value.c_str(),
                               0, 0);

                if (amx_address < 0) amx_address = tmp_addr;
            } break;
            case 'a':  // array
            {
                auto array_pair =
                    std::any_cast<std::pair<cell *, cell>>(param_val);
                cell *array_addr = array_pair.first;
                cell array_size = array_pair.second;
                amx_PushArray(m_AmxInstance, &tmp_addr, nullptr, array_addr,
                              array_size);

                // 释放 copied_array
                free(array_addr);
                if (amx_address < 0) amx_address = tmp_addr;
            } break;
            case 'r':  // reference
            {
                // to do
                // cell *const value = std::any_cast<cell *>(param_val);
                // amx_PushAddress(m_AmxInstance, value);
            } break;
        }
    }

    for (auto it = results.rbegin(); it != results.rend(); ++it) {
        if (std::holds_alternative<int>(*it)) {
            int value = std::get<int>(*it);
            // amx_push 传到amx
            amx_Push(m_AmxInstance, value);
        } else if (std::holds_alternative<float>(*it)) {
            float value = std::get<float>(*it);
            amx_Push(m_AmxInstance, (cell)value);
        } else if (std::holds_alternative<std::string>(*it)) {
            std::string value = std::get<std::string>(*it);
            int l = value.size();
            char *p = new char[l*3];
            utf8_to_gb(value.c_str(), p, l*3);
            amx_PushString(m_AmxInstance, &amx_address, nullptr, p,
                           0, 0);
            delete[] p;
        }
    }

    amx_Exec(m_AmxInstance, nullptr, m_AmxCallbackIndex);
    if (amx_address >= 0) amx_Release(m_AmxInstance, amx_address);
    return true;
}