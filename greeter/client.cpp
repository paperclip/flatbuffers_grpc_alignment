#include "server_address.h"

#include "greeter_generated.grpc.fb.h"
#include "greeter_generated.h"

#include <grpcpp/grpcpp.h>

#include <chrono>
#include <iostream>
#include <memory>
#include <string>
#include <thread>

class GreeterClient {
 public:
  GreeterClient(std::shared_ptr<grpc::Channel> channel)
    : stub_(Greeter::NewStub(channel)) {}

  std::string SayHello(const std::string &name) {
    flatbuffers::grpc::MessageBuilder mb;
    auto name_offset = mb.CreateString(name);
    auto request_offset = CreateHelloRequest(mb, name_offset);
    mb.Finish(request_offset);
    auto request_msg = mb.ReleaseMessage<HelloRequest>();

    flatbuffers::grpc::Message<HelloReply> response_msg;

    grpc::ClientContext context;

    auto status = stub_->SayHello(&context, request_msg, &response_msg);
    if (status.ok()) {
      const HelloReply *response = response_msg.GetRoot();
      return response->message()->str();
    } else {
      std::cerr << status.error_code() << ": " << status.error_message()
                << std::endl;
      return "RPC failed";
    }
  }

  void SayManyHellos(const std::string &name, int num_greetings,
                     std::function<void(const std::string &)> callback) {
    flatbuffers::grpc::MessageBuilder mb;
    auto name_offset = mb.CreateString(name);
    auto request_offset =
        CreateManyHellosRequest(mb, name_offset, num_greetings);
    mb.Finish(request_offset);
    auto request_msg = mb.ReleaseMessage<ManyHellosRequest>();

    flatbuffers::grpc::Message<HelloReply> response_msg;

    grpc::ClientContext context;

    auto stream = stub_->SayManyHellos(&context, request_msg);
    while (stream->Read(&response_msg)) {
      const HelloReply *response = response_msg.GetRoot();
      callback(response->message()->str());
    }
    auto status = stream->Finish();
    if (!status.ok()) {
      std::cerr << status.error_code() << ": " << status.error_message()
                << std::endl;
      callback("RPC failed");
    }
  }

  void SayVector(const std::vector<std::string>& names)
  {
    using reply_t = VectorReply;

    flatbuffers::grpc::MessageBuilder mb;

    std::vector<flatbuffers::Offset<flatbuffers::String> > namesOffsets;
    for (const auto& name : names)
    {
      namesOffsets.push_back(mb.CreateString(name));
    }

    auto namesVector = mb.CreateVector(namesOffsets);

    auto request_offset = CreateVectorRequest(mb, namesVector);
    mb.Finish(request_offset);
    auto request_msg = mb.ReleaseMessage<VectorRequest>();

    grpc::ClientContext context;

    flatbuffers::grpc::Message<reply_t> response_msg;
    auto status = stub_->SayVector(&context, request_msg, &response_msg);
    if (status.ok()) {
      const reply_t* response = response_msg.GetRoot();
      const flatbuffers::Vector<flatbuffers::Offset<flatbuffers::String>>* messages = response->messages();
      assert(messages != nullptr);
      for (const auto& message : *messages)
      {
        std::cout << message->str() << '\n';
      }
    } else {
      std::cerr << status.error_code() << ": " << status.error_message()
                << '\n';
    }
  }

 private:
  std::unique_ptr<Greeter::Stub> stub_;
};

int main(int argc, char **argv) {

  std::string server_address = SERVER_ADDRESS;
  

  auto channel =
      grpc::CreateChannel(server_address, grpc::InsecureChannelCredentials());
  GreeterClient greeter(channel);

  std::string name("world");

  int option = 1;
  if (argc > 1)
  {
      option = std::atoi(argv[1]);
  }

  if (option == 0)
  {
    std::string message = greeter.SayHello(name);
    std::cerr << "Greeter received: " << message << std::endl;
  }

  if (option == 1)
  {

    // std::this_thread::sleep_for(std::chrono::seconds(5));

    int num_greetings = 10;
    greeter.SayManyHellos(name, num_greetings, [](const std::string &message) {
      std::cerr << "Greeter received: " << message << std::endl;
    });
  }


  if (option == 2)
  {
  // std::this_thread::sleep_for(std::chrono::seconds(5));


    std::vector<std::string> names;
    for (int i=0; i < 10; i++)
    {
      names.push_back(std::to_string(i));
    }
    greeter.SayVector(names);
  }

  return 0;
}
