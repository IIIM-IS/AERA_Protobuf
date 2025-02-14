//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
//_/_/
//_/_/ AERA
//_/_/ Autocatalytic Endogenous Reflective Architecture
//_/_/ 
//_/_/ Copyright (c) 2018-2025 Jeff Thompson
//_/_/ Copyright (c) 2018-2025 Kristinn R. Thorisson
//_/_/ Copyright (c) 2018-2025 Icelandic Institute for Intelligent Machines
//_/_/ Copyright (c) 2021 Leonard Eberding
//_/_/ http://www.iiim.is
//_/_/
//_/_/ --- Open-Source BSD License, with CADIA Clause v 1.0 ---
//_/_/
//_/_/ Redistribution and use in source and binary forms, with or without
//_/_/ modification, is permitted provided that the following conditions
//_/_/ are met:
//_/_/ - Redistributions of source code must retain the above copyright
//_/_/   and collaboration notice, this list of conditions and the
//_/_/   following disclaimer.
//_/_/ - Redistributions in binary form must reproduce the above copyright
//_/_/   notice, this list of conditions and the following disclaimer 
//_/_/   in the documentation and/or other materials provided with 
//_/_/   the distribution.
//_/_/
//_/_/ - Neither the name of its copyright holders nor the names of its
//_/_/   contributors may be used to endorse or promote products
//_/_/   derived from this software without specific prior 
//_/_/   written permission.
//_/_/   
//_/_/ - CADIA Clause: The license granted in and to the software 
//_/_/   under this agreement is a limited-use license. 
//_/_/   The software may not be used in furtherance of:
//_/_/    (i)   intentionally causing bodily injury or severe emotional 
//_/_/          distress to any person;
//_/_/    (ii)  invading the personal privacy or violating the human 
//_/_/          rights of any person; or
//_/_/    (iii) committing or preparing for any act of war.
//_/_/
//_/_/ THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND 
//_/_/ CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, 
//_/_/ INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF 
//_/_/ MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE 
//_/_/ DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR 
//_/_/ CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, 
//_/_/ SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, 
//_/_/ BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR 
//_/_/ SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS 
//_/_/ INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, 
//_/_/ WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING 
//_/_/ NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE 
//_/_/ OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY 
//_/_/ OF SUCH DAMAGE.
//_/_/ 
//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/

#pragma once

#include <iostream>
#include <iomanip>
#include "tcp_data_message.pb.h"

using communication_id_t = int64_t;

namespace tcp_io_device {
  /**
  * MetaData is a class to store the meta-data of messages. Especially used for storing of available commands and their descriptions.
  * Additionally gives access to convenience funtions, like VariableDescription message parsing.
  */
  class MetaData {
    friend class MsgData;

  private:
    MetaData() {}

  protected:
    int entity_id_;
    int id_;
    VariableDescription_DataType type_;
    size_t type_size_;
    uint64_t data_size_ = 0;
    uint64_t data_length_ = 0;
    std::vector<uint64_t> dimensions_;
    std::string opcode_handle_;

  public:
    /**
    * Constructor for MetaData objects. Converts the passed VariableDescription message and creates a MetaData object from it.
    * \param meta_data The VariableDescription used to convert to a MetaData object.
    */
    MetaData(const VariableDescription* meta_data) {
      setMetaData(meta_data->entityid(),
        meta_data->id(),
        meta_data->datatype(),
        std::vector<uint64_t>(meta_data->dimensions().begin(), meta_data->dimensions().end()),
        meta_data->opcode_string_handle());
    }

    /**
    * Constructor for MetaData objects. Best only used to create the objects when first initializing the communicaiton. To parse
    * incoming messages please see the other constructor.
    * \param entity An identifier describing the entity to which this MetaData objects is related.
    * \param name An identifier for the name of the data.
    * \param t The VariableDescription_DataType of the data.
    * \param dimensions The dimensionality of the data associated with this MetaData object.
    */
    MetaData(int entity_id, int name_id, VariableDescription_DataType t, std::vector<uint64_t> dimensions, std::string opcode_handle = "") {
      setMetaData(entity_id, name_id, t, dimensions, opcode_handle);
    }

    /**
    * Returns the id of the entity for which this description is used.
    */
    int getEntityID() {
      return entity_id_;
    }

    /**
    * Returns the id of the property for which this description is used.
    */
    int getID() {
      return id_;
    }

    /**
    * Returns the full length of the data assigned to this MetaData object
    * Number of data parts (e.g. double values) * the data size of each part (e.g. 8)
    */
    uint64_t getDataLength() { return data_length_; }

    /**
    * Returns the number of bytes used to store a data object (e.g. 8 for double)
    */
    uint64_t getDataSize() { return data_size_; }

    /**
    * Returns the dimensions of the data. E.g. [1920, 1080] for a full HD image.
    */
    std::vector<uint64_t> getDimensions() { return dimensions_; }

    /**
    * Returns the type of the corresponding message.
    */
    VariableDescription_DataType getType() { return type_; }

    /**
    * Returns the opcode handle of the corresponding message.
    */
    std::string getOpCodeHandle() { return opcode_handle_; }


