PROTOC_DIR = ./nanopb-0.3.9-linux-x86
PROTOC = $(PROTOC_DIR)/generator-bin/protoc
PROTOC_OPTS =
OUT_DIR = ./install
PROTOB_DIR = ./protob
SRCS = ${wildcard $(PROTOB_DIR)/*.proto}
OBJS = ${patsubst %.proto, $(OUT_DIR)/%.pb.c, $(SRCS)}
OBJS += ${patsubst %.proto, $(OUT_DIR)/%.pb.h, $(SRCS)}


all:MK_DIR $(OBJS) INSTALL
	@echo "#"
	@echo "# make OK."
	@echo "#"

MK_DIR:
	@mkdir -p $(OUT_DIR)/protob

INSTALL:
	@cp -raf $(OUT_DIR)/pb.h $(OUT_DIR)/protob/pb.h
	@cp -raf $(OUT_DIR)/pb_common.h $(OUT_DIR)/protob/pb_common.h
	@cp -raf $(OUT_DIR)/pb_common.c $(OUT_DIR)/protob/pb_common.c
	@cp -raf $(OUT_DIR)/pb_decode.h $(OUT_DIR)/protob/pb_decode.h
	@cp -raf $(OUT_DIR)/pb_decode.c $(OUT_DIR)/protob/pb_decode.c
	@cp -raf $(OUT_DIR)/pb_encode.h $(OUT_DIR)/protob/pb_encode.h
	@cp -raf $(OUT_DIR)/pb_encode.c $(OUT_DIR)/protob/pb_encode.c

# Build rule for the protocol
$(OUT_DIR)/%.pb.c $(OUT_DIR)/%.pb.h: %.proto
	$(PROTOC) $(PROTOC_OPTS) --nanopb_out=$(OUT_DIR) $^

clean:
	@rm -rf $(OBJS)
	@rm -rf $(OUT_DIR)/protob

