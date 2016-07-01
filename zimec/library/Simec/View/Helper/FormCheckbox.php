<?php
/**
 * Abstract class for extension
 */
require_once 'Zend/View/Helper/FormElement.php';


class Simec_View_Helper_FormCheckbox extends Simec_View_Helper_FormOptions
{
    public function formCheckbox($name, $label = null, $value = null, $options = null, $attribs = null, $config = array())
    {
        $attribs['type'] = 'checkbox';
        return $this->formOptions($name, $label, $value, $options, $attribs, $config);
    }
}