    /**
    * Sets the fields of the MetaData object.
    * \param entity_id The id of the entity as received in the setup message.
    * \param id The id of the property as received in the setup message.
    * \param t The data type of the data (e.g. DOUBLE, INT, or similar).
    * \param d The dimensions of the data.
    */
    void setMetaData(int entity_id, int id, VariableDescription_DataType t, std::vector<uint64_t> d, std::string opcode_handle = "") {
      entity_id_ = entity_id;
      id_ = id;
      type_ = t;
      dimensions_ = d;
      opcode_handle_ = opcode_handle;
      switch (t)
      {
      case 0:
        type_size_ = 8;
        break;
      case 3:
        type_size_ = 8;
        break;
      case 15:
        type_size_ = 8;
        break;
      default:
        type_size_ = 1;
        break;
      }
      data_size_ = 1;
      data_length_ = 1;
      for (int i = 0; i < dimensions_.size(); ++i) {
        data_size_ *= dimensions_[i] * type_size_;
        data_length_ *= dimensions_[i];
      }
    }

    VariableDescription toVariableDescription() {
      VariableDescription var;
      var.set_entityid(entity_id_);
      var.set_id(id_);
      var.set_datatype(type_);
      var.mutable_dimensions()->Add(dimensions_.begin(), dimensions_.end());
      var.set_opcode_string_handle(opcode_handle_);
      return var;
    }
    void toMutableVariableDescription(VariableDescription* mutable_variable_description) {
      mutable_variable_description->set_entityid(entity_id_);
      mutable_variable_description->set_id(id_);
      mutable_variable_description->set_datatype(type_);
      mutable_variable_description->mutable_dimensions()->Add(dimensions_.begin(), dimensions_.end());
      mutable_variable_description->set_opcode_string_handle(opcode_handle_);
    }

    friend std::ostream& operator<<(std::ostream& output, const MetaData& meta_data) {
      output << "MetaData Object: " << std::endl
        << "EntityID: " << meta_data.entity_id_ << std::endl
        << "ID: " << meta_data.id_ << std::endl
        << "DataType: " << meta_data.type_ << std::endl
        << "TypeSize: " << meta_data.type_size_ << std::endl
        << "DataSize: " << meta_data.data_size_ << std::endl
        << "DataLength: " << meta_data.data_length_ << std::endl
        << "OpCodeHandle: " << meta_data.opcode_handle_ << std::endl
        << "Dimensions: { ";
      for (auto it = meta_data.dimensions_.begin(); it != meta_data.dimensions_.end(); ++it) {
        output << *it << " ";
      }
      output << "}" << std::endl;
      return output;
    }
  };

  /**
  * Class to store a DataMessage including a MetaData object and a string for the bytes of data.
  */
  class MsgData {
  private:
    MetaData meta_data_;
    std::string data_;
    bool valid_ = true;
    MsgData() {}
  public:

    /**
    * Constructor for MsgData objects. Converts a ProtoVariable message to a MsgData object.
    * \param msg The message used to convert and create a MsgData object from.
    */
    MsgData(const ProtoVariable* msg) : meta_data_(&(msg->metadata())) {
      setData(msg->data());
    }

    template<typename T>
    static MsgData createNewMsgData(MetaData meta_data, std::vector<T> data) {
      MsgData msg_data = MsgData();
      msg_data.meta_data_ = meta_data;
      msg_data.setData(data);
      msg_data.valid_ = true;
      return msg_data;
    }

    static MsgData createNewMsgData(MetaData meta_data, std::string data) {
      MsgData msg_data = MsgData();
      msg_data.meta_data_ = meta_data;
      msg_data.setData(data);
      msg_data.valid_ = true;
      return msg_data;
    }

    static MsgData createNewMsgData(MetaData meta_data) {
      MsgData msg_data = MsgData();
      msg_data.meta_data_ = meta_data;
      msg_data.setData("");
      msg_data.valid_ = true;
      return msg_data;
    }

    static MsgData invalidMsgData() {
      MsgData msg_data = MsgData();
      msg_data.valid_ = false;
      return msg_data;
    }

    /**
    * Setter for the data of the message. Data is stored as a byte representation in form of a std::string.
    * \param d The byte representation of the data in form of a std::string.
    */
    void setData(const std::string& d) {
      data_ = d;
    }

    template<typename T>
    void setData(const std::vector<T>& data) {
      std::string data_string;
      for (auto it = data.begin(); it != data.end(); ++it) {
        char data_char[sizeof(T)];
        T value = *it;
        memcpy(data_char, &value, sizeof(T));
        data_string.append(std::string(data_char, sizeof(T)));
      }
      setData(data_string);
    }

    /**
    * Returns the MetaData object corresponding to this MsgData.
    */
    MetaData getMetaData() {
      return meta_data_;
    }

    std::string _data() { return data_; }

    /**
    * Casts the data from the byte representation stored as a string to the template type. @todo: Check for dimensionality.
    */
    template <typename T> std::vector<T> getData() {
      T a;
      std::vector<T> values;
      for (int i = 0; i < data_.size(); i += sizeof(T)) {
        char* pos = &data_[i];
        memcpy(&a, pos, sizeof(T));
        values.push_back(a);
      }
      return values;
    }

    void toMutableProtoVariable(ProtoVariable* var) {
      VariableDescription* meta_data = var->mutable_metadata();
      meta_data_.toMutableVariableDescription(meta_data);
      var->set_data(data_);
    }

    friend std::ostream& operator<<(std::ostream& output, const MsgData& msg_data) {
      output << "MsgData Object: " << std::endl
        << msg_data.meta_data_ << std::endl
        << "Data: " << std::endl;
      for (int i = 0; i < msg_data.data_.size(); ++i) {
        output << std::setw(2) << std::setfill('0') << std::hex << (int)(msg_data.data_[i] & 0xFF) << std::dec;
      }
      return output;
    }
    bool isValid() { return valid_; }
  };
}