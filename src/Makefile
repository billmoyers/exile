NAME = exile

DEFINES =
LIBS = OGRE OIS
CXX = g++
CXXFLAGS = $(shell pkg-config --cflags $(LIBS)) $(DEFINES) -I. -g
LD = g++ 
LDFLAGS = $(shell pkg-config --libs $(LIBS))

MODEL_SRCS = model/World.cc model/MapTile.cc model/Map.cc model/Entity.cc
MODEL_OBJS = $(subst .cc,.o,$(MODEL_SRCS))
MODEL_INCS = include/model.h

VIEW_SRCS = view/ObjectView.cc view/View.cc view/MapView.cc view/WorldView.cc
VIEW_OBJS = $(subst .cc,.o,$(VIEW_SRCS))
VIEW_INCS = include/view.h $(MODEL_INCS)

CONTROLLER_SRCS = controller/Controller.cc
CONTROLLER_OBJS = $(subst .cc,.o,$(CONTROLLER_SRCS))
CONTROLLER_INCS = include/controller.h $(VIEW_INCS) $(MODEL_INCS)

SRCS = main.cc $(MODEL_SRCS) $(VIEW_SRCS) $(CONTROLLER_SRCS)
OBJS = $(subst .cc,.o,$(SRCS))

$(NAME): $(OBJS)
	$(CXX) $(LDFLAGS) $(OBJS) -o $(NAME)

$(MODEL_OBJS) : %.o: %.cc $(MODEL_INCS)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(VIEW_OBJS) : %.o: %.cc $(VIEW_INCS)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(CONTROLLER_OBJS) : %.o: %.cc $(CONTROLLER_INCS)
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(NAME) $(OBJS)

all: $(NAME)
