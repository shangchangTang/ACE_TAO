// -*- C++ -*-
//
// $Id$

// ****  Code generated by the The ACE ORB (TAO) IDL Compiler v1.6.9 ****
// TAO and the TAO IDL Compiler have been developed by:
//       Center for Distributed Object Computing
//       Washington University
//       St. Louis, MO
//       USA
//       http://www.cs.wustl.edu/~schmidt/doc-center.html
// and
//       Distributed Object Computing Laboratory
//       University of California at Irvine
//       Irvine, CA
//       USA
//       http://doc.ece.uci.edu/
// and
//       Institute for Software Integrated Systems
//       Vanderbilt University
//       Nashville, TN
//       USA
//       http://www.isis.vanderbilt.edu/
//
// Information about TAO is available at:
//     http://www.cs.wustl.edu/~schmidt/TAO.html

// TAO_IDL - Generated from
// be/be_codegen.cpp:1278

#include "Hello_Sender_exec.h"
#include "ace/OS_NS_unistd.h"
#include "ace/Task.h"
#include "ace/Date_Time.h"

namespace CIAO_Hello_DDS_Sender_Impl
{
  class Sending_Task : public virtual ACE_Task_Base
  {
  public:
    Sending_Task (const ACE_CString &msg,
                  CORBA::ULong iters,
                  ::CCM_DDS::string_Writer_ptr writer,
                  bool log_time)
      : msg_ (msg),
        iters_ (iters),
        writer_ (::CCM_DDS::string_Writer::_duplicate (writer)),
        log_time_ (log_time)
    {
    }

    virtual int svc (void)
    {
      // Allowing some time for discovery to happen
      ACE_OS::sleep (5);
      ACE_Time_Value tv (0, 1000); //1 msec
      for (size_t i = 0; i < this->iters_; ++i)
        {
          ACE_CString msg = create_message (this->msg_);
          this->writer_->write (msg.c_str ());
          ACE_DEBUG ((LM_DEBUG, "Sender has sent string\n"));
          ACE_OS::sleep (tv);
        }

      return 0;
    }

    ACE_CString create_message (const ACE_CString &msg)
    {
      if (!this->log_time_)
        return msg;
      ACE_TCHAR timestamp[16];
      ACE_Date_Time now;
      ACE_OS::sprintf (timestamp,
                        "%02d.%d",
                        now.second(),
                        now.microsec ());
      ACE_CString ret (timestamp);
      ret = ret + " " + msg;
      return ret.c_str ();
    }
  private:
    const ACE_CString &msg_;
    CORBA::ULong iters_;
    ::CCM_DDS::string_Writer_var writer_;
    bool log_time_;
  };


  //============================================================
  // Component Executor Implementation Class: Sender_exec_i
  //============================================================

  Sender_exec_i::Sender_exec_i (void)
    : iters_ (10),
      msg_ ("Hi Johnny, I'm a CCM component sending DDS messages!"),
      task_ (0),
      log_time_ (true)
  {
  }

  Sender_exec_i::~Sender_exec_i (void)
  {
    if (task_)
      delete task_;
  }

  // Supported operations and attributes.

  // Component attributes.

  char *
  Sender_exec_i::message (void)
  {
    return CORBA::string_dup (this->msg_.c_str());
  }

  void
  Sender_exec_i::message (const char *msg)
  {
    this->msg_ = msg;
  }

  ::CORBA::Boolean
  Sender_exec_i::log_time (void)
  {
    return this->log_time_;
  }

  void
	  Sender_exec_i::log_time (::CORBA::Boolean log_time)
  {
    this->log_time_ = log_time;
  }


  ::CORBA::ULong
  Sender_exec_i::iterations (void)
  {
    return this->iters_;
  }

  void
  Sender_exec_i::iterations (CORBA::ULong iters)
  {
    this->iters_ = iters;
  }

  // Port operations.

  // Operations from Components::SessionComponent.

  void
  Sender_exec_i::set_session_context (
                                      ::Components::SessionContext_ptr ctx)
  {
    this->context_ =
      ::Hello_DDS::CCM_Sender_Context::_narrow (ctx);

    if ( ::CORBA::is_nil (this->context_.in ()))
      {
        throw ::CORBA::INTERNAL ();
      }
  }

  void
  Sender_exec_i::configuration_complete (void)
  {
    /* Your code here. */
  }

  void
  Sender_exec_i::ccm_activate (void)
  {
    ::CCM_DDS::string_Writer_var writer =
      this->context_->get_connection_push_data_data ();

    this->task_ = new Sending_Task (this->msg_,
                                    this->iters_,
                                    writer,
                                    this->log_time_);

    this->task_->activate (THR_NEW_LWP | THR_JOINABLE,
                           1);
  }

  void
  Sender_exec_i::ccm_passivate (void)
  {
    /* Your code here. */
  }

  void
  Sender_exec_i::ccm_remove (void)
  {
    /* Your code here. */
  }

  extern "C" HELLO_SENDER_EXEC_Export ::Components::EnterpriseComponent_ptr
  create_Hello_DDS_Sender_Impl (void)
  {
    ::Components::EnterpriseComponent_ptr retval =
      ::Components::EnterpriseComponent::_nil ();

    ACE_NEW_RETURN (
                    retval,
                    Sender_exec_i,
                    ::Components::EnterpriseComponent::_nil ());

    return retval;
  }
}

namespace CIAO_Hello_DDS_Sender_Impl
{
  //============================================================
  // Home Executor Implementation Class: SenderHome_exec_i
  //============================================================

  SenderHome_exec_i::SenderHome_exec_i (void)
  {
  }

  SenderHome_exec_i::~SenderHome_exec_i (void)
  {
  }

  // All operations and attributes.

  // Factory operations.

  // Finder operations.

  // Implicit operations.

  ::Components::EnterpriseComponent_ptr
  SenderHome_exec_i::create (void)
  {
    ::Components::EnterpriseComponent_ptr retval =
      ::Components::EnterpriseComponent::_nil ();

    ACE_NEW_THROW_EX (
                      retval,
                      Sender_exec_i,
                      ::CORBA::NO_MEMORY ());

    return retval;
  }

  extern "C" HELLO_SENDER_EXEC_Export ::Components::HomeExecutorBase_ptr
  create_Hello_DDS_SenderHome_Impl (void)
  {
    ::Components::HomeExecutorBase_ptr retval =
      ::Components::HomeExecutorBase::_nil ();

    ACE_NEW_RETURN (
                    retval,
                    SenderHome_exec_i,
                    ::Components::HomeExecutorBase::_nil ());

    return retval;
  }
}

