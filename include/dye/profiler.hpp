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

#include <map>
#include <stack>

#include <iostream> ///< @todo Remove this?

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
        call_frame frame;
        frame.section_id.assign(section_id.str());
        frame.start_time = boost::posix_time::microsec_clock::universal_time();
        call_stack->push(frame);

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
            new_call_info.minimum_self_duration = new_call_info.minimum_child_duration = boost::posix_time::pos_infin;
            /// @todo make_pair?
            this_call_info = calls.insert(std::pair<std::string, call_info>(section_id, new_call_info)).first;
        }
        calls_lock.unlock();

        // Update the self call timers.
        this_call_info->second.call_count++;
        if (call_stack->top().self_duration < this_call_info->second.minimum_self_duration)
            this_call_info->second.minimum_self_duration = call_stack->top().self_duration;
        if (call_stack->top().self_duration > this_call_info->second.maximum_self_duration)
            this_call_info->second.maximum_self_duration = call_stack->top().self_duration;
        this_call_info->second.total_self_duration += call_stack->top().self_duration;

        // Update the accumulated children call timers.
        if (call_stack->top().child_duration < this_call_info->second.minimum_child_duration)
            this_call_info->second.minimum_child_duration = call_stack->top().child_duration;
        if (call_stack->top().child_duration > this_call_info->second.maximum_child_duration)
            this_call_info->second.maximum_child_duration = this_call_info->second.minimum_child_duration;
        this_call_info->second.total_child_duration += call_stack->top().child_duration;

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
        std::ostringstream result;
        result << " calls   minSelf   avgSelf   maxSelf   ttlSelf  minChild  avgChild  maxChild  ttlChild" << std::endl;

        // If not sorting, then simply convert all map items to string.
        boost::unique_lock<boost::mutex> calls_lock(calls_mutex);
        if (!sort) {
            for (typename std::map<std::string, call_info>::const_iterator iter = calls.begin(); iter != calls.end(); iter++)
                result << to_string(*iter) << std::endl;
            return result.str();
        }

        // Convert the map to a list.
        std::list<std::pair<std::string, call_info> > list;
        for (typename std::map<std::string, call_info>::const_iterator iter = calls.begin(); iter != calls.end(); iter++) {
            list.push_back(*iter);
        }
        calls_lock.unlock();

        // Sort the list.
        list.sort(compare_total_self_duration);
        list.reverse();

        // Print the results.
        for (typename std::list<std::pair<std::string, call_info> >::const_iterator iter = list.begin(); iter != list.end(); iter++)
            result << to_string(*iter) << std::endl;
        return result.str();
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
    typedef struct {
        uint_fast32_t call_count;
        boost::posix_time::time_duration minimum_self_duration;
        boost::posix_time::time_duration maximum_self_duration;
        boost::posix_time::time_duration total_self_duration;
        boost::posix_time::time_duration minimum_child_duration;
        boost::posix_time::time_duration maximum_child_duration;
        boost::posix_time::time_duration total_child_duration;
    } call_info;

    /// @brief  A call name, with its associated call information.
    std::map<std::string, call_info> calls;
    boost::mutex calls_mutex;

    typedef struct {
        std::string section_id;
        boost::posix_time::ptime start_time;
        boost::posix_time::time_duration self_duration;
        boost::posix_time::time_duration child_duration;
    } call_frame;

    boost::thread_specific_ptr<std::stack<call_frame> > call_stack;

    profile_decorator()
    {

    }

    static bool compare_total_self_duration(const std::pair<std::string, call_info> &first,
                                            const std::pair<std::string, call_info> &second)
    {
        return (first.second.total_self_duration < second.second.total_self_duration);
    }

    static std::string to_string(const std::pair<std::string, call_info> &pair)
    {
        std::ostringstream result;
        result << std::setw(6) << pair.second.call_count      << ' '
               << std::setw(9) << pair.second.minimum_self_duration.total_microseconds()    << ' '
               << std::setw(9) << (pair.second.total_self_duration.total_microseconds() / pair.second.call_count ) << ' '
               << std::setw(9) << pair.second.maximum_self_duration.total_microseconds()    << ' '
               << std::setw(9) << pair.second.total_self_duration.total_microseconds()  << ' '
               << std::setw(9) << pair.second.minimum_child_duration.total_microseconds()   << ' '
               << std::setw(9) << (pair.second.total_child_duration.total_microseconds() / pair.second.call_count ) << ' '
               << std::setw(9) << pair.second.maximum_child_duration.total_microseconds()   << ' '
               << std::setw(9) << pair.second.total_child_duration.total_microseconds() << ' '
               << pair.first;
        return result.str();
    }

private:
    static profile_decorator * instance;
};


typedef profile_decorator<basic_tracer> profiler;

}

#endif
#endif
