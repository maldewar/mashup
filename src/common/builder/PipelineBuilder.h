#ifndef MASHUP_COMMON_BUILDER_PIPELINEBUILDER_
#define MASHUP_COMMON_BUILDER_PIPELINEBUILDER_

#include "PipelineBuilder.h"
#include "../../model/Mashup.h"
#include "../../pipeline/Pipeline.h"
#include "../../pipeline/VideoActor.h"
#include "../../pipeline/ImageActor.h"

/**
 * @brief Creates a Pipeline object from a Mashup object.
 *
 * Creates all the GStreamer instances neccesary to play a pipeline
 * from a Mashup descriptor object.
 * @author Waldemar Sánchez
 * @date February, 2014
 */
class PipelineBuilder {
  public:
    /**
     * Creates Gstreamer element instances and link them together.
     * @return Pipeline object ready to be played.
     */
    static Pipeline* Build(const Mashup& mashup);
    static int BuildActors(const Mashup& mashup, Pipeline& pipeline);

  private:
    /**
     * Private constructor.
     */
    PipelineBuilder();
};

#endif // MASHUP_COMMON_BUILDER_PIPELINEBUILDER_
