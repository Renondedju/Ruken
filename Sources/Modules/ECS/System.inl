
template <EventHandlerType TEventHandler>
RkVoid System::SetupEventHandler() noexcept
{
    std::unique_ptr<TEventHandler> instance = std::make_unique<TEventHandler>();
    m_handlers.insert_or_assign(instance->GetHandledEvent(), std::move(instance));
}