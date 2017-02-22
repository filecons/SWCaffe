#ifndef _LAYERPARAMETER_H_
#define _LAYERPARAMETER_H_
#include <string>
#include <caffe/blob.hpp>
//#include <caffe/common.hpp>

namespace caffe {

enum Phase {
  TRAIN = 0,
  TEST = 1
};

class NetStateRule {
  // Set phase to require the NetState have a particular phase (TRAIN or TEST)
  // to meet this rule.
public:
  //TODO
  bool has_phase() const { return has_phase_; }
  Phase phase() const { return phase_; }
  bool has_min_level() const { return has_min_level_; }
  bool has_max_level() const { return has_max_level_; }
  int min_level() const { return min_level_; }
  int max_level() const { return max_level_; }
  int stage_size() const { return stage_.size(); }
  int not_stage_size() const { return not_stage_.size(); }
  std::string stage( int id ) const { return stage_[id]; }
  std::string not_stage( int id ) const { return not_stage_[id]; }
private:
  Phase phase_;
  bool has_phase_;

  // Set the minimum and/or maximum levels in which the layer should be used.
  // Leave undefined to meet the rule regardless of level.
  int min_level_;
  bool has_min_level_;
  int max_level_;
  bool has_max_level_;

  // Customizable sets of stages to include or exclude.
  // The net must have ALL of the specified stages and NONE of the specified
  // "not_stage"s to meet the rule.
  // (Use multiple NetStateRules to specify conjunctions of stages.)
  std::vector<std::string> stage_;
  std::vector<std::string> not_stage_;
};

class ParamSpec {

public:

  // Whether to require shared weights to have the same shape, or just the same
  // count -- defaults to STRICT if unspecified.
  enum DimCheckMode {
    // STRICT (default) requires that num, channels, height, width each match.
    STRICT = 0,
    // PERMISSIVE requires only the count (num*channels*height*width) to match.
    PERMISSIVE = 1
  };
  float lr_mult() const { return lr_mult_; }
  //TODO
  float decay_mult() const { return decay_mult_; }
  bool has_lr_mult() const { return false; }
  bool has_decay_mult() const { return false; }
  DimCheckMode share_mode() const { return share_mode_; }
  std::string name() const { return name_; }
private:
  // The names of the parameter blobs -- useful for sharing parameters among
  // layers, but never required otherwise.  To share a parameter between two
  // layers, give it a (non-empty) name.
  std::string name_;

  DimCheckMode share_mode_;

  // The multiplier on the global learning rate for this parameter.
  float lr_mult_; // = 1.0;

  // The multiplier on the global weight decay for this parameter.
  float decay_mult_; // = 1.0;
};




//template <typename DType>
class LayerParameter {
  public:
    inline const std::string name() const { return name_; }
    void set_name(std::string name) { name_ = name; }

    inline const std::string type() const { return type_; }
    void set_type(std::string type) { type_ = type; }

    inline int loss_weight_size() const {return loss_weight_.size();}
    inline float loss_weight(int id) const { return loss_weight_[id]; }
    inline std::vector<float> get_loss_weight_vec() const { return loss_weight_; }

    inline Phase phase() const {return phase_;}

    inline int blobs_size() const { return blobs_.size(); }

    inline const std::vector<std::string>& get_bottom_vec() const { return bottom_; }
    inline std::string bottom( int id ) const { return bottom_[id]; }
    inline void set_bottom (int id, std::string name) { bottom_[id] = name; }
    inline int bottom_size() const { return bottom_.size(); }
    void add_bottom(std::string bottom_name) { bottom_.push_back(bottom_name); }

    inline const std::vector<std::string>& get_top_vec() const { return top_; }
    inline std::string top( int id ) const { return top_[id]; }
    inline int top_size() const { return top_.size(); }
    void add_top(std::string top_name) { top_.push_back(top_name); }

    inline const std::vector<bool> get_propagate_down_vec() const 
    { return propagate_down_; } 
    inline int propagate_down_size() const { return propagate_down_.size(); }

    inline const std::vector<ParamSpec> get_param_vec() const {
      return param_;
    }
    inline int param_size() const { return param_.size(); }

    NetStateRule include( int id ) const { return include_[id]; }
    inline const std::vector<NetStateRule> get_include_vec() const {
      return include_;
    }
    inline int include_size() const { return include_.size(); }

    inline const std::vector<NetStateRule> get_exclude_vec() const {
      return exclude_;
    }
    inline int exclude_size() const { return exclude_.size(); }

    NetStateRule exclude( int id ) const { return exclude_[id]; }


    //TODO
    inline const ParamSpec& param( int id ) const { return param_[id]; }
    void clear_loss_weight() { loss_weight_.clear(); }
    void add_loss_weight(float loss_weight) { loss_weight_.push_back(loss_weight); }
    void Clear() {
      bottom_.clear();
      top_.clear();
      loss_weight_.clear();
      param_.clear();
      blobs_.clear();
      propagate_down_.clear();
      include_.clear();
      exclude_.clear();
    }
    //TODO
    bool has_phase() const { return false; }
    void set_phase( const Phase& newphase ) { phase_ = newphase; }
    bool propagate_down( int id ) const { return propagate_down_[id]; }

    //TODO type only support float
    inline const std::vector<shared_ptr<Blob<float> > > get_blobs_vec() const {
      return blobs_;
    }


#define COPY_VEC(name)\
    name##_.assign(other.get_##name##_vec().begin(), other.get_##name##_vec().end()) 

    void CopyFrom(const LayerParameter& other) {
      set_name(other.name());
      set_type(other.type());
      set_phase(other.phase());

      COPY_VEC(bottom);
      COPY_VEC(top);
      COPY_VEC(loss_weight);
      COPY_VEC(param);
      COPY_VEC(blobs);
      COPY_VEC(propagate_down);
      COPY_VEC(include);
      COPY_VEC(exclude);
    }

    //TODO
    LayerParameter() {
      set_name("NO_NAME");
      set_type("NO_TYPE");
      phase_ = TEST;
      bottom_.resize(0);
      top_.resize(0);
      blobs_.resize(0);
      propagate_down_.resize(0);
      include_.resize(0);
      exclude_.resize(0);
    }
    ~LayerParameter() {}

  private:
    std::string name_;
    std::string type_;
    Phase phase_;

    vector<std::string> bottom_;
    vector<std::string> top_;

    std::vector<float> loss_weight_;
    std::vector<ParamSpec> param_;
    //TODO why no template
    std::vector<shared_ptr<Blob<float> > > blobs_;
    std::vector<bool> propagate_down_;
    std::vector<NetStateRule> include_;
    std::vector<NetStateRule> exclude_;

};

}//end caffe
#endif