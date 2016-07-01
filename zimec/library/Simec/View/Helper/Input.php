<?php
/**
 * Abstract class for extension
 */
require_once 'Zend/View/Helper/FormElement.php';


class Simec_View_Helper_Input extends Simec_View_Helper_Element
{
    public function input($name, $label = null, $value = null, $attribs = array(), $config = array())
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
        $help = isset($attribs['help']) ? true : false;
        
        $config['label-for'] = isset($config['label-for']) ? $config['label-for'] : $id;
        $config['visible'] = isset($config['visible']) ? $config['visible'] : true;

        if($value && strpos($class, 'data') !== false){
            $value = Simec_Util::formatarData($value, Zend_Date::DATES);
        }

        unset($attribs['id'], $attribs['type'], $attribs['class']);

        // Construindo o input
        $xhtml = '<input'
                . ' name="' . $this->view->escape($name) . '"'
                . ' id="' . $this->view->escape($id) . '"'
                . ' type="' . $type . '"'
                . ' value="' . $value . '"'
                . ' class="' . $class . '"'
                . $this->_htmlAttribs($attribs)
                . ' />';
        
        if ($help) {
        	$xhtml.= "<span class='help-block m-b-none'><i class='fa fa-question-circle' style='color: #1c84c6;'></i> {$attribs['help']}</span>";
        }

        return $this->buildField($xhtml, $label, $attribs, $config);
    }
}
