// THIS .TPP SHOULD NOT BE COMPILED DIRECTLY
// THIS .TPP SHOULD ONLY BE INCLUDED BY COMMON.CPP AND ENTITIES .CPP

#ifdef COMMON_CPP
template class std::vector<CharacterAppearance>;
template class std::expected<void, Error>;
#endif