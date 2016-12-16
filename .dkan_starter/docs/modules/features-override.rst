Features Override Module
------------------------

`Features Override Module <https://www.drupal.org/project/features_override>`_ is used to change (override) existing Features. 

 * This allows us to keep Features overridden without it looking that way.
 * Also a (not very clean) way to deploy overrides from development environments to production.
 * Not the prettiest solution, but then again, Features.

It is very important to understand that features overrides should be the very last resource to make things work a certain way. Try the following first:

 1. Find a hook alter implementation that lets you modify the configuration item you need to override.
 2. If you don't find a hook alter implementation, look again for it.
 3. Don't expect google or drupalstackexchange to give you the answer, take a look at the code for the module that implements whatever you are trying to override
 4. If you need to set different values for a variable depending on the environment, do it in the drupal settings.php file. There's plenty of examples on how to achieve this in https://github.com/NuCivic/data_starter/blob/master/assets/sites/default/settings.php#L98
 5. Take a look on why the item you are trying to override is exported as a feature in the first place and, if possible, propose a way to take it off features and implement a hook alter implementation so devs can tweak it to their liking.
 6. If none of the above works for you, then go with features_overrides. But really, why are even you doing this to yourself?
