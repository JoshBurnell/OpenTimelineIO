#pragma once

#include "opentimelineio/version.h"
#include "opentimelineio/composition.h"

namespace opentimelineio { namespace OPENTIMELINEIO_VERSION  {

class Clip;

class Track : public Composition {
public:
    struct Kind {
        static auto constexpr video = "Video";
        static auto constexpr audio = "Audio";
    };
    
    enum NeighborGapPolicy {
        never = 0,
        around_transitions = 1
    };
        
    struct Schema {
        static auto constexpr name = "Track";
        static int constexpr version = 1;
    };

    using Parent = Composition;

    Track(std::string const& name = std::string(),
          optional<TimeRange> const& source_range = nullopt,
          std::string const& = Kind::video,
          AnyDictionary const& metadata = AnyDictionary());

    std::string const& kind() const {
        return _kind;
    }
    
    void set_kind(std::string const& kind) {
        _kind = kind;
    }

    virtual TimeRange range_of_child_at_index(int index, ErrorStatus* error_status) const;
    virtual TimeRange trimmed_range_of_child_at_index(int index, ErrorStatus* error_status) const;
    virtual TimeRange available_range(ErrorStatus* error_status) const;

    virtual std::pair<optional<RationalTime>, optional<RationalTime>>
    handles_of_child(Composable const* child, ErrorStatus* error_status) const;

    std::pair<Retainer<Composable>, Retainer<Composable>>
    neighbors_of(Composable const* item, ErrorStatus* error_status, NeighborGapPolicy insert_gap = NeighborGapPolicy::never) const;

    virtual std::map<Composable*, TimeRange> range_of_all_children(ErrorStatus* error_status) const;

    // Return a vector of clips.
    //
    // An optional search_range may be provided to limit the search.
    //
    // If shallow_search is false, will recurse into compositions.
    std::vector<Retainer<Clip> > clip_if(
        ErrorStatus* error_status,
        optional<TimeRange> const& search_range = nullopt,
        bool shallow_search = false) const;

protected:
    virtual ~Track();
    virtual std::string const& composition_kind() const;

    virtual bool read_from(Reader&);
    virtual void write_to(Writer&) const;

private:
    std::string _kind;
};

} }
