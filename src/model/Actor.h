#ifndef MASHUP_MODEL_ACTOR_
#define MASHUP_MODEL_ACTOR_

class Actor {
  public:
    /**
     * Class constructor.
     */
    Actor();

  public:
    long int asset_id;
    int instance_id;
    double x;
    double y;
    int z;
    int width;
    int height;
    int enters_at;
    int exits_at;

};

#endif // MASHUP_MODEL_ACTOR_
