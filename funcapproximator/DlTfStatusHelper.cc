
#include "tensorflow/c/tf_status_helper.h"

namespace tensorflow {

void Set_TF_Status_from_Status(TF_Status* tf_status, const Status& status) {
  tensorflow::error::Code code = status.code();
  const char* message(status.error_message().c_str());

  switch (code) {
    case tensorflow::error::OK:assert(TF_GetCode(tf_status) == TF_OK);
      break;
    case tensorflow::error::CANCELLED:TF_SetStatus(tf_status, TF_CANCELLED, message);
      break;
    case tensorflow::error::UNKNOWN:TF_SetStatus(tf_status, TF_UNKNOWN, message);
      break;
    case tensorflow::error::INVALID_ARGUMENT:TF_SetStatus(tf_status, TF_INVALID_ARGUMENT, message);
      break;
    case tensorflow::error::DEADLINE_EXCEEDED:TF_SetStatus(tf_status, TF_DEADLINE_EXCEEDED, message);
      break;
    case tensorflow::error::NOT_FOUND:TF_SetStatus(tf_status, TF_NOT_FOUND, message);
      break;
    case tensorflow::error::ALREADY_EXISTS:TF_SetStatus(tf_status, TF_ALREADY_EXISTS, message);
      break;
    case tensorflow::error::PERMISSION_DENIED:TF_SetStatus(tf_status, TF_PERMISSION_DENIED, message);
      break;
    case tensorflow::error::UNAUTHENTICATED:TF_SetStatus(tf_status, TF_UNAUTHENTICATED, message);
      break;
    case tensorflow::error::RESOURCE_EXHAUSTED:TF_SetStatus(tf_status, TF_RESOURCE_EXHAUSTED, message);
      break;
    case tensorflow::error::FAILED_PRECONDITION:TF_SetStatus(tf_status, TF_FAILED_PRECONDITION, message);
      break;
    case tensorflow::error::ABORTED:TF_SetStatus(tf_status, TF_ABORTED, message);
      break;
    case tensorflow::error::OUT_OF_RANGE:TF_SetStatus(tf_status, TF_OUT_OF_RANGE, message);
      break;
    case tensorflow::error::UNIMPLEMENTED:TF_SetStatus(tf_status, TF_UNIMPLEMENTED, message);
      break;
    case tensorflow::error::INTERNAL:TF_SetStatus(tf_status, TF_INTERNAL, message);
      break;
    case tensorflow::error::UNAVAILABLE:TF_SetStatus(tf_status, TF_UNAVAILABLE, message);
      break;
    case tensorflow::error::DATA_LOSS:TF_SetStatus(tf_status, TF_DATA_LOSS, message);
      break;
    default:assert(0);
      break;
  }
}

}  // namespace tensorflow