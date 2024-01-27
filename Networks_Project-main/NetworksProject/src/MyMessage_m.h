//
// Generated file, do not edit! Created by opp_msgtool 6.0 from MyMessage.msg.
//

#ifndef __MYMESSAGE_M_H
#define __MYMESSAGE_M_H

#if defined(__clang__)
#  pragma clang diagnostic ignored "-Wreserved-id-macro"
#endif
#include <omnetpp.h>

// opp_msgtool version check
#define MSGC_VERSION 0x0600
#if (MSGC_VERSION!=OMNETPP_VERSION)
#    error Version mismatch! Probably this file was generated by an earlier version of opp_msgtool: 'make clean' should help.
#endif

//class bits;
class MyMessage;
// cplusplus {{
#include <bitset>
typedef  std::bitset<8> bits;
// }}

/**
 * Class generated from <tt>MyMessage.msg:23</tt> by opp_msgtool.
 * <pre>
 * class bits
 * {
 *     \@existingclass;
 * }
 * </pre>
 */
/*
class bits
{
  protected:

  private:
    void copy(const bits& other);

  protected:
    bool operator==(const bits&) = delete;

  public:
    bits();
    bits(const bits& other);
    virtual ~bits();
    bits& operator=(const bits& other);
    virtual void parsimPack(omnetpp::cCommBuffer *b) const;
    virtual void parsimUnpack(omnetpp::cCommBuffer *b);
};
*/
/**
 * Class generated from <tt>MyMessage.msg:25</tt> by opp_msgtool.
 * <pre>
 * packet MyMessage
 * {
 *     \@customize(true);  // see the generated C++ header for more info
 *     int Seq_Num; //think of this as part 1 of header
 *     int M_Type; //0 for data, 1 for ACK, 2 for NAK, 3 for Coordinator initialize, think of this as part 2 of header
 *     int Ack_Num; //ack no
 *     string M_Payload;
 *     bits mycheckbits; //think of this as trailer
 * }
 * </pre>
 *
 * MyMessage_Base is only useful if it gets subclassed, and MyMessage is derived from it.
 * The minimum code to be written for MyMessage is the following:
 *
 * <pre>
 * class MyMessage : public MyMessage_Base
 * {
 *   private:
 *     void copy(const MyMessage& other) { ... }

 *   public:
 *     MyMessage(const char *name=nullptr, short kind=0) : MyMessage_Base(name,kind) {}
 *     MyMessage(const MyMessage& other) : MyMessage_Base(other) {copy(other);}
 *     MyMessage& operator=(const MyMessage& other) {if (this==&other) return *this; MyMessage_Base::operator=(other); copy(other); return *this;}
 *     virtual MyMessage *dup() const override {return new MyMessage(*this);}
 *     // ADD CODE HERE to redefine and implement pure virtual functions from MyMessage_Base
 * };
 * </pre>
 *
 * The following should go into a .cc (.cpp) file:
 *
 * <pre>
 * Register_Class(MyMessage)
 * </pre>
 */
class MyMessage_Base : public ::omnetpp::cPacket
{
  protected:
    int Seq_Num = 0;
    int M_Type = 0;
    int Ack_Num = 0;
    omnetpp::opp_string M_Payload;
    bits mycheckbits;

  private:
    void copy(const MyMessage_Base& other);

  protected:
    bool operator==(const MyMessage_Base&) = delete;
    // make constructors protected to avoid instantiation

    // make assignment operator protected to force the user override it
    MyMessage_Base& operator=(const MyMessage_Base& other);

  public:
    virtual ~MyMessage_Base();
    virtual MyMessage_Base *dup() const override {return new MyMessage_Base(*this);}
    virtual void parsimPack(omnetpp::cCommBuffer *b) const override;
    virtual void parsimUnpack(omnetpp::cCommBuffer *b) override;
    MyMessage_Base(const char *name=nullptr, short kind=0);
    MyMessage_Base(const MyMessage_Base& other);

    virtual int getSeq_Num() const;
    virtual void setSeq_Num(int Seq_Num);

    virtual int getM_Type() const;
    virtual void setM_Type(int M_Type);

    virtual int getAck_Num() const;
    virtual void setAck_Num(int Ack_Num);

    virtual const char * getM_Payload() const;
    virtual void setM_Payload(const char * M_Payload);

    virtual const bits& getMycheckbits() const;
    virtual bits& getMycheckbitsForUpdate() { return const_cast<bits&>(const_cast<MyMessage_Base*>(this)->getMycheckbits());}
    virtual void setMycheckbits(const bits& mycheckbits);
};


namespace omnetpp {

inline any_ptr toAnyPtr(const bits *p) {if (auto obj = as_cObject(p)) return any_ptr(obj); else return any_ptr(p);}
template<> inline bits *fromAnyPtr(any_ptr ptr) { return ptr.get<bits>(); }
template<> inline MyMessage_Base *fromAnyPtr(any_ptr ptr) { return check_and_cast<MyMessage_Base*>(ptr.get<cObject>()); }

}  //namespace omnetpp

#endif // ifndef __MYMESSAGE_M_H
