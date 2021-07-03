#include "texture.hpp"
#include "ui.hpp"
#include "path.hpp"

extern TextureManager* g_texture_manager;
const Texture* reform, * button_ppv,* checkbox_ppv_on,* checkbox_ppv_off;

extern std::pair<int, int> mouse_pos;

#include "nation.hpp"
extern Nation* curr_nation;

void ui_reform_policies(UI::Widget&, void *) {
	reform = &g_texture_manager->load_texture(Path::get("ui/reform_win.png"));
	button_ppv = &g_texture_manager->load_texture(Path::get("ui/button_pvw.png"));
	checkbox_ppv_on = &g_texture_manager->load_texture(Path::get("ui/checkbox_ppv_on.png"));
	checkbox_ppv_off = &g_texture_manager->load_texture(Path::get("ui/checkbox_ppv_off.png"));
	
	UI::Window* reform_win = new UI::Window(mouse_pos.first, mouse_pos.second, reform->width, reform->height);
	reform_win->current_texture = reform;
	
	UI::Button* borders_btn = new UI::Button(9, 43, button_ppv->width, button_ppv->height, reform_win);
	borders_btn->text("Population control");
	borders_btn->current_texture = button_ppv;
	borders_btn->on_click = [](UI::Widget&, void *) {
		UI::Window* reform_win = new UI::Window(mouse_pos.first, mouse_pos.second, reform->width, reform->height);
		reform_win->current_texture = reform;
		
		UI::Button* migration_btn = new UI::Button(9, 43, button_ppv->width, button_ppv->height, reform_win);
		migration_btn->text("Migration");
		migration_btn->current_texture = button_ppv;
		migration_btn->on_click = [](UI::Widget&, void *) {
			UI::Window* reform_win = new UI::Window(mouse_pos.first, mouse_pos.second, reform->width, reform->height);
			reform_win->current_texture = reform;
			reform_win->text("Migration");
			
			UI::Button* nobody_btn = new UI::Button(9, 43, button_ppv->width, button_ppv->height, reform_win);
			nobody_btn->text("Nobody");
			nobody_btn->current_texture = button_ppv;
			nobody_btn->on_click = [](UI::Widget&, void *) {
				curr_nation->current_policy.migration = ALLOW_NOBODY;
			};
			
			UI::Button* accepted_only_btn = new UI::Button(9, 43, button_ppv->width, button_ppv->height, reform_win);
			accepted_only_btn->text("Accepted culture");
			accepted_only_btn->below_of(dynamic_cast<const UI::Widget&>(*nobody_btn));
			accepted_only_btn->current_texture = button_ppv;
			accepted_only_btn->on_click = [](UI::Widget&, void *) {
				curr_nation->current_policy.migration = ALLOW_ACCEPTED_CULTURES;
			};
			
			UI::Button* all_payment_btn = new UI::Button(9, 43, button_ppv->width, button_ppv->height, reform_win);
			all_payment_btn->text("All with payment");
			all_payment_btn->below_of(dynamic_cast<const UI::Widget&>(*accepted_only_btn));
			all_payment_btn->current_texture = button_ppv;
			all_payment_btn->on_click = [](UI::Widget&, void *) {
				curr_nation->current_policy.migration = ALLOW_ALL_PAYMENT;
			};
			
			UI::Button* allow_all_btn = new UI::Button(9, 43, button_ppv->width, button_ppv->height, reform_win);
			allow_all_btn->text("All");
			allow_all_btn->below_of(dynamic_cast<const UI::Widget&>(*all_payment_btn));
			allow_all_btn->current_texture = button_ppv;
			allow_all_btn->on_click = [](UI::Widget&, void *) {
				curr_nation->current_policy.migration = ALLOW_ALL;
			};
			
			UI::CloseButton* ok_btn = new UI::CloseButton(9, 0, button_ppv->width, button_ppv->height, reform_win);
			ok_btn->text("OK");
			ok_btn->below_of(dynamic_cast<const UI::Widget&>(*allow_all_btn));
			ok_btn->current_texture = button_ppv;
		};
		
		UI::Button* immigration_btn = new UI::Button(9, 43, button_ppv->width, button_ppv->height, reform_win);
		immigration_btn->text("Immigration");
		immigration_btn->below_of(dynamic_cast<const UI::Widget&>(*migration_btn));
		immigration_btn->current_texture = button_ppv;
		immigration_btn->on_click = [](UI::Widget&, void *) {
			UI::Window* reform_win = new UI::Window(mouse_pos.first, mouse_pos.second, reform->width, reform->height);
			reform_win->current_texture = reform;
			reform_win->text("Immigration");
			
			UI::Button* nobody_btn = new UI::Button(9, 43, button_ppv->width, button_ppv->height, reform_win);
			nobody_btn->text("Nobody");
			nobody_btn->current_texture = button_ppv;
			nobody_btn->on_click = [](UI::Widget&, void *) {
				curr_nation->current_policy.immigration = ALLOW_NOBODY;
			};
			
			UI::Button* accepted_only_btn = new UI::Button(9, 43, button_ppv->width, button_ppv->height, reform_win);
			accepted_only_btn->text("Accepted culture");
			accepted_only_btn->below_of(dynamic_cast<const UI::Widget&>(*nobody_btn));
			accepted_only_btn->current_texture = button_ppv;
			accepted_only_btn->on_click = [](UI::Widget&, void *) {
				curr_nation->current_policy.immigration = ALLOW_ACCEPTED_CULTURES;
			};
			
			UI::Button* all_payment_btn = new UI::Button(9, 43, button_ppv->width, button_ppv->height, reform_win);
			all_payment_btn->text("All with payment");
			all_payment_btn->below_of(dynamic_cast<const UI::Widget&>(*accepted_only_btn));
			all_payment_btn->current_texture = button_ppv;
			all_payment_btn->on_click = [](UI::Widget&, void *) {
				curr_nation->current_policy.immigration = ALLOW_ALL_PAYMENT;
			};
			
			UI::Button* allow_all_btn = new UI::Button(9, 43, button_ppv->width, button_ppv->height, reform_win);
			allow_all_btn->text("All");
			allow_all_btn->below_of(dynamic_cast<const UI::Widget&>(*all_payment_btn));
			allow_all_btn->current_texture = button_ppv;
			allow_all_btn->on_click = [](UI::Widget&, void *) {
				curr_nation->current_policy.immigration = ALLOW_ALL;
			};
			
			UI::CloseButton* ok_btn = new UI::CloseButton(9, 0, button_ppv->width, button_ppv->height, reform_win);
			ok_btn->text("OK");
			ok_btn->below_of(dynamic_cast<const UI::Widget&>(*allow_all_btn));
			ok_btn->current_texture = button_ppv;
		};
		
		UI::Button* treatment_btn = new UI::Button(9, 43, button_ppv->width, button_ppv->height, reform_win);
		treatment_btn->text("Treatment");
		treatment_btn->below_of(dynamic_cast<const UI::Widget&>(*immigration_btn));
		treatment_btn->current_texture = button_ppv;
		treatment_btn->on_click = [](UI::Widget&, void *) {
			UI::Window* reform_win = new UI::Window(mouse_pos.first, mouse_pos.second, reform->width, reform->height);
			reform_win->current_texture = reform;
			reform_win->text("Treatment");
			
			UI::Button* everyone_equal_btn = new UI::Button(9, 43, button_ppv->width, button_ppv->height, reform_win);
			everyone_equal_btn->text("Everyone equal");
			everyone_equal_btn->current_texture = button_ppv;
			everyone_equal_btn->on_click = [](UI::Widget&, void *) {
				curr_nation->current_policy.treatment = TREATMENT_EVERYONE_EQUAL;
			};
			
			UI::Button* only_accepted_btn = new UI::Button(9, 43, button_ppv->width, button_ppv->height, reform_win);
			only_accepted_btn->text("Only accepted");
			only_accepted_btn->below_of(dynamic_cast<const UI::Widget&>(*everyone_equal_btn));
			only_accepted_btn->current_texture = button_ppv;
			only_accepted_btn->on_click = [](UI::Widget&, void *) {
				curr_nation->current_policy.treatment = TREATMENT_ONLY_ACCEPTED;
			};
			
			UI::Button* enslaved_btn = new UI::Button(9, 43, button_ppv->width, button_ppv->height, reform_win);
			enslaved_btn->text("Enslaved");
			enslaved_btn->below_of(dynamic_cast<const UI::Widget&>(*only_accepted_btn));
			enslaved_btn->current_texture = button_ppv;
			enslaved_btn->on_click = [](UI::Widget&, void *) {
				curr_nation->current_policy.treatment = TREATMENT_ENSLAVED;
			};
			
			UI::Button* exterminate_btn = new UI::Button(9, 43, button_ppv->width, button_ppv->height, reform_win);
			exterminate_btn->text("Exterminate");
			exterminate_btn->below_of(dynamic_cast<const UI::Widget&>(*enslaved_btn));
			exterminate_btn->current_texture = button_ppv;
			exterminate_btn->on_click = [](UI::Widget&, void *) {
				curr_nation->current_policy.treatment = TREATMENT_EXTERMINATE;
			};
			
			UI::CloseButton* ok_btn = new UI::CloseButton(9, 0, button_ppv->width, button_ppv->height, reform_win);
			ok_btn->text("OK");
			ok_btn->below_of(dynamic_cast<const UI::Widget&>(*exterminate_btn));
			ok_btn->current_texture = button_ppv;
		};
		
		UI::CloseButton* ok_btn = new UI::CloseButton(9, 0, button_ppv->width, button_ppv->height, reform_win);
		ok_btn->text("OK");
		ok_btn->below_of(dynamic_cast<const UI::Widget&>(*treatment_btn));
		ok_btn->current_texture = button_ppv;
	};
	
	UI::Button* goverment_btn = new UI::Button(9, 0, button_ppv->width, button_ppv->height, reform_win);
	goverment_btn->text("Goverment");
	goverment_btn->below_of(dynamic_cast<const UI::Widget&>(*borders_btn));
	goverment_btn->current_texture = button_ppv;
	goverment_btn->on_click = [](UI::Widget&, void *) {
		UI::Window* reform_win = new UI::Window(mouse_pos.first, mouse_pos.second, reform->width, reform->height);
		reform_win->text("Goverment policies");
		reform_win->current_texture = reform;
		
		UI::Button* censorship_btn = new UI::Button(9, 43, button_ppv->width, button_ppv->height, reform_win);
		censorship_btn->text("Free press");
		censorship_btn->current_texture = button_ppv;
		censorship_btn->on_click = [](UI::Widget&, void *) {
			UI::Window* reform_win = new UI::Window(mouse_pos.first, mouse_pos.second, reform->width, reform->height);
			reform_win->text("Free press");
			reform_win->current_texture = reform;
			
			UI::Button* all_censored_btn = new UI::Button(9, 43, button_ppv->width, button_ppv->height, reform_win);
			all_censored_btn->text("All censored");
			all_censored_btn->current_texture = button_ppv;
			all_censored_btn->on_click = [](UI::Widget&, void *) {
				curr_nation->current_policy.censorship = CENSORSHIP_ALL_CENSORED;
			};
			
			UI::Button* only_state_btn = new UI::Button(9, 43, button_ppv->width, button_ppv->height, reform_win);
			only_state_btn->text("State only");
			only_state_btn->current_texture = button_ppv;
			only_state_btn->below_of(dynamic_cast<const UI::Widget&>(*all_censored_btn));
			only_state_btn->on_click = [](UI::Widget&, void *) {
				curr_nation->current_policy.censorship = CENSORSHIP_ONLY_STATE;
			};
			
			UI::Button* only_approved_btn = new UI::Button(9, 43, button_ppv->width, button_ppv->height, reform_win);
			only_approved_btn->text("Approved only");
			only_approved_btn->current_texture = button_ppv;
			only_approved_btn->below_of(dynamic_cast<const UI::Widget&>(*only_state_btn));
			only_approved_btn->on_click = [](UI::Widget&, void *) {
				curr_nation->current_policy.censorship = CENSORSHIP_ONLY_APPROVED;
			};
			
			UI::Button* all_allowed_btn = new UI::Button(9, 43, button_ppv->width, button_ppv->height, reform_win);
			all_allowed_btn->text("All allowed");
			all_allowed_btn->current_texture = button_ppv;
			all_allowed_btn->below_of(dynamic_cast<const UI::Widget&>(*only_approved_btn));
			all_allowed_btn->on_click = [](UI::Widget&, void *) {
				curr_nation->current_policy.censorship = CENSORSHIP_ALL_ALLOWED;
			};
			
			UI::CloseButton* ok_btn = new UI::CloseButton(9, 0, button_ppv->width, button_ppv->height, reform_win);
			ok_btn->text("OK");
			ok_btn->below_of(dynamic_cast<const UI::Widget&>(*all_allowed_btn));
			ok_btn->current_texture = button_ppv;
		};
		
		UI::Button* build_infra_btn = new UI::Button(9, 43, button_ppv->width, button_ppv->height, reform_win);
		build_infra_btn->text("Infrastructure");
		build_infra_btn->below_of(dynamic_cast<const UI::Widget&>(*censorship_btn));
		build_infra_btn->current_texture = button_ppv;
		build_infra_btn->on_click = [](UI::Widget&, void *) {
			UI::Window* reform_win = new UI::Window(mouse_pos.first, mouse_pos.second, reform->width, reform->height);
			reform_win->text("Infrastructure built by POPs");
			reform_win->current_texture = reform;
			
			UI::Button* not_allowed_btn = new UI::Button(9, 43, button_ppv->width, button_ppv->height, reform_win);
			not_allowed_btn->text("Not allowed");
			not_allowed_btn->current_texture = button_ppv;
			not_allowed_btn->on_click = [](UI::Widget&, void *) {
				curr_nation->current_policy.build_infrastructure = AUTO_BUILD_NONE;
			};
			
			UI::Button* with_approval_btn = new UI::Button(9, 43, button_ppv->width, button_ppv->height, reform_win);
			with_approval_btn->text("With approval");
			with_approval_btn->current_texture = button_ppv;
			with_approval_btn->below_of(dynamic_cast<const UI::Widget&>(*not_allowed_btn));
			with_approval_btn->on_click = [](UI::Widget&, void *) {
				curr_nation->current_policy.build_infrastructure = AUTO_BUILD_ONLY_APPROVED;
			};
			
			UI::Button* allowed_btn = new UI::Button(9, 43, button_ppv->width, button_ppv->height, reform_win);
			allowed_btn->text("Allowed");
			allowed_btn->current_texture = button_ppv;
			allowed_btn->below_of(dynamic_cast<const UI::Widget&>(*with_approval_btn));
			allowed_btn->on_click = [](UI::Widget&, void *) {
				curr_nation->current_policy.build_infrastructure = AUTO_BUILD_ALLOWED;
			};
			
			UI::CloseButton* ok_btn = new UI::CloseButton(9, 0, button_ppv->width, button_ppv->height, reform_win);
			ok_btn->text("OK");
			ok_btn->below_of(dynamic_cast<const UI::Widget&>(*allowed_btn));
			ok_btn->current_texture = button_ppv;
		};
		
		UI::Button* build_factories_btn = new UI::Button(9, 43, button_ppv->width, button_ppv->height, reform_win);
		build_factories_btn->text("Industry");
		build_factories_btn->below_of(dynamic_cast<const UI::Widget&>(*build_infra_btn));
		build_factories_btn->current_texture = button_ppv;
		build_factories_btn->on_click = [](UI::Widget&, void *) {
			UI::Window* reform_win = new UI::Window(mouse_pos.first, mouse_pos.second, reform->width, reform->height);
			reform_win->text("Industry built by POPs");
			reform_win->current_texture = reform;
			
			UI::Button* not_allowed_btn = new UI::Button(9, 43, button_ppv->width, button_ppv->height, reform_win);
			not_allowed_btn->text("Not allowed");
			not_allowed_btn->current_texture = button_ppv;
			not_allowed_btn->on_click = [](UI::Widget&, void *) {
				curr_nation->current_policy.build_factories = AUTO_BUILD_NONE;
			};
			
			UI::Button* with_approval_btn = new UI::Button(9, 43, button_ppv->width, button_ppv->height, reform_win);
			with_approval_btn->text("With approval");
			with_approval_btn->current_texture = button_ppv;
			with_approval_btn->below_of(dynamic_cast<const UI::Widget&>(*not_allowed_btn));
			with_approval_btn->on_click = [](UI::Widget&, void *) {
				curr_nation->current_policy.build_factories = AUTO_BUILD_ONLY_APPROVED;
			};
			
			UI::Button* allowed_btn = new UI::Button(9, 43, button_ppv->width, button_ppv->height, reform_win);
			allowed_btn->text("Allowed");
			allowed_btn->current_texture = button_ppv;
			allowed_btn->below_of(dynamic_cast<const UI::Widget&>(*with_approval_btn));
			allowed_btn->on_click = [](UI::Widget&, void *) {
				curr_nation->current_policy.build_factories = AUTO_BUILD_ALLOWED;
			};
			
			UI::CloseButton* ok_btn = new UI::CloseButton(9, 0, button_ppv->width, button_ppv->height, reform_win);
			ok_btn->text("OK");
			ok_btn->below_of(dynamic_cast<const UI::Widget&>(*allowed_btn));
			ok_btn->current_texture = button_ppv;
		};
		
		UI::Checkbox* national_id_check = new UI::Checkbox(9, 43, button_ppv->width, button_ppv->height, reform_win);
		national_id_check->text("National ID");
		national_id_check->below_of(dynamic_cast<const UI::Widget&>(*build_factories_btn));
		if(curr_nation->current_policy.national_id) {
			national_id_check->current_texture = checkbox_ppv_on;
		} else {
			national_id_check->current_texture = checkbox_ppv_off;
		}
		national_id_check->on_click = [](UI::Widget& w, void *) {
			curr_nation->current_policy.national_id = !curr_nation->current_policy.national_id;
			if(curr_nation->current_policy.national_id) {
				w.current_texture = checkbox_ppv_on;
			} else {
				w.current_texture = checkbox_ppv_off;
			}
		};
		
		UI::CloseButton* ok_btn = new UI::CloseButton(9, 0, button_ppv->width, button_ppv->height, reform_win);
		ok_btn->text("OK");
		ok_btn->below_of(dynamic_cast<const UI::Widget&>(*national_id_check));
		ok_btn->current_texture = button_ppv;
	};
	
	UI::Button* taxes_btn = new UI::Button(9, 0, button_ppv->width, button_ppv->height, reform_win);
	taxes_btn->text("Taxes");
	taxes_btn->below_of(dynamic_cast<const UI::Widget&>(*goverment_btn));
	taxes_btn->current_texture = button_ppv;
	taxes_btn->on_click = [](UI::Widget&, void *) {
		// TODO: Actually do something
	};
	
	UI::Button* property_btn = new UI::Button(9, 0, button_ppv->width, button_ppv->height, reform_win);
	property_btn->text("Property");
	property_btn->below_of(dynamic_cast<const UI::Widget&>(*taxes_btn));
	property_btn->current_texture = button_ppv;
	property_btn->on_click = [](UI::Widget&, void *) {
		UI::Window* reform_win = new UI::Window(mouse_pos.first, mouse_pos.second, reform->width, reform->height);
		reform_win->text("Property policies");
		reform_win->current_texture = reform;
		
		UI::Checkbox* private_property_check = new UI::Checkbox(9, 43, button_ppv->width, button_ppv->height, reform_win);
		private_property_check->text("Private property");
		private_property_check->on_click = [](UI::Widget& w, void *) {
			curr_nation->current_policy.private_property = !curr_nation->current_policy.private_property;
		};
		private_property_check->on_update = [](UI::Widget& w, void *) {
			w.current_texture =
				(curr_nation->current_policy.private_property)
				? checkbox_ppv_on
				: checkbox_ppv_off;
		};

		UI::Checkbox* companies_check = new UI::Checkbox(9, 43, button_ppv->width, button_ppv->height, reform_win);
		companies_check->text("Companies");
		companies_check->below_of(dynamic_cast<const UI::Widget&>(*private_property_check));
		companies_check->on_click = [](UI::Widget& w, void *) {
			curr_nation->current_policy.companies_allowed = !curr_nation->current_policy.companies_allowed;
		};
		companies_check->on_update = [](UI::Widget& w, void *) {
			w.current_texture =
				(curr_nation->current_policy.companies_allowed)
				? checkbox_ppv_on
				: checkbox_ppv_off;
		};

		UI::Checkbox* public_education_check = new UI::Checkbox(9, 43, button_ppv->width, button_ppv->height, reform_win);
		public_education_check->text("Public education");
		public_education_check->below_of(dynamic_cast<const UI::Widget&>(*companies_check));
		public_education_check->on_click = [](UI::Widget& w, void *) {
			curr_nation->current_policy.public_education = !curr_nation->current_policy.public_education;
		};
		public_education_check->on_update = [](UI::Widget& w, void *) {
			w.current_texture =
				(curr_nation->current_policy.public_education)
				? checkbox_ppv_on
				: checkbox_ppv_off;
		};

		UI::Checkbox* public_healthcare_check = new UI::Checkbox(9, 43, button_ppv->width, button_ppv->height, reform_win);
		public_healthcare_check->text("Public education");
		public_healthcare_check->below_of(dynamic_cast<const UI::Widget&>(*companies_check));
		public_healthcare_check->on_click = [](UI::Widget& w, void *) {
			curr_nation->current_policy.public_healthcare = !curr_nation->current_policy.public_healthcare;
		};
		public_healthcare_check->on_update = [](UI::Widget& w, void *) {
			w.current_texture =
				(curr_nation->current_policy.public_healthcare)
				? checkbox_ppv_on
				: checkbox_ppv_off;
		};

		UI::CloseButton* ok_btn = new UI::CloseButton(9, 0, button_ppv->width, button_ppv->height, reform_win);
		ok_btn->text("OK");
		ok_btn->below_of(dynamic_cast<const UI::Widget&>(*public_healthcare_check));
		ok_btn->current_texture = button_ppv;
	};
	
	UI::Button* enact_btn = new UI::Button(9, 0, button_ppv->width, button_ppv->height, reform_win);
	enact_btn->text("Enact policy");
	enact_btn->below_of(dynamic_cast<const UI::Widget&>(*property_btn));
	enact_btn->current_texture = button_ppv;
	enact_btn->on_click = [](UI::Widget&, void *) {
		// TODO: Actually do something
	};
	
	UI::CloseButton* ok_btn = new UI::CloseButton(9, 0, button_ppv->width, button_ppv->height, reform_win);
	ok_btn->text("OK");
	ok_btn->below_of(dynamic_cast<const UI::Widget&>(*enact_btn));
	ok_btn->current_texture = button_ppv;
}
