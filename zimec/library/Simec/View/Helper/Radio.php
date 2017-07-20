<?php
/**
 * Abstract class for extension
 */
require_once 'Zend/View/Helper/FormElement.php';


class Simec_View_Helper_Radio extends Simec_View_Helper_Options
{
    public function radio($name, $label = null, $value = null, $options = null, $attribs = null, $config = array())
    {
        $attribs['type'] = 'radio';

        $podeEditar = isset($config['pode-editar']) ? $config['pode-editar'] : true;
        if(!$podeEditar || $podeEditar==='N'){
            $attribs['disabled'] = 'disabled';
        }

        return $this->options($name, $label, $value, $options, $attribs, $config);
    }
}
