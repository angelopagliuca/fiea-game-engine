#pragma once

#include "Attributed.h"

namespace FieaGameEngine
{
	/// <summary>
	/// This is basic component for Reactions, it implements Attributes and holds the SubType of Reactions
	/// </summary>
	class EventMessageAttributed final : public Attributed
	{
		RTTI_DECLARATIONS(EventMessageAttributed, Attributed);

	public:
		/// <summary>
		/// Constructor that passes down rtti id type
		/// </summary>
		EventMessageAttributed();
		/// <summary>
		/// Constructor that passes down rtti id type and sets the subtype
		/// </summary>
		/// <param name="subType"></param>
		explicit EventMessageAttributed(const std::string& subType);
		EventMessageAttributed(const EventMessageAttributed&) = default;
		EventMessageAttributed(EventMessageAttributed&&) = default;
		EventMessageAttributed& operator=(const EventMessageAttributed&) = default;
		EventMessageAttributed& operator=(EventMessageAttributed&&) = default;
		~EventMessageAttributed() = default;

		/// <summary>
		/// SubType Getter
		/// </summary>
		/// <returns></returns>
		const std::string& SubType();
		/// <summary>
		/// SubType Setter
		/// </summary>
		/// <param name="subType">subtype to set</param>
		void SetSubType(const std::string& subType);

		/// <summary>
		/// Clones the EventMessageAttributed
		/// </summary>
		/// <returns>new clones event message</returns>
		gsl::owner<EventMessageAttributed*> Clone() const override;

		/// <summary>
		/// Vector of Prescribed Attributes Signatures of EventMessageAttributed
		/// </summary>
		/// <returns>signatures</returns>
		static const Vector<Signature> Signatures();

	private:
		std::string _subType;
	};
}
