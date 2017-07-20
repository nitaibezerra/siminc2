<?php
/**
 * Abstract class for extension
 */
require_once 'Zend/View/Helper/FormElement.php';


class Simec_View_Helper_Data extends Simec_View_Helper_Element
{
    public function data($name, $label = null, $value = null, $attribs = array(), $config = array())
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
        $class = isset($attribs['class']) ? 'form-control ' .  $attribs['class'] : 'form-control';

        $config['date'] = 'date';
        $config['icon'] = $config['icon'] ? $config['icon'] : 'fa fa-calendar';
        $config['label-for'] = isset($config['label-for']) ? $config['label-for'] : $id;

        unset($attribs['id'], $attribs['type'], $attribs['class']);

        if($value){
            $value = Simec_Util::formatarData($value, Zend_Date::DATES);
        }

        $podeEditar = isset($config['pode-editar']) ? $config['pode-editar'] : true;
        if(!$podeEditar || $podeEditar==='N'){
            $xhtml = '<p class="form-control-static" id="' . $id . '">' . $value . '</p>';
            $config['icon'] = $config['date'] = null;
        } else {
            // Construindo o input
            $xhtml = '<input'
                . ' name="' . $this->view->escape($name) . '"'
                . ' id="' . $this->view->escape($id) . '"'
                . ' type="' . $type . '"'
                . ' value="' . $value . '"'
                . ' class="datemask ' . $class . '"'
                . $this->_htmlAttribs($attribs)
                . ' />';
        }
        return $this->buildField($xhtml, $label, $attribs, $config);
    }
}
