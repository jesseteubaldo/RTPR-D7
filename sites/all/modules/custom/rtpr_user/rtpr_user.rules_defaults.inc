<?php
/**
 * @file
 * rtpr_user.rules_defaults.inc
 */

/**
 * Implements hook_default_rules_configuration().
 */
function rtpr_user_default_rules_configuration() {
  $items = array();
  $items['rules_create_an_affiliate_account_on_pap_after_using_referral'] = entity_import('rules_config', '{ "rules_create_an_affiliate_account_on_pap_after_using_referral" : {
      "LABEL" : "Update referral relationship on PAP after using referral",
      "PLUGIN" : "reaction rule",
      "WEIGHT" : "5",
      "OWNER" : "rules",
      "TAGS" : [ "Post Affiliate Pro" ],
      "REQUIRES" : [ "referral", "rtpr_user", "rules" ],
      "ON" : { "user_insert" : [] },
      "IF" : [ { "user_referred" : { "user" : [ "account" ] } } ],
      "DO" : [
        { "rtpr_user_action_get_referring_user" : {
            "USING" : { "account" : [ "account" ] },
            "PROVIDE" : { "referring_user" : { "referring_user" : "Referring user" } }
          }
        },
        { "rtpr_user_action_add_affiliate_relations" : { "referring-user" : [ "referring-user" ], "new-user" : [ "account" ] } }
      ]
    }
  }');
  $items['rules_remove_scheduled_roles_upgrade_downgrade_tasks_for_a_user'] = entity_import('rules_config', '{ "rules_remove_scheduled_roles_upgrade_downgrade_tasks_for_a_user" : {
      "LABEL" : "Remove scheduled roles upgrade\\/downgrade tasks for a user",
      "PLUGIN" : "rule",
      "OWNER" : "rules",
      "TAGS" : [ "Post Affiliate Pro" ],
      "REQUIRES" : [ "rules", "rules_scheduler" ],
      "USES VARIABLES" : { "account" : { "label" : "Account", "type" : "user" } },
      "DO" : [
        { "schedule_delete" : {
            "component" : "rules_set_affiliate_role_commission_groups",
            "task" : "Scheduled rules evaluation for Affiliate user [account:uid]"
          }
        },
        { "schedule_delete" : {
            "component" : "rules_set_vendor_role_commission_groups",
            "task" : "Scheduled rules evaluation for Vendor [account:uid]"
          }
        },
        { "schedule_delete" : {
            "component" : "rules_set_gold_role_commission_groups",
            "task" : "Scheduled rules evaluation for Gold user [account:uid]"
          }
        },
        { "schedule_delete" : {
            "component" : "commerce_shipping_service_free_shipping",
            "task" : "Scheduled rules evaluation for Platinum user [account:uid]"
          }
        }
      ]
    }
  }');
  $items['rules_set_account_manager_commission_groups'] = entity_import('rules_config', '{ "rules_set_account_manager_commission_groups" : {
      "LABEL" : "Set Account Manager Commission Groups",
      "PLUGIN" : "rule set",
      "OWNER" : "rules",
      "TAGS" : [ "Post Affiliate Pro" ],
      "REQUIRES" : [ "rules", "rtpr_user" ],
      "USES VARIABLES" : { "account" : { "label" : "Account", "type" : "user" } },
      "RULES" : [
        { "RULE" : {
            "IF" : [
              { "user_has_role" : {
                  "account" : [ "account" ],
                  "roles" : { "value" : { "26" : "26" } },
                  "operation" : "OR"
                }
              }
            ],
            "DO" : [
              { "rtpr_user_action_remove_commissiongroup" : {
                  "account" : [ "account" ],
                  "commissiongroup" : { "value" : {
                      "11111111|bd4a45e0" : "11111111|bd4a45e0",
                      "11111111|e928564e" : "11111111|e928564e",
                      "33fadfd9|481fe3b5" : "33fadfd9|481fe3b5",
                      "33fadfd9|6a5b2cbb" : "33fadfd9|6a5b2cbb",
                      "39e673de|98f04a65" : "39e673de|98f04a65",
                      "39e673de|ae95c184" : "39e673de|ae95c184",
                      "c5d380ba|d22f9795" : "c5d380ba|d22f9795",
                      "c5d380ba|45609ce9" : "c5d380ba|45609ce9",
                      "c602cb08|d7df1364" : "c602cb08|d7df1364",
                      "c602cb08|7c51e99a" : "c602cb08|7c51e99a"
                    }
                  }
                }
              },
              { "user_remove_role" : {
                  "account" : [ "account" ],
                  "roles" : { "value" : { "27" : "27", "28" : "28" } }
                }
              },
              { "rtpr_user_action_add_commissiongroup" : {
                  "account" : [ "account" ],
                  "commissiongroup" : { "value" : {
                      "11111111|fd3dc104" : "11111111|fd3dc104",
                      "33fadfd9|10dbb040" : "33fadfd9|10dbb040",
                      "39e673de|8a65e5be" : "39e673de|8a65e5be",
                      "c5d380ba|93abaa31" : "c5d380ba|93abaa31",
                      "c602cb08|7720362e" : "c602cb08|7720362e"
                    }
                  },
                  "status" : "A"
                }
              }
            ],
            "LABEL" : "Set commission groups for Account Manager - AM Basic"
          }
        },
        { "RULE" : {
            "IF" : [
              { "user_has_role" : {
                  "account" : [ "account" ],
                  "roles" : { "value" : { "27" : "27" } },
                  "operation" : "OR"
                }
              }
            ],
            "DO" : [
              { "rtpr_user_action_remove_commissiongroup" : {
                  "account" : [ "account" ],
                  "commissiongroup" : { "value" : {
                      "11111111|fd3dc104" : "11111111|fd3dc104",
                      "11111111|e928564e" : "11111111|e928564e",
                      "33fadfd9|10dbb040" : "33fadfd9|10dbb040",
                      "39e673de|ae95c184" : "39e673de|ae95c184",
                      "39e673de|8a65e5be" : "39e673de|8a65e5be",
                      "c5d380ba|93abaa31" : "c5d380ba|93abaa31",
                      "c5d380ba|45609ce9" : "c5d380ba|45609ce9",
                      "c602cb08|7720362e" : "c602cb08|7720362e",
                      "c602cb08|7c51e99a" : "c602cb08|7c51e99a"
                    }
                  }
                }
              },
              { "user_remove_role" : { "account" : [ "account" ], "roles" : { "value" : { "23" : "23" } } } },
              { "rtpr_user_action_add_commissiongroup" : {
                  "account" : [ "account" ],
                  "commissiongroup" : { "value" : {
                      "11111111|bd4a45e0" : "11111111|bd4a45e0",
                      "33fadfd9|6a5b2cbb" : "33fadfd9|6a5b2cbb",
                      "39e673de|98f04a65" : "39e673de|98f04a65",
                      "c5d380ba|d22f9795" : "c5d380ba|d22f9795",
                      "c602cb08|d7df1364" : "c602cb08|d7df1364"
                    }
                  },
                  "status" : "A"
                }
              }
            ],
            "LABEL" : "Set commission groups for Account Manager - AM WC"
          }
        },
        { "RULE" : {
            "IF" : [
              { "user_has_role" : {
                  "account" : [ "account" ],
                  "roles" : { "value" : { "28" : "28" } },
                  "operation" : "OR"
                }
              }
            ],
            "DO" : [
              { "rtpr_user_action_add_commissiongroup" : {
                  "account" : [ "account" ],
                  "commissiongroup" : { "value" : { "33fadfd9|481fe3b5" : "33fadfd9|481fe3b5" } },
                  "status" : "A"
                }
              },
              { "user_add_role" : { "account" : [ "account" ], "roles" : { "value" : { "24" : "24" } } } }
            ],
            "LABEL" : "Set commission groups for Account Manager - AM FLT"
          }
        }
      ]
    }
  }');
  $items['rules_set_affiliate_role_after_using_referral'] = entity_import('rules_config', '{ "rules_set_affiliate_role_after_using_referral" : {
      "LABEL" : "Set Affiliate Role after using referral",
      "PLUGIN" : "reaction rule",
      "ACTIVE" : false,
      "OWNER" : "rules",
      "TAGS" : [ "Post Affiliate Pro" ],
      "REQUIRES" : [ "rules", "referral" ],
      "ON" : { "referral_recorded" : [] },
      "IF" : [
        { "user_has_role" : {
            "account" : [ "referring-user" ],
            "roles" : { "value" : { "8" : "8", "9" : "9", "4" : "4", "22" : "22", "23" : "23", "24" : "24" } },
            "operation" : "OR"
          }
        }
      ],
      "DO" : [
        { "data_set" : { "data" : [ "new-user:roles" ], "value" : { "value" : { "4" : "4" } } } },
        { "entity_save" : { "data" : [ "new-user" ], "immediate" : "1" } }
      ]
    }
  }');
  $items['rules_set_affiliate_role_commission_groups'] = entity_import('rules_config', '{ "rules_set_affiliate_role_commission_groups" : {
      "LABEL" : "Set Affiliate Role Commission Groups",
      "PLUGIN" : "rule set",
      "OWNER" : "rules",
      "TAGS" : [ "Post Affiliate Pro" ],
      "REQUIRES" : [ "rules", "rtpr_user", "rules_scheduler" ],
      "USES VARIABLES" : { "account" : { "label" : "Account", "type" : "user" } },
      "RULES" : [
        { "RULE" : {
            "IF" : [
              { "rtpr_user_condition_msp" : {
                  "account" : [ "account" ],
                  "campaigns" : { "value" : {
                      "c5d380ba" : "c5d380ba",
                      "c602cb08" : "c602cb08",
                      "39e673de" : "39e673de"
                    }
                  },
                  "tier" : "1",
                  "msp" : "249",
                  "op" : "\\u003C",
                  "daterange" : "none",
                  "datetime_start" : "-1 year",
                  "datetime_end" : "now"
                }
              }
            ],
            "DO" : [
              { "user_remove_role" : {
                  "account" : [ "account" ],
                  "roles" : { "value" : {
                      "5" : "5",
                      "7" : "7",
                      "8" : "8",
                      "9" : "9",
                      "10" : "10",
                      "11" : "11",
                      "3" : "3",
                      "4" : "4",
                      "22" : "22",
                      "20" : "20",
                      "25" : "25",
                      "23" : "23",
                      "24" : "24",
                      "26" : "26",
                      "27" : "27",
                      "28" : "28",
                      "29" : "29",
                      "30" : "30",
                      "31" : "31"
                    }
                  }
                }
              },
              { "user_add_role" : { "account" : [ "account" ], "roles" : { "value" : { "4" : "4" } } } },
              { "rtpr_user_action_remove_commissiongroup" : {
                  "account" : [ "account" ],
                  "commissiongroup" : { "value" : {
                      "11111111|bd4a45e0" : "11111111|bd4a45e0",
                      "11111111|fd3dc104" : "11111111|fd3dc104",
                      "11111111|e928564e" : "11111111|e928564e",
                      "33fadfd9|481fe3b5" : "33fadfd9|481fe3b5",
                      "33fadfd9|6a5b2cbb" : "33fadfd9|6a5b2cbb",
                      "33fadfd9|10dbb040" : "33fadfd9|10dbb040",
                      "39e673de|98f04a65" : "39e673de|98f04a65",
                      "39e673de|ae95c184" : "39e673de|ae95c184",
                      "39e673de|8a65e5be" : "39e673de|8a65e5be",
                      "c5d380ba|d22f9795" : "c5d380ba|d22f9795",
                      "c5d380ba|93abaa31" : "c5d380ba|93abaa31",
                      "c5d380ba|45609ce9" : "c5d380ba|45609ce9",
                      "c602cb08|d7df1364" : "c602cb08|d7df1364",
                      "c602cb08|7720362e" : "c602cb08|7720362e",
                      "c602cb08|7c51e99a" : "c602cb08|7c51e99a"
                    }
                  }
                }
              }
            ],
            "LABEL" : "If MSP (for last 12 months) \\u003C $249"
          }
        },
        { "RULE" : {
            "IF" : [
              { "rtpr_user_condition_msp" : {
                  "account" : [ "account" ],
                  "campaigns" : { "value" : {
                      "c5d380ba" : "c5d380ba",
                      "c602cb08" : "c602cb08",
                      "39e673de" : "39e673de"
                    }
                  },
                  "tier" : "1",
                  "msp" : "249",
                  "op" : "\\u003E=",
                  "daterange" : "none",
                  "datetime_start" : "-1 year",
                  "datetime_end" : "now"
                }
              },
              { "AND" : [
                  { "OR" : [
                      { "rtpr_user_condition_msp" : {
                          "account" : [ "account" ],
                          "campaigns" : { "value" : {
                              "c5d380ba" : "c5d380ba",
                              "c602cb08" : "c602cb08",
                              "39e673de" : "39e673de"
                            }
                          },
                          "tier" : "1",
                          "msp" : "49",
                          "op" : "\\u003C",
                          "daterange" : "LM",
                          "datetime_start" : "-2 month",
                          "datetime_end" : "-1 months"
                        }
                      },
                      { "rtpr_user_condition_msp" : {
                          "account" : [ "account" ],
                          "campaigns" : { "value" : {
                              "c5d380ba" : "c5d380ba",
                              "c602cb08" : "c602cb08",
                              "39e673de" : "39e673de"
                            }
                          },
                          "tier" : "1",
                          "msp" : "49",
                          "op" : "\\u003C",
                          "daterange" : "TM",
                          "datetime_start" : "-1 month",
                          "datetime_end" : "now"
                        }
                      }
                    ]
                  }
                ]
              }
            ],
            "DO" : [
              { "user_remove_role" : {
                  "account" : [ "account" ],
                  "roles" : { "value" : {
                      "5" : "5",
                      "7" : "7",
                      "8" : "8",
                      "9" : "9",
                      "10" : "10",
                      "11" : "11",
                      "3" : "3",
                      "4" : "4",
                      "22" : "22",
                      "20" : "20",
                      "25" : "25",
                      "23" : "23",
                      "24" : "24",
                      "26" : "26",
                      "27" : "27",
                      "28" : "28",
                      "29" : "29",
                      "30" : "30",
                      "31" : "31"
                    }
                  }
                }
              },
              { "rtpr_user_action_remove_commissiongroup" : {
                  "account" : [ "account" ],
                  "commissiongroup" : { "value" : {
                      "11111111|bd4a45e0" : "11111111|bd4a45e0",
                      "11111111|fd3dc104" : "11111111|fd3dc104",
                      "33fadfd9|481fe3b5" : "33fadfd9|481fe3b5",
                      "33fadfd9|6a5b2cbb" : "33fadfd9|6a5b2cbb",
                      "33fadfd9|10dbb040" : "33fadfd9|10dbb040",
                      "39e673de|98f04a65" : "39e673de|98f04a65",
                      "39e673de|8a65e5be" : "39e673de|8a65e5be",
                      "c5d380ba|d22f9795" : "c5d380ba|d22f9795",
                      "c5d380ba|93abaa31" : "c5d380ba|93abaa31",
                      "c602cb08|d7df1364" : "c602cb08|d7df1364",
                      "c602cb08|7720362e" : "c602cb08|7720362e"
                    }
                  }
                }
              },
              { "user_add_role" : { "account" : [ "account" ], "roles" : { "value" : { "8" : "8" } } } },
              { "rtpr_user_action_add_commissiongroup" : {
                  "account" : [ "account" ],
                  "commissiongroup" : { "value" : {
                      "11111111|e928564e" : "11111111|e928564e",
                      "39e673de|ae95c184" : "39e673de|ae95c184",
                      "c5d380ba|45609ce9" : "c5d380ba|45609ce9",
                      "c602cb08|7c51e99a" : "c602cb08|7c51e99a"
                    }
                  },
                  "status" : "A"
                }
              }
            ],
            "LABEL" : "If MSP (last 12 months) \\u003E $249 and MSP (last month or previous month) \\u003C $49"
          }
        },
        { "RULE" : {
            "IF" : [
              { "rtpr_user_condition_msp" : {
                  "account" : [ "account" ],
                  "campaigns" : { "value" : {
                      "c5d380ba" : "c5d380ba",
                      "c602cb08" : "c602cb08",
                      "39e673de" : "39e673de"
                    }
                  },
                  "tier" : "1",
                  "msp" : "249",
                  "op" : "\\u003E=",
                  "daterange" : "none",
                  "datetime_start" : "-1 year",
                  "datetime_end" : "now"
                }
              },
              { "AND" : [
                  { "OR" : [
                      { "rtpr_user_condition_msp" : {
                          "account" : [ "account" ],
                          "campaigns" : { "value" : {
                              "c5d380ba" : "c5d380ba",
                              "c602cb08" : "c602cb08",
                              "39e673de" : "39e673de"
                            }
                          },
                          "tier" : "1",
                          "msp" : "49",
                          "op" : "\\u003E=",
                          "daterange" : "LM",
                          "datetime_start" : "-2 months",
                          "datetime_end" : "-1 month"
                        }
                      },
                      { "rtpr_user_condition_msp" : {
                          "account" : [ "account" ],
                          "campaigns" : { "value" : {
                              "c5d380ba" : "c5d380ba",
                              "c602cb08" : "c602cb08",
                              "39e673de" : "39e673de"
                            }
                          },
                          "tier" : "1",
                          "msp" : "49",
                          "op" : "\\u003E=",
                          "daterange" : "TM",
                          "datetime_start" : "-1 month",
                          "datetime_end" : "now"
                        }
                      }
                    ]
                  }
                ]
              },
              { "AND" : [
                  { "OR" : [
                      { "rtpr_user_condition_msp" : {
                          "account" : [ "account" ],
                          "campaigns" : { "value" : {
                              "c5d380ba" : "c5d380ba",
                              "c602cb08" : "c602cb08",
                              "39e673de" : "39e673de"
                            }
                          },
                          "tier" : "1",
                          "msp" : "449",
                          "op" : "\\u003C",
                          "daterange" : "LM",
                          "datetime_start" : "-2 months",
                          "datetime_end" : "-1 month"
                        }
                      },
                      { "rtpr_user_condition_msp" : {
                          "account" : [ "account" ],
                          "campaigns" : { "value" : {
                              "c5d380ba" : "c5d380ba",
                              "c602cb08" : "c602cb08",
                              "39e673de" : "39e673de"
                            }
                          },
                          "tier" : "1",
                          "msp" : "449",
                          "op" : "\\u003C",
                          "daterange" : "TM",
                          "datetime_start" : "-1 month",
                          "datetime_end" : "now"
                        }
                      }
                    ]
                  }
                ]
              }
            ],
            "DO" : [
              { "user_remove_role" : {
                  "account" : [ "account" ],
                  "roles" : { "value" : {
                      "5" : "5",
                      "7" : "7",
                      "8" : "8",
                      "9" : "9",
                      "10" : "10",
                      "11" : "11",
                      "3" : "3",
                      "4" : "4",
                      "22" : "22",
                      "20" : "20",
                      "25" : "25",
                      "23" : "23",
                      "24" : "24",
                      "26" : "26",
                      "27" : "27",
                      "28" : "28",
                      "29" : "29",
                      "30" : "30",
                      "31" : "31"
                    }
                  }
                }
              },
              { "rtpr_user_action_remove_commissiongroup" : {
                  "account" : [ "account" ],
                  "commissiongroup" : { "value" : {
                      "11111111|bd4a45e0" : "11111111|bd4a45e0",
                      "11111111|e928564e" : "11111111|e928564e",
                      "33fadfd9|481fe3b5" : "33fadfd9|481fe3b5",
                      "33fadfd9|6a5b2cbb" : "33fadfd9|6a5b2cbb",
                      "39e673de|98f04a65" : "39e673de|98f04a65",
                      "39e673de|ae95c184" : "39e673de|ae95c184",
                      "c5d380ba|d22f9795" : "c5d380ba|d22f9795",
                      "c5d380ba|45609ce9" : "c5d380ba|45609ce9",
                      "c602cb08|d7df1364" : "c602cb08|d7df1364",
                      "c602cb08|7c51e99a" : "c602cb08|7c51e99a"
                    }
                  }
                }
              },
              { "user_add_role" : { "account" : [ "account" ], "roles" : { "value" : { "9" : "9" } } } },
              { "rtpr_user_action_add_commissiongroup" : {
                  "account" : [ "account" ],
                  "commissiongroup" : { "value" : {
                      "11111111|fd3dc104" : "11111111|fd3dc104",
                      "39e673de|8a65e5be" : "39e673de|8a65e5be",
                      "c5d380ba|93abaa31" : "c5d380ba|93abaa31",
                      "c602cb08|7720362e" : "c602cb08|7720362e"
                    }
                  },
                  "status" : "A"
                }
              }
            ],
            "LABEL" : "If MSP (last 12 months) \\u003E $249 and $49 \\u003C MSP (last month or previous month) \\u003C $449"
          }
        },
        { "RULE" : {
            "IF" : [
              { "rtpr_user_condition_msp" : {
                  "account" : [ "account" ],
                  "campaigns" : { "value" : {
                      "c5d380ba" : "c5d380ba",
                      "c602cb08" : "c602cb08",
                      "39e673de" : "39e673de"
                    }
                  },
                  "tier" : "1",
                  "msp" : "499",
                  "op" : "\\u003E=",
                  "daterange" : "none",
                  "datetime_start" : "-1 year",
                  "datetime_end" : "now"
                }
              },
              { "AND" : [
                  { "OR" : [
                      { "rtpr_user_condition_msp" : {
                          "account" : [ "account" ],
                          "campaigns" : { "value" : {
                              "c5d380ba" : "c5d380ba",
                              "c602cb08" : "c602cb08",
                              "39e673de" : "39e673de"
                            }
                          },
                          "tier" : "1",
                          "msp" : "499",
                          "op" : "\\u003E=",
                          "daterange" : "LM",
                          "datetime_start" : "-2 months",
                          "datetime_end" : "-1 month"
                        }
                      },
                      { "rtpr_user_condition_msp" : {
                          "account" : [ "account" ],
                          "campaigns" : { "value" : {
                              "c5d380ba" : "c5d380ba",
                              "c602cb08" : "c602cb08",
                              "39e673de" : "39e673de"
                            }
                          },
                          "tier" : "1",
                          "msp" : "499",
                          "op" : "\\u003E=",
                          "daterange" : "TM",
                          "datetime_start" : "-1 month",
                          "datetime_end" : "now"
                        }
                      }
                    ]
                  }
                ]
              }
            ],
            "DO" : [
              { "user_remove_role" : {
                  "account" : [ "account" ],
                  "roles" : { "value" : {
                      "5" : "5",
                      "7" : "7",
                      "8" : "8",
                      "9" : "9",
                      "10" : "10",
                      "11" : "11",
                      "3" : "3",
                      "4" : "4",
                      "22" : "22",
                      "20" : "20",
                      "25" : "25",
                      "23" : "23",
                      "24" : "24",
                      "26" : "26",
                      "27" : "27",
                      "28" : "28",
                      "29" : "29",
                      "30" : "30",
                      "31" : "31"
                    }
                  }
                }
              },
              { "rtpr_user_action_remove_commissiongroup" : {
                  "account" : [ "account" ],
                  "commissiongroup" : { "value" : {
                      "11111111|fd3dc104" : "11111111|fd3dc104",
                      "11111111|e928564e" : "11111111|e928564e",
                      "33fadfd9|10dbb040" : "33fadfd9|10dbb040",
                      "39e673de|ae95c184" : "39e673de|ae95c184",
                      "39e673de|8a65e5be" : "39e673de|8a65e5be",
                      "c5d380ba|93abaa31" : "c5d380ba|93abaa31",
                      "c5d380ba|45609ce9" : "c5d380ba|45609ce9",
                      "c602cb08|7720362e" : "c602cb08|7720362e",
                      "c602cb08|7c51e99a" : "c602cb08|7c51e99a"
                    }
                  }
                }
              },
              { "user_add_role" : {
                  "account" : [ "account" ],
                  "roles" : { "value" : { "22" : "22", "23" : "23" } }
                }
              },
              { "rtpr_user_action_add_commissiongroup" : {
                  "account" : [ "account" ],
                  "commissiongroup" : { "value" : {
                      "11111111|bd4a45e0" : "11111111|bd4a45e0",
                      "33fadfd9|6a5b2cbb" : "33fadfd9|6a5b2cbb",
                      "39e673de|98f04a65" : "39e673de|98f04a65",
                      "c5d380ba|d22f9795" : "c5d380ba|d22f9795",
                      "c602cb08|d7df1364" : "c602cb08|d7df1364"
                    }
                  },
                  "status" : "A"
                }
              }
            ],
            "LABEL" : "If MSP (last 12 months) \\u003E $449 and MSP (last month or previous month) \\u003E $449"
          }
        },
        { "RULE" : {
            "IF" : [
              { "rtpr_user_condition_msp" : {
                  "account" : [ "account" ],
                  "campaigns" : { "value" : {
                      "c5d380ba" : "c5d380ba",
                      "c602cb08" : "c602cb08",
                      "39e673de" : "39e673de"
                    }
                  },
                  "tier" : "1",
                  "msp" : "449",
                  "op" : "\\u003E=",
                  "daterange" : "TM",
                  "datetime_start" : 1435151686,
                  "datetime_end" : 1435151686
                }
              },
              { "AND" : [
                  { "rtpr_user_condition_msp" : {
                      "account" : [ "account" ],
                      "campaigns" : { "value" : {
                          "c5d380ba" : "c5d380ba",
                          "c602cb08" : "c602cb08",
                          "39e673de" : "39e673de"
                        }
                      },
                      "tier" : "1",
                      "msp" : "449",
                      "op" : "\\u003E=",
                      "daterange" : "LM",
                      "datetime_start" : 1435151761,
                      "datetime_end" : 1435151761
                    }
                  }
                ]
              },
              { "AND" : [
                  { "rtpr_user_condition_msp" : {
                      "account" : [ "account" ],
                      "campaigns" : { "value" : {
                          "c5d380ba" : "c5d380ba",
                          "c602cb08" : "c602cb08",
                          "39e673de" : "39e673de"
                        }
                      },
                      "tier" : "1",
                      "msp" : "449",
                      "op" : "\\u003E=",
                      "daterange" : "none",
                      "datetime_start" : "first day of -2 month",
                      "datetime_end" : "last day of -2 month"
                    }
                  }
                ]
              }
            ],
            "DO" : [
              { "user_add_role" : { "account" : [ "account" ], "roles" : { "value" : { "24" : "24" } } } }
            ],
            "LABEL" : "If MSP \\u003E $449 (for last 3 months)"
          }
        },
        { "RULE" : {
            "DO" : [
              { "component_rules_remove_scheduled_roles_upgrade_downgrade_tasks_for_a_user" : { "account" : [ "account" ] } },
              { "schedule" : {
                  "component" : "rules_set_affiliate_role_commission_groups",
                  "date" : "+1 day",
                  "identifier" : "Scheduled rules evaluation for Affiliate user [account:uid]",
                  "param_account" : [ "account" ]
                }
              }
            ],
            "LABEL" : "Reschedule rules evaluation for Affiliate Role"
          }
        }
      ]
    }
  }');
  $items['rules_set_commission_groups_for_account_manager'] = entity_import('rules_config', '{ "rules_set_commission_groups_for_account_manager" : {
      "LABEL" : "Set commission groups for Account Manager",
      "PLUGIN" : "reaction rule",
      "OWNER" : "rules",
      "TAGS" : [ "Post Affiliate Pro" ],
      "REQUIRES" : [ "rules" ],
      "ON" : { "user_update" : [] },
      "IF" : [
        { "user_has_role" : { "account" : [ "account" ], "roles" : { "value" : { "20" : "20" } } } }
      ],
      "DO" : [
        { "component_rules_set_account_manager_commission_groups" : { "account" : [ "account" ] } }
      ]
    }
  }');
  $items['rules_set_commission_groups_for_affiliate_role'] = entity_import('rules_config', '{ "rules_set_commission_groups_for_affiliate_role" : {
      "LABEL" : "Set commission groups for Affiliate Role",
      "PLUGIN" : "reaction rule",
      "OWNER" : "rules",
      "TAGS" : [ "Post Affiliate Pro" ],
      "REQUIRES" : [ "rules" ],
      "ON" : { "user_login" : [] },
      "IF" : [
        { "user_has_role" : {
            "account" : [ "account" ],
            "roles" : { "value" : { "4" : "4" } },
            "operation" : "OR"
          }
        },
        { "NOT user_has_role" : { "account" : [ "account" ], "roles" : { "value" : { "30" : "30" } } } }
      ],
      "DO" : [
        { "component_rules_set_affiliate_role_commission_groups" : { "account" : [ "account" ] } }
      ]
    }
  }');
  $items['rules_set_commission_groups_for_gold_role'] = entity_import('rules_config', '{ "rules_set_commission_groups_for_gold_role" : {
      "LABEL" : "Set commission groups for Gold Role",
      "PLUGIN" : "reaction rule",
      "OWNER" : "rules",
      "TAGS" : [ "Post Affiliate Pro" ],
      "REQUIRES" : [ "rules" ],
      "ON" : { "user_login" : [] },
      "IF" : [
        { "user_has_role" : {
            "account" : [ "account" ],
            "roles" : { "value" : { "9" : "9" } },
            "operation" : "OR"
          }
        },
        { "NOT user_has_role" : { "account" : [ "account" ], "roles" : { "value" : { "30" : "30" } } } }
      ],
      "DO" : [
        { "component_rules_set_gold_role_commission_groups" : { "account" : [ "account" ] } }
      ]
    }
  }');
  $items['rules_set_commission_groups_for_platinum_role'] = entity_import('rules_config', '{ "rules_set_commission_groups_for_platinum_role" : {
      "LABEL" : "Set commission groups for Platinum Role",
      "PLUGIN" : "reaction rule",
      "OWNER" : "rules",
      "TAGS" : [ "Post Affiliate Pro" ],
      "REQUIRES" : [ "rules" ],
      "ON" : { "user_login" : [] },
      "IF" : [
        { "user_has_role" : {
            "account" : [ "account" ],
            "roles" : { "value" : { "22" : "22" } },
            "operation" : "OR"
          }
        },
        { "NOT user_has_role" : { "account" : [ "account" ], "roles" : { "value" : { "30" : "30" } } } }
      ],
      "DO" : [
        { "component_rules_set_platinum_role_commission_groups" : { "account" : [ "account" ] } }
      ]
    }
  }');
  $items['rules_set_commission_groups_for_vendor_role'] = entity_import('rules_config', '{ "rules_set_commission_groups_for_vendor_role" : {
      "LABEL" : "Set commission groups for Vendor Role",
      "PLUGIN" : "reaction rule",
      "OWNER" : "rules",
      "TAGS" : [ "Post Affiliate Pro" ],
      "REQUIRES" : [ "rules" ],
      "ON" : { "user_login" : [] },
      "IF" : [
        { "user_has_role" : {
            "account" : [ "account" ],
            "roles" : { "value" : { "8" : "8" } },
            "operation" : "OR"
          }
        },
        { "NOT user_has_role" : { "account" : [ "account" ], "roles" : { "value" : { "30" : "30" } } } }
      ],
      "DO" : [
        { "component_rules_set_vendor_role_commission_groups" : { "account" : [ "account" ] } }
      ]
    }
  }');
  $items['rules_set_gold_role_commission_groups'] = entity_import('rules_config', '{ "rules_set_gold_role_commission_groups" : {
      "LABEL" : "Set Gold Role Commission Groups",
      "PLUGIN" : "rule set",
      "OWNER" : "rules",
      "TAGS" : [ "Post Affiliate Pro" ],
      "REQUIRES" : [ "rules", "rtpr_user", "rules_scheduler" ],
      "USES VARIABLES" : { "account" : { "label" : "Account", "type" : "user" } },
      "RULES" : [
        { "RULE" : {
            "IF" : [
              { "rtpr_user_condition_msp" : {
                  "account" : [ "account" ],
                  "campaigns" : { "value" : {
                      "c5d380ba" : "c5d380ba",
                      "c602cb08" : "c602cb08",
                      "39e673de" : "39e673de"
                    }
                  },
                  "tier" : "1",
                  "msp" : "249",
                  "op" : "\\u003C",
                  "daterange" : "none",
                  "datetime_start" : "-1 year",
                  "datetime_end" : "now"
                }
              }
            ],
            "DO" : [
              { "user_remove_role" : {
                  "account" : [ "account" ],
                  "roles" : { "value" : {
                      "5" : "5",
                      "7" : "7",
                      "8" : "8",
                      "9" : "9",
                      "10" : "10",
                      "11" : "11",
                      "3" : "3",
                      "4" : "4",
                      "22" : "22",
                      "20" : "20",
                      "25" : "25",
                      "23" : "23",
                      "24" : "24",
                      "26" : "26",
                      "27" : "27",
                      "28" : "28",
                      "29" : "29",
                      "30" : "30",
                      "31" : "31"
                    }
                  }
                }
              },
              { "rtpr_user_action_remove_commissiongroup" : {
                  "account" : [ "account" ],
                  "commissiongroup" : { "value" : {
                      "11111111|bd4a45e0" : "11111111|bd4a45e0",
                      "11111111|fd3dc104" : "11111111|fd3dc104",
                      "11111111|e928564e" : "11111111|e928564e",
                      "33fadfd9|481fe3b5" : "33fadfd9|481fe3b5",
                      "33fadfd9|6a5b2cbb" : "33fadfd9|6a5b2cbb",
                      "33fadfd9|10dbb040" : "33fadfd9|10dbb040",
                      "39e673de|98f04a65" : "39e673de|98f04a65",
                      "39e673de|ae95c184" : "39e673de|ae95c184",
                      "39e673de|8a65e5be" : "39e673de|8a65e5be",
                      "c5d380ba|d22f9795" : "c5d380ba|d22f9795",
                      "c5d380ba|93abaa31" : "c5d380ba|93abaa31",
                      "c5d380ba|45609ce9" : "c5d380ba|45609ce9",
                      "c602cb08|d7df1364" : "c602cb08|d7df1364",
                      "c602cb08|7720362e" : "c602cb08|7720362e",
                      "c602cb08|7c51e99a" : "c602cb08|7c51e99a"
                    }
                  }
                }
              },
              { "user_add_role" : { "account" : [ "account" ], "roles" : { "value" : { "4" : "4" } } } }
            ],
            "LABEL" : "If MSP (for last 12 months) \\u003C $249"
          }
        },
        { "RULE" : {
            "IF" : [
              { "rtpr_user_condition_msp" : {
                  "account" : [ "account" ],
                  "campaigns" : { "value" : {
                      "c5d380ba" : "c5d380ba",
                      "c602cb08" : "c602cb08",
                      "39e673de" : "39e673de"
                    }
                  },
                  "tier" : "1",
                  "msp" : "249",
                  "op" : "\\u003E=",
                  "daterange" : "none",
                  "datetime_start" : "-1 year",
                  "datetime_end" : "now"
                }
              },
              { "AND" : [
                  { "OR" : [
                      { "rtpr_user_condition_msp" : {
                          "account" : [ "account" ],
                          "campaigns" : { "value" : {
                              "c5d380ba" : "c5d380ba",
                              "c602cb08" : "c602cb08",
                              "39e673de" : "39e673de"
                            }
                          },
                          "tier" : "1",
                          "msp" : "49",
                          "op" : "\\u003C",
                          "daterange" : "LM",
                          "datetime_start" : 1430188817,
                          "datetime_end" : 1430188817
                        }
                      },
                      { "rtpr_user_condition_msp" : {
                          "account" : [ "account" ],
                          "campaigns" : { "value" : {
                              "c5d380ba" : "c5d380ba",
                              "c602cb08" : "c602cb08",
                              "39e673de" : "39e673de"
                            }
                          },
                          "tier" : "1",
                          "msp" : "49",
                          "op" : "\\u003C",
                          "daterange" : "TM",
                          "datetime_start" : 1430188779,
                          "datetime_end" : 1430188779
                        }
                      }
                    ]
                  }
                ]
              }
            ],
            "DO" : [
              { "user_remove_role" : {
                  "account" : [ "account" ],
                  "roles" : { "value" : {
                      "5" : "5",
                      "7" : "7",
                      "8" : "8",
                      "9" : "9",
                      "10" : "10",
                      "11" : "11",
                      "3" : "3",
                      "4" : "4",
                      "22" : "22",
                      "20" : "20",
                      "25" : "25",
                      "23" : "23",
                      "24" : "24",
                      "26" : "26",
                      "27" : "27",
                      "28" : "28",
                      "29" : "29",
                      "30" : "30",
                      "31" : "31"
                    }
                  }
                }
              },
              { "rtpr_user_action_remove_commissiongroup" : {
                  "account" : [ "account" ],
                  "commissiongroup" : { "value" : {
                      "11111111|bd4a45e0" : "11111111|bd4a45e0",
                      "11111111|fd3dc104" : "11111111|fd3dc104",
                      "33fadfd9|481fe3b5" : "33fadfd9|481fe3b5",
                      "33fadfd9|6a5b2cbb" : "33fadfd9|6a5b2cbb",
                      "33fadfd9|10dbb040" : "33fadfd9|10dbb040",
                      "39e673de|98f04a65" : "39e673de|98f04a65",
                      "39e673de|8a65e5be" : "39e673de|8a65e5be",
                      "c5d380ba|d22f9795" : "c5d380ba|d22f9795",
                      "c5d380ba|93abaa31" : "c5d380ba|93abaa31",
                      "c602cb08|d7df1364" : "c602cb08|d7df1364",
                      "c602cb08|7720362e" : "c602cb08|7720362e"
                    }
                  }
                }
              },
              { "user_add_role" : { "account" : [ "account" ], "roles" : { "value" : { "8" : "8" } } } },
              { "rtpr_user_action_add_commissiongroup" : {
                  "account" : [ "account" ],
                  "commissiongroup" : { "value" : {
                      "11111111|e928564e" : "11111111|e928564e",
                      "39e673de|ae95c184" : "39e673de|ae95c184",
                      "c5d380ba|45609ce9" : "c5d380ba|45609ce9",
                      "c602cb08|7c51e99a" : "c602cb08|7c51e99a"
                    }
                  },
                  "status" : "A"
                }
              }
            ],
            "LABEL" : "If MSP (last 12 months) \\u003E $249 and MSP (last month or previous month) \\u003C $49"
          }
        },
        { "RULE" : {
            "IF" : [
              { "rtpr_user_condition_msp" : {
                  "account" : [ "account" ],
                  "campaigns" : { "value" : {
                      "c5d380ba" : "c5d380ba",
                      "c602cb08" : "c602cb08",
                      "39e673de" : "39e673de"
                    }
                  },
                  "tier" : "1",
                  "msp" : "249",
                  "op" : "\\u003E=",
                  "daterange" : "none",
                  "datetime_start" : "-1 year",
                  "datetime_end" : "now"
                }
              },
              { "AND" : [
                  { "OR" : [
                      { "rtpr_user_condition_msp" : {
                          "account" : [ "account" ],
                          "campaigns" : { "value" : {
                              "c5d380ba" : "c5d380ba",
                              "c602cb08" : "c602cb08",
                              "39e673de" : "39e673de"
                            }
                          },
                          "tier" : "1",
                          "msp" : "49",
                          "op" : "\\u003E=",
                          "daterange" : "LM",
                          "datetime_start" : 1430189175,
                          "datetime_end" : 1430189175
                        }
                      },
                      { "rtpr_user_condition_msp" : {
                          "account" : [ "account" ],
                          "campaigns" : { "value" : {
                              "c5d380ba" : "c5d380ba",
                              "c602cb08" : "c602cb08",
                              "39e673de" : "39e673de"
                            }
                          },
                          "tier" : "1",
                          "msp" : "49",
                          "op" : "\\u003E=",
                          "daterange" : "TM",
                          "datetime_start" : 1430189145,
                          "datetime_end" : 1430189145
                        }
                      }
                    ]
                  }
                ]
              },
              { "AND" : [
                  { "OR" : [
                      { "rtpr_user_condition_msp" : {
                          "account" : [ "account" ],
                          "campaigns" : { "value" : {
                              "c5d380ba" : "c5d380ba",
                              "c602cb08" : "c602cb08",
                              "39e673de" : "39e673de"
                            }
                          },
                          "tier" : "1",
                          "msp" : "449",
                          "op" : "\\u003C",
                          "daterange" : "LM",
                          "datetime_start" : 1430256811,
                          "datetime_end" : 1430256811
                        }
                      },
                      { "rtpr_user_condition_msp" : {
                          "account" : [ "account" ],
                          "campaigns" : { "value" : {
                              "c5d380ba" : "c5d380ba",
                              "c602cb08" : "c602cb08",
                              "39e673de" : "39e673de"
                            }
                          },
                          "tier" : "1",
                          "msp" : "449",
                          "op" : "\\u003C",
                          "daterange" : "TM",
                          "datetime_start" : 1430256781,
                          "datetime_end" : 1430256781
                        }
                      }
                    ]
                  }
                ]
              }
            ],
            "DO" : [
              { "rtpr_user_action_remove_commissiongroup" : {
                  "account" : [ "account" ],
                  "commissiongroup" : { "value" : {
                      "11111111|bd4a45e0" : "11111111|bd4a45e0",
                      "11111111|e928564e" : "11111111|e928564e",
                      "33fadfd9|481fe3b5" : "33fadfd9|481fe3b5",
                      "33fadfd9|6a5b2cbb" : "33fadfd9|6a5b2cbb",
                      "39e673de|98f04a65" : "39e673de|98f04a65",
                      "39e673de|ae95c184" : "39e673de|ae95c184",
                      "c5d380ba|d22f9795" : "c5d380ba|d22f9795",
                      "c5d380ba|45609ce9" : "c5d380ba|45609ce9",
                      "c602cb08|d7df1364" : "c602cb08|d7df1364",
                      "c602cb08|7c51e99a" : "c602cb08|7c51e99a"
                    }
                  }
                }
              },
              { "user_remove_role" : {
                  "account" : [ "account" ],
                  "roles" : { "value" : {
                      "5" : "5",
                      "7" : "7",
                      "8" : "8",
                      "10" : "10",
                      "11" : "11",
                      "3" : "3",
                      "4" : "4",
                      "22" : "22",
                      "20" : "20",
                      "25" : "25",
                      "23" : "23",
                      "24" : "24",
                      "26" : "26",
                      "27" : "27",
                      "28" : "28",
                      "29" : "29",
                      "30" : "30",
                      "31" : "31"
                    }
                  }
                }
              },
              { "rtpr_user_action_add_commissiongroup" : {
                  "account" : [ "account" ],
                  "commissiongroup" : { "value" : {
                      "11111111|fd3dc104" : "11111111|fd3dc104",
                      "39e673de|8a65e5be" : "39e673de|8a65e5be",
                      "c5d380ba|93abaa31" : "c5d380ba|93abaa31",
                      "c602cb08|7720362e" : "c602cb08|7720362e"
                    }
                  },
                  "status" : "A"
                }
              },
              { "user_add_role" : { "account" : [ "account" ], "roles" : { "value" : { "9" : "9" } } } }
            ],
            "LABEL" : "If MSP (last 12 months) \\u003E $249 and $49 \\u003C MSP (last month or previous month) \\u003C $449"
          }
        },
        { "RULE" : {
            "IF" : [
              { "rtpr_user_condition_msp" : {
                  "account" : [ "account" ],
                  "campaigns" : { "value" : {
                      "c5d380ba" : "c5d380ba",
                      "c602cb08" : "c602cb08",
                      "39e673de" : "39e673de"
                    }
                  },
                  "tier" : "1",
                  "msp" : "449",
                  "op" : "\\u003E=",
                  "daterange" : "none",
                  "datetime_start" : "-1 year",
                  "datetime_end" : "now"
                }
              },
              { "AND" : [
                  { "OR" : [
                      { "rtpr_user_condition_msp" : {
                          "account" : [ "account" ],
                          "campaigns" : { "value" : {
                              "c5d380ba" : "c5d380ba",
                              "c602cb08" : "c602cb08",
                              "39e673de" : "39e673de"
                            }
                          },
                          "tier" : "1",
                          "msp" : "499",
                          "op" : "\\u003E=",
                          "daterange" : "LM",
                          "datetime_start" : 1430189341,
                          "datetime_end" : 1430189341
                        }
                      },
                      { "rtpr_user_condition_msp" : {
                          "account" : [ "account" ],
                          "campaigns" : { "value" : {
                              "c5d380ba" : "c5d380ba",
                              "c602cb08" : "c602cb08",
                              "39e673de" : "39e673de"
                            }
                          },
                          "tier" : "1",
                          "msp" : "499",
                          "op" : "\\u003E=",
                          "daterange" : "TM",
                          "datetime_start" : 1430189307,
                          "datetime_end" : 1430189307
                        }
                      }
                    ]
                  }
                ]
              }
            ],
            "DO" : [
              { "user_remove_role" : {
                  "account" : [ "account" ],
                  "roles" : { "value" : {
                      "5" : "5",
                      "7" : "7",
                      "8" : "8",
                      "9" : "9",
                      "10" : "10",
                      "11" : "11",
                      "3" : "3",
                      "4" : "4",
                      "22" : "22",
                      "20" : "20",
                      "25" : "25",
                      "23" : "23",
                      "24" : "24",
                      "26" : "26",
                      "27" : "27",
                      "28" : "28",
                      "29" : "29",
                      "30" : "30",
                      "31" : "31"
                    }
                  }
                }
              },
              { "rtpr_user_action_remove_commissiongroup" : {
                  "account" : [ "account" ],
                  "commissiongroup" : { "value" : {
                      "11111111|fd3dc104" : "11111111|fd3dc104",
                      "11111111|e928564e" : "11111111|e928564e",
                      "33fadfd9|10dbb040" : "33fadfd9|10dbb040",
                      "39e673de|ae95c184" : "39e673de|ae95c184",
                      "39e673de|8a65e5be" : "39e673de|8a65e5be",
                      "c5d380ba|93abaa31" : "c5d380ba|93abaa31",
                      "c5d380ba|45609ce9" : "c5d380ba|45609ce9",
                      "c602cb08|7720362e" : "c602cb08|7720362e",
                      "c602cb08|7c51e99a" : "c602cb08|7c51e99a"
                    }
                  }
                }
              },
              { "user_add_role" : {
                  "account" : [ "account" ],
                  "roles" : { "value" : { "22" : "22", "23" : "23" } }
                }
              },
              { "rtpr_user_action_add_commissiongroup" : {
                  "account" : [ "account" ],
                  "commissiongroup" : { "value" : {
                      "11111111|bd4a45e0" : "11111111|bd4a45e0",
                      "33fadfd9|6a5b2cbb" : "33fadfd9|6a5b2cbb",
                      "39e673de|98f04a65" : "39e673de|98f04a65",
                      "c5d380ba|d22f9795" : "c5d380ba|d22f9795",
                      "c602cb08|d7df1364" : "c602cb08|d7df1364"
                    }
                  },
                  "status" : "A"
                }
              }
            ],
            "LABEL" : "If MSP (last 12 months) \\u003E $449 and MSP (last month or previous month) \\u003E $449"
          }
        },
        { "RULE" : {
            "IF" : [
              { "rtpr_user_condition_msp" : {
                  "account" : [ "account" ],
                  "campaigns" : { "value" : {
                      "c5d380ba" : "c5d380ba",
                      "c602cb08" : "c602cb08",
                      "39e673de" : "39e673de"
                    }
                  },
                  "tier" : "1",
                  "msp" : "449",
                  "op" : "\\u003E=",
                  "daterange" : "TM",
                  "datetime_start" : 1435152023,
                  "datetime_end" : 1435152023
                }
              },
              { "AND" : [
                  { "rtpr_user_condition_msp" : {
                      "account" : [ "account" ],
                      "campaigns" : { "value" : {
                          "c5d380ba" : "c5d380ba",
                          "c602cb08" : "c602cb08",
                          "39e673de" : "39e673de"
                        }
                      },
                      "tier" : "1",
                      "msp" : "449",
                      "op" : "\\u003E=",
                      "daterange" : "LM",
                      "datetime_start" : 1435152054,
                      "datetime_end" : 1435152054
                    }
                  }
                ]
              },
              { "AND" : [
                  { "rtpr_user_condition_msp" : {
                      "account" : [ "account" ],
                      "campaigns" : { "value" : {
                          "c5d380ba" : "c5d380ba",
                          "c602cb08" : "c602cb08",
                          "39e673de" : "39e673de"
                        }
                      },
                      "tier" : "1",
                      "msp" : "449",
                      "op" : "\\u003E=",
                      "daterange" : "none",
                      "datetime_start" : "first day of -2 month",
                      "datetime_end" : "last day of -2 month"
                    }
                  }
                ]
              }
            ],
            "DO" : [
              { "user_add_role" : { "account" : [ "account" ], "roles" : { "value" : { "24" : "24" } } } }
            ],
            "LABEL" : "If MSP \\u003E $449 (for last 3 months)"
          }
        },
        { "RULE" : {
            "DO" : [
              { "component_rules_remove_scheduled_roles_upgrade_downgrade_tasks_for_a_user" : { "account" : [ "account" ] } },
              { "schedule" : {
                  "component" : "rules_set_gold_role_commission_groups",
                  "date" : "+1 day",
                  "identifier" : "Scheduled rules evaluation for Gold user [account:uid]",
                  "param_account" : [ "account" ]
                }
              }
            ],
            "LABEL" : "Reschedule rules evaluation for Gold Role"
          }
        }
      ]
    }
  }');
  $items['rules_set_hcp_role_after_using_referral'] = entity_import('rules_config', '{ "rules_set_hcp_role_after_using_referral" : {
      "LABEL" : "Set HCP Role after using referral",
      "PLUGIN" : "reaction rule",
      "ACTIVE" : false,
      "OWNER" : "rules",
      "TAGS" : [ "Post Affiliate Pro" ],
      "REQUIRES" : [ "rules", "referral" ],
      "ON" : { "referral_recorded" : [] },
      "IF" : [
        { "user_has_role" : {
            "account" : [ "referring-user" ],
            "roles" : { "value" : { "10" : "10" } }
          }
        }
      ],
      "DO" : [
        { "data_set" : { "data" : [ "new-user:roles" ], "value" : { "value" : { "10" : "10" } } } },
        { "entity_save" : { "data" : [ "new-user" ], "immediate" : "1" } }
      ]
    }
  }');
  $items['rules_set_merchant_role_after_using_referral'] = entity_import('rules_config', '{ "rules_set_merchant_role_after_using_referral" : {
      "LABEL" : "Set Merchant Role after using referral",
      "PLUGIN" : "reaction rule",
      "ACTIVE" : false,
      "OWNER" : "rules",
      "TAGS" : [ "Post Affiliate Pro" ],
      "REQUIRES" : [ "rules", "referral" ],
      "ON" : { "referral_recorded" : [] },
      "IF" : [
        { "user_has_role" : {
            "account" : [ "referring-user" ],
            "roles" : { "value" : { "11" : "11" } }
          }
        }
      ],
      "DO" : [
        { "data_set" : { "data" : [ "new-user:roles" ], "value" : { "value" : { "8" : "8" } } } },
        { "entity_save" : { "data" : [ "new-user" ], "immediate" : "1" } }
      ]
    }
  }');
  $items['rules_set_platinum_role_commission_groups'] = entity_import('rules_config', '{ "rules_set_platinum_role_commission_groups" : {
      "LABEL" : "Set Platinum Role Commission Groups",
      "PLUGIN" : "rule set",
      "OWNER" : "rules",
      "TAGS" : [ "Post Affiliate Pro" ],
      "REQUIRES" : [ "rules", "rtpr_user", "rules_scheduler" ],
      "USES VARIABLES" : { "account" : { "label" : "Account", "type" : "user" } },
      "RULES" : [
        { "RULE" : {
            "IF" : [
              { "rtpr_user_condition_msp" : {
                  "account" : [ "account" ],
                  "campaigns" : { "value" : {
                      "c5d380ba" : "c5d380ba",
                      "c602cb08" : "c602cb08",
                      "39e673de" : "39e673de"
                    }
                  },
                  "tier" : "1",
                  "msp" : "249",
                  "op" : "\\u003C",
                  "daterange" : "none",
                  "datetime_start" : "-1 year",
                  "datetime_end" : "now"
                }
              }
            ],
            "DO" : [
              { "user_remove_role" : {
                  "account" : [ "account" ],
                  "roles" : { "value" : {
                      "5" : "5",
                      "7" : "7",
                      "8" : "8",
                      "9" : "9",
                      "10" : "10",
                      "11" : "11",
                      "3" : "3",
                      "4" : "4",
                      "22" : "22",
                      "20" : "20",
                      "25" : "25",
                      "23" : "23",
                      "24" : "24",
                      "26" : "26",
                      "27" : "27",
                      "28" : "28",
                      "29" : "29",
                      "30" : "30",
                      "31" : "31"
                    }
                  }
                }
              },
              { "rtpr_user_action_remove_commissiongroup" : {
                  "account" : [ "account" ],
                  "commissiongroup" : { "value" : {
                      "11111111|bd4a45e0" : "11111111|bd4a45e0",
                      "11111111|fd3dc104" : "11111111|fd3dc104",
                      "11111111|e928564e" : "11111111|e928564e",
                      "33fadfd9|481fe3b5" : "33fadfd9|481fe3b5",
                      "33fadfd9|6a5b2cbb" : "33fadfd9|6a5b2cbb",
                      "33fadfd9|10dbb040" : "33fadfd9|10dbb040",
                      "39e673de|98f04a65" : "39e673de|98f04a65",
                      "39e673de|ae95c184" : "39e673de|ae95c184",
                      "39e673de|8a65e5be" : "39e673de|8a65e5be",
                      "c5d380ba|d22f9795" : "c5d380ba|d22f9795",
                      "c5d380ba|93abaa31" : "c5d380ba|93abaa31",
                      "c5d380ba|45609ce9" : "c5d380ba|45609ce9",
                      "c602cb08|d7df1364" : "c602cb08|d7df1364",
                      "c602cb08|7720362e" : "c602cb08|7720362e",
                      "c602cb08|7c51e99a" : "c602cb08|7c51e99a"
                    }
                  }
                }
              },
              { "user_add_role" : { "account" : [ "account" ], "roles" : { "value" : { "4" : "4" } } } }
            ],
            "LABEL" : "If MSP (for last 12 months) \\u003C $249"
          }
        },
        { "RULE" : {
            "IF" : [
              { "rtpr_user_condition_msp" : {
                  "account" : [ "account" ],
                  "campaigns" : { "value" : {
                      "c5d380ba" : "c5d380ba",
                      "c602cb08" : "c602cb08",
                      "39e673de" : "39e673de"
                    }
                  },
                  "tier" : "1",
                  "msp" : "249",
                  "op" : "\\u003E=",
                  "daterange" : "none",
                  "datetime_start" : "-1 year",
                  "datetime_end" : "now"
                }
              },
              { "AND" : [
                  { "OR" : [
                      { "rtpr_user_condition_msp" : {
                          "account" : [ "account" ],
                          "campaigns" : { "value" : {
                              "c5d380ba" : "c5d380ba",
                              "c602cb08" : "c602cb08",
                              "39e673de" : "39e673de"
                            }
                          },
                          "tier" : "1",
                          "msp" : "49",
                          "op" : "\\u003C",
                          "daterange" : "LM",
                          "datetime_start" : 1430189927,
                          "datetime_end" : 1430189927
                        }
                      },
                      { "rtpr_user_condition_msp" : {
                          "account" : [ "account" ],
                          "campaigns" : { "value" : {
                              "c5d380ba" : "c5d380ba",
                              "c602cb08" : "c602cb08",
                              "39e673de" : "39e673de"
                            }
                          },
                          "tier" : "1",
                          "msp" : "49",
                          "op" : "\\u003C",
                          "daterange" : "TM",
                          "datetime_start" : 1430189894,
                          "datetime_end" : 1430189894
                        }
                      }
                    ]
                  }
                ]
              }
            ],
            "DO" : [
              { "user_remove_role" : {
                  "account" : [ "account" ],
                  "roles" : { "value" : {
                      "5" : "5",
                      "7" : "7",
                      "8" : "8",
                      "9" : "9",
                      "10" : "10",
                      "11" : "11",
                      "3" : "3",
                      "4" : "4",
                      "22" : "22",
                      "20" : "20",
                      "25" : "25",
                      "23" : "23",
                      "24" : "24",
                      "26" : "26",
                      "27" : "27",
                      "28" : "28",
                      "29" : "29",
                      "30" : "30",
                      "31" : "31"
                    }
                  }
                }
              },
              { "rtpr_user_action_remove_commissiongroup" : {
                  "account" : [ "account" ],
                  "commissiongroup" : { "value" : {
                      "11111111|bd4a45e0" : "11111111|bd4a45e0",
                      "11111111|fd3dc104" : "11111111|fd3dc104",
                      "33fadfd9|481fe3b5" : "33fadfd9|481fe3b5",
                      "33fadfd9|6a5b2cbb" : "33fadfd9|6a5b2cbb",
                      "33fadfd9|10dbb040" : "33fadfd9|10dbb040",
                      "39e673de|98f04a65" : "39e673de|98f04a65",
                      "39e673de|8a65e5be" : "39e673de|8a65e5be",
                      "c5d380ba|d22f9795" : "c5d380ba|d22f9795",
                      "c5d380ba|93abaa31" : "c5d380ba|93abaa31",
                      "c602cb08|d7df1364" : "c602cb08|d7df1364",
                      "c602cb08|7720362e" : "c602cb08|7720362e"
                    }
                  }
                }
              },
              { "user_add_role" : { "account" : [ "account" ], "roles" : { "value" : { "8" : "8" } } } },
              { "rtpr_user_action_add_commissiongroup" : {
                  "account" : [ "account" ],
                  "commissiongroup" : { "value" : {
                      "11111111|e928564e" : "11111111|e928564e",
                      "39e673de|ae95c184" : "39e673de|ae95c184",
                      "c5d380ba|45609ce9" : "c5d380ba|45609ce9",
                      "c602cb08|7c51e99a" : "c602cb08|7c51e99a"
                    }
                  },
                  "status" : "A"
                }
              }
            ],
            "LABEL" : "If MSP (last 12 months) \\u003E $249 and MSP (last month or previous month) \\u003C $49"
          }
        },
        { "RULE" : {
            "IF" : [
              { "rtpr_user_condition_msp" : {
                  "account" : [ "account" ],
                  "campaigns" : { "value" : {
                      "c5d380ba" : "c5d380ba",
                      "c602cb08" : "c602cb08",
                      "39e673de" : "39e673de"
                    }
                  },
                  "tier" : "1",
                  "msp" : "249",
                  "op" : "\\u003E=",
                  "daterange" : "none",
                  "datetime_start" : "-1 year",
                  "datetime_end" : "now"
                }
              },
              { "AND" : [
                  { "OR" : [
                      { "rtpr_user_condition_msp" : {
                          "account" : [ "account" ],
                          "campaigns" : { "value" : {
                              "c5d380ba" : "c5d380ba",
                              "c602cb08" : "c602cb08",
                              "39e673de" : "39e673de"
                            }
                          },
                          "tier" : "1",
                          "msp" : "49",
                          "op" : "\\u003E=",
                          "daterange" : "LM",
                          "datetime_start" : 1430256956,
                          "datetime_end" : 1430256956
                        }
                      },
                      { "rtpr_user_condition_msp" : {
                          "account" : [ "account" ],
                          "campaigns" : { "value" : { "c602cb08" : "c602cb08", "39e673de" : "39e673de" } },
                          "tier" : "1",
                          "msp" : "49",
                          "op" : "\\u003E=",
                          "daterange" : "TM",
                          "datetime_start" : 1430256906,
                          "datetime_end" : 1430256906
                        }
                      }
                    ]
                  }
                ]
              },
              { "AND" : [
                  { "OR" : [
                      { "rtpr_user_condition_msp" : {
                          "account" : [ "account" ],
                          "campaigns" : { "value" : {
                              "c5d380ba" : "c5d380ba",
                              "c602cb08" : "c602cb08",
                              "39e673de" : "39e673de"
                            }
                          },
                          "tier" : "1",
                          "msp" : "449",
                          "op" : "\\u003C",
                          "daterange" : "LM",
                          "datetime_start" : 1430190216,
                          "datetime_end" : 1430190216
                        }
                      },
                      { "rtpr_user_condition_msp" : {
                          "account" : [ "account" ],
                          "campaigns" : { "value" : {
                              "c5d380ba" : "c5d380ba",
                              "c602cb08" : "c602cb08",
                              "39e673de" : "39e673de"
                            }
                          },
                          "tier" : "1",
                          "msp" : "449",
                          "op" : "\\u003C",
                          "daterange" : "TM",
                          "datetime_start" : 1430190142,
                          "datetime_end" : 1430190142
                        }
                      }
                    ]
                  }
                ]
              }
            ],
            "DO" : [
              { "user_remove_role" : {
                  "account" : [ "account" ],
                  "roles" : { "value" : {
                      "5" : "5",
                      "7" : "7",
                      "8" : "8",
                      "9" : "9",
                      "10" : "10",
                      "11" : "11",
                      "3" : "3",
                      "4" : "4",
                      "22" : "22",
                      "20" : "20",
                      "25" : "25",
                      "23" : "23",
                      "24" : "24",
                      "26" : "26",
                      "27" : "27",
                      "28" : "28",
                      "29" : "29",
                      "30" : "30",
                      "31" : "31"
                    }
                  }
                }
              },
              { "rtpr_user_action_remove_commissiongroup" : {
                  "account" : [ "account" ],
                  "commissiongroup" : { "value" : {
                      "11111111|bd4a45e0" : "11111111|bd4a45e0",
                      "11111111|e928564e" : "11111111|e928564e",
                      "33fadfd9|481fe3b5" : "33fadfd9|481fe3b5",
                      "33fadfd9|6a5b2cbb" : "33fadfd9|6a5b2cbb",
                      "39e673de|98f04a65" : "39e673de|98f04a65",
                      "39e673de|ae95c184" : "39e673de|ae95c184",
                      "c5d380ba|d22f9795" : "c5d380ba|d22f9795",
                      "c5d380ba|45609ce9" : "c5d380ba|45609ce9",
                      "c602cb08|d7df1364" : "c602cb08|d7df1364",
                      "c602cb08|7c51e99a" : "c602cb08|7c51e99a"
                    }
                  }
                }
              },
              { "user_add_role" : { "account" : [ "account" ], "roles" : { "value" : { "9" : "9" } } } },
              { "rtpr_user_action_add_commissiongroup" : {
                  "account" : [ "account" ],
                  "commissiongroup" : { "value" : {
                      "11111111|fd3dc104" : "11111111|fd3dc104",
                      "39e673de|8a65e5be" : "39e673de|8a65e5be",
                      "c5d380ba|93abaa31" : "c5d380ba|93abaa31",
                      "c602cb08|7720362e" : "c602cb08|7720362e"
                    }
                  },
                  "status" : "A"
                }
              }
            ],
            "LABEL" : "If MSP (last 12 months) \\u003E $249 and $49 \\u003C MSP (last month or previous month) \\u003C $449"
          }
        },
        { "RULE" : {
            "IF" : [
              { "rtpr_user_condition_msp" : {
                  "account" : [ "account" ],
                  "campaigns" : { "value" : {
                      "c5d380ba" : "c5d380ba",
                      "c602cb08" : "c602cb08",
                      "39e673de" : "39e673de"
                    }
                  },
                  "tier" : "1",
                  "msp" : "449",
                  "op" : "\\u003E=",
                  "daterange" : "none",
                  "datetime_start" : "-1 year",
                  "datetime_end" : "now"
                }
              },
              { "AND" : [
                  { "OR" : [
                      { "rtpr_user_condition_msp" : {
                          "account" : [ "account" ],
                          "campaigns" : { "value" : {
                              "c5d380ba" : "c5d380ba",
                              "c602cb08" : "c602cb08",
                              "39e673de" : "39e673de"
                            }
                          },
                          "tier" : "1",
                          "msp" : "449",
                          "op" : "\\u003E=",
                          "daterange" : "LM",
                          "datetime_start" : 1430190542,
                          "datetime_end" : 1430190542
                        }
                      },
                      { "rtpr_user_condition_msp" : {
                          "account" : [ "account" ],
                          "campaigns" : { "value" : {
                              "c5d380ba" : "c5d380ba",
                              "c602cb08" : "c602cb08",
                              "39e673de" : "39e673de"
                            }
                          },
                          "tier" : "1",
                          "msp" : "449",
                          "op" : "\\u003E=",
                          "daterange" : "TM",
                          "datetime_start" : 1430190515,
                          "datetime_end" : 1430190515
                        }
                      }
                    ]
                  }
                ]
              }
            ],
            "DO" : [
              { "user_remove_role" : {
                  "account" : [ "account" ],
                  "roles" : { "value" : {
                      "5" : "5",
                      "7" : "7",
                      "8" : "8",
                      "9" : "9",
                      "10" : "10",
                      "11" : "11",
                      "3" : "3",
                      "4" : "4",
                      "22" : "22",
                      "20" : "20",
                      "25" : "25",
                      "23" : "23",
                      "24" : "24",
                      "26" : "26",
                      "27" : "27",
                      "28" : "28",
                      "29" : "29",
                      "30" : "30",
                      "31" : "31"
                    }
                  }
                }
              },
              { "rtpr_user_action_remove_commissiongroup" : {
                  "account" : [ "account" ],
                  "commissiongroup" : { "value" : {
                      "11111111|fd3dc104" : "11111111|fd3dc104",
                      "11111111|e928564e" : "11111111|e928564e",
                      "33fadfd9|10dbb040" : "33fadfd9|10dbb040",
                      "39e673de|ae95c184" : "39e673de|ae95c184",
                      "39e673de|8a65e5be" : "39e673de|8a65e5be",
                      "c5d380ba|93abaa31" : "c5d380ba|93abaa31",
                      "c5d380ba|45609ce9" : "c5d380ba|45609ce9",
                      "c602cb08|7720362e" : "c602cb08|7720362e",
                      "c602cb08|7c51e99a" : "c602cb08|7c51e99a"
                    }
                  }
                }
              },
              { "user_add_role" : {
                  "account" : [ "account" ],
                  "roles" : { "value" : { "22" : "22", "23" : "23" } }
                }
              },
              { "rtpr_user_action_add_commissiongroup" : {
                  "account" : [ "account" ],
                  "commissiongroup" : { "value" : {
                      "11111111|bd4a45e0" : "11111111|bd4a45e0",
                      "33fadfd9|6a5b2cbb" : "33fadfd9|6a5b2cbb",
                      "39e673de|98f04a65" : "39e673de|98f04a65",
                      "c5d380ba|d22f9795" : "c5d380ba|d22f9795",
                      "c602cb08|d7df1364" : "c602cb08|d7df1364"
                    }
                  },
                  "status" : "A"
                }
              }
            ],
            "LABEL" : "If MSP (last 12 months) \\u003E $449 and MSP (last month or previous month) \\u003E $449"
          }
        },
        { "RULE" : {
            "IF" : [
              { "rtpr_user_condition_msp" : {
                  "account" : [ "account" ],
                  "campaigns" : { "value" : {
                      "c5d380ba" : "c5d380ba",
                      "c602cb08" : "c602cb08",
                      "39e673de" : "39e673de"
                    }
                  },
                  "tier" : "1",
                  "msp" : "449",
                  "op" : "\\u003E=",
                  "daterange" : "TM",
                  "datetime_start" : 1435152161,
                  "datetime_end" : 1435152161
                }
              },
              { "AND" : [
                  { "rtpr_user_condition_msp" : {
                      "account" : [ "account" ],
                      "campaigns" : { "value" : {
                          "c5d380ba" : "c5d380ba",
                          "c602cb08" : "c602cb08",
                          "39e673de" : "39e673de"
                        }
                      },
                      "tier" : "1",
                      "msp" : "449",
                      "op" : "\\u003E=",
                      "daterange" : "LM",
                      "datetime_start" : 1435152191,
                      "datetime_end" : 1435152191
                    }
                  }
                ]
              },
              { "AND" : [
                  { "rtpr_user_condition_msp" : {
                      "account" : [ "account" ],
                      "campaigns" : { "value" : {
                          "c5d380ba" : "c5d380ba",
                          "c602cb08" : "c602cb08",
                          "39e673de" : "39e673de"
                        }
                      },
                      "tier" : "1",
                      "msp" : "449",
                      "op" : "\\u003E=",
                      "daterange" : "none",
                      "datetime_start" : "first day of -2 month",
                      "datetime_end" : "last day of -2 month"
                    }
                  }
                ]
              }
            ],
            "DO" : [
              { "user_add_role" : { "account" : [ "account" ], "roles" : { "value" : { "24" : "24" } } } }
            ],
            "LABEL" : "If MSP \\u003E $449 (for last 3 months)"
          }
        },
        { "RULE" : {
            "DO" : [
              { "component_rules_remove_scheduled_roles_upgrade_downgrade_tasks_for_a_user" : { "account" : [ "account" ] } },
              { "schedule" : {
                  "component" : "rules_set_platinum_role_commission_groups",
                  "date" : "+1 day",
                  "identifier" : "Scheduled rules evaluation for Platinum user [account:uid]",
                  "param_account" : [ "account" ]
                }
              }
            ],
            "LABEL" : "Reschedule rules evaluation for Platinum Role"
          }
        }
      ]
    }
  }');
  $items['rules_set_vendor_role_commission_groups'] = entity_import('rules_config', '{ "rules_set_vendor_role_commission_groups" : {
      "LABEL" : "Set Vendor Role Commission Groups",
      "PLUGIN" : "rule set",
      "OWNER" : "rules",
      "TAGS" : [ "Post Affiliate Pro" ],
      "REQUIRES" : [ "rules", "rtpr_user", "rules_scheduler" ],
      "USES VARIABLES" : { "account" : { "label" : "Account", "type" : "user" } },
      "RULES" : [
        { "RULE" : {
            "IF" : [
              { "rtpr_user_condition_msp" : {
                  "account" : [ "account" ],
                  "campaigns" : { "value" : {
                      "c5d380ba" : "c5d380ba",
                      "c602cb08" : "c602cb08",
                      "39e673de" : "39e673de"
                    }
                  },
                  "tier" : "1",
                  "msp" : "249",
                  "op" : "\\u003C",
                  "daterange" : "none",
                  "datetime_start" : "-1 year",
                  "datetime_end" : "now"
                }
              }
            ],
            "DO" : [
              { "user_remove_role" : {
                  "account" : [ "account" ],
                  "roles" : { "value" : {
                      "5" : "5",
                      "7" : "7",
                      "8" : "8",
                      "9" : "9",
                      "10" : "10",
                      "11" : "11",
                      "3" : "3",
                      "4" : "4",
                      "22" : "22",
                      "20" : "20",
                      "25" : "25",
                      "23" : "23",
                      "24" : "24",
                      "26" : "26",
                      "27" : "27",
                      "28" : "28",
                      "29" : "29",
                      "30" : "30",
                      "31" : "31"
                    }
                  }
                }
              },
              { "rtpr_user_action_remove_commissiongroup" : {
                  "account" : [ "account" ],
                  "commissiongroup" : { "value" : {
                      "11111111|bd4a45e0" : "11111111|bd4a45e0",
                      "11111111|fd3dc104" : "11111111|fd3dc104",
                      "11111111|e928564e" : "11111111|e928564e",
                      "33fadfd9|481fe3b5" : "33fadfd9|481fe3b5",
                      "33fadfd9|6a5b2cbb" : "33fadfd9|6a5b2cbb",
                      "33fadfd9|10dbb040" : "33fadfd9|10dbb040",
                      "39e673de|98f04a65" : "39e673de|98f04a65",
                      "39e673de|ae95c184" : "39e673de|ae95c184",
                      "39e673de|8a65e5be" : "39e673de|8a65e5be",
                      "c5d380ba|d22f9795" : "c5d380ba|d22f9795",
                      "c5d380ba|93abaa31" : "c5d380ba|93abaa31",
                      "c5d380ba|45609ce9" : "c5d380ba|45609ce9",
                      "c602cb08|d7df1364" : "c602cb08|d7df1364",
                      "c602cb08|7720362e" : "c602cb08|7720362e",
                      "c602cb08|7c51e99a" : "c602cb08|7c51e99a"
                    }
                  }
                }
              },
              { "user_add_role" : { "account" : [ "account" ], "roles" : { "value" : { "4" : "4" } } } }
            ],
            "LABEL" : "If MSP (for last 12 months) \\u003C $249"
          }
        },
        { "RULE" : {
            "IF" : [
              { "rtpr_user_condition_msp" : {
                  "account" : [ "account" ],
                  "campaigns" : { "value" : {
                      "c5d380ba" : "c5d380ba",
                      "c602cb08" : "c602cb08",
                      "39e673de" : "39e673de"
                    }
                  },
                  "tier" : "1",
                  "msp" : "249",
                  "op" : "\\u003E=",
                  "daterange" : "none",
                  "datetime_start" : "-1 year",
                  "datetime_end" : "now"
                }
              },
              { "AND" : [
                  { "OR" : [
                      { "rtpr_user_condition_msp" : {
                          "account" : [ "account" ],
                          "campaigns" : { "value" : {
                              "c5d380ba" : "c5d380ba",
                              "c602cb08" : "c602cb08",
                              "39e673de" : "39e673de"
                            }
                          },
                          "tier" : "1",
                          "msp" : "49",
                          "op" : "\\u003C",
                          "daterange" : "LM",
                          "datetime_start" : 1430187535,
                          "datetime_end" : 1430187535
                        }
                      },
                      { "rtpr_user_condition_msp" : {
                          "account" : [ "account" ],
                          "campaigns" : { "value" : {
                              "c5d380ba" : "c5d380ba",
                              "c602cb08" : "c602cb08",
                              "39e673de" : "39e673de"
                            }
                          },
                          "tier" : "1",
                          "msp" : "49",
                          "op" : "\\u003C",
                          "daterange" : "TM",
                          "datetime_start" : 1430187489,
                          "datetime_end" : 1430187489
                        }
                      }
                    ]
                  }
                ]
              }
            ],
            "DO" : [
              { "rtpr_user_action_remove_commissiongroup" : {
                  "account" : [ "account" ],
                  "commissiongroup" : { "value" : {
                      "11111111|bd4a45e0" : "11111111|bd4a45e0",
                      "11111111|fd3dc104" : "11111111|fd3dc104",
                      "33fadfd9|481fe3b5" : "33fadfd9|481fe3b5",
                      "33fadfd9|6a5b2cbb" : "33fadfd9|6a5b2cbb",
                      "33fadfd9|10dbb040" : "33fadfd9|10dbb040",
                      "39e673de|98f04a65" : "39e673de|98f04a65",
                      "39e673de|8a65e5be" : "39e673de|8a65e5be",
                      "c5d380ba|d22f9795" : "c5d380ba|d22f9795",
                      "c5d380ba|93abaa31" : "c5d380ba|93abaa31",
                      "c602cb08|d7df1364" : "c602cb08|d7df1364",
                      "c602cb08|7720362e" : "c602cb08|7720362e"
                    }
                  }
                }
              },
              { "user_remove_role" : {
                  "account" : [ "account" ],
                  "roles" : { "value" : {
                      "5" : "5",
                      "7" : "7",
                      "9" : "9",
                      "10" : "10",
                      "11" : "11",
                      "3" : "3",
                      "4" : "4",
                      "22" : "22",
                      "20" : "20",
                      "25" : "25",
                      "23" : "23",
                      "24" : "24",
                      "26" : "26",
                      "27" : "27",
                      "28" : "28",
                      "29" : "29",
                      "30" : "30",
                      "31" : "31"
                    }
                  }
                }
              },
              { "rtpr_user_action_add_commissiongroup" : {
                  "account" : [ "account" ],
                  "commissiongroup" : { "value" : {
                      "11111111|e928564e" : "11111111|e928564e",
                      "39e673de|ae95c184" : "39e673de|ae95c184",
                      "c5d380ba|45609ce9" : "c5d380ba|45609ce9",
                      "c602cb08|7c51e99a" : "c602cb08|7c51e99a"
                    }
                  },
                  "status" : "A"
                }
              },
              { "user_add_role" : { "account" : [ "account" ], "roles" : { "value" : { "8" : "8" } } } }
            ],
            "LABEL" : "If MSP (last 12 months) \\u003E $249 and MSP (last month or previous month) \\u003C $49"
          }
        },
        { "RULE" : {
            "IF" : [
              { "rtpr_user_condition_msp" : {
                  "account" : [ "account" ],
                  "campaigns" : { "value" : {
                      "c5d380ba" : "c5d380ba",
                      "c602cb08" : "c602cb08",
                      "39e673de" : "39e673de"
                    }
                  },
                  "tier" : "1",
                  "msp" : "249",
                  "op" : "\\u003E=",
                  "daterange" : "none",
                  "datetime_start" : "-1 year",
                  "datetime_end" : "now"
                }
              },
              { "AND" : [
                  { "OR" : [
                      { "rtpr_user_condition_msp" : {
                          "account" : [ "account" ],
                          "campaigns" : { "value" : {
                              "c5d380ba" : "c5d380ba",
                              "c602cb08" : "c602cb08",
                              "39e673de" : "39e673de"
                            }
                          },
                          "tier" : "1",
                          "msp" : "49",
                          "op" : "\\u003E=",
                          "daterange" : "LM",
                          "datetime_start" : 1430188019,
                          "datetime_end" : 1430188019
                        }
                      },
                      { "rtpr_user_condition_msp" : {
                          "account" : [ "account" ],
                          "campaigns" : { "value" : {
                              "c5d380ba" : "c5d380ba",
                              "c602cb08" : "c602cb08",
                              "39e673de" : "39e673de"
                            }
                          },
                          "tier" : "1",
                          "msp" : "49",
                          "op" : "\\u003E=",
                          "daterange" : "TM",
                          "datetime_start" : 1430187984,
                          "datetime_end" : 1430187984
                        }
                      }
                    ]
                  }
                ]
              },
              { "AND" : [
                  { "OR" : [
                      { "rtpr_user_condition_msp" : {
                          "account" : [ "account" ],
                          "campaigns" : { "value" : {
                              "c5d380ba" : "c5d380ba",
                              "c602cb08" : "c602cb08",
                              "39e673de" : "39e673de"
                            }
                          },
                          "tier" : "1",
                          "msp" : "449",
                          "op" : "\\u003C",
                          "daterange" : "LM",
                          "datetime_start" : 1430256699,
                          "datetime_end" : 1430256699
                        }
                      },
                      { "rtpr_user_condition_msp" : {
                          "account" : [ "account" ],
                          "campaigns" : { "value" : {
                              "c5d380ba" : "c5d380ba",
                              "c602cb08" : "c602cb08",
                              "39e673de" : "39e673de"
                            }
                          },
                          "tier" : "1",
                          "msp" : "449",
                          "op" : "\\u003C",
                          "daterange" : "TM",
                          "datetime_start" : 1430256661,
                          "datetime_end" : 1430256661
                        }
                      }
                    ]
                  }
                ]
              }
            ],
            "DO" : [
              { "user_remove_role" : {
                  "account" : [ "account" ],
                  "roles" : { "value" : {
                      "5" : "5",
                      "7" : "7",
                      "8" : "8",
                      "9" : "9",
                      "10" : "10",
                      "11" : "11",
                      "3" : "3",
                      "4" : "4",
                      "22" : "22",
                      "20" : "20",
                      "25" : "25",
                      "23" : "23",
                      "24" : "24",
                      "26" : "26",
                      "27" : "27",
                      "28" : "28",
                      "29" : "29",
                      "30" : "30",
                      "31" : "31"
                    }
                  }
                }
              },
              { "rtpr_user_action_remove_commissiongroup" : {
                  "account" : [ "account" ],
                  "commissiongroup" : { "value" : {
                      "11111111|bd4a45e0" : "11111111|bd4a45e0",
                      "11111111|e928564e" : "11111111|e928564e",
                      "33fadfd9|481fe3b5" : "33fadfd9|481fe3b5",
                      "33fadfd9|6a5b2cbb" : "33fadfd9|6a5b2cbb",
                      "39e673de|98f04a65" : "39e673de|98f04a65",
                      "39e673de|ae95c184" : "39e673de|ae95c184",
                      "c5d380ba|d22f9795" : "c5d380ba|d22f9795",
                      "c5d380ba|45609ce9" : "c5d380ba|45609ce9",
                      "c602cb08|d7df1364" : "c602cb08|d7df1364",
                      "c602cb08|7c51e99a" : "c602cb08|7c51e99a"
                    }
                  }
                }
              },
              { "user_add_role" : { "account" : [ "account" ], "roles" : { "value" : { "9" : "9" } } } },
              { "rtpr_user_action_add_commissiongroup" : {
                  "account" : [ "account" ],
                  "commissiongroup" : { "value" : {
                      "11111111|fd3dc104" : "11111111|fd3dc104",
                      "39e673de|8a65e5be" : "39e673de|8a65e5be",
                      "c5d380ba|93abaa31" : "c5d380ba|93abaa31",
                      "c602cb08|7720362e" : "c602cb08|7720362e"
                    }
                  },
                  "status" : "A"
                }
              }
            ],
            "LABEL" : "If MSP (last 12 months) \\u003E $249 and $49 \\u003C MSP (last month or previous month) \\u003C $449"
          }
        },
        { "RULE" : {
            "IF" : [
              { "rtpr_user_condition_msp" : {
                  "account" : [ "account" ],
                  "campaigns" : { "value" : {
                      "c5d380ba" : "c5d380ba",
                      "c602cb08" : "c602cb08",
                      "39e673de" : "39e673de"
                    }
                  },
                  "tier" : "1",
                  "msp" : "449",
                  "op" : "\\u003E=",
                  "daterange" : "none",
                  "datetime_start" : "-1 year",
                  "datetime_end" : "now"
                }
              },
              { "AND" : [
                  { "OR" : [
                      { "rtpr_user_condition_msp" : {
                          "account" : [ "account" ],
                          "campaigns" : { "value" : {
                              "c5d380ba" : "c5d380ba",
                              "c602cb08" : "c602cb08",
                              "39e673de" : "39e673de"
                            }
                          },
                          "tier" : "1",
                          "msp" : "449",
                          "op" : "\\u003E=",
                          "daterange" : "LM",
                          "datetime_start" : 1430188323,
                          "datetime_end" : 1430188323
                        }
                      },
                      { "rtpr_user_condition_msp" : {
                          "account" : [ "account" ],
                          "campaigns" : { "value" : {
                              "c5d380ba" : "c5d380ba",
                              "c602cb08" : "c602cb08",
                              "39e673de" : "39e673de"
                            }
                          },
                          "tier" : "1",
                          "msp" : "449",
                          "op" : "\\u003E=",
                          "daterange" : "TM",
                          "datetime_start" : 1430188292,
                          "datetime_end" : 1430188292
                        }
                      }
                    ]
                  }
                ]
              }
            ],
            "DO" : [
              { "user_remove_role" : {
                  "account" : [ "account" ],
                  "roles" : { "value" : {
                      "5" : "5",
                      "7" : "7",
                      "8" : "8",
                      "9" : "9",
                      "10" : "10",
                      "11" : "11",
                      "3" : "3",
                      "4" : "4",
                      "22" : "22",
                      "20" : "20",
                      "25" : "25",
                      "23" : "23",
                      "24" : "24",
                      "26" : "26",
                      "27" : "27",
                      "28" : "28",
                      "29" : "29",
                      "30" : "30",
                      "31" : "31"
                    }
                  }
                }
              },
              { "rtpr_user_action_remove_commissiongroup" : {
                  "account" : [ "account" ],
                  "commissiongroup" : { "value" : {
                      "11111111|fd3dc104" : "11111111|fd3dc104",
                      "11111111|e928564e" : "11111111|e928564e",
                      "33fadfd9|10dbb040" : "33fadfd9|10dbb040",
                      "39e673de|ae95c184" : "39e673de|ae95c184",
                      "39e673de|8a65e5be" : "39e673de|8a65e5be",
                      "c5d380ba|93abaa31" : "c5d380ba|93abaa31",
                      "c5d380ba|45609ce9" : "c5d380ba|45609ce9",
                      "c602cb08|7720362e" : "c602cb08|7720362e",
                      "c602cb08|7c51e99a" : "c602cb08|7c51e99a"
                    }
                  }
                }
              },
              { "user_add_role" : {
                  "account" : [ "account" ],
                  "roles" : { "value" : { "22" : "22", "23" : "23" } }
                }
              },
              { "rtpr_user_action_add_commissiongroup" : {
                  "account" : [ "account" ],
                  "commissiongroup" : { "value" : {
                      "11111111|bd4a45e0" : "11111111|bd4a45e0",
                      "33fadfd9|6a5b2cbb" : "33fadfd9|6a5b2cbb",
                      "39e673de|98f04a65" : "39e673de|98f04a65",
                      "c5d380ba|d22f9795" : "c5d380ba|d22f9795",
                      "c602cb08|d7df1364" : "c602cb08|d7df1364"
                    }
                  },
                  "status" : "A"
                }
              }
            ],
            "LABEL" : "If MSP (last 12 months) \\u003E $449 and MSP (last month or previous month) \\u003E $449"
          }
        },
        { "RULE" : {
            "IF" : [
              { "rtpr_user_condition_msp" : {
                  "account" : [ "account" ],
                  "campaigns" : { "value" : {
                      "c5d380ba" : "c5d380ba",
                      "c602cb08" : "c602cb08",
                      "39e673de" : "39e673de"
                    }
                  },
                  "tier" : "1",
                  "msp" : "449",
                  "op" : "\\u003E=",
                  "daterange" : "TM",
                  "datetime_start" : 1435152332,
                  "datetime_end" : 1435152332
                }
              },
              { "AND" : [
                  { "rtpr_user_condition_msp" : {
                      "account" : [ "account" ],
                      "campaigns" : { "value" : {
                          "c5d380ba" : "c5d380ba",
                          "c602cb08" : "c602cb08",
                          "39e673de" : "39e673de"
                        }
                      },
                      "tier" : "1",
                      "msp" : "449",
                      "op" : "\\u003E=",
                      "daterange" : "LM",
                      "datetime_start" : 1435152363,
                      "datetime_end" : 1435152363
                    }
                  }
                ]
              },
              { "AND" : [
                  { "rtpr_user_condition_msp" : {
                      "account" : [ "account" ],
                      "campaigns" : { "value" : {
                          "c5d380ba" : "c5d380ba",
                          "c602cb08" : "c602cb08",
                          "39e673de" : "39e673de"
                        }
                      },
                      "tier" : "1",
                      "msp" : "449",
                      "op" : "\\u003E=",
                      "daterange" : "none",
                      "datetime_start" : "first day of -2 month",
                      "datetime_end" : "last day of -2 month"
                    }
                  }
                ]
              }
            ],
            "DO" : [
              { "user_add_role" : { "account" : [ "account" ], "roles" : { "value" : { "24" : "24" } } } }
            ],
            "LABEL" : "If MSP \\u003E $449 (for last 3 months)"
          }
        },
        { "RULE" : {
            "DO" : [
              { "component_rules_remove_scheduled_roles_upgrade_downgrade_tasks_for_a_user" : { "account" : [ "account" ] } },
              { "schedule" : {
                  "component" : "rules_set_vendor_role_commission_groups",
                  "date" : "+1 day",
                  "identifier" : "Scheduled rules evaluation for Vendor [account:uid]",
                  "param_account" : [ "account" ]
                }
              }
            ],
            "LABEL" : "Reschedule rules evaluation for Vendor Role"
          }
        }
      ]
    }
  }');
  $items['rules_update_hcp_s_campaign_and_commission_groups_information'] = entity_import('rules_config', '{ "rules_update_hcp_s_campaign_and_commission_groups_information" : {
      "LABEL" : "Update hcp\\u0027s campaign and commission groups information",
      "PLUGIN" : "reaction rule",
      "WEIGHT" : "5",
      "OWNER" : "rules",
      "TAGS" : [ "Post Affiliate Pro" ],
      "REQUIRES" : [ "rules", "rtpr_user" ],
      "ON" : { "user_insert" : [], "user_login" : [] },
      "IF" : [
        { "user_has_role" : { "account" : [ "account" ], "roles" : { "value" : { "10" : "10" } } } }
      ],
      "DO" : [
        { "rtpr_user_action_add_commissiongroup" : {
            "account" : [ "account" ],
            "commissiongroup" : { "value" : {
                "11111111|96f944c6" : "11111111|96f944c6",
                "c5d380ba|12b433fc" : "c5d380ba|12b433fc",
                "c602cb08|2fb044f4" : "c602cb08|2fb044f4"
              }
            },
            "status" : "A"
          }
        }
      ]
    }
  }');
  $items['rules_update_merchant_s_campaign_and_commission_groups_informati'] = entity_import('rules_config', '{ "rules_update_merchant_s_campaign_and_commission_groups_informati" : {
      "LABEL" : "Update merchant\\u0027s campaign and commission groups information",
      "PLUGIN" : "reaction rule",
      "WEIGHT" : "5",
      "OWNER" : "rules",
      "TAGS" : [ "Post Affiliate Pro" ],
      "REQUIRES" : [ "rules", "rtpr_user" ],
      "ON" : { "user_insert" : [], "user_login" : [] },
      "IF" : [
        { "user_has_role" : { "account" : [ "account" ], "roles" : { "value" : { "11" : "11" } } } }
      ],
      "DO" : [
        { "rtpr_user_action_add_commissiongroup" : {
            "account" : [ "account" ],
            "commissiongroup" : { "value" : {
                "11111111|5a1ea620" : "11111111|5a1ea620",
                "c5d380ba|69b77e47" : "c5d380ba|69b77e47",
                "c602cb08|7e8c0a9e" : "c602cb08|7e8c0a9e"
              }
            },
            "status" : "A"
          }
        }
      ]
    }
  }');
  $items['rules_update_the_affiliate_id'] = entity_import('rules_config', '{ "rules_update_the_affiliate_id" : {
      "LABEL" : "Create an affiliate account on PAP after saving a new user account",
      "PLUGIN" : "reaction rule",
      "WEIGHT" : "-1",
      "OWNER" : "rules",
      "TAGS" : [ "Post Affiliate Pro" ],
      "REQUIRES" : [ "rules", "php", "rtpr_user" ],
      "ON" : { "user_insert" : [] },
      "IF" : [
        { "user_has_role" : {
            "account" : [ "account" ],
            "roles" : { "value" : { "4" : "4", "10" : "10", "11" : "11" } },
            "operation" : "OR"
          }
        }
      ],
      "DO" : [
        { "rtpr_user_action_create_affiliate" : {
            "USING" : {
              "account" : [ "account" ],
              "first_name" : [ "account:field-first-name" ],
              "last_name" : [ "account:field-last-name" ],
              "email" : [ "account:mail" ],
              "password" : "\\u003C?php \\r\\nreturn user_password();\\r\\n?\\u003E ",
              "phone" : [ "account:field-phone" ],
              "street" : [ "account:field-address:thoroughfare" ],
              "city" : [ "account:field-address:locality" ],
              "states" : [ "account:field-address:administrative-area" ],
              "zip" : [ "account:field-address:postal-code" ],
              "country" : [ "account:field-address:country" ],
              "status" : { "value" : { "A" : "A" } }
            },
            "PROVIDE" : { "affiliate_id" : { "affiliate_id" : "Affiliate id" } }
          }
        },
        { "data_set" : {
            "data" : [ "account:field-pap-affiliate-id" ],
            "value" : [ "affiliate-id" ]
          }
        },
        { "entity_save" : { "data" : [ "account" ], "immediate" : "1" } }
      ]
    }
  }');
  $items['rules_update_the_affiliate_referral_relation_on_pap'] = entity_import('rules_config', '{ "rules_update_the_affiliate_referral_relation_on_pap" : {
      "LABEL" : "Update the affiliate referral relation on PAP",
      "PLUGIN" : "reaction rule",
      "ACTIVE" : false,
      "OWNER" : "rules",
      "TAGS" : [ "Post Affiliate Pro" ],
      "REQUIRES" : [ "referral", "rtpr_user", "rules" ],
      "ON" : { "user_insert" : [] },
      "IF" : [ { "user_referred" : { "user" : [ "account" ] } } ],
      "DO" : [
        { "rtpr_user_action_get_referring_user" : {
            "USING" : { "account" : [ "account" ] },
            "PROVIDE" : { "referring_user" : { "referring_user" : "Referring user" } }
          }
        },
        { "rtpr_user_action_add_affiliate_relations" : { "referring-user" : [ "referring-user" ], "new-user" : [ "account" ] } }
      ]
    }
  }');
  return $items;
}
