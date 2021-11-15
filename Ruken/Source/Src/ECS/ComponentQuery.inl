
template <ComponentType ... TComponents>
RkVoid ComponentQuery::SetupInclusionQuery() noexcept
{
    (m_included.Add(TComponents::GetId()), ...);
}

template <ComponentType ... TComponents>
RkVoid ComponentQuery::SetupExclusionQuery() noexcept
{
    (m_excluded.Add(TComponents::GetId()), ...);
}