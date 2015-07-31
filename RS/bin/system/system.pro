TEMPLATE = aux
CONFIG -= qt
CONFIG -= app_bundle

SYSTEMC_PATH= 

CONFIG -= debug_and_release

INCLUDEPATH = $${SYSTEMC_PATH}/include

win32 {
  LIBS += $${SYSTEMC_PATH}/lib-mingw/libsystemc.a
}

unix {
    contains(QMAKE_HOST.arch, x86_64) {
         # 64-bit Unix
        mac {
            LIBS = -L$${SYSTEMC_PATH}/lib-macosx64 -lsystemc -Xlinker -rpath -Xlinker $${SYSTEMC_PATH}/lib-macosx64
        } else {
            LIBS = -L$${SYSTEMC_PATH}/lib-linux64 -lsystemc -Xlinker -rpath -Xlinker $${SYSTEMC_PATH}/lib-linux64
        }
    } else {
        # 32-bit Unix not OS X
        LIBS = -L$${SYSTEMC_PATH}/lib-linux -lsystemc -Xlinker -rpath -Xlinker $${SYSTEMC_PATH}/lib-linux
    }
}


compile:HEADERS += \
    and2.h \
    arb_ag.h \
    arb_rr.h \
    arbiter.h \
    class_checker.h \
    fg.h \
    fg_2vc.h \
    fifo_in.h \
    fifo_out.h \
    global_clock.h \
    ic.h \
    ifc.h \
    ifc_credit.h \
    ifc_credit_vct.h \
    ifc_handshake.h \
    irs.h \
    oc.h \
    oc_ag.h \
    ods.h \
    ofc.h \
    ofc_credit.h \
    ofc_credit_vct.h \
    ofc_handshake.h \
    ows.h \
    parameters.h \
    paris.h \
    paris_2vc.h \
    pg.h \
    pg_rd_c_style.h \
    pg_rd_lfsr_fibo_var.h \
    pg_rd_lfsr_fibo4.h \
    pg_rd_lfsr_galois8.h \
    pg_rr.h \
    pg_rt.h \
    pg_st.h \
    ppe.h \
    req_reg.h \
    req_reg_cs.h \
    reset.h \
    routing_nf.h \
    routing_nl.h \
    routing_oe.h \
    routing_wf.h \
    routing_xy.h \
    routing_yx.h \
    socin.h \
    stamp_checker.h \
    stampler.h \
    stampler_control.h \
    stampler_counter.h \
    stopsim.h \
    t_probe.h \
    tg.h \
    tg_2vc.h \
    tm_2vc.h \
    tm_single.h \
    unboundedfifo.h \
    xin.h \
    xin_2vc.h \
    xin_ag.h \
    xout.h \
    xout_2vc.h \
    xout_ag.h


compile:SOURCES += \
    arb_ag.cpp \
    class_checker.cpp \
    fg.cpp \
    fg_2vc.cpp \
    fifo_in.cpp \
    fifo_out.cpp \
    global_clock.cpp \
    ifc_credit.cpp \
    ifc_credit_vct.cpp \
    ifc_handshake.cpp \
    irs.cpp \
    main.cpp \
    ods.cpp \
    ofc_credit.cpp \
    ofc_credit_vct.cpp \
    ofc_handshake.cpp \
    ows.cpp \
    pg.cpp \
    pg_rd_c_style.cpp \
    pg_rd_lfsr_fibo_var.cpp \
    pg_rd_lfsr_fibo4.cpp \
    pg_rd_lfsr_galois8.cpp \
    pg_rr.cpp \
    pg_rt.cpp \
    ppe.cpp \
    req_reg.cpp \
    req_reg_cs.cpp \
    routing_nf.cpp \
    routing_nl.cpp \
    routing_oe.cpp \
    routing_wf.cpp \
    routing_xy.cpp \
    routing_yx.cpp \
    stamp_checker.cpp \
    stampler.cpp \
    stampler_control.cpp \
    stampler_counter.cpp \
    stopsim.cpp \
    t_probe.cpp \
    tm_2vc.cpp \
    tm_single.cpp \
    unboundedfifo.cpp


