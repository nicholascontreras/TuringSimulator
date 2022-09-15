#include "MachineState.h"

size_t hash<MachineState>::operator()(MachineState const& state) const noexcept {
	return hash<string>{}(state.name);
}