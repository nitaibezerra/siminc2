<?php
/**
 * Abstract class for extension
 */
require_once 'Zend/View/Helper/FormElement.php';


class Simec_View_Helper_Email extends Simec_View_Helper_Element
{
    public function email($name, $label = null, $value = null, $attribs = array(), $config = array())
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
        $class = isset($attribs['class']) ? 'form-control cep ' .  $attribs['class'] : 'form-control email';

        $config['email'] = 'email';
        $config['icon'] = $config['icon'] ? $config['icon'] : 'fa fa-envelope';
        $config['label-for'] = isset($config['label-for']) ? $config['label-for'] : $id;

        unset($attribs['id'], $attribs['type'], $attribs['class'], $attribs['']);

        $podeEditar = isset($config['pode-editar']) ? $config['pode-editar'] : true;
        if(!$podeEditar || $podeEditar==='N'){
            $xhtml = '<p class="form-control-static" id="' . $id . '">' . $value . '</p>';
            $config['icon'] = null;
        } else {
            // Construindo o input
            $xhtml = '<input'
                    . ' data-alias="\'email\'"'
                    . ' name="' . $this->view->escape($name) . '"'
                    . ' id="' . $this->view->escape($id) . '"'
                    . ' type="' . $type . '"'
                    . ' value="' . $value . '"'
                    . ' class="' . $class . '"'
                    . $this->_htmlAttribs($attribs)
                    . ' />';
        }

        return $this->buildField($xhtml, $label, $attribs, $config);
    }
}
