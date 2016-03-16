<?php
/**
 * Abstract class for extension
 */
require_once 'Zend/View/Helper/FormElement.php';


class Simec_View_Helper_Checkbox extends Simec_View_Helper_Options
{
    public function checkbox($name, $label = null, $value = null, $options = null, $attribs = array(), $config = array())
    {
        $attribs['type'] = 'checkbox';
        return $this->options($name, $label, $value, $options, $attribs, $config);
    }
}
