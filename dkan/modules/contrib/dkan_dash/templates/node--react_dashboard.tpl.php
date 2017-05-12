<?php
$needle = 'iframe';
$length = strlen($needle);
$path = current_path();
$offset = strlen($path) - $length;
?>
<?php if (strpos($path, $needle, $offset) === false && node_access('update', $node, $user) === TRUE): ?>
  <!-- Embed button -->
  <div class="react_dashboard-embed">
    <a class="embed-link" href="#embed-wrapper"><?php print t('Embed'); ?></a>
      <div id="embed-wrapper" class="embed-code-wrapper" style="display: none">
        <form>
          <div class="form-group">
            <label for="embed-code">Embed code</label>
            <textarea id="embed-code" class="form-control embed-code" onclick="select()"><script type="text/javascript">var node = document.scripts[document.scripts.length - 1];var parent = node.parentElement;var iframe = document.createElement('iframe');iframe.setAttribute('src', '<?php print $GLOBALS['base_url'] . '/dashboard/' . $node->nid . '/iframe' ?>');iframe.width = '100%';iframe.frameBorder = 0;parent.insertBefore(iframe, node.nextSibling);window.addEventListener('message', function(e){ iframe.height = e.data }, false);</script></textarea>
          </div>
        </form>
      </div>
  </div>
<?php endif; ?>
<!-- Dashboard Root Node -->
<div id="root"></div>
