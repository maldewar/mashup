#include "MashupInspector.h"

#include "../../util/Log.h"
#include "../../util/Print.h"
#include "../BaseProperty.h"

MashupInspector::MashupInspector() {};

std::string MashupInspector::Print(const Mashup& mashup) {
  std::stringstream sstream;
  Print(mashup, sstream);
  return sstream.str();
};

void MashupInspector::Print(const Mashup& mashup, std::stringstream& out, int indent) {
  Print::Duple("Version", mashup.version, out, indent);
  Print::Duple("ID", mashup.id, out, indent);
  Print::Duple("Title", mashup.title, out, indent);
  Print::Duple("Creator", mashup.creator_id, out, indent);
  if (mashup.assets.size() == 0) {
    Print::Duple("Assets", "<empty>", out, indent);
    out << std::endl;
  } else {
    Print::Duple("Assets", mashup.assets.size(), out, indent);
    for(const auto& asset_desc : mashup.assets) {
      Print(*asset_desc, out, indent + 2);
    }
  }
  Print::Simple("Scene", out, indent);
  out << std::endl;
  Print(*mashup.scene, out, indent + 2);
};

void MashupInspector::Print(const AssetDescriptor& asset_desc,
                            std::stringstream& out, int indent) {
  Print::Duple("ID", asset_desc.id, out, indent);
  Print::Duple("Type", asset_desc.type, out, indent);
  Print::Duple("Length", asset_desc.length, out, indent);
  if (asset_desc.qualities.size() == 0) {
    Print::Duple("Qualities", "<empty>", out, indent);
    out << std::endl;
  } else {
    Print::Duple("Qualities", asset_desc.qualities.size(), out, indent);
    for(const auto& asset_quality_desc : asset_desc.qualities) {
      Print(*asset_quality_desc, out, indent + 2);
      out << std::endl;
    }
  }
};

void MashupInspector::Print(const AssetQualityDescriptor& asset_quality_desc,
                            std::stringstream& out, int indent) {
  Print::Duple("Path", asset_quality_desc.path, out, indent);
  Print::Duple("Bitrate", asset_quality_desc.bitrate, out, indent);
  Print::Duple("Width", asset_quality_desc.natural_width, out, indent);
  Print::Duple("Height", asset_quality_desc.natural_height, out, indent);
};

void MashupInspector::Print(const Scene& scene,
                            std::stringstream& out, int indent) {
  Print::Duple("Camera", scene.camera, out, indent);
  Print::Duple("Width", scene.width, out, indent);
  Print::Duple("Height", scene.height, out, indent);
  Print::Duple("Length", scene.length, out, indent);
  if (scene.actors.size() == 0) {
    Print::Duple("Actors", "<empty>", out, indent);
    out << std::endl;
  } else {
    Print::Duple("Actors", scene.actors.size(), out, indent);
    for(const auto& actor : scene.actors) {
      Print(*actor, out, indent + 2);
      out << std::endl;
    }
  }
  if (scene.transitions.size() == 0) {
    Print::Duple("Transitions", "<empty>", out, indent);
    out << std::endl;
  } else {
    Print::Duple("Transtions", scene.transitions.size(), out, indent);
    for(const auto& transition : scene.transitions) {
      Print(*transition, out, indent + 2);
      out << std::endl;
    }
  }
};

void MashupInspector::Print(const Actor& actor,
                            std::stringstream& out, int indent) {
  Print::Duple("Asset ID", actor.asset_id, out, indent);
  Print::Duple("Asset Inst", actor.instance_id, out, indent);
  Print::Duple("Enters At", actor.enters_at, out, indent);
  Print::Duple("Exists At", actor.exits_at, out, indent);
  Print::Duple("X", actor.x, out, indent);
  Print::Duple("Y", actor.y, out, indent);
  Print::Duple("Z", actor.z, out, indent);
  Print::Duple("Rotation", actor.rotation, out, indent);
  Print::Duple("Width", actor.width, out, indent);
  Print::Duple("Height", actor.height, out, indent);
};

void MashupInspector::Print(const Transition& transition,
                            std::stringstream& out, int indent) {
  Print::Duple("Asset ID", transition.asset_id, out, indent);
  Print::Duple("Asset Inst", transition.instance_id, out, indent);
  Print::Duple("Target", BaseProperty::Get(transition.target), out, indent);
  Print::Duple("Starts at", transition.starts_at, out, indent);
  Print::Duple("Duration", transition.duration, out, indent);
  Print::Duple("Easing", transition.easing, out, indent);
};
