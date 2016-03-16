<?php
/**
 * Abstract class for extension
 */
require_once 'Zend/View/Helper/FormElement.php';


class Simec_View_Helper_Cnpj extends Simec_View_Helper_Element
{
    public function cnpj($name, $label = null, $value = null, $attribs = array(), $config = array())
    {
        $attribs = (array) $attribs;

        foreach($attribs as $chave => $attrib){
            if(is_numeric($chave)){
                $attribs[$attrib] = $attrib;
                unset($attribs[$chave]);
            }
        }

        $id = isset($attribs['id']) ? $attribs['id'] : $name;
        $type = isset($attribs['type']) ? $attribs['type'] : 'text';
        $class = isset($attribs['class']) ? 'form-control cpf ' .  $attribs['class'] : 'form-control cpf';

        $config['cnpj'] = 'cnpj';
        $config['icon'] = $config['icon'] ? $config['icon'] : 'fa fa-building';
        $config['label-for'] = isset($config['label-for']) ? $config['label-for'] : $id;

        unset($attribs['id'], $attribs['type'], $attribs['class']);

        // Construindo o input
        $xhtml = '<input'
        		. ' data-inputmask="\'mask\': \'99.999.999/9999-99\', \'clearIncomplete\': \'true\'"'
                . ' name="' . $this->view->escape($name) . '"'
                . ' id="' . $this->view->escape($id) . '"'
                . ' type="' . $type . '"'
                . ' value="' . $value . '"'
                . ' class="' . $class . '"'
                . $this->_htmlAttribs($attribs)
                . ' />';

        return $this->buildField($xhtml, $label, $attribs, $config);
    }
}
