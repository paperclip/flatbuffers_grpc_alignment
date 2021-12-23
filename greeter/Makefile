
FLATBUFFER_DIR ?= ${HOME}/.local

GRPC_DIR ?= ${HOME}/.local


CXXFLAGS ?= -I$(FLATBUFFER_DIR)/include -I$(GRPC_DIR)/include  -g -fsanitize=alignment -fno-omit-frame-pointer
LDFLAGS ?= -L$(GRPC_DIR)/lib
FLATC ?= flatc

.PHONY: all clean
all: server client

GENERATED_CC = \
	greeter_generated.grpc.fb.cc


GENERATED = \
	greeter_generated.h \
	greeter_generated.grpc.fb.h \
	$(GENERATED_CC)

$(GENERATED) : greeter.fbs
	$(FLATC) --grpc --cpp $<

clean :
	rm -f $(GENERATED) server client

ABSL_LIBS_LATEST := \
    -labsl_bad_optional_access \
    -labsl_bad_variant_access \
    -labsl_cord \
    -labsl_cord_internal \
    -labsl_cordz_info \
    -labsl_cordz_functions \
    -labsl_cordz_handle \
    -labsl_exponential_biased \
    -labsl_hash \
    -labsl_low_level_hash \
    -labsl_random_internal_pool_urbg \
    -labsl_random_internal_randen \
    -labsl_random_internal_randen_slow \
    -labsl_random_internal_randen_hwaes \
    -labsl_random_internal_randen_hwaes_impl \
    -labsl_random_internal_platform \
    -labsl_random_seed_gen_exception \
    -labsl_random_internal_seed_material \
    -labsl_raw_logging_internal \
    -labsl_stacktrace \
    -labsl_status \
    -labsl_statusor \
    -labsl_str_format_internal \
    -labsl_strings \
    -labsl_strings_internal \
    -labsl_synchronization \
    -labsl_graphcycles_internal \
    -labsl_symbolize \
    -labsl_throw_delegate \
    -labsl_time \
    -labsl_time_zone \
    -labsl_int128 \
    -labsl_base \
    -labsl_city \
    -labsl_spinlock_wait \
    -labsl_malloc_internal \
    -labsl_demangle_internal \
    -labsl_debugging_internal

ABSL_LIBS_20211124 := \
    -labsl_bad_optional_access \
    -labsl_bad_variant_access \
    -labsl_cord \
    -labsl_exponential_biased \
    -labsl_hash \
    -labsl_random_internal_pool_urbg \
    -labsl_random_internal_randen \
    -labsl_random_internal_randen_slow \
    -labsl_random_internal_randen_hwaes \
    -labsl_random_internal_randen_hwaes_impl \
    -labsl_random_internal_platform \
    -labsl_random_seed_gen_exception \
    -labsl_random_internal_seed_material \
    -labsl_raw_logging_internal \
    -labsl_status \
    -labsl_statusor \
    -labsl_str_format_internal \
    -labsl_strings \
    -labsl_strings_internal \
    -labsl_synchronization \
    -labsl_stacktrace \
    -labsl_graphcycles_internal \
    -labsl_symbolize \
    -labsl_throw_delegate \
    -labsl_time \
    -labsl_time_zone \
    -labsl_int128 \
    -labsl_base \
    -labsl_city \
    -labsl_spinlock_wait \
    -labsl_malloc_internal \
    -labsl_demangle_internal \
    -labsl_debugging_internal

LIBRARIES := \
	-lgrpc++ \
	-lgrpc \
	-lgpr \
	$(ABSL_LIBS_LATEST) \
    -lcares \
    -lre2 \
    -lz \
    -laddress_sorting \
    -lupb \
    -lssl \
    -lcrypto

server: server.cpp $(GENERATED) server_address.h
	g++ -pthread -std=c++11 -O2 $(CXXFLAGS) $(LDFLAGS) server.cpp $(GENERATED_CC) $(LIBRARIES) -o $@

client: client.cpp $(GENERATED) server_address.h
	g++ -pthread -std=c++11 -O2 $(CXXFLAGS) $(LDFLAGS) client.cpp $(GENERATED_CC) $(LIBRARIES) -o $@
