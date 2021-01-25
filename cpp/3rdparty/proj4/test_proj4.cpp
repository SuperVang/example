//
// Created by books on 2020/9/1.
//

#include <gtest/gtest.h>

#include <proj.h>

TEST(PROJ4, init_proj)
{
  PJ_CONTEXT *C;
  PJ *P;
  PJ_COORD a1, b1, a2, b2;

  /* or you may set C=PJ_DEFAULT_CTX if you are sure you will     */
  /* use PJ objects from only one thread                          */
  C = proj_context_create();

  //  proj_log_level(C, PJ_LOG_DEBUG);
  P = proj_create(C,
                  "+proj=tmerc +lat_0=0 +lon_0=120 +k=1 +x_0=40500000 +y_0=0 +ellps=GRS80 +units=m +no_defs");

  /* a coordinate union representing Copenhagen: 55d N, 12d E    */
  /* note: PROJ.4 works in radians, hence the proj_torad() calls */
  a1 = proj_coord(-652.17000000000+72000, 1458.56000000000, 0, 0);
  /* transform to UTM zone 32, then back to geographical */
  b1 = proj_trans(P, PJ_INV, a1);

  printf("%.8f,%.8f\n", proj_todeg(b1.uv.u), proj_todeg(b1.uv.v));

  a2 = proj_coord(proj_torad(119.14825516957), proj_torad(40.07728927189), 0, 0);
  b2 = proj_trans(P, PJ_FWD, a2);
  printf("%.8f,%.8f\n", b2.xy.x, b2.xy.y);

  /* Clean up */
  proj_destroy(P);
  proj_context_destroy(C); /* may be omitted in the single threaded case */
}