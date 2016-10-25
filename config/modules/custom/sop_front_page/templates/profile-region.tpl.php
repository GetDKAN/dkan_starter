<!-- right navigation -->
<ul class="list-inline top-social-icons">
  <li>
    <a href="http://www.facebook.com/worldbank"><i class="sprite icon-top-fb"></i></a>
    <a href="http://www.twitter.com/worldbank"><i class="sprite icon-top-twitter"></i></a>
    <a href="https://instagram.com/worldbank/"><i class="sprite icon-top-instagram"></i></a>
    <a href="http://www.linkedin.com/company/the-world-bank"><i class="sprite icon-top-linkedin"></i></a>
    <a href="http://live.worldbank.org/connect"><i class="sprite icon-top-leftarrow"></i></a>
  </li>
</ul>
<?php if ($user->uid): ?>
  <ul class="list-inline">
    <li><a href="/admin/workbench" class="top-mydataset"><?php print t('My Dataset'); ?></a></li>
  </ul>
  <ul class="list-inline">
    <li class="dropdown">
      <a href="#" class = "dropdown-toggle" data-toggle="dropdown">
        <?php print $params['picture']; ?>
        <?php print $params['name']; ?>
        <span class="caret"></span></a>
        <ul class="dropdown-menu">
          <li><a href="<?php print $params['base_path'] . drupal_get_path_alias('user/' . $user->uid); ?>"><?php print $params['picture']; ?><?php print t('Profile');?></a></li>
          <li><a href="/user/logout"><i class="sprite icon-logout"></i> <?php print t('Logout'); ?></a></li>
          <li><a href="#"><i class="sprite icon-help"></i> <?php print t('Help'); ?></a></li>
      </ul>
    </li>
  </ul>

<?php else: ?>
  <ul class="list-inline top-login">
    <li>
      <a href="/saml_login?ReturnTo=<?php print $params['base_url'] . $params['base_path'] ?>"><i class="sprite icon-login"></i> Login</a>
    </li>
  </ul>
<?php endif; ?>
<!-- right navigation -->

