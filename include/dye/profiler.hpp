//               Copyright Paul Colby 2014.
// Distributed under the Boost Software License, Version 1.0.
//       (See accompanying file LICENSE.md or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef __DYE_PROFILER_HPP__
#define __DYE_PROFILER_HPP__
#ifdef DYE_ENABLED

#include "basic_dye.hpp"

#include <boost/date_time/posix_time/posix_time_types.hpp>
#include <boost/thread.hpp>

#include <algorithm>
#include <map>
#include <numeric>
#include <stack>

namespace dye {

template<class Base>
class profile_decorator : public Base {

public:
    static profile_decorator * get_instance() { return instance; }

    void begin(const std::string &file, const int line,
               const std::string &function,
               const std::string &section_name = std::string(),
               const operation_type operation = generic_operation)
    {
        // Pause the current stack item (if there is one).
        if (call_stack.get() == NULL) {
            call_stack.reset(new std::stack<call_frame>);
        } else if (!call_stack->empty()) {
            call_stack->top().self_duration +=
                boost::posix_time::microsec_clock::universal_time() - call_stack->top().start_time;
        }

        // Push this new call / code section onto the stack.
        std::ostringstream section_id;
        section_id << file << ':' << line << ':' << function;
        if (!section_name.empty()) {
            section_id << ':' << section_name;
        }
        call_stack->push(call_frame(section_id.str()));

        Base::begin(file, line, function, section_name, operation);
    }

    void end()   {
        Base::end();

        // Make sure our stack is not empty (ie begin was called before end).
        if (call_stack->empty()) {
            //syslog(LOG_ERR, "end called with empty stack");
            return;
        }

        // Get the finalised self-time for this call.
        call_stack->top().self_duration += boost::posix_time::microsec_clock::universal_time() - call_stack->top().start_time;


        // Find / create a call-info entry for this function.
        const std::string &section_id = call_stack->top().section_id;
        boost::unique_lock<boost::mutex> calls_lock(calls_mutex);
        typename std::map<std::string, call_info>::iterator this_call_info = calls.find(section_id);
        if (this_call_info == calls.end()) {
            call_info new_call_info;
            new_call_info.call_count = 0;
            /// @todo Move this to a call_info constructor?
            new_call_info.duration.self.minimum = new_call_info.duration.child.minimum = boost::posix_time::pos_infin;
            this_call_info = calls.insert(std::make_pair(section_id, new_call_info)).first;
        }
        calls_lock.unlock();

        // Update the self call timers.
        this_call_info->second.call_count++;
        if (call_stack->top().self_duration < this_call_info->second.duration.self.minimum)
            this_call_info->second.duration.self.minimum = call_stack->top().self_duration;
        if (call_stack->top().self_duration > this_call_info->second.duration.self.maximum)
            this_call_info->second.duration.self.maximum = call_stack->top().self_duration;
        this_call_info->second.duration.self.total += call_stack->top().self_duration;

        // Update the accumulated children call timers.
        if (call_stack->top().child_duration < this_call_info->second.duration.child.minimum)
            this_call_info->second.duration.child.minimum = call_stack->top().child_duration;
        if (call_stack->top().child_duration > this_call_info->second.duration.child.maximum)
            this_call_info->second.duration.child.maximum = this_call_info->second.duration.child.minimum;
        this_call_info->second.duration.child.total += call_stack->top().child_duration;

        // Remove this call from the stack, and add its total (self + children) time to the parent's children time.
        const boost::posix_time::time_duration total_duration = call_stack->top().self_duration + call_stack->top().child_duration;
        call_stack->pop();
        if (!call_stack->empty()) {
            call_stack->top().child_duration += total_duration;
            call_stack->top().start_time = boost::posix_time::microsec_clock::universal_time();
        }
    }

    void reset()
    {
        Base::reset();
        boost::lock_guard<boost::mutex> calls_lock(calls_mutex);
        calls.clear();
    }

