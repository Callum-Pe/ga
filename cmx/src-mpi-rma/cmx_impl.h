#ifndef COMEX_IMPL_H_
#define COMEX_IMPL_H_

#include <semaphore.h>

#include <stdio.h>

#include <mpi.h>

#define COMEX_MAX_NB_OUTSTANDING 8
#define SHM_NAME_SIZE 20

typedef int cmxInt;

typedef struct {
    MPI_Comm world_comm;
    int rank;
    int size;
} local_state;

typedef struct win_link {
    struct win_link *next;
      struct win_link *prev;
        MPI_Win win;
} win_link_t;

typedef struct group_link {
  struct group_link *next;
  MPI_Comm comm;
  MPI_Group group;
  win_link_t *win_list;
} _cmx_group;

typedef _cmx_group cmx_igroup_t;

typedef struct {
  MPI_Win win;
  MPI_Comm comm;
  cmx_igroup_t *group;
  cmxInt bytes;
  int rank;
  void *buf;
} _cmx_handle;

typedef struct {
  MPI_Request request;
  MPI_Win win;
  int active;
#ifdef USE_MPI_FLUSH_LOCAL
  int remote_proc;
#endif
} _cmx_request;

typedef cmx_igroup_t* cmx_group_t;

extern local_state l_state;

extern cmx_group_t CMX_GROUP_WORLD;

#define DEBUG 0
#define COMEX_STRINGIFY(x) #x
#ifdef NDEBUG
#define COMEX_ASSERT(WHAT) ((void) (0))
#else
#define COMEX_ASSERT(WHAT) \
  ((WHAT) \
   ? (void) (0) \
   : comex_assert_fail (COMEX_STRINGIFY(WHAT), __FILE__, __LINE__, __func__))
#endif

  static inline void comex_assert_fail(
      const char *assertion,
      const char *file,
      unsigned int line,
      const char *function)
{
  fprintf(stderr, "[%d] %s:%u: %s: Assertion `%s' failed",
      l_state.rank, file, line, function, assertion);
  fflush(stderr);
#if DEBUG
  printf("[%d] %s:%u: %s: Assertion `%s' failed",
      l_state.rank, file, line, function, assertion);
#endif
  comex_error("comex_assert_fail", -1);
}
#endif /* COMEX_IMPL_H_ */