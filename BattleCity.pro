TEMPLATE = subdirs

SUBDIRS = \
    app \
    engine engine_test \
    math math_test

engine_test.depends += engine
engine.depends += math
math_test.depends += math
app.depends += engine