    std::string get_call_graph()
    {
        return "get_call_graph not yet implemented";
    }

    std::string get_flat_profile(const bool sort = true)
    {
        if (sort) {
            // Convert the map to a list.
            std::list<typename call_info_map::value_type> list;
            boost::unique_lock<boost::mutex> calls_lock(calls_mutex);
            std::copy(calls.begin(), calls.end(), std::back_inserter(list));
            calls_lock.unlock();

            // Sort the list.
            list.sort(compare_total_self_duration);
            list.reverse();
            return to_string(list);
        } else {
            boost::unique_lock<boost::mutex> calls_lock(calls_mutex);
            return to_string(calls);
        }
    }

    void print_call_graph(std::ostream &output_stream = std::clog)
    {
        output_stream << get_call_graph() << std::endl;
    }

    void print_flat_profile(std::ostream &output_stream = std::clog,
                            const bool sort = true)
    {
        output_stream << get_flat_profile(sort) << std::endl;
    }

protected:
    template <typename Type>
    struct min_max_total {
        Type minimum, maximum, total;
    };

    typedef struct {
        uint_fast32_t call_count;
        struct {
            min_max_total<boost::posix_time::time_duration> self;
            min_max_total<boost::posix_time::time_duration> child;
        } duration;
    } call_info;

    typedef std::map<std::string, call_info> call_info_map;
    /// @brief  A call name, with its associated call information.
    call_info_map calls;
    boost::mutex calls_mutex;

    typedef struct call_frame_struct {
        std::string section_id;
        boost::posix_time::ptime start_time;
        boost::posix_time::time_duration self_duration;
        boost::posix_time::time_duration child_duration;

        call_frame_struct(const std::string &section_id,
                          const boost::posix_time::ptime &start_time = boost::posix_time::microsec_clock::universal_time())
            : section_id(section_id), start_time(start_time)
        {

        }

    } call_frame;

    boost::thread_specific_ptr<std::stack<call_frame> > call_stack;

    profile_decorator()
    {

    }

    static bool compare_total_self_duration(const typename call_info_map::value_type &first,
                                            const typename call_info_map::value_type &second)
    {
        return (first.second.duration.self.total < second.second.duration.self.total);
    }

    static std::string accumulate(const std::string &prefix,
                                  const typename call_info_map::value_type &pair)
    {
        return prefix + to_string(pair) + '\n';
    }

    template<class CollectionType>
    static std::string to_string(const CollectionType &collection)
    {
        return
            " calls   minSelf   avgSelf   maxSelf   ttlSelf  minChild  avgChild  maxChild  ttlChild\n" +
            std::accumulate(collection.begin(), collection.end(), std::string(), profile_decorator<Base>::accumulate);
    }

    static std::string to_string(const typename call_info_map::value_type &pair)
    {
        return to_string(pair.first, pair.second);
    }

    static std::string to_string(const std::string &call_id, const call_info &call_info)
    {
        std::ostringstream result;
        result << std::setw(6) << call_info.call_count      << ' '
               << std::setw(9) << call_info.duration.self.minimum.total_microseconds()    << ' '
               << std::setw(9) << (call_info.duration.self.total.total_microseconds() / call_info.call_count ) << ' '
               << std::setw(9) << call_info.duration.self.maximum.total_microseconds()    << ' '
               << std::setw(9) << call_info.duration.self.total.total_microseconds()  << ' '
               << std::setw(9) << call_info.duration.child.minimum.total_microseconds()   << ' '
               << std::setw(9) << (call_info.duration.child.total.total_microseconds() / call_info.call_count ) << ' '
               << std::setw(9) << call_info.duration.child.maximum.total_microseconds()   << ' '
               << std::setw(9) << call_info.duration.child.total.total_microseconds() << ' '
               << call_id;
        return result.str();
    }

private:
    static profile_decorator * instance;

};

typedef profile_decorator<basic_dye> profiler;

}

#endif
#endif
