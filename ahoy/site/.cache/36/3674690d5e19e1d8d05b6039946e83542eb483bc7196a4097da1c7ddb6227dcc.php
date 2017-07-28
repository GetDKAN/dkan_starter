<?php

/* config.php.twig */
class __TwigTemplate_828f08b45229f756082e9d8f8d5c2cad54c1ad1caac76416b248e7d033591e6a extends Twig_Template
{
    public function __construct(Twig_Environment $env)
    {
        parent::__construct($env);

        $this->parent = false;

        $this->blocks = array(
        );
    }

    protected function doDisplay(array $context, array $blocks = array())
    {
        // line 1
        echo "<?php
/**
 * @file
 * Contents generated automatically by `ahoy site config` command.
 * DO NOT EDIT.
 */
\$conf = ";
        // line 7
        echo twig_escape_filter($this->env, var_export((isset($context["config"]) ? $context["config"] : null)), "html", null, true);
        echo ";";
    }

    public function getTemplateName()
    {
        return "config.php.twig";
    }

    public function isTraitable()
    {
        return false;
    }

    public function getDebugInfo()
    {
        return array (  27 => 7,  19 => 1,);
    }

    /** @deprecated since 1.27 (to be removed in 2.0). Use getSourceContext() instead */
    public function getSource()
    {
        @trigger_error('The '.__METHOD__.' method is deprecated since version 1.27 and will be removed in 2.0. Use getSourceContext() instead.', E_USER_DEPRECATED);

        return $this->getSourceContext()->getCode();
    }

    public function getSourceContext()
    {
        return new Twig_Source("", "config.php.twig", "/Users/msolv/Projects/nucivic/dkan_starter/.ahoy/site/.templates/config.php.twig");
    }
}
