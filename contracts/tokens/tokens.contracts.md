<h1 class="contract">close</h1>

---

spec_version: "0.2.0"
title: Close Token Balance
summary: 'Close {{nowrap owner}}’s zero quantity balance'
icon: @ICON_BASE_URL@/@TOKEN_ICON_URI@

---

{{owner}} agrees to close their zero quantity balance for the {{symbol_to_symbol_code symbol}} token.

RAM will be refunded to the RAM payer of the {{symbol_to_symbol_code symbol}} token balance for {{owner}}.

<h1 class="contract">create</h1>

---

spec_version: "0.2.0"
title: Create New Token
summary: 'Create a new token'
icon: @ICON_BASE_URL@/@TOKEN_ICON_URI@

---

{{$action.account}} agrees to create a new token with symbol {{asset_to_symbol_code maximum_supply}} to be managed by {{issuer}}.

This action will not result any any tokens being issued into circulation.

{{issuer}} will be allowed to issue tokens into circulation, up to a maximum supply of {{maximum_supply}}.

RAM will deducted from {{$action.account}}’s resources to create the necessary records.

<h1 class="contract">setmaxsupply</h1>

---

spec_version: "0.2.0"
title: Set Max Supply
summary: 'Set max supply for token'
icon: @ICON_BASE_URL@/@TOKEN_ICON_URI@

---

{{issuer}} will be allowed to issue tokens into circulation, up to a maximum supply of {{maximum_supply}}.

This action will not result any any tokens being issued into circulation.

<h1 class="contract">issue</h1>

---

spec_version: "0.2.0"
title: Issue Tokens into Circulation
summary: 'Issue {{nowrap quantity}} into circulation and transfer into {{nowrap to}}’s account'
icon: @ICON_BASE_URL@/@TOKEN_ICON_URI@

---

The token manager agrees to issue {{quantity}} into circulation, and transfer it into {{to}}’s account.

{{#if memo}}There is a memo attached to the transfer stating:
{{memo}}
{{/if}}

If {{to}} does not have a balance for {{asset_to_symbol_code quantity}}, or the token manager does not have a balance for {{asset_to_symbol_code quantity}}, the token manager will be designated as the RAM payer of the {{asset_to_symbol_code quantity}} token balance for {{to}}. As a result, RAM will be deducted from the token manager’s resources to create the necessary records.

This action does not allow the total quantity to exceed the max allowed supply of the token.

<h1 class="contract">issuefixed</h1>

---

spec_version: "0.2.0"
title: Issue Fixed Supply of Tokens into Circulation
summary: 'Issue up to {{nowrap supply}} supply into circulation and transfer into {{nowrap to}}’s account'
icon: @ICON_BASE_URL@/@TOKEN_ICON_URI@

---

The token manager agrees to issue tokens up to {{supply}} fixed supply into circulation, and transfer it into {{to}}’s account.

{{#if memo}}There is a memo attached to the transfer stating:
{{memo}}
{{/if}}

If {{to}} does not have a balance for {{asset_to_symbol_code quantity}}, or the token manager does not have a balance for {{asset_to_symbol_code quantity}}, the token manager will be designated as the RAM payer of the {{asset_to_symbol_code quantity}} token balance for {{to}}. As a result, RAM will be deducted from the token manager’s resources to create the necessary records.

This action does not allow the total quantity to exceed the max allowed supply of the token.

<h1 class="contract">open</h1>

---

spec_version: "0.2.0"
title: Open Token Balance
summary: 'Open a zero quantity balance for {{nowrap owner}}'
icon: @ICON_BASE_URL@/@TOKEN_ICON_URI@

---

{{ram_payer}} agrees to establish a zero quantity balance for {{owner}} for the {{symbol_to_symbol_code symbol}} token.

If {{owner}} does not have a balance for {{symbol_to_symbol_code symbol}}, {{ram_payer}} will be designated as the RAM payer of the {{symbol_to_symbol_code symbol}} token balance for {{owner}}. As a result, RAM will be deducted from {{ram_payer}}’s resources to create the necessary records.

<h1 class="contract">retire</h1>

---

spec_version: "0.2.0"
title: Remove Tokens from Circulation
summary: 'Remove {{nowrap quantity}} from circulation'
icon: @ICON_BASE_URL@/@TOKEN_ICON_URI@

---

The token manager agrees to remove {{quantity}} from circulation, taken from their own account.

{{#if memo}} There is a memo attached to the action stating:
{{memo}}
{{/if}}

<h1 class="contract">transfer</h1>

---

spec_version: "0.2.0"
title: Transfer Tokens
summary: 'Send {{nowrap quantity}} from {{nowrap from}} to {{nowrap to}}'
icon: @ICON_BASE_URL@/@TRANSFER_ICON_URI@

---

{{from}} agrees to send {{quantity}} to {{to}}.

{{#if memo}}There is a memo attached to the transfer stating:
{{memo}}
{{/if}}

If {{from}} is not already the RAM payer of their {{asset_to_symbol_code quantity}} token balance, {{from}} will be designated as such. As a result, RAM will be deducted from {{from}}’s resources to refund the original RAM payer.

If {{to}} does not have a balance for {{asset_to_symbol_code quantity}}, {{from}} will be designated as the RAM payer of the {{asset_to_symbol_code quantity}} token balance for {{to}}. As a result, RAM will be deducted from {{from}}’s resources to create the necessary records.

<h1 class="contract">regtoken</h1>

---

spec_version: "0.2.0"
title: Register Token
summary: 'Register a new token'
icon: @ICON_BASE_URL@/@TOKEN_ICON_URI@

---

<h1 class="contract">distribute</h1>

---

spec_version: "0.2.0"
title: Distribute Token
summary: 'Distribute newly minted tokens'
icon: @ICON_BASE_URL@/@TOKEN_ICON_URI@

---

<h1 class="contract">setconfig</h1>

---

spec_version: "0.2.0"
title: Set Configuration
summary: 'Set the tokens contract current configuration'
icon: @ICON_BASE_URL@/@TOKEN_ICON_URI@

---

<h1 class="contract">cleartable</h1>

---

spec_version: "0.2.0"
title: 'DEBUG: Clear Table'
summary: 'DEBUG: Removes all data from a table.'
icon: https://avatars.githubusercontent.com/u/147292861?s=400&u=3b1af66e90dd851f4d7c096ed6a2fbb4b9e190da

---

<h1 class="contract">reset</h1>

---

spec_version: "0.2.0"
title: 'DEBUG: Reset Contract State'
summary: 'DEBUG: Resets the contract state to default values.'
icon: https://avatars.githubusercontent.com/u/147292861?s=400&u=3b1af66e90dd851f4d7c096ed6a2fbb4b9e190da

---

<h1 class="contract">wipe</h1>

---

spec_version: "0.2.0"
title: 'DEBUG: Wipe Contract Data'
summary: 'DEBUG: Removes all contract state.'
icon: https://avatars.githubusercontent.com/u/147292861?s=400&u=3b1af66e90dd851f4d7c096ed6a2fbb4b9e190da

---
