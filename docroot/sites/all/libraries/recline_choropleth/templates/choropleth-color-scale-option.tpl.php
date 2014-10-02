<?php
/**
 * @file
 * Template for the color scale options for the choropleth admin.
 */

foreach ($colors as $color):
?>
<span style="width:10px;height:10px;display:inline-block;background-color:<?php print $color ?>;border:1px black solid;"></span>
<?php
endforeach;
?>
