#ifndef POLICY_H
#define POLICY_H

enum AllowancePolicy {
	// Nobody can enter the country
	ALLOW_NOBODY,

	// Only accepted cultures can enter the country
	ALLOW_ACCEPTED_CULTURES,

	// Everyone can enter, but at a cost
	ALLOW_ALL_PAYMENT,

	// Everyone can exit freely
	ALLOW_ALL,
};

enum CensorshipPolicy {
	// All media censored
	CENSORSHIP_ALL_CENSORED,

	// Only state run media allowed
	CENSORSHIP_ONLY_STATE,

	// Only "approved" media is allowed
	CENSORSHIP_ONLY_APPROVED,

	// All media allowed
	CENSORSHIP_ALL_ALLOWED,
};

enum AutoBuildPolicy {
	// POPs cannot auto build stuff
	AUTO_BUILD_NONE,

	// POPs can only build with approval
	AUTO_BUILD_ONLY_APPROVED,

	// All POPs can build freely
	AUTO_BUILD_ALLOWED,
};

enum TreatmentPolicy {
	// Everyone is equal
	TREATMENT_EVERYONE_EQUAL,
	
	// Only accepted POPs are treated
	TREATMENT_ONLY_ACCEPTED,
	
	// Non-accepted are enslaved
	TREATMENT_ENSLAVED,
	
	// Exterminate
	TREATMENT_EXTERMINATE,
};

class Policies {
public:
	TreatmentPolicy treatment;
	
	AllowancePolicy migration;
	AllowancePolicy immigration;
	CensorshipPolicy censorship;

	// Whetever POPs are able to build infrastructure
	AutoBuildPolicy build_infrastructure;

	// Whatever POPs are able to build factories
	AutoBuildPolicy build_factories;

	bool national_id;

	bool men_suffrage;
	bool men_labour;
	
	bool women_suffrage;
	bool women_labour;

	bool private_property;
	bool companies_allowed;

	bool public_education;
	bool secular_education;

	bool public_healthcare;

	// Goverment pays bought food
	bool social_security;

	// Is slavery allowed?
	bool slavery;
	
	// Is there a parliament that is needed to approve laws?
	bool legislative_parliament;
	
	// Is there a parliament that is needed to do executive actions?
	bool executive_parliament;

	// Is there a constitution to limit the power of the leader?
	bool constitutional;

	// Do we allow foreign trade?
	bool foreign_trade;

	// Tax for importing products
	float import_tax;

	// Tax for exporting products
	float export_tax;

	// Flat tax rhs on the low-wage pops
	float poor_flat_tax;

	// Flat tax rhs on the medium-wage pops
	float med_flat_tax;

	// Flat tax rhs on the high-wage pops
	float rich_flat_tax;

	// Spending done on military (% of budget)
	float military_spending;
	
	int difference(Policies& rhs) {
		int diff;
		diff += std::abs(rhs.migration - this->migration);
		diff += std::abs(rhs.immigration - this->immigration);
		diff += std::abs(rhs.censorship - this->censorship);
		diff += std::abs(rhs.build_factories - this->build_factories);
		diff += std::abs(rhs.build_infrastructure - this->build_infrastructure);
		diff += (rhs.national_id != this->national_id) ? 1 : 0;
		diff += (rhs.men_suffrage != this->men_suffrage) ? 1 : 0;
		diff += (rhs.men_labour != this->men_labour) ? 1 : 0;
		diff += (rhs.women_suffrage != this->women_suffrage) ? 1 : 0;
		diff += (rhs.women_labour != this->women_labour) ? 1 : 0;
		diff += (rhs.private_property != this->private_property) ? 1 : 0;
		diff += (rhs.companies_allowed != this->companies_allowed) ? 1 : 0;
		diff += (rhs.public_education != this->public_education) ? 1 : 0;
		diff += (rhs.secular_education != this->secular_education) ? 1 : 0;
		diff += (rhs.public_healthcare != this->public_healthcare) ? 1 : 0;
		diff += (rhs.social_security != this->social_security) ? 1 : 0;
		diff += (rhs.slavery != this->slavery) ? 1 : 0;
		diff += std::abs(rhs.import_tax != this->import_tax);
		diff += std::abs(rhs.export_tax != this->export_tax);
		diff += std::abs(rhs.poor_flat_tax != this->poor_flat_tax);
		diff += std::abs(rhs.med_flat_tax != this->med_flat_tax);
		diff += std::abs(rhs.rich_flat_tax != this->rich_flat_tax);
		return diff;
	}
};

#endif