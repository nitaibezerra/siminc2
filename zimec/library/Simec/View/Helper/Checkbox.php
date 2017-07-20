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

        $podeEditar = isset($config['pode-editar']) ? $config['pode-editar'] : true;
        if(!$podeEditar || $podeEditar==='N'){
            $attribs['disabled'] = 'disabled';
        }

        return $this->options($name, $label, $value, $options, $attribs, $config);
    }
}
