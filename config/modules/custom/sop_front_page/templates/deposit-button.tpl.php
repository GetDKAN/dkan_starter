<?php global $base_path, $base_url; $user ?>
<?php if ($user->uid == 0): ?>
  <a href="<?php print $base_path . 'saml_login?ReturnTo=' . $base_url . $base_path . 'node/add/dataset' ?>" id="deposit_button"><span></span><?php print t('DEPOSIT DATA'); ?></a>
<?php else: ?>
  <a href="/node/add/dataset" id="deposit_button"><span></span><?php print t('DEPOSIT DATA'); ?></a>
<?php endif; ?>
