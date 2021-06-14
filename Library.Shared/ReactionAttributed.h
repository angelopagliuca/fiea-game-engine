#pragma once

#include "Reaction.h"
#include "Event.h"
#include "EventMessageAttributed.h"

namespace FieaGameEngine
{
	/// <summary>
	/// ReactionAttributed is the base Reaction standalone class, it subscribes to events of EventMessageAttributed and has all the
	/// special members implemented. It accepts attributed events in its notify so that if the event subtype matches the reaction subtype,
	/// it executes its ActionList::Update.
	/// </summary>
	class ReactionAttributed final : public Reaction
	{
		RTTI_DECLARATIONS(ReactionAttributed, Reaction)

	public:
		/// <summary>
		/// Constructor that passes down rtti id type, subscribes to Events of EventMessageAttributed
		/// </summary>
		ReactionAttributed();
		/// <summary>
		/// passes down rtti id type and sets name and subtype and subscribes to Events of EventMessageAttributed
		/// </summary>
		/// <param name="name">name to set</param>
		/// <param name="subType">subtype to set</param>
		ReactionAttributed(const std::string& name, const std::string& subType);
		/// <summary>
		/// passes down rtti id type and sets subtype and subscribes to Events of EventMessageAttributed
		/// </summary>
		/// <param name="subType"></param>
		explicit ReactionAttributed(const std::string& subType);
		/// <summary>
		/// Copy Constructor of ReactionAttributed, subscribes to Events of EventMessageAttributed
		/// </summary>
		/// <param name=""></param>
		ReactionAttributed(const ReactionAttributed&);
		/// <summary>
		/// Move Constructor of ReactionAttributed, subscribes to Events of EventMessageAttributed
		/// </summary>
		/// <param name=""></param>
		/// <returns></returns>
		ReactionAttributed(ReactionAttributed&&) noexcept;
		/// <summary>
		/// Copy Assignment operator of ReactionAttributed, subscribes to Events of EventMessageAttributed
		/// </summary>
		/// <param name=""></param>
		/// <returns>copied reaction attributed</returns>
		ReactionAttributed& operator=(const ReactionAttributed&);
		/// <summary>
		/// Move Assignment Operator of ReactionAttributed, subscribes to Events of EventMessageAttributed
		/// </summary>
		/// <param name=""></param>
		/// <returns>new moved ReactionAttributed</returns>
		ReactionAttributed& operator=(ReactionAttributed&&) noexcept;
		/// <summary>
		/// Destructor and unsubscribes to Events of EventMessageAttributed
		/// </summary>
		~ReactionAttributed();

		/// <summary>
		/// Accepts attributed events. If the event subtype matches the reaction subtype, it copies the
		/// attribute “arguments” to this instance of ReactionAttribute, and then executes ActionList::Update.
		/// </summary>
		/// <param name="ePublisher">attributed event to accept</param>
		void Notify(const class EventPublisher& ePublisher) override;

		/// <summary>
		/// SubType Getter
		/// </summary>
		/// <returns>SubType</returns>
		const std::string& SubType();
		/// <summary>
		/// SubType Setter
		/// </summary>
		/// <param name="subType">subType to set</param>
		void SetSubType(const std::string& subType);

		/// <summary>
		/// Clones the Reaction (virtual)
		/// </summary>
		/// <returns>new clone</returns>
		gsl::owner<ReactionAttributed*> Clone() const override;

		/// <summary>
		/// Vector of Prescribed Attributes Signatures of ReactionAttributed
		/// </summary>
		/// <returns>signatures</returns>
		static const Vector<Signature> Signatures();
	private:
		std::string _subType;
	};

	ConcreteFactory(ReactionAttributed, Scope)
}
