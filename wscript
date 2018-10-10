# -*- mode: python -*-
top = "."

def options(opt):
    opt.load('compiler_cxx')

def configure(conf):
    conf.load('compiler_cxx')

def build(bld):
    sources = ["src/lexer.cpp",
               "src/lick.cpp"
    ]
    incs = ["include"]
    bld.program(target="lick",
                includes = incs,
                source = sources+["src/interpreter.cpp"])
    bld.program(target="tests",
                includes = incs,
                source = sources+["src/tests.cpp"])

